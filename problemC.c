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
#define SEED 'C'
#define MASK1 BITD
#define MASK2 BITA
#define MASK3 BIT7 + BIT2
#define LED2PORT GPIO_PORT_P1
#define LED2PIN GPIO_PIN1

//function prototypes
void myData(uint8_t, uint16_t*);

void main (void){

    //variables
    uint8_t check = 0, vCount = 0, vFirst, vLast, temp = 1;
    uint16_t u16bData;
    uint32_t vSum = 0;

    //stop WDT
    WDT_A_hold(WDT_A_BASE);

    //define I/O
    GPIO_setAsOutputPin(LED2PORT, LED2PIN);
    GPIO_setOutputLowOnPin(LED2PORT, LED2PIN);
    PMM_unlockLPM5();

    //initialize myData
    myData(SEED, &u16bData);

    //get random numbers into array
    myData(3, &u16bData);
    while(u16bData != 0){
        if((u16bData & MASK1) == BITD){
            if(check == 0){
                vFirst = temp;
                check++;
            }
            vLast = temp;
            vSum += u16bData;
            vCount++;
        }
        else if((u16bData & MASK2) == 0){
            if(check == 0){
                vFirst = temp;
                check++;
            }
            vLast = temp;
            vSum += u16bData;
            vCount++;
        }
        else if((u16bData & MASK3) == BIT2){
            if(check == 0){
                vFirst = temp;
                check++;
            }
            vLast = temp;
            vSum += u16bData;
            vCount++;
        }
        temp++;
        myData(3, &u16bData);
    }

    puts("Kyle Crawford");
    puts("Group Number: 11-2");
    puts("My seed number is 0x43");
    puts("Fall 2020");
    puts("Problem C");
    puts("The results are: ");

    printf("vSum: %ld\n", vSum);
    printf("vCount: %d\n", vCount);
    printf("vFirst: %d\n", vFirst);
    printf("vLast: %d\n", vLast);

    GPIO_setOutputHighOnPin(LED2PORT, LED2PIN);

    //spin
    while(1);
}
