#include "FreeRTOS.h"
#include "device.h"
#include <platform_opts.h>
#include <platform_stdlib.h>
#include "pwmout_api.h"

#define PWM_R PC_0
#define PWM_G PC_3
#define PWM_B PC_1
#define PWM_PERIOD 2000

pwmout_t pwm_r;
pwmout_t pwm_g;
pwmout_t pwm_b;

void main() {
  console_init();

  printf("Hello, iDo!\r\n");

  pwmout_init(&pwm_r, PWM_R);
  pwmout_init(&pwm_g, PWM_G);
  pwmout_init(&pwm_b, PWM_B);

  pwmout_period_us(&pwm_r, PWM_PERIOD);
  pwmout_period_us(&pwm_g, PWM_PERIOD);
  pwmout_period_us(&pwm_b, PWM_PERIOD);

  pwmout_pulsewidth_us(&pwm_r, PWM_PERIOD / 2);
  pwmout_pulsewidth_us(&pwm_g, PWM_PERIOD / 3);
  pwmout_pulsewidth_us(&pwm_b, PWM_PERIOD / 4);

  vTaskStartScheduler();
}
