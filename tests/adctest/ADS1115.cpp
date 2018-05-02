/*  ADS1115 ADC test code  */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ADS1115.h"


// //    @brief  Abstract away platform differences in Arduino wire library
// static uint8_t i2cread(void) {
//   #if ARDUINO >= 100
//   return Wire.read();
//   #else
//   return Wire.receive();
//   #endif
// }


// //    @brief  Abstract away platform differences in Arduino wire library
// static void i2cwrite(uint8_t x) {
//   #if ARDUINO >= 100
//   Wire.write((uint8_t)x);
//   #else
//   Wire.send(x);
//   #endif
// }


//    @brief  Writes 16-bits to the specified destination register
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  int file; 
  char const *bus = "/dev/i2c-1";

  if ((file = open(bus, O_RDWR)) < 0) {  
     printf("Failed to open the bus. \n");
     exit(1);
  }
  ioctl(file, I2C_SLAVE, i2cAddress);

  char wreg[3] = {reg, ((value >> 8) & 0xFF), (value & 0xFF)}; // value high byte, value low byte

  write(file, wreg, 3);
  if (close(file) < 0) {
    printf("Failed to close the bus (writeRegister). \n");
    exit(1);
  }

//   Wire.beginTransmission(i2cAddress);
//   i2cwrite((uint8_t)reg);
//   i2cwrite((uint8_t)(value>>8));
//   i2cwrite((uint8_t)(value & 0xFF));
//   Wire.endTransmission();
}


//    @brief  Reads 16-bits from specified register
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  char i2c_rx_buff[2] = {0};
  char wreg[1] = {reg};
  uint16_t value;
  int file;
  char const *bus = "/dev/i2c-1";
  if ((file = open(bus, O_RDWR)) < 0) {
     printf("Failed to open the bus. \n");
     exit(1);
  }
  ioctl(file, I2C_SLAVE, i2cAddress);

  write(file, wreg, 1);
  read(file, i2c_rx_buff, 2);

  if (close(file) < 0) {
    printf("Failed to close the bus (readRegister). \n");
    exit(1);
  }

  value  = (uint16_t)(i2c_rx_buff[0] << 8); // value high byte
  value |= (uint16_t)(i2c_rx_buff[1]);      // value low byte

  return value;

//   Wire.beginTransmission(i2cAddress);
//   i2cwrite(ADS1015_REG_POINTER_CONVERT);
//   Wire.endTransmission();
//   Wire.requestFrom(i2cAddress, (uint8_t)2);
//   return ((i2cread() << 8) | i2cread());  
}


// //    @brief  Instantiates a new ADS1015 class w/appropriate properties
// ADS1115::ADS1115(uint8_t i2cAddress) 
// {
//    m_i2cAddress = i2cAddress;
//    m_conversionDelay = ADS1015_CONVERSIONDELAY;
//    m_bitShift = 4;
//    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
// }


//    @brief  Instantiates a new ADS1115 class w/appropriate properties
ADS1115::ADS1115(uint8_t i2cAddress)
{
   m_i2cAddress = i2cAddress;
   m_conversionDelay = ADS1115_CONVERSIONDELAY;
   m_bitShift = 0;
   m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}


//    @brief  Sets up the HW (reads coefficients values, etc.)
void ADS1115::begin() {
  //Wire.begin();
}


//    @brief  Sets the gain and input voltage range
void ADS1115::setGain(adsGain_t gain)
{
  m_gain = gain;
}


//    @brief  Gets a gain and input voltage range
adsGain_t ADS1115::getGain()
{
  return m_gain;
}


//    @brief  Gets a single-ended ADC reading from the specified channel
uint16_t ADS1115::readADC_SingleEnded(uint8_t channel) {
  if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  usleep(8000);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;  
}

 
/*    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative. */
int16_t ADS1115::readADC_Differential_0_1() {
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;
                    
  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;          // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  usleep(8000);

  // Read the conversion results
  uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

 
/*    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative. */
int16_t ADS1115::readADC_Differential_2_3() {
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;

  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  usleep(8000);

  // Read the conversion results
  uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}


/*    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.
            This will also set the ADC in continuous conversion mode.   */
void ADS1115::startComparator_SingleEnded(uint8_t channel, int16_t threshold)
{
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
                    ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
                    ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= m_gain;
                    
  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1015
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);

  // Write config register to the ADC
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}


/*    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value. */
int16_t ADS1115::getLastConversionResults()
{
  // Wait for the conversion to complete
  usleep(8000);

  // Read the conversion results
  uint16_t res = readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}