/* --COPYRIGHT--,BSD
 /
  * Template File for DTMF Project
  * EEL-4746 Fall 2020
  */

//******************************************************************************
#include "driverlib.h"
#include "DSPLib.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Input signal parameters */
#define SAMPLES             256    // Number of Samples


// FFT using LEA. Setup LEA memory
/* Input signal and FFT result */
DSPLIB_DATA(input,MSP_ALIGN_FFT_Q15(SAMPLES))
_q15 input[SAMPLES];

/* Temporary data array for processing */
DSPLIB_DATA(temp,4)
_q15 temp[3*SAMPLES/2];


/* Function prototypes */

extern void adc_init();  // located in separate file
extern uint8_t adcFlag;  // Indicates that adcBuffer is full.
extern uint16_t adcBuffer[];
void updateClock();

void main(void)
{
    msp_status status;
    msp_fft_q15_params fftParams;
    msp_abs_q15_params absParams;
    uint16_t ii;
    volatile uint32_t waitCount = 0;  // used to estimate the number of counts between conversions

    /* Disable WDT */
     WDT_A_hold(WDT_A_BASE);
     PMM_unlockLPM5();

     updateClock();  // Increase clock frequency to 8 MHz



    adc_init();  // Initialize ADC

    // Start ADC in Main. Could also be done in adc_init()
    ADC12_B_startConversion(ADC12_B_BASE,
      ADC12_B_MEMORY_0,
      ADC12_B_REPEATED_SINGLECHANNEL);

    __enable_interrupt();  // Turn on global interrupts

    while(1){
        adcFlag = 0;   // Clear adc buffer flag to start new conversion
        waitCount = 0; // temp variable to estimate the time between conversions
        while(adcFlag == 0){   // wait until adcbuffer is full
            waitCount++;
        }

//  copy adc Buffer data into LEA memory space

       for(ii=0;ii<256;ii++){
           input[ii] = adcBuffer[ii];
       }

    /* Initialize ABS parameter for DSP Library */
    absParams.length = SAMPLES;

    /* Initialize the fft parameter structure for DSP Library */
    fftParams.length = SAMPLES;
    fftParams.bitReverse = true;
    fftParams.twiddleTable = MAP_msp_cmplx_twiddle_table_2048_q15;

    /* Perform real FFT with fixed scaling */

    status = MAP_msp_fft_fixed_q15(&fftParams, input);
    msp_checkStatus(status);  // if something is wrong, will hang-up here
    status = msp_abs_q15(&absParams,input,input);
    msp_checkStatus(status); // if something is wrong, will hang-up here

    // if you make it here, variable input will hold the magnitude spectrum of the FFT
    
    __no_operation();

    }
}

// This function will increase the MCLK and SMCLK clock frequency to 8 MHz
// You need to take this into account when calculating your timing delays

void updateClock(void){

    // Per Device Errata set divider to 4 before changing frequency to
     // prevent out of spec operation from overshoot transient
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_4);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_4);

    //Set DCO Frequency to 8MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_6);

    // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
    __delay_cycles(60);

    //configure MCLK, SMCLK to be source by DCOCLK
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

}


