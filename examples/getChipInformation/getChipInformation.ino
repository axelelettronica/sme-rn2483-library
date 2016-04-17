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
    
    // get SwVersion
    SerialUSB.print("FW Version :");
    SerialUSB.println(lora.getVersion());
    
    // Read the EEprom
    SerialUSB.print("\nget EEProm (0x00): ");
    SerialUSB.println(lora.getUserEEprom(0));
    SerialUSB.print("get EEProm (0x100): ");
    SerialUSB.println(lora.getUserEEprom(0x1A));
    SerialUSB.print("get EEProm (0x300): ");
    SerialUSB.println(lora.getUserEEprom(0x30));
    
    
    // Read Power
    SerialUSB.print("\Vdd is: ");
    SerialUSB.print(lora.getPower());
    SerialUSB.println(" mV");
    
    
    
    // Change EEProm
    SerialUSB.println("\n change data on EEProm address.");
    SerialUSB.println("ATTENTION this will change your EEProm, do U want to continue [Y/n] ?");
    int answer='n';
    
    do {
        delay(100);
    
    
    if (SerialUSB.available()) {
        answer = SerialUSB.read();
    }    
    } while (answer != 'Y');   
    SerialUSB.println(lora.setUserEEprom(0, 0xa5));
    SerialUSB.println(lora.setUserEEprom(0x1A, 0x5A));
    SerialUSB.println(lora.setUserEEprom(0x30, 0x55));
    SerialUSB.print("get EEProm (0x00): ");
    SerialUSB.println(lora.getUserEEprom(0));
    SerialUSB.print("get EEProm (0x100): ");
    SerialUSB.println(lora.getUserEEprom(0x1A));
    SerialUSB.print("get EEProm (0x300): ");
    SerialUSB.println(lora.getUserEEprom(0x30));
    
    
    //endless loop
    while(1){
        ;
    }
}
