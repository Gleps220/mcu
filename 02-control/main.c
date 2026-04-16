#include "pico/stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "hardware/gpio.h"


#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"


void led_blink_set_period_ms_callback(const char* args){
uint period_ms = 0;
sscanf(args, "%u", &period_ms);
if (period_ms == 0){
	printf("Error period_ms != 0");
	return;
}
else {
led_task_set_blink_period_ms(period_ms);
return;
}

}

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}


void led_on_callback(){
	led_task_state_set(LED_STATE_ON);
}
void led_off_callback(){
	led_task_state_set(LED_STATE_OFF);
}
void led_blink_callback(){
	led_task_state_set(LED_STATE_BLINK);
}


extern api_t device_api[];


void help_callback(){
	for (int i=0;device_api[i].command_name!=NULL; i++){
		printf("Command: %s : %s\n", device_api[i].command_name, device_api[i].command_help);

}
}

void mem_callback(const char* args){
unsigned int address = 0;

if (sscanf(args, "%x", &address) != 1)
{
	printf("Error: invalid address format\n");
	return;
    }
    
volatile unsigned int* ptr = (volatile unsigned int*)address;
unsigned int value = *ptr;
    
printf("Address 0x%08X = 0x%08X (%u)\n", address, value, value);
}


void wmem_callback(const char* args)
{
unsigned int address = 0;
unsigned int value = 0;
    
if (sscanf(args, "%x %x", &address, &value) != 2)
{
	printf("Error: need two hex arguments: wmem <address> <value>\n");
	return;
}
    
volatile unsigned int* ptr = (volatile unsigned int*)address;
*ptr = value;
    
printf("Written 0x%08X to address 0x%08X\n", value, address);
}


api_t device_api[] =
{	
	{"mem",mem_callback,"print value use your address"},
	{"wmem",wmem_callback,"write value use your address"},
	{"help",help_callback,"print commands description"},
	{"set_period",led_blink_set_period_ms_callback , "Change blinking period"},
	{"on", led_on_callback, "LED ON!"},
	{"version", version_callback, "get device name and firmware version"},
	{"off", led_off_callback, "LED OFF, NOoOW!"},
	{"blink", led_blink_callback, "LED BLINK, WOW!"},
	{NULL, NULL, NULL},
};



int main()
{
protocol_task_init(device_api);
stdio_init_all();
stdio_task_init();
led_task_init();



while (1)
{
protocol_task_handle(stdio_task_handle());
led_task_handle();

}
}

