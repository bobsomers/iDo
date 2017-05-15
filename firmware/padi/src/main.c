#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "device.h"
#include <platform_opts.h>
#include <platform_stdlib.h>
#include "gpio_api.h"
#include "pwmout_api.h"
#include "lwip_netconf.h"
#include "wifi_conf.h"
#include "main.h"

// Test Config
#define TEST_STACK_SIZE 256
#define TEST_PRIORITY tskIDLE_PRIORITY + 1

// Status Config
#define STATUS_RED_LED_PIN PA_3
#define STATUS_ORANGE_LED_PIN PA_5
#define STATUS_STACK_SIZE 256
#define STATUS_PRIORITY tskIDLE_PRIORITY + 2
#define STATUS_POWERED 1
#define STATUS_CONNECTED 2

// PWM Config
#define PWM_R PC_1
#define PWM_G PC_3
#define PWM_B PC_0
#define PWM_PERIOD 2000 // 500 Hz

// WLAN Config
#define WLAN_SSID "iDoNetwork"
#define WLAN_PASS "wildflower"
#define WLAN_CHANNEL 6
#define WLAN_INIT_STACK_SIZE (512 + 768)
#define WLAN_INIT_PRIORITY tskIDLE_PRIORITY + 3

// Listener Config
#define LISTENER_STACK_SIZE WLAN_INIT_STACK_SIZE
#define LISTENER_PRIORITY tskIDLE_PRIORITY + 2
#define LISTENER_PORT 12345

// Identification Config
#define UNIT_NUMBER 0

struct Color {
  u8_t r;
  u8_t g;
  u8_t b;
};

pwmout_t pwm_r;
pwmout_t pwm_g;
pwmout_t pwm_b;

volatile int current_status = STATUS_POWERED;
gpio_t status_red_led;
gpio_t status_orange_led;

struct udp_pcb* listener_pcb;

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

void status_task(void* pvParameters) {
  const TickType_t short_delay = 100 / portTICK_PERIOD_MS;
  const TickType_t long_delay = 5000 / portTICK_PERIOD_MS;

  while (1) {
    gpio_write(&status_red_led, 1);
    vTaskDelay(short_delay);
    gpio_write(&status_red_led, 0);

    if (current_status == STATUS_CONNECTED) {
      gpio_write(&status_orange_led, 1);
      vTaskDelay(short_delay);
      gpio_write(&status_orange_led, 0);
    }

    vTaskDelay(long_delay);
  }
}

void status_init() {
  gpio_init(&status_red_led, STATUS_RED_LED_PIN);
  gpio_dir(&status_red_led, PIN_OUTPUT);
  gpio_mode(&status_red_led, PullNone);
  gpio_write(&status_red_led, 0);

  gpio_init(&status_orange_led, STATUS_ORANGE_LED_PIN);
  gpio_dir(&status_orange_led, PIN_OUTPUT);
  gpio_mode(&status_orange_led, PullNone);
  gpio_write(&status_orange_led, 0);

  if (xTaskCreate(status_task, "status_task", STATUS_STACK_SIZE, NULL, STATUS_PRIORITY, NULL) != pdPASS) {
    printf("%s xTaskCreate(status_task) failed\r\n", __FUNCTION__);
  } else {
    printf("%s created status_task\r\n", __FUNCTION__);
  }
}

void listener_recv(void* arg, struct udp_pcb* pcb, struct pbuf* buf, struct ip_addr* addr, u16_t port) {
  if (buf != NULL) {
    if (buf->tot_len == 78) { // 78 bytes is RGB (3 bytes) for 26 channels
      u8_t* data = (u8_t*)(buf->payload);
      struct Color clr;
      clr.r = data[UNIT_NUMBER * 3 + 0];
      clr.g = data[UNIT_NUMBER * 3 + 1];
      clr.b = data[UNIT_NUMBER * 3 + 2];
      rgb_led_set(clr);
    }
    pbuf_free(buf);
  }
}

void listener_init() {
  listener_pcb = udp_new();
  if (udp_bind(listener_pcb, IP_ADDR_ANY, LISTENER_PORT) != ERR_OK) {
    printf("%s Failed to bind UDP socket to addr/port!\r\n", __FUNCTION__);
    return;
  }
  udp_recv(listener_pcb, listener_recv, NULL);
  printf("%s UDP socket bound to port %d and listening!\r\n", __FUNCTION__, LISTENER_PORT);

  //if (xTaskCreate(udp_task), "udp_task", UDP_STACK_SIZE, NULL, UDP_PRIORITY, NULL) != pdPass) {
  //  printf("%s xTaskCreate(udp_task) failed.\r\n", __FUNCTION__);
  //} else {
  //  printf("%s created udp_task\r\n", __FUNCTION__);
  //}
}

int wlan_connect() {
  int ret = RTW_ERROR;
  int ssid_len = strlen(WLAN_SSID);
  int pw_len = strlen(WLAN_PASS);
  rtw_wifi_setting_t setting;

  while (1) {
    printf("%s Disconnecting just in case.\r\n", __FUNCTION__);
    wifi_disconnect();

    printf("%s Attempting partial scan.\r\n", __FUNCTION__);
    uint8_t channel = WLAN_CHANNEL;
    uint8_t pscan_config = PSCAN_ENABLE | PSCAN_SIMPLE_CONFIG;
    wifi_set_pscan_chan(&channel, &pscan_config, 1);

    printf("%s Attempting to connect.\r\n", __FUNCTION__);
    ret = wifi_connect(WLAN_SSID,
                       RTW_SECURITY_WPA2_AES_PSK,
                       WLAN_PASS,
                       ssid_len,
                       pw_len,
                       0,
                       NULL);

    if (ret == RTW_SUCCESS) {
      printf("%s Connection succeeded, attempting DHCP.\r\n", __FUNCTION__);
      ret = LwIP_DHCP(0, DHCP_START);
      wifi_get_setting(WLAN0_NAME, &setting);
      wifi_show_setting(WLAN0_NAME, &setting);
      if (ret == DHCP_ADDRESS_ASSIGNED) {
        printf("%s DHCP succeeded.\r\n", __FUNCTION__);
        return RTW_SUCCESS;
      } else {
        printf("%s DHCP failed.\r\n", __FUNCTION__);
        return RTW_ERROR;
      }
    } else {
      printf("%s Connection failed.\r\n", __FUNCTION__);
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
  } else {
    current_status = STATUS_CONNECTED;
    listener_init();
  }
  vTaskDelete(NULL);
}

void wlan_init() {
  if (xTaskCreate(wlan_init_thread, "wlan_init_thread", WLAN_INIT_STACK_SIZE, NULL, WLAN_INIT_PRIORITY, NULL) != pdPASS) {
    printf("%s xTaskCreate(wlan_init_thread) failed\r\n", __FUNCTION__);
  }
}

void main() {
  printf("iDo is booting!\r\n");

  // Initialize status LEDs.
  status_init();

  // Initialize RGB LED.
  rgb_led_init();
  printf("Initialized RGB LED.\r\n");
  struct Color clr;
  clr.r = 0;
  clr.g = 0;
  clr.b = 0;
  rgb_led_set(clr);
  printf("Set initial color to <%d, %d, %d>\r\n", clr.r, clr.g, clr.b);

  // Start Wi-Fi connection.
  wlan_init();

  // Test mode.
  //test_init();

  printf("Initializing console and starting scheduler.\r\n");
  console_init();
  vTaskStartScheduler();
}
