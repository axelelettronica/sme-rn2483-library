#include <Arduino.h>
#include <rn2483.h>

char buffer[100];

void setup() {
    SerialUSB.begin(115200);
    lora.begin();
    delay(100);

    while (!SerialUSB) {
        ;
    }

    // first time get from Hw
    lora.getVersion();
}

void loop() {
    bool ok=false;
    // get SwVersion
    SerialUSB.print("FW Version :");
    SerialUSB.println(lora.getVersion());

    // Read the EEprom
    SerialUSB.print("\nget EEProm (0x00): ");
    SerialUSB.println(lora.getUserEEprom(0), HEX);
    SerialUSB.print("get EEProm (0x100): ");
    SerialUSB.println(lora.getUserEEprom(0x1A), HEX);
    SerialUSB.print("get EEProm (0x300): ");
    SerialUSB.println(lora.getUserEEprom(0x30), HEX);


    // Read Power
    SerialUSB.print("\nVdd is: ");
    SerialUSB.print(lora.getPower());
    SerialUSB.println(" mV");


    // Read HwEUI
    SerialUSB.print("\nhwEUI is : ");
    SerialUSB.println(lora.getHwEUI());

    // Read Radio modulation Mode
    SerialUSB.print("\nRadio is in mode: ");
    radioModeE radio = lora.getRadioMode();
    if (radio == LoRa) {
        SerialUSB.println("LoRa");
    } else if (radio == FSK) {
        SerialUSB.println("FSK");
    }

    // Change EEProm
    SerialUSB.println("\n change data on EEProm address.");
    SerialUSB.println("ATTENTION this will change your EEProm, do U want to continue [Y/n] ?");
    int answer='a';

    do {
        delay(100);

        if (SerialUSB.available()) {
            answer = SerialUSB.read();
        }
    } while ((answer != 'Y') && (answer != 'n'));

    if ('Y' == answer) {
        SerialUSB.println(lora.setUserEEprom(0, 0x01));
        SerialUSB.println(lora.setUserEEprom(0x1A, 00));
        SerialUSB.println(lora.setUserEEprom(0x30, 0x7E));
        SerialUSB.print("get EEProm (0x00): ");
        SerialUSB.println(lora.getUserEEprom(0), HEX);
        SerialUSB.print("get EEProm (0x100): ");
        SerialUSB.println(lora.getUserEEprom(0x1A), HEX);
        SerialUSB.print("get EEProm (0x300): ");
        SerialUSB.println(lora.getUserEEprom(0x30), HEX);
    }

    // Change EEProm
    SerialUSB.println("\n change radio mode in (L)ora or (F)sk") ;
    SerialUSB.println(" do U want to continue [L/F] ?");
    answer='a';

    do {
        delay(100);

        if (SerialUSB.available()) {
            answer = SerialUSB.read();
        }
    } while ((answer != 'L') && (answer != 'F'));

    if ('L' == answer) {
        ok = lora.setRadioMode(LORA_MODE);
    } else if ('F' == answer) {
        ok = lora.setRadioMode(FSK_MODE);
    }
    delay(100);

    // Read Radio modulation Mode
    if (!ok) {
        SerialUSB.print("\nWRONG CONFIGURATION.\nRadio remain in mode: ");
    } else {
        SerialUSB.print("\nRadio is in mode: ");
    }    
    radio = lora.getRadioMode();
    if (radio == LoRa) {
        SerialUSB.println("LoRa");
    } else if (radio == FSK) {
        SerialUSB.println("FSK");
    }

    //endless loop
    while(1){
        ;
    }
}


