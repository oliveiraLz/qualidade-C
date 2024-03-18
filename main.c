#include <stdio.h>
#include <stdint.h>
#include "esp_system.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"

#define DEFAULT_VREF 1100
#define NO_OF_SAMPLES 64

// Define o pino ao qual o sensor está conectado
#define MQ135_PIN 34

/*
 * 1. Include de Bibliotecas: Inclui as bibliotecas necessárias para o código.
 * 2. Definição de Constantes: Define constantes como DEFAULT_VREF e NO_OF_SAMPLES.
 * 3. Configuração do Pino do Sensor: Configura o pino do sensor MQ-135 como uma entrada analógica.
 * 4. Caracterização do ADC: Caracteriza o ADC do ESP32 com as configurações especificadas.
 * 5. Loop Infinito: Inicia um loop infinito para leitura contínua do sensor e cálculo da concentração de CO2.
 */
void app_main()
{
    // Configura o pino do sensor como entrada analógica
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    // Caracteriza o ADC
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    /*
     * 5. Loop Infinito (continuação): Realiza a leitura do sensor e o cálculo da concentração de CO2.
     */
    while (1)
    {
        uint32_t adc_reading = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
            adc_reading += adc1_get_raw(ADC1_CHANNEL_6);
        }
        adc_reading /= NO_OF_SAMPLES;

        // Converte a leitura bruta do ADC em tensão
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);

        // Converte a tensão em concentração de CO2 em ppm
        float ppm = (116.6020682 * (1.0 / voltage) - 0.001665485) / 0.0002270381;

        printf("Concentracao de CO2: %.2f ppm\n", ppm);

        // Adiciona um atraso de 1 segundo antes da próxima leitura
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
