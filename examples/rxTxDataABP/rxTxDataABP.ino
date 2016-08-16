#include <Arduino.h>
#include <rn2483.h>

/*
 * INPUT DATA (for ABP)
 *  1) DevEUI
 *  2) AppEUI
 *  3) AppKey
 *
 * Once these 3 data are written in the network server, this one 
 * provide the remaining infos for join the connection
 *
 *  1) DevAddr
 *  2) NwkSKey
 *  3) AppSKey
 *
 *  So, in the end, in the setup phase the module has to set all
 *  these 6 parameters before issuing a "mac join abp"
 *
 */

void setup() {

    bool err = false;
    loraDbg = true;
    
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

     // Write HwEUI
    SerialUSB.println("Writing DEV EUI ...");
    //err = lora.macSetDevEUICmd("0004A30B001A2A9E");
    err = lora.sendRawCmd("mac set deveui ffffffffffff0000");
    if (err) {
        SerialUSB.println("\nFailed writing Dev EUI");
    }

    
    SerialUSB.println("Writing APP EUI ...");
        err = lora.sendRawCmd("mac set appeui ffffffffffff0000");
    //err = lora.macSetAppEUICmd("0000000000000001");
    if (err) {
        SerialUSB.println("\nFailed writing APP EUI");
    }

    err = lora.macSetNtwSessKeyCmd("2b7e151628aed2a6abf7158809cf4f3c");
    if (err) {
        SerialUSB.println("\nFailed writing Network Session Key");
    }

    err = lora.macSetAppSessKeyCmd("2b7e151628aed2a6abf7158809cf4f3c");
    if (err) {
        SerialUSB.println("\nFailed writing APP Session Key");
    }

    lora.macSetAppKeyCmd("ffffffffffffffffffffffffffff0000");

    if (err) {
        SerialUSB.println("\nFailed writing raw APP Key"); 
    }
    //err = lora.sendRawCmd("mac set devaddr 001a2a9e");
    //err = lora.sendRawCmd("mac set devaddr 001A2A9E");
    //err = lora.sendRawCmd("mac set devaddr 00000000");
    err = lora.sendRawCmd("mac set devaddr ffff0001");
    if (err) {
        SerialUSB.println("\nFailed writing Dev Address");
    }

    err = lora.sendRawCmd("mac set ar on");
    if (err) {
        SerialUSB.println("\nFailed settin automatic reply");
    }
    
    while (lora.macJoinCmd(ABP) /* lora.sendRawCmd("mac join abp")*/) {
        SerialUSB.println("\nABP JOIN FAILED ");
        delay(5000);
    }     
    SerialUSB.println("\nABP Network JOINED! ");
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
      }
    }

    if (lora.available()) {
        //String data received from Lora Module;
        SerialUSB.print("\nRx> ");  
        SerialUSB.print(lora.read());
    }

    if (!(loop_cnt % 5000)) {
    // Sending String to the Lora Module towards the gateway
       lora.sendRawCmd("mac pause");
       delay(10);
       lora.sendRawCmd("mac resume");
     //  lora.macTxCmd("0123", 1, TX_ACK);
       lora.macTxCmd("0123");
       //lora.macTxCmd("0123", 1);
    } else {
      //lora.macTxCmd("0");   
    }
    loop_cnt++;
    delay(10);
}


