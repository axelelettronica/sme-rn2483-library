#include <Arduino.h>
#include <rn2483.h>

/*
 * Steps to do a Lora Fw upgrade:
 * 1- Set SerialUSB speed to 57600
 * 2- Load this scketch
 * 3- reset the board
 * 4- execute the loRa Development utility Java app
 * 5- Scan COM ports
 * 6- Issue Fw upgrade
 * 7- If freeze (bootloader changed the speed :-(  )
 * 8- Set SerialUSB and Lora serial speed to 19200
 * 9- Load the Scketch again
 * 10 - Scan COM ports with Bootloader flag checked
 * 11 - Execute again the Fw download
 * 12- Wait till Successful notification
 * 13- Replace the Lora and USB speed to the previous values
 * 14 DONE
 *
 */

void setup() {

    bool err = false;
    loraDbg = true;
    
    //SerialUSB.begin(115200);
    SerialUSB.begin(57600);
    //SerialUSB.begin(19200);
    lora.begin();
    delay(100);

    // Waiting for the USB serial connection
    //while (!SerialUSB) {
    //    ;
    //}
}


char c;
void loop() {
    static int loop_cnt = 0;
    
    if (SerialUSB.available()) {
      c = SerialUSB.read();    
      //SerialUSB.print(c);
      iotAntenna.print(c);

    }
    if (iotAntenna.available()) {
      c = iotAntenna.read();    
      SerialUSB.print(c);
    }

    delay(1);
}


