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

// Test Config
#define TEST_STACK_SIZE 256
#define TEST_PRIORITY tskIDLE_PRIORITY + 2

// PWM Config
#define PWM_R PC_1
#define PWM_G PC_3
#define PWM_B PC_0
#define PWM_PERIOD 2000 // 500 Hz

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

void rgb_led_set(struct Color clr) {
  int red = (clr.r * PWM_PERIOD) / 255;
  int green = (clr.g * PWM_PERIOD) / 255;
  int blue = (clr.b * PWM_PERIOD) / 255;
  pwmout_pulsewidth_us(&pwm_r, red);
  pwmout_pulsewidth_us(&pwm_g, green);
  pwmout_pulsewidth_us(&pwm_b, blue);
}

void rgb_led_init() {
  pwmout_init(&pwm_r, PWM_R);
  pwmout_init(&pwm_g, PWM_G);
  pwmout_init(&pwm_b, PWM_B);

  pwmout_period_us(&pwm_r, PWM_PERIOD);
  pwmout_period_us(&pwm_g, PWM_PERIOD);
  pwmout_period_us(&pwm_b, PWM_PERIOD);

  pwmout_pulsewidth_us(&pwm_r, 0);
  pwmout_pulsewidth_us(&pwm_g, 0);
  pwmout_pulsewidth_us(&pwm_b, 0);
}

void test_task(void* pvParameters) {
  const TickType_t delay = 10 / portTICK_PERIOD_MS;

  printf("%s delay = %d\r\n", __FUNCTION__, 10 / portTICK_PERIOD_MS);

  struct Color clr;
  clr.r = 255;
  clr.g = 0;
  clr.b = 0;

  while (1) {
    if (clr.r > 0) {
      --clr.r;
      if (clr.r == 0) {
        clr.g = 255;
      }
    } else if (clr.g > 0) {
      --clr.g;
      if (clr.g == 0) {
        clr.b = 255;
      }
    } else if (clr.b > 0) {
      --clr.b;
      if (clr.b == 0) {
        clr.r = 255;
      }
    } else {
      printf("%s WAT. All colors were 0 in test_task!\r\n", __FUNCTION__);
      clr.r = 255;
      clr.g = 0;
      clr.b = 0;
    }

    rgb_led_set(clr);
    vTaskDelay(delay);
  }
}

void test_init() {
  if (xTaskCreate(test_task, "test_task", TEST_STACK_SIZE, NULL, TEST_PRIORITY, NULL) != pdPASS) {
    printf("%s xTaskCreate(test_task) failed\r\n", __FUNCTION__);
  } else {
    printf("%s created test_task\r\n", __FUNCTION__);
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
        printf("%s DHCP failed\r\n", __FUNCTION__);
        return RTW_ERROR;
      }
    }
  }
}

void wlan_init_thread(void* params) {
  LwIP_Init();
  if (wifi_manager_init() != RTW_SUCCESS) {
    printf("%s wifi_manager_init() failed\r\n", __FUNCTION__);
  }
  if (wifi_on(RTW_MODE_STA) != RTW_SUCCESS) {
    printf("%s wifi_on() failed\r\n", __FUNCTION__);
  }
  if (wifi_set_autoreconnect(1) != RTW_SUCCESS) {
    printf("%s wifi_set_autoreconnect() failed\r\n", __FUNCTION__);
  }
  if (wlan_connect() != RTW_SUCCESS) {
    printf("%s wifi_connect() failed\r\n", __FUNCTION__);
  }
  vTaskDelete(NULL);
}

void wlan_init() {
  if (xTaskCreate(wlan_init_thread, ((const char*)"wlan_init_thread"), WLAN_INIT_STACK_SIZE, NULL, WLAN_INIT_PRIORITY, NULL) != pdPASS) {
    printf("%s xTaskCreate(wlan_init_thread) failed\r\n", __FUNCTION__);
  }
}

void main() {
  printf("iDo is booting!\r\n");

  /*wlan_init();*/

  rgb_led_init();
  printf("Initialized RGB LED.\r\n");
  struct Color clr;
  clr.r = 128;
  clr.g = 30;
  clr.b = 150;
  rgb_led_set(clr);
  printf("Set initial color to <%d, %d, %d>\r\n", clr.r, clr.g, clr.b);

  // Run test mode. Comment out for production.
  test_init();

  printf("Initializing console and starting scheduler.\r\n");
  console_init();
  vTaskStartScheduler();
}
