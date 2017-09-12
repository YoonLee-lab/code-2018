/*!
LTC2983: Multi-Sensor High Accuracy Digital Temperature Measurement System
@verbatim

DATASHEET_CUSTOM_THERMISTOR_TABLE.ino:
Generated Linduino code from the LTC2983 Demo Software.
This code  is designed to be used by a Linduino,
but can also be used to understand how to program the LTC2983.


@endverbatim

http://www.linear.com/product/LTC2983

http://www.linear.com/product/LTC2983#demoboards

$Revision: 5797 $
$Date: 2016-09-23 10:30:32 -0700 (Fri, 23 Sep 2016) $
Copyright (c) 2014, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/



/*! @file
    @ingroup LTC2983

*/




#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "SPI.h"
#include "Wire.h"
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LT_I2C.h"
#include "QuikEval_EEPROM.h"
#include "stdio.h"
#include "math.h"

#include "configuration_constants_LTC2983.h"
#include "support_functions_LTC2983.h"
#include "table_coeffs_LTC2983.h"

#define CHIP_SELECT QUIKEVAL_CS  // Chip select pin

// Function prototypes
void configure_memory_table();
void configure_channels();
void configure_global_parameters();


// -------------- Configure the LTC2983 -------------------------------
void setup()
{
  char demo_name[]="DC2209";     // Demo Board Name stored in QuikEval EEPROM
  quikeval_I2C_init();          // Configure the EEPROM I2C port for 100kHz
  quikeval_SPI_init();          // Configure the spi port for 4MHz SCK
  quikeval_SPI_connect();       // Connect SPI to main data port
  pinMode(CHIP_SELECT, OUTPUT); // Configure chip select pin on Linduino

  Serial.begin(115200);         // Initialize the serial port to the PC
  print_title();
  discover_demo_board(demo_name);

  configure_channels();
  configure_memory_table();
  configure_global_parameters();
}


void configure_channels()
{
  uint8_t channel_number;
  uint32_t channel_assignment_data;

  // ----- Channel 4: Assign Sense Resistor -----
  channel_assignment_data =
    SENSOR_TYPE__SENSE_RESISTOR |
    (uint32_t) 0x9DD000 << SENSE_RESISTOR_VALUE_LSB;    // sense resistor - value: 10100.
  assign_channel(CHIP_SELECT, 4, channel_assignment_data);
  // ----- Channel 5: Assign Thermistor Custom Table -----
  channel_assignment_data =
    SENSOR_TYPE__THERMISTOR_CUSTOM_TABLE |
    THERMISTOR_RSENSE_CHANNEL__4 |
    THERMISTOR_SINGLE_ENDED |
    THERMISTOR_EXCITATION_MODE__NO_SHARING_NO_ROTATION |
    THERMISTOR_EXCITATION_CURRENT__1UA |
    (uint32_t) 0x14 << THERMISTOR_CUSTOM_ADDRESS_LSB |    // thermistor - custom address: 20.
    (uint32_t) 0x9 << THERMISTOR_CUSTOM_LENGTH_1_LSB;   // thermistor - custom length-1: 9.
  assign_channel(CHIP_SELECT, 5, channel_assignment_data);

}


void configure_memory_table()
{
  uint16_t start_address;
  uint16_t table_length;
  // int i;

  // -- Channel 5 custom table --
  table_coeffs ch_5_coefficients[] =
  {
    { 0, 468480 },  // -- 0.0, 457.5
    { 1280, 409805 }, // -- 80.0, 400.2
    { 2944, 381235 }, // -- 184.0, 372.3
    { 6771, 327782 }, // -- 423.2, 320.1
    { 15574, 297523 },  // -- 973.36, 290.55
    { 35820, 255304 },  // -- 2238.728, 249.32
    { 82385, 246067 },  // -- 5149.0744, 240.3
    { 428403, 235520 }, // -- 26775.18688, 230.0
    { 2227696, 220467 },  // -- 139230.978, 215.3
    { 11584017, 204800 }  // -- 724001.0532, 200.0
  };
  start_address = (uint16_t) 712; // Real address = 6*20 + 0x250 = 712
  table_length = (uint8_t) 10;  // Real table length = 9 + 1 = 10
  write_custom_table(CHIP_SELECT, ch_5_coefficients, start_address, table_length);


}



void configure_global_parameters()
{
  // -- Set global parameters
  transfer_byte(CHIP_SELECT, WRITE_TO_RAM, 0xF0, TEMP_UNIT__C |
                REJECTION__50_60_HZ);
  // -- Set any extra delay between conversions (in this case, 0*100us)
  transfer_byte(CHIP_SELECT, WRITE_TO_RAM, 0xFF, 0);
}

// -------------- Run the LTC2983 -------------------------------------

void loop()
{
  measure_channel(CHIP_SELECT, 5, TEMPERATURE);      // Ch 5: Thermistor Custom Table
}