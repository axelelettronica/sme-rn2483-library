/*
 *  SmartEverything Lion RN2483 Library - Passthrough
 *
 *  Thi example provides the direct access, through the USB 
 *  console, to the RN2483 Microchip module for a direct configuration 
 *
 *  created 19 Nov 2016
 *  by Seve (seve@ioteam.it)
 *
 *  This example is in the public domain
 *  https://github.com/axelelettronica/sme-rn2483-library
 *
 *  SE868  more information available here:
 *  http://www.microchip.com/wwwproducts/en/RN2483
 */

#include <Arduino.h>
#include <rn2483.h>


#define TX_LED 0
#define RX_LED 1

char buff[100] = {};
char c;
unsigned char i = 0;
 

void ledBlink (int led, bool status, unsigned int msec)
{
    if (led == TX_LED) {
        ledYellowOneLight(HIGH); // turn the LED On by making the voltage HIGH  
        delay(msec);             // wait for msec ms  
        ledYellowOneLight(LOW);  // turn the LED off by making the voltage LOW
    } else  {
        ledYellowTwoLight(HIGH); // turn the LED On by making the voltage HIGH  
        delay(msec);             // wait for msec ms  
        ledYellowTwoLight(LOW);  // turn the LED off by making the voltage LOW
    }
}


void setup() {

    ledYellowOneLight(LOW);
    ledYellowTwoLight(LOW);   

    SerialUSB.begin(57600);

    lora.begin();

    // Waiting for Console connection
    while (!SerialUSB) {
        ;
    }

    lora.sendRawCmd("sys reset");
}


void loop() {
    
    if (SerialUSB.available()) {
        c = SerialUSB.read();    
        SerialUSB.write(c); 
        buff[i++] = c;

        if (c == '\n') {
            lora.sendRawCmd(buff);
            i = 0;
            memset(buff, 0, sizeof(buff));
            ledBlink(TX_LED, HIGH, 50);
        }
    }

    if (lora.available()) {
        SerialUSB.print(lora.read());
        ledBlink(RX_LED, HIGH, 50);
    }

    delay(10);
}


