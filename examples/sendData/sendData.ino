#include <Arduino.h>
#include <rn2483.h>

char buffer[100];

void setup() {

    bool err = false;
    //loraDbg = true;
    
    SerialUSB.begin(115200);

    lora.begin();
    delay(100);
    
    // Waiting for the USB serial connection
    while (!SerialUSB) {
        ;
    }

    // first time get from Hw
    lora.getVersion();
    
    SerialUSB.print("FW Version :");
    SerialUSB.println(lora.getVersion());

    // Read Power
    SerialUSB.print("\nVdd is: ");
    SerialUSB.print(lora.getPower());
    SerialUSB.println(" mV");

    // Read HwEUI
    SerialUSB.print("DEV EUI :");
    SerialUSB.println(lora.getHwEUI());

    // Read AppEUI
    SerialUSB.print("APP EUI :");
    SerialUSB.println(lora.getMacAppEUI());
    
    // Write HwEUI
    SerialUSB.println("Writing DEV EUI ...");
    err = lora.macSetDevEUICmd("0004A30B001A2A9E");
    if (err) {
        SerialUSB.println("\Failed writing Dev EUI");
    }

    
    // Write AppEUI
    SerialUSB.println("Writing APP EUI ...");
    err = lora.macSetAppEUICmd("FEDCBA9876543210");
    if (err) {
        SerialUSB.println("\nFailed writing APP EUI");
    }

    err = lora.macSetNtwSessKeyCmd("1029384756AFBECD5647382910DACFEB");
   // err = lora.macSetNtwSessKeyCmd("00112233445566778899AABBCCDDEEFF");
    if (err) {
        SerialUSB.println("\nFailed writing Network Session Key");
    }
    
    err = lora.macSetAppSessKeyCmd("00112233445566778899AABBCCDDEEFF");
    if (err) {
        SerialUSB.println("\nFailed writing APP Session Key");
    }
    err = lora.macSetAppKeyCmd("00112233445566778899AABBCCDDEEFF");
    if (err) {
        SerialUSB.println("\nFailed writing APP Key");
    }


    /* Read HwEUI
     * SerialUSB.print("DEV EUI :");
     * SerialUSB.println(lora.getHwEUI());
     */

    // Read AppEUI
    SerialUSB.print("APP EUI :");
    SerialUSB.println(lora.getMacAppEUI());   
    
    // Read Radio modulation Mode
    SerialUSB.print("\nRadio is in mode: ");
    radioModeE radio = lora.getRadioMode();
    if (radio == LoRa) {
        SerialUSB.println("LoRa");
    } else if (radio == FSK) {
        SerialUSB.println("FSK");
    }
    

    err = lora.sendRawCmd("mac set appkey 00112233445566778899AABBCCDDEEFF");
    if (err) {
        SerialUSB.println("\nFailed writing raw APP Key");
    }

    //while (lora.macJoinCmd(OTAA)) {
    while (lora.sendRawCmd("mac join otaa")) {
        SerialUSB.println("\nOTA JOIN FAILED ");
        delay(5000);
    }     
    SerialUSB.println("\nOTA Network JOINED! ");

}

void loop() {

    if (lora.available()) {
        //String data received from Lora Module;
        SerialUSB.print("\nRx> ");  
        SerialUSB.print(lora.read());
    }

    // Sending String to the Lora Module towards the gateway
    lora.macTxCmd("0123");
    //lora.macTxCmd("0123", 1, TX_ACK);
    delay(1000);
}


