/**
 * Template File for Pre ABET Assignment
 * See Class notes for explicit design requirements
 */
#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


// Your Defines Here

/*  This is the last define */
#define c64K 0x10000
#define seed 'C'    // Your SEED value here
// Your global variables Here
static char base64Table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                             '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


// Last Global Variables
uint32_t ii;
char my1st16chars[16];
uint32_t  ABETClockCycles;
uint16_t  numberOfEncodedChars;
Timer_B_initContinuousModeParam myTimerB;

// Function ProtoTypes Here

uint8_t myABETData(uint8_t,uint8_t*);

void main(void)
{
// DO NOT MODIFY THIS CODE.  Your Code goes below.
    // No C Source Code above this line
 	ABETClockCycles = 0;
 	WDT_A_hold(WDT_A_BASE);   // Stop WDT Timer

// Timer B is used to estimate clock cycles for your program
// Therefore,  you cannot use Timer B in your design

   myTimerB.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
   myTimerB.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
   myTimerB.timerClear = TIMER_B_DO_CLEAR;
   myTimerB.startTimer = false;

   Timer_B_initContinuousMode(TIMER_B0_BASE,&myTimerB);
   Timer_B_selectCounterLength(TIMER_B0_BASE,TIMER_B_COUNTER_16BIT);
   Timer_B_clearTimerInterrupt(TIMER_B0_BASE);
   Timer_B_enableInterrupt(TIMER_B0_BASE);
   Timer_B_startCounter(TIMER_B0_BASE,TIMER_B_CONTINUOUS_MODE);


    _bis_SR_register(GIE);        // Enable global interrupts
//*******************************************************************************
// No Code above this line

// Your code goes here including declaration of local variables

    //initialize variables
    uint8_t data, prevData, i = 0, j = 0, count = 0, x = 0;

    //temp arrays
    char tempArray[4] = {'=', '=', '=', '='};
    unsigned char encodedChars[16];

    //initialize seed
    myABETData(seed, &data);
    myABETData(1, &data);

    while(data != 0){
        count++;

        //get encoded characters into temp array
        if(i == 0){
            tempArray[i] = base64Table[(data >> 2)];
            tempArray[i + 1] = base64Table[((data << 4) & 0b001100000)];
            count++;
        }
        else if(i == 1){
            tempArray[i] = base64Table[(((prevData << 4) & 0b00110000) | data >> 4)];
            tempArray[i + 1] = base64Table[((data << 2) & 0b00111100)];
        }
        else if(i == 2){
            tempArray[i] = base64Table[(((prevData << 2) & 0b00111100) | (data >> 6))];
            tempArray[i + 1] = base64Table[(data & 0b00111111)];
        }

        i++;

        //put the four encoded characters into array
        if(i == 3 && j < 16){
            for(x = 0; x < 4; x++){
                encodedChars[j++] = tempArray[x];
            }
        }

        //reset i and tempArray
        if(i == 3){
            i = 0;
            for(x = 0; x < 4; x++){
                tempArray[x] = '=';
            }
        }

        //store previous character
        prevData = data;

        //get new character
        myABETData(1, &data);
    }

    //get any remaining chars if there were less than 12 random chars
    if(count < 16){
        for(x = 0; x < 4; x++){
            encodedChars[j++] = tempArray[x];
        }
    }

	numberOfEncodedChars = count;   // Replace 0 with your count
	sprintf(my1st16chars,"%s", encodedChars);  // Replace with your 1st 16 chars


//  Do NOT add any statements below this line
//  Turn OFF Timer and disable interrupts
    Timer_B_stop(TIMER_B0_BASE);
// Add current counter value to ABET Clock Cycles
    ABETClockCycles += Timer_B_getCounterValue(TIMER_B0_BASE);

//  Include a screen capture of the below in your final report
//  Replace First Name Last Name with your name.

    puts("Kyle Crawford");
    puts("Partner: Rotchy Moricette");
    puts("Fall 2020 Semester");
    puts("PreABET Assignment");
    printf("Seed is %c,0x%x,%d\n",seed,seed,seed);
    printf("# of Encoded Characters 0x%04x\n",numberOfEncodedChars);
    printf("My first 16 encoded characters are %s:\n", my1st16chars);
    printf("My ABET Clock Cycles are %#lx\n",ABETClockCycles);


    while(1);   // SPIN Loop

}
/* Place any local functions here */


/* This should be the last function */
/* Do not change or remove this code */
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMERB0(void){

    ABETClockCycles += c64K;
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);
}


