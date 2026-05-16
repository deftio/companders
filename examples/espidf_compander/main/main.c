/**
 * ESP-IDF example for companders library
 *
 * Demonstrates A-Law / Mu-Law companding with ADC input and IIR DC offset
 * correction on an ESP32.
 *
 * Wiring:
 *   Audio input capacitively coupled to GPIO34 (ADC1_CH6) with a resistor
 *   divider biasing the pin to ~VCC/2:
 *
 *       +3.3V
 *         |
 *         R  (10k)
 *         |
 *  GPIO34 <--------C---- audio_input
 *         |
 *         R  (10k)
 *         |
 *        GND
 *
 * Build:
 *   idf.py build
 *   idf.py flash monitor
 *
 * Copyright (c) 2001-2024, M. A. Chatterjee <deftio at deftio dot com>
 * BSD 2-Clause License (see LICENSE.txt)
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "companders.h"

static const char *TAG = "compander";

// ADC configuration
#define ADC_CHANNEL     ADC_CHANNEL_6    // GPIO34
#define ADC_ATTEN       ADC_ATTEN_DB_12  // 0-3.3V range
#define ADC_WIDTH       ADC_BITWIDTH_12  // 12-bit resolution (0..4095)

// IIR averager configuration
// Window = 2^8 = 256 samples, radix = 6 bits fractional precision
#define IIR_WINDOW_BITS  8
#define IIR_RADIX        6

// Sampling
#define SAMPLE_RATE_HZ   8000
#define SAMPLE_PERIOD_MS (1000 / SAMPLE_RATE_HZ)  // ~0.125 ms per sample

static void compander_task(void *arg)
{
    // Configure ADC
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_WIDTH,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &chan_cfg));

    // Seed DC bias estimate at midpoint of 12-bit ADC range
    DIO_s32 dcBias = DIO_I2FR(2048, IIR_RADIX);
    int sampleCount = 0;

    ESP_LOGI(TAG, "Companders ESP-IDF Example");
    ESP_LOGI(TAG, "Library version: %s (0x%06X)", DIO_COMPANDERS_VER_STRING, DIO_COMPANDERS_VER_HEX);
    ESP_LOGI(TAG, "sample, raw_adc, dc_bias, corrected, alaw, ulaw");

    while (1) {
        int rawAdc = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL, &rawAdc));

        // Update IIR DC bias estimate
        dcBias = DIO_IIRavgPower2FR(dcBias, IIR_WINDOW_BITS, (DIO_s16)rawAdc, IIR_RADIX);

        // Subtract DC bias to get signed, zero-centered sample
        DIO_s16 corrected = (DIO_s16)(rawAdc - DIO_FR2I(dcBias, IIR_RADIX));

        // Scale 12-bit corrected value to 16-bit range for compander
        // (-2048..2047) -> (-32768..32752)
        DIO_s16 scaled = corrected << 4;

        // Compress with both A-Law and Mu-Law
        DIO_s8 alawValue = DIO_LinearToALaw(scaled);
        DIO_s8 ulawValue = DIO_LinearToULaw(scaled);

        // Log every 800th sample (~10 lines/sec at 8 kHz)
        if ((sampleCount % 800) == 0) {
            ESP_LOGI(TAG, "%d, %d, %d, %d, %d, %d",
                     sampleCount, rawAdc,
                     (int)DIO_FR2I(dcBias, IIR_RADIX),
                     (int)corrected, (int)alawValue, (int)ulawValue);
        }
        sampleCount++;

        // Approximate sample rate via task delay (for demo purposes;
        // production code should use a hardware timer or I2S DMA)
        vTaskDelay(1);
    }
}

void app_main(void)
{
    xTaskCreate(compander_task, "compander", 4096, NULL, 5, NULL);
}
