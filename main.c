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
/* ---------------------------------------------------------
    Kyle Crawford
    Fall 2020
    Section 3
    HW 5
* ---------------------------------------------------------- */

// Includes
#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>

// Defines
#define SEED 'C'
#define GETLOWER 0b0000000011111111
#define LED2PORT GPIO_PORT_P1
#define LED2PIN GPIO_PIN1



uint8_t myData(uint8_t, int16_t*);

void main (void){

    // Define and Initialize Local Variables
    int16_t data;
    int8_t aOUT[5] = {0,0,0,0,0};
    int8_t upper = 0;
    int8_t lower = 0;
    int16_t q1x = 0, q1y = 0, q2x = 0, q2y = 0, q3x = 0, q3y = 0, q4x = 0, q4y = 0, xAxis = 0, yAxis = 0;
    float totalX = 0, totalY = 0;
    float avgQ1x, avgQ1y, avgQ2x, avgQ2y, avgQ3x, avgQ3y, avgQ4x, avgQ4y;

    // Hold Watch Dog Timer
    WDT_A_hold(WDT_A_BASE);

    // Configure and Activate I/O
    GPIO_setAsOutputPin(LED2PORT,LED2PIN);
    GPIO_setOutputLowOnPin(LED2PORT,LED2PIN);

    PMM_unlockLPM5();

    myData(SEED, &data);

    myData(4, &data);

    while(data != 0){

        upper = (int8_t)(data >> 8);    // shifts bits of data to right converts to signed 8bit
        lower = (int8_t)(data & GETLOWER);   // Bitwise AND with GETLOWER and converts to signed 8 bit

        if((upper > 0) && (lower > 0)){
            // ++ Quadrant 4 (Pos , Pos) A
            q1x += upper;
            q1y += lower;
            aOUT[4]++;
        }
        else if((upper < 0) && (lower < 0)){
            // -- Quadrant 3 (Neg , Neg) C
            q3x += upper;
            q3y += lower;
            aOUT[3]++;
        }
        else if((upper < 0) && (lower > 0)){
            // -+ Quadrant 2 (Neg , Pos) B
            q2x += upper;
            q2y += lower;
            aOUT[2]++;
        }
        else if((upper > 0) && (lower < 0)){
            // +- Quadrant 1 (Pos , Neg) D
            q4x += upper;
            q4y += lower;
            aOUT[1]++;
        }

        myData(4, &data);

    }

    //calculate averages for each quadrant's x and y
    avgQ1x = ((float)q1x) / ((float)aOUT[4]);
    avgQ1y = ((float)q1y) / ((float)aOUT[4]);
    avgQ2x = ((float)q2x) / ((float)aOUT[2]);
    avgQ2y = ((float)q2y) / ((float)aOUT[2]);
    avgQ3x = ((float)q3x) / ((float)aOUT[3]);
    avgQ3y = ((float)q3y) / ((float)aOUT[3]);
    avgQ4x = ((float)q4x) / ((float)aOUT[1]);
    avgQ4y = ((float)q4y) / ((float)aOUT[1]);

    //calculate the sum of all x and y points to find average
    totalX = ((float)q1x + (float)q2x + (float)q3x + (float)q4x) / ((float)aOUT[1] + (float)aOUT[2] + (float)aOUT[3] + (float)aOUT[4]);
    totalY = ((float)q1y + (float)q2y + (float)q3y + (float)q4y) / ((float)aOUT[1] + (float)aOUT[2] + (float)aOUT[3] + (float)aOUT[4]);

   // Printing Header and Information
   puts("Kyle Crawford");
   puts("Fall 2020");
   puts("CHW#5");
   printf("My seed value is: %c\n", SEED);
   puts("Quad#, Avg X, Avg Y");

   printf("A: x%.1f, y%.1f\n", avgQ1x, avgQ1y);
   printf("B: x%.1f, y%.1f\n", avgQ2x, avgQ2y);
   printf("C: x%.1f, y%.1f\n", avgQ3x, avgQ3y);
   printf("D: x%.1f, y%.1f\n", avgQ4x, avgQ4y);
   printf("All: x%.1f, y%.1f\n", totalX, totalY);



   // Turn on LED2
   GPIO_setOutputHighOnPin(LED2PORT,LED2PIN);

   // Spin
   while(1);

}
