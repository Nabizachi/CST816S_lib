#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "CST816S.h"

static const char *TAG = "CST816S ";

esp_err_t i2c_master_init(void)
{
  int i2c_master_port = I2C_MASTER_NUM;

  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = _sda,
      .scl_io_num = _scl,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = I2C_MASTER_FREQ_HZ,
  };
  i2c_param_config(i2c_master_port, &conf);
  return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void cst816s_read_touch()
{
  uint8_t rx_data[6];
  uint8_t tx_data[5];
  tx_data[0] = 0x01;
  i2c_master_write_to_device(I2C_NUM_0, CST816S_ADDRESS, tx_data, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  vTaskDelay(50 / portTICK_RATE_MS);
  i2c_master_read_from_device(I2C_NUM_0, CST816S_ADDRESS, rx_data, 6, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  // ESP_LOG_BUFFER_HEX(TAG, rx_data, 6);
  vTaskDelay(50 / portTICK_RATE_MS);
  data.gestureID = rx_data[0];
  data.points = rx_data[1];
  data.event = rx_data[2] >> 6;
  data.x = rx_data[3];
  data.y = rx_data[5];
}

void IRAM_ATTR gpioHandler(void *arg)
{
  _event_available = true;
}

void cst816s_begin(int sda, int scl, int rst, int irq)
{
  _sda = sda;
  _scl = scl;
  _rst = rst;
  _irq = irq;
  ESP_ERROR_CHECK(i2c_master_init());
  gpio_pad_select_gpio(_rst);
  gpio_set_direction(_rst, GPIO_MODE_OUTPUT);
  gpio_pad_select_gpio(_irq);
  gpio_set_direction(_irq, GPIO_MODE_INPUT);

  gpio_set_level(_rst, 1);
  vTaskDelay(50 / portTICK_RATE_MS);
  gpio_set_level(_rst, 0);
  vTaskDelay(5 / portTICK_RATE_MS);
  gpio_set_level(_rst, 1);
  vTaskDelay(50 / portTICK_RATE_MS);

  uint8_t rx_data[5];
  uint8_t tx_data[5];
  // проверка работоспособности чипа
  tx_data[0] = 0x15;
  ESP_LOGE(TAG, "tx_data = 0x15");
  i2c_master_write_to_device(I2C_NUM_0, CST816S_ADDRESS, tx_data, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  vTaskDelay(50 / portTICK_RATE_MS);
  i2c_master_read_from_device(I2C_NUM_0, CST816S_ADDRESS, rx_data, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);
  vTaskDelay(50 / portTICK_RATE_MS);
  //проверка версии чипа
  tx_data[0] = 0xA7;
  ESP_LOGE(TAG, "tx_data = 0xA7");
  i2c_master_write_to_device(I2C_NUM_0, CST816S_ADDRESS, tx_data, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  vTaskDelay(50 / portTICK_RATE_MS);
  i2c_master_read_from_device(I2C_NUM_0, CST816S_ADDRESS, rx_data, 3, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  ESP_LOG_BUFFER_HEX(TAG, rx_data, 3);
  vTaskDelay(50 / portTICK_RATE_MS);
  //установка и инициализация прерываний
  gpio_install_isr_service(0);
  gpio_set_intr_type(_irq, GPIO_INTR_NEGEDGE);
  gpio_isr_handler_add(_irq, gpioHandler, (void *)_irq);
  gpio_intr_enable(_irq);
}

bool cst816s_available()
{
  if (_event_available)
  {
    cst816s_read_touch();
    _event_available = false;
    return true;
  }
  else
  {
    vTaskDelay(1);
  }
  return false;
}