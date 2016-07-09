#include <Arduino.h>
#include <rn2483.h>

char buffer[100];

void setup() {

    bool err = false;

    Serial1.begin(115200);
    lora.init();
    delay(100);

    while (!Serial1) {
        ;
    }

    // first time get from Hw
    lora.getVersion();
    
    Serial1.print("FW Version :");
    Serial1.println(lora.getVersion());
    // Read Power
    Serial1.print("\nVdd is: ");
    Serial1.print(lora.getPower());
    Serial1.println(" mV");

    // Read HwEUI
    Serial1.print("DEV EUI :");
    Serial1.println(lora.getHwEUI());
    // Read AppEUI
    Serial1.print("APP EUI :");
    Serial1.println(lora.getMacAppEUI());
    
     // Read HwEUI
     Serial1.println("Writing DEV EUI");
     err = lora.macSetDevEUICmd("0004A30B001A2A9E");
    //if (err) {
    //    Serial1.println("\Failed writing Dev EUI");
    //}
     // Read AppEUI
     Serial1.println("Writing APP EUI");
     err = lora.macSetAppEUICmd("F000000000000000");

    if (err) {
        Serial1.println("\nFailed writing APP EUI");
    }
    err = lora.macSetAppKeyCmd("00112233445566778899AABBCCDDEEFF");
    if (err) {
        Serial1.println("\nFailed writing APP Key");
    }
    

     // Read HwEUI
    // Serial1.print("DEV EUI :");
    // Serial1.println(lora.getHwEUI());
     // Read AppEUI
     Serial1.print("APP EUI :");
     Serial1.println(lora.getMacAppEUI());   
    
    // Read Radio modulation Mode
    Serial1.print("\nRadio is in mode: ");
    radioModeE radio = lora.getRadioMode();
    if (radio == LoRa) {
        Serial1.println("LoRa");
        } else if (radio == FSK) {
        Serial1.println("FSK");
    }
    
     while (lora.macJoinCmd(OTAA)) {
        Serial1.print("\nOTA JOIN FAILED: ");
        delay(5000);
     }     
}

void loop() {
    
/*
    // Change EEProm
    Serial1.println("\n change data on EEProm address.");
    Serial1.println("ATTENTION this will change your EEProm, do U want to continue [Y/n] ?");
    int answer='a';

    do {
        delay(100);

        if (Serial1.available()) {
            answer = Serial1.read();
        }
    } while ((answer != 'Y') && (answer != 'n'));

    if ('Y' == answer) {
        Serial1.println(lora.setUserEEprom(0, 0x01));
        Serial1.println(lora.setUserEEprom(0x1A, 00));
        Serial1.println(lora.setUserEEprom(0x30, 0x7E));
        Serial1.print("get EEProm (0x00): ");
        Serial1.println(lora.getUserEEprom(0), HEX);
        Serial1.print("get EEProm (0x100): ");
        Serial1.println(lora.getUserEEprom(0x1A), HEX);
        Serial1.print("get EEProm (0x300): ");
        Serial1.println(lora.getUserEEprom(0x30), HEX);
    }

    // Change EEProm
    Serial1.println("\n change radio mode in (L)ora or (F)sk") ;
    Serial1.println(" do U want to continue [L/F] ?");
    answer='a';

    do {
        delay(100);

        if (Serial1.available()) {
            answer = Serial1.read();
        }
    } while ((answer != 'L') && (answer != 'F'));

    if ('L' == answer) {
        ok = lora.setRadioMode(LORA_MODE);
        } else if ('F' == answer) {
        ok = lora.setRadioMode(FSK_MODE);
    }
    delay(100);

    */


    //endless loop
    while(1){
        //String data;
        if (lora.available()) {
           Serial1.print("\nRx> ");  
           Serial1.println(lora.read());

        }


        //lora.macTxCmd("0123");
        lora.macTxCmd("0123", 1, TX_ACK);
        delay(1000);
        ;
    }
}


