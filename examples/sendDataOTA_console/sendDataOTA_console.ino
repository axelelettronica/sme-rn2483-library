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
 * * INPUT DATA (for OTA)
 * 
 *  1) device EUI, 
 *  2) application EUI 
 *  3) and application key 
 *  the OTAA procedure can start.
 *  
 *  - sys get hweui 0004A30B001B2bla
 *  
 *  - mac set deveui 0004A30B001B2bla
 *  - mac set appkey 01020304050607080102030blablabla //same as in ttncl
 *  - mac set appeui 70B3D57ED0000bla //same as ttncl
 *  - mac set adr off
 *  - mac set rx2 3 869525bla
 *  - mac save
 *  - mac join otaa
ok
**mac tx uncnf 1 7B39387D**
 */

bool joined = false;
int len = 0;
void setup() {

    bool err = false;
    loraDbg = true;
    ledYellowOneLight(LOW); // turn the LED off
    ledYellowTwoLight(HIGH); // turn the LED on
    delay(500);   
    SerialUSB.begin(115200);

    lora.begin();
    // Waiting for the USB serial connection
    while (!SerialUSB) {
        ;
    }


    // first time get from Hw
    lora.getVersion();
    
    SerialUSB.print("FW Version :");
    SerialUSB.println(lora.getVersion());

     // Write HwEUI
    SerialUSB.println("Writing DEV EUI ffffffffffff0000");
    //err = lora.macSetDevEUICmd("0004A30B001A2A9E");
    err = lora.sendRawCmd("mac set deveui ffffffffffff0000");
    if (err) {
        SerialUSB.println("\nFailed writing Dev EUI");
    }

    
    SerialUSB.println("Writing APP EUI ffffffffffff0000");
        err = lora.sendRawCmd("mac set appeui ffffffffffff0000");
    //err = lora.macSetAppEUICmd("0000000000000001");
    if (err) {
        SerialUSB.println("\nFailed writing APP EUI");
    }
/*
    err = lora.macSetNtwSessKeyCmd("2b7e151628aed2a6abf7158809cf4f3c");
    if (err) {
        SerialUSB.println("\nFailed writing Network Session Key");
    }

    err = lora.macSetAppSessKeyCmd("2b7e151628aed2a6abf7158809cf4f3c");
    if (err) {
        SerialUSB.println("\nFailed writing APP Session Key");
    }
*/
    SerialUSB.println("Writing APP KEY ffffffffffffffffffffffffffff0000");
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
    //err = lora.sendRawCmd("mac set adr on");
    //if (err) {
    //    SerialUSB.println("\nFailed settin automatic reply");
    //}
    delay(30000);
   
    ledYellowTwoLight(LOW); // turn the LED off 
    //SEVE    
           lora.sendRawCmd("mac pause");
       delay(10);
       lora.sendRawCmd("mac resume");
              delay(10);
    while (lora.macJoinCmd(OTAA)) {
                 lora.sendRawCmd("mac pause");
       delay(10);
       lora.sendRawCmd("mac resume");
              delay(10);
        SerialUSB.println("\nOTA JOIN FAILED ");
        delay(5000);
    }     
    SerialUSB.println("\nOTA Network JOINED! ");
    delay(25000);
}

uint8_t buff_size = 100;
char buff[100] = {};
uint8_t i = 0;
char c;

static long LOOP_PERIOD = 30000;
static long loop_cnt = LOOP_PERIOD - 300;  
void loop() {

    
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
        memcpy(buff, lora.read(&len), len);
        //SerialUSB.print(lora.read(&len));
        SerialUSB.print(buff);
        if (!joined && (len > 6) && 
            (buff[0] == 'a') &&
            (buff[1] == 'c') && (buff[1] == 'c')) {
              joined = true;
            }
    }

    if (!(loop_cnt % LOOP_PERIOD)) { //5 minuti
       if (joined) {
       // Sending String to the Lora Module towards the gateway
       lora.sendRawCmd("mac pause");
       delay(10);
       lora.sendRawCmd("mac resume");
       ledYellowTwoLight(HIGH); // turn the LED on
       lora.macTxCmd("0123", 1, TX_ACK);
       // SEVE
       //lora.macTxCmd("0123",1);
       delay(1000);
       ledYellowTwoLight(LOW); // turn the LED off 
       } else {
       lora.sendRawCmd("mac pause");
       delay(10);
       lora.sendRawCmd("mac resume");
              delay(10);
        lora.macJoinCmd(OTAA);
       }
      loop_cnt = 0; 
    }
    loop_cnt++;
    delay(10);
}


