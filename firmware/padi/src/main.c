#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "device.h"
#include <platform_opts.h>
#include <platform_stdlib.h>
#include "pwmout_api.h"
#include "main.h"

#define PWM_R PC_0
#define PWM_G PC_3
#define PWM_B PC_1
#define PWM_PERIOD 2000 // 500 Hz
#define PWM_STACK_SIZE 128
#define PWM_PRIORITY tskIDLE_PRIORITY + 1

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

pwmout_t pwm_r;
pwmout_t pwm_g;
pwmout_t pwm_b;

QueueHandle_t color_queue;

void pwm_task(void* pvParameters) {
  struct Color clr;
  if (color_queue != 0) {
    while (1) {
      if (xQueueReceive(color_queue, &clr, portMAX_DELAY)) {
        int red = (clr.r * PWM_PERIOD) / 255;
        int green = (clr.g * PWM_PERIOD) / 255;
        int blue = (clr.b * PWM_PERIOD) / 255;
        pwmout_pulsewidth_us(&pwm_r, red);
        pwmout_pulsewidth_us(&pwm_g, green);
        pwmout_pulsewidth_us(&pwm_b, blue);
      }
    }
  }
}

void pwm_init() {
  pwmout_init(&pwm_r, PWM_R);
  pwmout_init(&pwm_g, PWM_G);
  pwmout_init(&pwm_b, PWM_B);

  pwmout_period_us(&pwm_r, PWM_PERIOD);
  pwmout_period_us(&pwm_g, PWM_PERIOD);
  pwmout_period_us(&pwm_b, PWM_PERIOD);

  pwmout_pulsewidth_us(&pwm_r, 0);
  pwmout_pulsewidth_us(&pwm_g, 0);
  pwmout_pulsewidth_us(&pwm_b, 0);

  if (xTaskCreate(pwm_task, "pwm_task", PWM_STACK_SIZE, NULL, PWM_PRIORITY, NULL) != pdPASS) {
    printf("\r\n%s xTaskCreate(pwm_task) failed", __FUNCTION__);
  }
}

void main() {
  printf("iDo is booting!\r\n");

  wlan_network();
  printf("Initialized WiFi.\r\n");

  color_queue = xQueueCreate(4, sizeof(struct Color));
  if (color_queue == 0) {
    printf("Failed to create color_queue!\r\n");
  }
  printf("Created color queue.\r\n");

  pwm_init();
  printf("Initialized PWM.\r\n");

  struct Color clr;
  clr.r = 128;
  clr.g = 64;
  clr.b = 32;
  xQueueSend(color_queue, &clr, 0);

  printf("Initializing console and starting scheduler.\r\n");
  console_init();
  vTaskStartScheduler();
}
