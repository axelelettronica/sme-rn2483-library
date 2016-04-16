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
    
    SerialUSB.print("FW Version :");
    SerialUSB.println(lora.getVersion());
    
    SerialUSB.print("\nget EEProm (0x00) :");
    SerialUSB.println(lora.getUserEEprom(0));
    SerialUSB.print("get EEProm (0x100) :");
    SerialUSB.println(lora.getUserEEprom(0x1A));
    SerialUSB.print("get EEProm (0x300) :");
    SerialUSB.println(lora.getUserEEprom(0x30));
    //endless loop
    while(1){
        ;
    }
}
