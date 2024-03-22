//include files
#include "driverlib.h"
#include <stdint.h>

//define structure for UP parameters
Timer_A_initUpModeParam MyTimerA;

//Function: configTimerA()
//configuration parameters for TimerA
//input parameters: none
//output parameters: none

void configTimerA(uint16_t delayValue, uint16_t clockDividerValue){
    MyTimerA.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    MyTimerA.clockSourceDivider = clockDividerValue;
    MyTimerA.timerPeriod = delayValue;
    MyTimerA.timerClear = TIMER_ADO_CLEAR;
    MyTimerA.startTimer = false;
}

//myTimerADelay
//Hardware Timer Delay function using Timer A
//Time delay = delayValue*SMCLK/clockDividerValue
//Inputs:
//  delayValue, uint16_t, number of count cycles
//  clockDividerValue, uint16_t, clock divider
//Returns: none

void myTimerADelay(uint16_t delayValue, uint16_t clockDividerValue){
    //configure the timer parameters
    configTimerA(delayValue, clockDividerValue);

    //initialize the timer
    Timer_A_initUpMode(TIMER_A0_BASE, &MyTimerA);

    //start timer
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    //wait for TAIFG to become set
    while((TA0CLT&TAIFG) == 0);
    Timer_A_stop(TIMER_A0_BASE);     //stop timer

    //reset TAIFG to zero
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

}
