#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "Globals_and_Defines.h"

void Timer0A_Handler(void)
{
	g_tick_flag = true;
		// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer1A_Handler(void)
{
	BPS_Update_flag = true;
		// Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void Tick_Timer_Setup(void)
{
	uint32_t ui32Period;  //used for setting up timer

	//Enable clock for TIMER0 and configure for periodic interrupt
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //Enable clock for TIMER0
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
	{}
		
	//Enable clock for TIMER1 and configure for periodic interrupt
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //Enable clock for TIMER0
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1))
	{}
		
		
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  //Set Timer0 to periodic mode
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);  //Set Timer0 to periodic mode

	//Set the interrupt period  (How fast the interrupt will happen)
	ui32Period = (SysCtlClockGet()/Update_Rate);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

	//Set the interrupt period  (How fast the interrupt will happen)
	ui32Period = (SysCtlClockGet()/5000);
	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period - 1);
		
	//Set the priority for the Update timer as the highest priority 0
	IntPrioritySet(INT_TIMER0A, 0x00);
			//Set the priority for the Update timer as the highest priority 0
	IntPrioritySet(INT_TIMER1A, 0x40);
	
	//Enable TIMER0 interrupt in the module and the NVIC
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);   //Enable interrupt in the timer module
	IntEnable(INT_TIMER0A);  //Enable TIMER0 interrupt in the NVIC
	
		//Enable TIMER0 interrupt in the module and the NVIC
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);   //Enable interrupt in the timer module
	IntEnable(INT_TIMER1A);  //Enable TIMER0 interrupt in the NVIC
	
	//Start the timer
	TimerEnable(TIMER0_BASE, TIMER_A);
	
		//Start the timer
	TimerEnable(TIMER1_BASE, TIMER_A);
}



