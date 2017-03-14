#include "FreeRTOS.h"
#include "task.h"
#include "device.h"
#include <platform_opts.h>
#include <platform_stdlib.h>
#include "pwmout_api.h"

#define PWM_R PC_0
#define PWM_G PC_3
#define PWM_B PC_1
#define PWM_PERIOD 2000 // 500 Hz
#define PWM_STACK_SIZE 128
#define PWM_PRIORITY tskIDLE_PRIORITY + 1

pwmout_t pwm_r;
pwmout_t pwm_g;
pwmout_t pwm_b;

void pwm_task(void* pvParameters) {
  while (1) {
    pwmout_pulsewidth_us(&pwm_r, PWM_PERIOD / 2);
    pwmout_pulsewidth_us(&pwm_g, PWM_PERIOD / 3);
    pwmout_pulsewidth_us(&pwm_b, PWM_PERIOD / 4);
  }
}

void pwm_init() {
  pwmout_init(&pwm_r, PWM_R);
  pwmout_init(&pwm_g, PWM_G);
  pwmout_init(&pwm_b, PWM_B);

  pwmout_period_us(&pwm_r, PWM_PERIOD);
  pwmout_period_us(&pwm_g, PWM_PERIOD);
  pwmout_period_us(&pwm_b, PWM_PERIOD);

  if (xTaskCreate(pwm_task, "pwm_task", PWM_STACK_SIZE, NULL, PWM_PRIORITY, NULL) != pdPASS) {
    printf("\r\n%s xTaskCreate(pwm_task) failed", __FUNCTION__);
  }
}

void main() {
  console_init();
  pwm_init();

  printf("Hello, iDo!\r\n");

  vTaskStartScheduler();
}
