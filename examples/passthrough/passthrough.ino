#include <Arduino.h>
#include <rn2483.h>

/*
 * This sketch provides the direct access, through the USB 
 * console to the RN2483 modulefor for a direct configuration 
 */

#define TX_LED 0
#define RX_LED 1

 

void ledBlink (int led, bool status, unsigned int msec)
{
    if (led == TX_LED) {
        ledYellowOneLight(HIGH);   // turn the LED On by making the voltage HIGH  
        delay(msec);          // wait for msec ms  
        ledYellowOneLight(LOW);    // turn the LED off by making the voltage LOW
    } else  {
        ledYellowTwoLight(HIGH);   // turn the LED On by making the voltage HIGH  
        delay(msec);          // wait for msec ms  
        ledYellowTwoLight(LOW);    // turn the LED off by making the voltage LOW
    }
}


void setup() {

    bool err = false;
    loraDbg = true;
    ledYellowOneLight(LOW);
    ledYellowTwoLight(LOW);   
    //SerialUSB.begin(115200);
    SerialUSB.begin(57600);
    lora.begin();
    delay(100);
    lora.sendRawCmd("sys reset");
    
    // Waiting for the USB serial connection
   // while (!SerialUSB) {
    //    ;
    //}

    // first time get from Hw
    //lora.getVersion();
    
    //SerialUSB.print("FW Version :");
    //SerialUSB.println(lora.getVersion());
}

uint8_t buff_size = 100;
char buff[100] = {};
uint8_t i = 0;
char c;
void loop() {
    static int loop_cnt = 0;
    
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
        //String data received from Lora Module;
        //SerialUSB.print("\nRx> ");  
        SerialUSB.print(lora.read());
        ledBlink(RX_LED, HIGH, 50);
    }

    loop_cnt++;
    delay(10);
}


