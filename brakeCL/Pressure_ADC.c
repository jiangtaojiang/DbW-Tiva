#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "Globals_and_Defines.h"


//Function to read the brake pressure sensor
uint32_t get_brake_pressure(int select)
{
	uint32_t ADC_value[3];
	ADCIntClear(ADC0_BASE, 1);
	
	//Start conversion sequence
	ADCProcessorTrigger(ADC0_BASE, 1);
	while(!ADCIntStatus(ADC0_BASE, 1, false))
	{
	}
	//Get the ADC value
	ADCSequenceDataGet(ADC0_BASE, 1, ADC_value);
	//1 B5 E-STOP
	//0 = D3 BPS
	if(select == 0||select == 1||select == 2)
		return ADC_value[select];
	else
		return 0;
}


void ADC0_Setup(void)
{
	//Enable PORTB
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
	{}
	//Enable PORTD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
	{}
	//Enable PORTE
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
	{}

	//Configure PORTE pin 3 for analog input
	//PD3 brake pressure sensor nput CH4
	//PB5 E-STOP input CH11
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);		
	//Enable ADC0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
	{}
	
	//Setup ADC0 sequencer 1 to read the throttle pedal input
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH4);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH11);
	ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END);

		ADCSequenceEnable(ADC0_BASE, 1);
}

