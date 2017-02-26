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
 *  RN2483  more information available here:
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


void setup() 
{
    loraDbg = false;      // Set to 'true' to enable RN2483 TX/RX traces
    
    ledYellowOneLight(LOW);
    ledYellowTwoLight(LOW);   
    
    Serial.begin(57600);

    lora.begin();

    // Waiting for Console connection
    while (!Serial) {
        ;
    }
}


void loop() 
{    
    if (Serial.available()) {
        c = Serial.read();    
        Serial.write(c); 
        buff[i++] = c;

        if (c == '\n') {
            Serial.print(lora.sendRawCmdAndAnswer(buff));
            i = 0;
            memset(buff, 0, sizeof(buff));
            ledBlink(TX_LED, HIGH, 50);
        }
    }

    if (lora.available()) {
        // Printing asychronous rx msgs from RN2483
        Serial.print(lora.read());
        ledBlink(RX_LED, HIGH, 50);
    }

    delay(10);
}


