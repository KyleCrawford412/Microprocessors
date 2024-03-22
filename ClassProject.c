/*
 * ----------------------------------------
 *  EEL-4647L Class Project
 *  Fall 2020
 *  Carolina Hau Loo
 *  Rotchy Moricette
 *  Kyle Crawford
 * ----------------------------------------
 */

TEST LINE

//Include files
#include "driverlib.h"
#include <stdint.h>

//global variables
volatile uint8_t motorSeq, PBS1, PBS2;
typedef enum {CW,CCW,off} MOTORMODES;
MOTORMODES motorMode;
typedef enum {tA,tB,tC,tD} TONES;
TONES tones = tA;
enum {noDebug, debugCW, debugCCW, debugSeq} debugMode;

//Define Structure for UP parameters
Timer_B_initUpModeParam MyTimerB;

void configIO();
void myMotorDriver();
void myMotorController();
void sequenceFunction();
void getTone();

void main (void)
{
    // Define local variables for Timer Params
    Timer_A_initUpModeParam myTimerAParams;

    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    configIO();

    motorSeq = 0;

    //Configure Timer A Parameters
    myTimerAParams.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    myTimerAParams.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_20;
    myTimerAParams.timerPeriod = 15000;
    myTimerAParams.startTimer = false;
    myTimerAParams.timerClear = TIMER_A_DO_CLEAR;

    // Call Timer A0 Initialization
    Timer_A_initUpMode(TIMER_A0_BASE, &myTimerAParams);

    // Start Timer A0
    Timer_A_enableInterrupt(TIMER_A0_BASE);     //enable timer interrupt
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE);
    __enable_interrupt();                       //enable global interrupt

    while(1){

        PBS2 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3);
        PBS1 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2);

        if (PBS1 == GPIO_INPUT_PIN_LOW && PBS2 == GPIO_INPUT_PIN_HIGH){  //Reverse direction (CCW)?
            debugMode = debugCCW;
        }
        else if (PBS1 == GPIO_INPUT_PIN_HIGH && PBS2 == GPIO_INPUT_PIN_LOW){   //Forward direction (CW)?
            debugMode = debugCW;
        }
        else if (PBS1 == GPIO_INPUT_PIN_LOW && PBS2 == GPIO_INPUT_PIN_LOW) {   //Debug Sequence
            debugMode = debugSeq;
        }
        else {
            debugMode = noDebug;
            //GET ADC VALUE
            //SET tones to the correct value
            getTone();
        }
    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void motorISR(){

    if (debugMode != noDebug){
        switch (debugMode){
            case debugCW:
                GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);

                motorMode = CW;
                myMotorController();
                break;
            case debugCCW:
                GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN1);

                motorMode = CCW;
                myMotorController();
                break;
            case debugSeq:
                sequenceFunction();
                break;
        }
    }
    else {
        switch (tones){
            case tA:
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);

                motorMode = off;
                myMotorController();
                break;
            case tB:
                GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
                motorMode = CW;
                myMotorController();
                break;
            case tC:
                GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN1);
                motorMode = CCW;
                myMotorController();
                break;
            case tD:
                sequenceFunction();
                break;
        }
    }

    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);     //reset interrupt flag
}


/* configIO
* Function to configure the port I/O
* for all the LEDS and Pushbuttons
* Inputs: None
* Return Values: None
* Author: Carolina Hau Loo
* Date Written: 11/06/2020 */
void configIO(void) {

    //Launchpad RED LED1 P1.0
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN7);    //To mirror LED1
    //Launchpad GREEN LED2 P1.1
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN1);    //To toggle in standby mode
    //MKII RGB LED (RED) P3.6
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN6);
    //MKII RBG LED (GREEN) P3.5
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN5);
    //MKII RGB LED (BLUE) P3.4
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN4);
    //MKII PBS1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN2);
    //MKII PBS2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN3);

    //MKII Mic
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1, GPIO_PIN3, GPIO_TERNARY_MODULE_FUNCTION);

    //Set all pins LOW
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);

    //Unlock PMM-LPM5 to Active Configuration
    PMM_unlockLPM5();
}

