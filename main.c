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
#include "driverlib.h"

typedef enum {Red, Green, Blue, Cyan, Yellow, Magenta, White, Off} LedColors;

//function definitions
void rgbDriver(LedColors color);
void config_mkII();

void main (void)
{
}

void rgbDriver(LedColors color){
    switch(color){
    case Red:
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Green:
        GPIO_setOutputLowhOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Blue:
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Cyan:
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Yellow:
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Magenta:
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case White:
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    case Off:
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
        break;
    }
}

void config_mkII(){
    //set red LED
    GPIO_setAsOutputPin(GPIO_PORT_J4, GPIO_PIN39);
    //set green LED
    GPIO_setAsOutputPin(GPIO_PORT_J4, GPIO_PIN38);
    //set blue LED
    GPIO_setAsOutputPin(GPIO_PORT_J4, GPIO_PIN37);

    //set pushbuttons with pull up resistors
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_J4, GPIO_PIN33);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_J4, GPIO_PIN32);

    //unlock LPM5
    PMM_unlockLPM5();

    //turn off LEDs
    rgbDriver(off);
}
