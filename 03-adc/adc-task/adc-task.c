#include "hardware/adc.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"


const uint LED_PIN_Volts = 26;
const uint CHANNAL_Temp = 4;
const uint CHANNAL_Volts = 0;
uint64_t ts;
int flagg = 0;
uint64_t PERIOD = 100000;



void adc_task_init(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(LED_PIN_Volts);
}


float adc_task_napruga(){
    adc_select_input(CHANNAL_Volts);
    uint16_t voltage_counts = adc_read();
    float Voltage = ((float)voltage_counts / 4096.0f) * 3.3f;
    return Voltage;

}

float adc_task_temp(){
    adc_select_input(CHANNAL_Temp);
    uint16_t temp_counts = adc_read();
    float temp_V = ((float)temp_counts / 4096.0f) * 3.3f;
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
    return temp_C;
}

void telemetry_handle(){
    if (flagg == 1){
    uint64_t ts = time_us_64();
        if (time_us_64()>ts){
        float voltage_V = adc_task_napruga();
        float temp_C = adc_task_temp();
        printf("%f %f\n", voltage_V, temp_C);
        ts = time_us_64() + PERIOD;}
    }
    if (flagg == 0){
        return;
        }
}
