#include <stdio.h>
#include <stdint.h>
#include "esp_system.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"

#define DEFAULT_VREF 1100
#define NO_OF_SAMPLES 64

void app_main()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    while (1)
    {
        uint32_t adc_reading = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            adc_reading += adc1_get_raw(ADC1_CHANNEL_0);
        }
        adc_reading /= NO_OF_SAMPLES;

        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);

        float ppm = (116.6020682 * (1.0 / voltage) - 0.001665485) / 0.0002270381;

        printf("Concentracao de CO2: %.2f ppm\n", ppm);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
