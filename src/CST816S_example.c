#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "CST816S.h"

void app_main() 
{
    uint8_t data[7];
    //ESP_ERROR_CHECK(i2c_master_init());
    //ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read the MPU9250 WHO_AM_I register, on power up the register should have the value 0x71 */
    ESP_ERROR_CHECK(cst816s_register_read(CST816S_ADDRESS, data, 1));
    //ESP_LOGI(TAG, "WHO_AM_I = %X", data[0]);

    /* Demonstrate writing by reseting the MPU9250 */
    ESP_ERROR_CHECK(cst816s_register_write_byte(CST816S_ADDRESS, 1 << CST816S_RESET_BIT));

    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
    //ESP_LOGI(TAG, "I2C de-initialized successfully");
}