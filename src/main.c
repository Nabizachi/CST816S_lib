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

void app_main()
{
  ESP_LOGE(TAG, "CST816S example begin");
  cst816s_begin(21, 22, 19, 34);
  while (1)
  {
    if (cst816s_available())
    {
      /*printf("gesture.id %u \n", data.gestureID);
      printf("points %u \n", data.points);
      printf("event %u \n", data.event);*/
      printf("x %i \n", data.x);
      printf("y %i \n", data.y);

      switch (data.gestureID)
      {
      case NONE:
        // printf("NONE");
        break;
      case SWIPE_DOWN:
        printf("SWIPE DOWN \n");
        break;
      case SWIPE_UP:
        printf("SWIPE UP \n");
        break;
      case SWIPE_LEFT:
        printf("SWIPE LEFT \n");
        break;
      case SWIPE_RIGHT:
        printf("SWIPE RIGHT \n");
        break;
      case SINGLE_CLICK:
        printf("SINGLE CLICK \n");
        break;
      case DOUBLE_CLICK:
        printf("DOUBLE CLICK \n");
        break;
      case LONG_PRESS:
        printf("LONG PRESS \n");
        break;
      default:
        printf("UNKNOWN \n");
        break;
      }
    }
  }
}