/*
    SmarteEveryting Lion RN2483 Library - Downaload

    This example shows how to prepare the RN2483 for the Fw download procedure
    making leverage on the 'loRa Development Utility Java' app from Microchip.
 
    created 25 Feb 2017
    by Seve (seve@ioteam.it)

    This example is in the public domain
    https://github.com/axelelettronica/sme-rn2483-library

    More information on RN2483 available here:
    http://www.microchip.com/wwwproducts/en/RN2483
    
 */

/*
 * Steps to perform a RN2483 Fw upgrade:
 *
 * 1  - Set Serial speed to 57600
 * 2  - Load this scketch
 * 3  - reset the board
 * 4  - execute the loRa Development Utility Java app from Microchip
 * 5  - Scan COM ports
 * 6  - Issue Fw upgrade
 *      
 * iF THE DOWNLOAD IS NOT SUCCESSFUL (SEE LOG)
 * It freezed because an old bootleader was present (old bootloader used 19200 speed :-(  )
 *
 * Hence, follow the following steps to recover:
 *
 * 7  - Set Serial and Lora serial speed to 19200
 * 8  - Load the Scketch again
 * 9  - Scan COM ports with Bootloader flag checked
 * 10 - Execute again the Fw download
 * 11 - Wait till Successful notification. OK, Upgrade Done!
 * 12 - Replace the Lora and USB speed to the previous values
 *
 */
#include <Arduino.h>
#include <rn2483.h>


void setup() {

    bool err = false;
   
    // Comment when required
    Serial.begin(57600);
    lora.begin();  // Default speed 57600

    // Uncomment when required
    //Serial.begin(19200);
    //lora.begin(19200);
    
    delay(100);
}


char c;

void loop() {

    if (Serial.available()) {
      c = Serial.read();    
      iotAntenna.print(c);

    }
    if (iotAntenna.available()) {
      c = iotAntenna.read();    
      Serial.print(c);
    }

    delay(1);
}


