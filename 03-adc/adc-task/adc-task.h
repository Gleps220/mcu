#pragma once

extern int flagg ;
extern uint64_t PERIOD ;

void adc_task_init();
float adc_task_napruga();
float adc_task_temp();
void telemetry_handle();
void telemetry_state();
