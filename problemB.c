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
#define LED2PORT GPIO_PORT_P1
#define LED2PIN GPIO_PIN1

//function prototypes
void myData(uint8_t, int8_t*);

void main (void){

    //variables
    uint8_t vCount = 0, i, j, vPos = 0, vNeg = 0;
    int8_t s8bData, temp, vMax, vMin, vAvg;
    int8_t numArray[256];
    int16_t vSum = 0;

    //stop WDT
    WDT_A_hold(WDT_A_BASE);

    //define I/O
    GPIO_setAsOutputPin(LED2PORT, LED2PIN);
    GPIO_setOutputLowOnPin(LED2PORT, LED2PIN);
    PMM_unlockLPM5();

    //initialize myData
    myData(SEED, &s8bData);

    //get random numbers into array
    myData(2, &s8bData);
    while(s8bData != 0){
        numArray[vCount] = s8bData;

        //get count and sum
        vCount++;
        vSum += s8bData;

        //count number of positives and negatives
        if(s8bData < 0){
            vNeg++;
        }
        else{
            vPos++;
        }

        myData(2, &s8bData);
    }

    //sort array
    for(i = 0; i < vCount; i++){
        for(j = i+1; j < vCount; j++){
            //check if next index is greater than current one
            if(numArray[j] < numArray[i]){
                //if it is then swap
                temp = numArray[i];
                numArray[i] = numArray[j];
                numArray[j] = temp;
            }
        }

    }

    //print out results
    vMin = numArray[0];
    vMax = numArray[vCount - 1];
    vAvg = vSum/vCount;

    puts("Kyle Crawford");
    puts("Group Number: 11-2");
    puts("My seed number is 0x43");
    puts("Fall 2020");
    puts("Problem B");
    puts("The results are: ");

    printf("vSum: %d\n", vSum);
    printf("vAvg: %d\n", vAvg);
    printf("vMax: %d\n", vMax);
    printf("vMin: %d\n", vMin);
    printf("vPos: %d\n", vPos);
    printf("vNeg: %d\n", vNeg);


    GPIO_setOutputHighOnPin(LED2PORT, LED2PIN);

    //spin
    while(1);
}

