#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "device.h"
#include <platform_opts.h>
#include <platform_stdlib.h>
#include "pwmout_api.h"
#include "lwip_netconf.h"
#include "wifi_conf.h"
#include "main.h"

// PWM Config
#define PWM_R PC_1
#define PWM_G PC_3
#define PWM_B PC_0
#define PWM_PERIOD 2000 // 500 Hz
#define PWM_STACK_SIZE 128
#define PWM_PRIORITY tskIDLE_PRIORITY + 1

// WLAN Config
#define WLAN_SSID ""
#define WLAN_PASS ""
#define WLAN_INIT_STACK_SIZE (512 + 768)
#define WLAN_INIT_PRIORITY tskIDLE_PRIORITY + 3

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

int wlan_connect() {
  int ret = RTW_ERROR;
  int ssid_len = strlen(WLAN_SSID);
  int pw_len = strlen(WLAN_PASS);
  rtw_wifi_setting_t setting;
  while (1) {
    // TODO: wifi_set_pscan_chan()???

    ret = wifi_connect(WLAN_SSID,
                       RTW_SECURITY_WPA2_AES_PSK,
                       WLAN_PASS,
                       ssid_len,
                       pw_len,
                       0,
                       NULL);

    if (ret == RTW_SUCCESS) {
      ret = LwIP_DHCP(0, DHCP_START);
      wifi_get_setting(WLAN0_NAME, &setting);
      wifi_show_setting(WLAN0_NAME, &setting);
      if (ret == DHCP_ADDRESS_ASSIGNED) {
        return RTW_SUCCESS;
      } else {
        printf("\r\n%s DHCP failed", __FUNCTION__);
        return RTW_ERROR;
      }
    }
  }
}

void wlan_init_thread(void* params) {
  LwIP_Init();
  if (wifi_manager_init() != RTW_SUCCESS) {
    printf("\r\n%s wifi_manager_init() failed", __FUNCTION__);
  }
  if (wifi_on(RTW_MODE_STA) != RTW_SUCCESS) {
    printf("\r\n%s wifi_on() failed", __FUNCTION__);
  }
  if (wifi_set_autoreconnect(1) != RTW_SUCCESS) {
    printf("\r\n%s wifi_set_autoreconnect() failed", __FUNCTION__);
  }
  if (wlan_connect() != RTW_SUCCESS) {
    printf("\r\n%s wifi_connect() failed", __FUNCTION__);
  }
  vTaskDelete(NULL);
}

void wlan_init() {
  if (xTaskCreate(wlan_init_thread, ((const char*)"wlan_init_thread"), WLAN_INIT_STACK_SIZE, NULL, WLAN_INIT_PRIORITY, NULL) != pdPASS) {
    printf("\r\n%s xTaskCreate(wlan_init_thread) failed", __FUNCTION__);
  }
}

void main() {
  printf("iDo is booting!\r\n");

  wlan_init();

  color_queue = xQueueCreate(4, sizeof(struct Color));
  if (color_queue == 0) {
    printf("Failed to create color_queue!\r\n");
  }
  printf("Created color queue.\r\n");

  pwm_init();
  printf("Initialized PWM.\r\n");

  struct Color clr;
  clr.r = 128;
  clr.g = 30;
  clr.b = 150;
  xQueueSend(color_queue, &clr, 0);

  printf("Set initial color to <%d, %d, %d>\r\n", clr.r, clr.g, clr.b);

  printf("Initializing console and starting scheduler.\r\n");
  console_init();
  vTaskStartScheduler();
}
