/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

//Kyle Crawford
//Assignment two
//Fall 2020
//11 AM section

//includes
#include "driverlib.h"
#include <stdio.h>
#include <stdint.h>

//defines
#define SEED 'z'
#define LED2PORT GPIO_PORT_P1
#define LED2PIN GPIO_PIN1

//function prototypes
void myData(uint8_t, int16_t*);

void main (void){

    //variables
    uint8_t numQ1 = 0, numQ2 = 0, numQ3 = 0, numQ4 = 0, numAxis = 0;
    int16_t s16bData;
    uint8_t aOUT[5];

    //stop WDT
    WDT_A_hold(WDT_A_BASE);

    //define I/O
    GPIO_setAsOutputPin(LED2PORT, LED2PIN);
    GPIO_setOutputLowOnPin(LED2PORT, LED2PIN);
    PMM_unlockLPM5();

    //initialize myData
    myData(SEED, &s16bData);

    //get random numbers into array
    myData(4, &s16bData);
    while(s16bData != 0){
        //check for first quadrant
        if((((s16bData & BITF) == 0) && ((s16bData & BIT7) == 0))){
            numQ4++;
        }
        //check for second quadrant
        else if(((s16bData & BITF) == BITF) && ((s16bData & BIT7) == 0)){
            numQ2++;
        }
        //check for third quadrant
        else if(((s16bData & BITF) == BITF) && ((s16bData & BIT7) == BIT7)){
            numQ3++;
        }
        //check for fourth quadrant
        else if(((s16bData & BITF) == 0) && ((s16bData & BIT7) == BIT7)){
            numQ1++;
        }
        //check for point on axis
        if(((s16bData << 8) == 0) || ((s16bData >> 8) == 0)){
            numAxis++;
        }
        myData(4, &s16bData);
    }

    //put values into array
    aOUT[0] = numAxis;
    aOUT[1] = numQ1;
    aOUT[2] = numQ2;
    aOUT[3] = numQ3;
    aOUT[4] = numQ4;


    puts("Kyle Crawford");
    puts("Group Number: 11-2");
    puts("My seed number is 0x43");
    puts("Fall 2020");
    puts("Problem D");
    puts("The results are: ");

    printf("aOUT[0]: %d\n", aOUT[0]);
    printf("aOUT[1]: %d\n", aOUT[1]);
    printf("aOUT[2]: %d\n", aOUT[2]);
    printf("aOUT[3]: %d\n", aOUT[3]);
    printf("aOUT[4]: %d\n", aOUT[4]);

    GPIO_setOutputHighOnPin(LED2PORT, LED2PIN);

    //spin
    while(1);
}

