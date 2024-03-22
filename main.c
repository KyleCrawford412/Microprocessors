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
void myData(uint8_t, uint8_t*);

void main (void){

    //variables
    uint8_t count0 = 0, count1 = 0, count2 = 0, count3 = 0, count4 = 0, u8bData, avg0, avg1, avg2, avg3, avg4;
    uint16_t sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
    uint16_t v0 = 0, v1 = 0;
    uint32_t v2 = 0, v3 = 0, v4 = 0;
    uint32_t temp = 0;


    //bins
    uint8_t bin0[256], bin1[256], bin2[256], bin3[256], bin4[256];

    //stop WDT
    WDT_A_hold(WDT_A_BASE);

    //define I/O
    GPIO_setAsOutputPin(LED2PORT, LED2PIN);
    GPIO_setOutputLowOnPin(LED2PORT, LED2PIN);
    PMM_unlockLPM5();

    //initialize myData
    myData(SEED, &u8bData);

    //get random numbers into array
    myData(1, &u8bData);
    while(u8bData != 0){
       //range 1 to 49
        if(u8bData < 50){
            bin0[count0] = u8bData;
            count0++;
            sum0 += u8bData;
            temp = u8bData * u8bData;
            v0 += temp;
        }
        //range 50 to 99
        else if((u8bData >= 50) && (u8bData < 100)){
            bin1[count1] = u8bData;
            count1++;
            sum1 += u8bData;
            temp = u8bData * u8bData;
            v1 += temp;
        }
        //range 100 to 149
        else if((u8bData >= 100) && (u8bData < 150)){
            bin2[count2] = u8bData;
            count2++;
            sum2 += u8bData;
            temp = u8bData * u8bData;
            v2 += temp;
        }
        //range 150 to 199
        else if((u8bData >= 150) && (u8bData < 200)){
            bin3[count3] = u8bData;
            count3++;
            sum3 += u8bData;
            temp = u8bData * u8bData;
            v3 += temp;
        }
        //range 200 to 255
        else if((u8bData >= 200) && (u8bData < 256)){
            bin4[count4] = u8bData;
            count4++;
            sum4 += u8bData;
            temp = u8bData * u8bData;
            v4 += temp;
        }
    }

    //calculate averages
    avg0 = sum0 / count0;
    avg1 = sum1 / count1;
    avg2 = sum2 / count2;
    avg3 = sum3 / count3;
    avg4 = sum4 / count4;

    //calculate variances
    v0 = (v0 / count0) - (avg0 * avg0);
    v1 = (v1 / count1) - (avg1 * avg1);
    v2 = (v2 / count2) - (avg2 * avg2);
    v3 = (v3 / count3) - (avg3 * avg3);
    v4 = (v4 / count4) - (avg4 * avg4);

    //print out results
    puts("Kyle Crawford");
    puts("Fall 2020");
    puts("CHW#3");
    puts("My seed number is 0x43");
    puts("N, #, Avg, Variance");

    printf("0: %d\n", vSum);
    printf("1: %d\n", vAvg);
    printf("2: %d\n", vMax);
    printf("3: %d\n", vMin);
    printf("4: %d\n", vPos);


    GPIO_setOutputHighOnPin(LED2PORT, LED2PIN);

    //spin
    while(1);
}

