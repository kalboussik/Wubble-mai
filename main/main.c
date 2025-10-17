
#include <stdio.h>
#include <esp_log.h>
#include "wur_sx1261.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_timer.h"
//#include "Nimble_adv.h"
//#include "wubble.h"

    // Use a RTC GPIO capable pin (like GPIO33)
    #define WAKEUP_GPIO_PIN_SEL  (1ULL << CONFIG_DIO1_GPIO)

    static const char *TAG = "DeepSleepWake";

    // RTC memory to retain value during deep sleep
    RTC_DATA_ATTR static int wake_counter = 0;

    void app_main(void)
    {    

        spi_bus_config_t spi_bus_config = {
        .sclk_io_num = PIN_SPI_SCLK,
        .mosi_io_num = PIN_SPI_MOSI,
        .miso_io_num = PIN_SPI_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
        };

        esp_err_t ret = spi_bus_initialize(USED_SPI_HOST, &spi_bus_config, SPI_DMA_CH_AUTO);
        if (ret != ESP_OK) {
            printf("Failed to initialize SPI bus: %s\n", esp_err_to_name(ret));
        }
        
        Listen_WUR();

        // Log reason for wakeup
        esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

        switch (wakeup_reason) {
            case ESP_SLEEP_WAKEUP_EXT1:
                wake_counter++;
                ESP_LOGI(TAG, "Woke up from EXT1 (GPIO) - Wake count: %d", wake_counter);
                break;
            case ESP_SLEEP_WAKEUP_UNDEFINED:
                ESP_LOGI(TAG, "Power-on reset");
                wake_counter = 0;  // reset counter on first boot
                break;
            default:
                ESP_LOGI(TAG, "Wakeup was not caused by EXT1. Reason: %d", wakeup_reason);
                break;
        }


        vTaskDelay(pdMS_TO_TICKS(10));  // simulate some task (10 ms)

        // Configure GPIO as wakeup source (EXT1)
        ESP_LOGI(TAG, "Configuring EXT1 wakeup on GPIO %d", CONFIG_DIO1_GPIO);

        // Configure GPIO as input (optional but clearer)
        gpio_set_direction(CONFIG_DIO1_GPIO, GPIO_MODE_INPUT);


        // EXT1: can wake up from multiple GPIOs, use bitmask
        ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup(WAKEUP_GPIO_PIN_SEL, ESP_EXT1_WAKEUP_ALL_LOW));

        ESP_LOGI(TAG, "Entering deep sleep now");
        esp_deep_sleep_start();
    }




    