/* myMotorDriver()
* Function implement the motor sequence
* Inputs: None
* Return Values: None
* Author: Carolina Hau Loo
* Date Written: 11/06/2020 */

void myMotorDriver(void) {

    switch (motorSeq){      //global variable
        case 0:
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad ON (mirror)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);     //RED MKII  OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    OFF
            break;
        case 1:
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad ON (mirror)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII OFF
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  ON
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    OFF
            break;
        case 2:
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad OFF (mirror)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII OFF
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  ON
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    OFF
            break;
        case 3:
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad OFF (mirror)
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII ON
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  ON
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    OFF
            break;
        case 4:
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad OFF (mirror)
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII ON
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    OFF
            break;
        case 5:
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad OFF (mirror)
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII ON
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  OFF
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    ON
            break;
        case 6:
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad OFF (mirror)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  OFF
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    ON
            break;
        case 7:
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);    //RED Launchpad ON (mirror)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN4);     //BLUE MKII OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);    //RED MKII  OFF
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN5);     //GREEN MKII    ON
            break;
    }
}

/* myMotorController()
* Function to control the motor rotation
* Inputs: None
* Return Values: None
* Author: Carolina Hau Loo
* Date Written: 11/20/2020 */

void myMotorController(void) {

    if (motorMode == CW){
        myMotorDriver();
        motorSeq++;
        if(motorSeq == 8){ motorSeq = 0; }
    }
    else if (motorMode == CCW){
        myMotorDriver();
        if(motorSeq == 0){ motorSeq = 8; }
        motorSeq--;
    }
    else if (motorMode == off){
        __delay_cycles(1000000);         //1Hz delay
        GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN1);   //toggle LED2 (GREEN LED)
    }
}

/* sequenceFunction()
* Function to execute the pattern
* Inputs: None
* Return Values: None
* Author: Carolina Hau Loo
* Date Written: 11/20/2020 */
void sequenceFunction(){

    MOTORMODES toneSeq[9] = {CW, CW, CW, CCW, CW, CCW, CW, CCW, CW};
    uint8_t seqNum[9] = {67, 67, 33, 83, 50, 83, 100, 100, 50};
    uint8_t x, i; //variables for the for loops

    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);

    for (x = 0; x<9; x ++){
        motorMode = toneSeq[x];
        for (i = 0; i < seqNum[x]; i++){
            myMotorController();
            __delay_cycles(20000); //delay of 2ms
        }
           __delay_cycles(250000); //delay for 0.25s
    }
}

/* getTone()
* Function to determine which key tone is being played into microphone and set tones variable
* Inputs: None
* Return Values: None
* Author: Kyle Crawford
* Date Written: 11/22/2020 */

void getTone(){

    //get average of inputs
    int16_t avg = 0;
    uint8_t i;

    //don't include index zero -- weird readings
    for(i = 1; i < 256; i++){
        avg += input[i];
    }

    avg = avg / 255;

    //check for relevant tones
    if((input[145] > avg*15) || (input[146] > avg*15) || (input[147] > avg*15)){
        //check for tone A
        //may need to lower avg multiplier to get cases to work
        if((input[68] > avg*10) || (input[69] > avg*10) || (input[70] > avg*10)){
            tones = tA;
        }
        //check for tone B
        else if((input[76] > avg*10) || (input[77] > avg*10) || (input[78] > avg*10)){
            tones = tB;
        }
        //check for tone C
        else if((input[84] > avg*10) || (input[85] > avg*10) || (input[86] > avg*10)){
            tones = tC;
        }
        //check for tone D
        else if((input[93] > avg*10) || (input[94] > avg*10) || (input[95] > avg*10)){
            tones = tD;
        }
    }

}
