#include <Arduino.h>
#include <rn2483.h>

char buffer[100];

void setup() {
    SerialUSB.begin(115200);
    lora.init();
    delay(100);
    
    while (!SerialUSB) {
        ;
    }
    
    // first time get from Hw
    lora.getVersion();
}

void loop() {
    
    SerialUSB.write(lora.getVersion());
    
    //endless loop
    while(1){
        ;
    }
}


