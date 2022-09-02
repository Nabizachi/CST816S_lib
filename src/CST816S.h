#ifndef __CST816S_H__
#define __CST816S_H__
#define I2C_MASTER_NUM 0            /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ 400000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000
#define CST816S_ADDRESS 0x15
#define CST816S_RESET_BIT 8
#define CST816S_RAW_DATA_SIZE 8

typedef struct data_struct
{
  uint8_t gestureID; // Gesture ID
  uint8_t points;    // Number of touch points
  uint8_t event;     // Event (0 = Down, 1 = Up, 2 = Contact)
  int x;
  int y;
  uint8_t version;
  uint8_t versionInfo[3];
} data_struct_t;

typedef enum GESTURE
{
  NONE = 0x00,
  SWIPE_UP = 0x01,
  SWIPE_DOWN = 0x02,
  SWIPE_LEFT = 0x04,
  SWIPE_RIGHT = 0x03,
  SINGLE_CLICK = 0x05,
  DOUBLE_CLICK = 0x0B,
  LONG_PRESS = 0x0C

} gesture_t;

int _sda;
int _scl;
int _rst;
int _irq;

void cst816s_begin(int sda, int scl, int rst, int irq);
bool cst816s_available();
void cst816s_read_touch();
bool _event_available;
void IRAM_ATTR gpioHandler(void *arg);
esp_err_t i2c_master_init(void);
data_struct_t data;
// String gesture();

#endif