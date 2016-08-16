#include <Arduino.h>
#include <rn2483.h>

/*
 * Steps to perform a RN2483 Fw upgrade:
 *
 * 1  - Set SerialUSB speed to 57600
 * 2  - Load this scketch
 * 3  - reset the board
 * 4  - execute the loRa Development Utility Java app
 * 5  - Scan COM ports
 * 6  - Issue Fw upgrade
 *      
 * iF THE DOWNLOAD IS NOT SUCCESSFUL (SEE LOG)
 * It freezed because an old bootleader was present (old bootloader used 19200 speed :-(  )
 *
 * Hence, follow the following steps to recover:
 *
 * 7  - Set SerialUSB and Lora serial speed to 19200
 * 8  - Load the Scketch again
 * 9  - Scan COM ports with Bootloader flag checked
 * 10 - Execute again the Fw download
 * 11 - Wait till Successful notification. OK, Upgrade Done!
 * 12 - Replace the Lora and USB speed to the previous values
 *
 */

void setup() {

    bool err = false;
    loraDbg = true;
    
    // Comment if necessary
    SerialUSB.begin(57600);
    lora.begin();  // Default speed 57600

    // Uncomment if necessary
    //SerialUSB.begin(19200);
    //lora.begin(19200);
    
    delay(100);
}


char c;

void loop() {

    if (SerialUSB.available()) {
      c = SerialUSB.read();    
      iotAntenna.print(c);

    }
    if (iotAntenna.available()) {
      c = iotAntenna.read();    
      SerialUSB.print(c);
    }

    delay(1);
}


