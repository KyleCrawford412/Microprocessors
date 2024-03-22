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


//include files
#include "driverlib.h"
#include <stdio.h>
#include <stdint.h>
#include <HAL_UART_4746.h>

//function prototypes
bool checkPrime(uint16_t x);


//checkPrime
//Inputs: usigned 16 bit
//outputs: bool
//returns true if number is a prime, false if number is not a prime
bool checkPrime(uint16_t x){

    //variables
    uint16_t i;

    //special cases
    if((x == 0) || (x == 1)){
        return false;
    }
    else if((x == 2) || (x == 3)){
        return true;
    }
    else{
        for(i = 4; i<= x; i++){
            if(x%i == 0){
                return false;
            }
        }
        return true;
    }
}

//main
void main (void){

    //define variables
    uint8_t i = 0;
    uint16_t ii, maxNum = 0;
    volatile static uint16_t numPrimes = 0;
    char buffer[100];

    //hold WDT
    WDT_A_hold(WDT_A_BASE);

    UART_initGPIO();
    UART_init();
    _delay_cycles(500000);

    //print
    sprintf(buffer, "Kyle Crawford \r\n");
    UART_transmitString(buffer);
    UART_transmitString("Fall 2020\r\n");


    //loop i from 0 to 10
    for(i = 0; i <= 10; i++){
        maxNum = 500*(1+2*i);
        //check all the numbers
        for(ii = 0; ii <= maxNum; ii++){
            //check for prime number
            if(checkPrime(ii)){
                numPrimes++;
            }
        }
        //print results and reset count
        sprintf(buffer, "Number of primes: %d\r\n", numPrimes);
        UART_transmitString(buffer);
        numPrimes = 0;
    }



    sprintf(buffer, "Number of primes: %d\r\n", numPrimes);
    UART_transmitString(buffer);


    //spin
    while(1);

}

