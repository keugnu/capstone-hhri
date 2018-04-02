
#include <stdio.h>
#include <unistd.h>
#include "ADS1115.h"

ADS1115 ads;     /* Use this for the 16-bit version */

int main(int argc, char *argv[]) {
  printf("Getting single-ended readings from AIN0..3\n");
  printf("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)\n \n");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    while(1) {
        int16_t adc0, adc1, adc2, adc3;
        float adc0_volt, adc1_volt, adc2_volt, adc3_volt;

        adc0 = ads.readADC_SingleEnded(0);
        adc1 = ads.readADC_SingleEnded(1);
        adc2 = ads.readADC_SingleEnded(2);
        adc3 = ads.readADC_SingleEnded(3);

        adc0_volt = adc0 * 0.0001875;
        adc1_volt = adc1 * 0.0001875;
        adc2_volt = adc2 * 0.0001875;
        adc3_volt = adc3 * 0.0001875;

        printf("AIN0: %.2f\n", adc0_volt);
        printf("AIN1: %.2f\n", adc1_volt);
        printf("AIN2: %.2f\n", adc2_volt);
        printf("AIN3: %.2f\n", adc3_volt);
        printf("\n");

        usleep(1000000); // delay 1 second
    }
}