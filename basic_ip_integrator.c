/********************************************************************************************
 * This file contains an example of using the GPIO driver to provide communication between
 * the Zynq Processing System (PS) and the AXI GPIO block implemented in the Zynq Programmable
 * Logic (PL). The AXI GPIO is connected to the LEDs on the Zybo.
 *
 * The provided code demonstrates how to use the GPIO driver to write to the memory mapped AXI
 * GPIO block, which in turn controls the LEDs.
 ********************************************************************************************/

/* Include Files */
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"

/* Definitions */
#define LEDS_DEVICE_ID  XPAR_AXI_GPIO_0_DEVICE_ID	/* GPIO device that LEDs are connected to */
#define BTNS_DEVICE_ID  XPAR_AXI_GPIO_1_DEVICE_ID	/* GPIO device that btns are connected to */
#define LED 0x9										/* Initial LED value - X00X */
#define BTN 0x9										/* Initial LED value - X00X */
#define LED_DELAY 100000000							/* Software delay length */
#define CHANNEL 1								/* GPIO port for LEDs */
#define printf xil_printf							/* smaller, optimised printf */

XGpio Gpio;											/* GPIO Device driver instance */
XGpio LEDInst;
XGpio BTNInst;

int LEDOutputExample(void)
{

	volatile int Delay;
	int status; // Initialize 'status' variable which is used to check the button status
	int led = LED; /* Hold current LED value. Initialise to LED definition */
	led = 0; // set LED value equal to 0
	int btn_read = 0x0; //initialize btn_read variable to 0
	int previous = 0; // Initialize 'previous' variable which is used to check if buttons 3 and 0 was depressed at the same time

		/* GPIO driver initialisation */
		// Initialize LEDs
		status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
		if (status != XST_SUCCESS){
			return XST_FAILURE;
		}
		// Initialize Push Buttons
		status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
		if (status != XST_SUCCESS){
			return XST_FAILURE;
		}
		// Set LEDs direction to outputs
		XGpio_SetDataDirection(&LEDInst, 1, 0x00);
		// Set buttons direction to inputs
		XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

		while(1){
			// Set btn_read to current button number
			btn_read = XGpio_DiscreteRead(&BTNInst, CHANNEL);

			//start switch case statements
			switch(btn_read){

			// Case when buttons 0 and 3 are depressed
			case(0x9):
			    previous = 9;
				XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
				if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x9){
					break;
				}
				break;

			// Case when button 3 is released and button 0 is still depressed
			case(0x1):
				if (previous == 9){
					for(int i = 0; i<= 15; i++){
						if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x1){
							break;
						}

						if(led>15){
							led = 0;
						}
						XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
						led = led+2;
						for (Delay = 0; Delay < LED_DELAY; Delay++);
					}
				}
				break;

			// Case when button 0 is released and button 3 is still depressed
			case(0x8):
				if (previous == 9){
					for(int j = 0; j<= 15; j++){
						if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x8){
							break;
						}

						if(led>15){
							led = 0;
						}
						XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
						led = led+4;
						for (Delay = 0; Delay < LED_DELAY; Delay++);
					}
				}
				break;

			// Case when button 1 is depressed
			case(0x2):
				led = 0;
				XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
				if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x2){
					break;
				}
				break;

			// Case when button 2 is depressed
			case(0x4):
				led = 15;
				XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
				if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x4){
					XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
					break;
				}
				break;

			default:
				for(int i=0; i<=15;i++){
					if (XGpio_DiscreteRead(&BTNInst, CHANNEL) != 0x0){
						led = led-1;
						break;
					}
					previous = 0;
					XGpio_DiscreteWrite(&LEDInst, CHANNEL, led);
					led = led +1;
					if (led>15){
						led = 0;
					}
					for (Delay = 0; Delay < LED_DELAY; Delay++);
				}
			}
		}


		return XST_SUCCESS; /* Should be unreachable */
}

/* Main function. */
int main(void){

	int status;

	/* Execute the LED output. */
	status = LEDOutputExample();
	if (status != XST_SUCCESS) {
		xil_printf("GPIO output to the LEDs failed!\r\n");
	}

	return 0;
}

