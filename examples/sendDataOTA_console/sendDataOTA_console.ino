/*
    SmarteEveryting Lion RN2483 Library - sendDataOTA_console

    This example shows how to configure and send messages after a OTA Join.
    Please consider the storeConfiguration example can be uset to store
    the required keys and skip the configuration part in curent example.
 
    created 25 Feb 2017
    by Seve (seve@ioteam.it)

    This example is in the public domain
    https://github.com/axelelettronica/sme-rn2483-library

    More information on RN2483 available here:
    http://www.microchip.com/wwwproducts/en/RN2483
    
 */

#include <Arduino.h>
#include <rn2483.h>


/* * INPUT DATA (for OTA)
 * 
 *  1) device EUI, 
 *  2) application EUI 
 *  3) and application key 
 *  
 *  then OTAA procedure can start.
 */
 
char c; 
char buff[100]  = {};
int  len        = 0;
unsigned char i = 0;

unsigned char buff_size = 100;

//30 seconds, default should be more than : 30000 is about 5 minutes;
static long LOOP_PERIOD = 3000; 
static long loop_cnt = LOOP_PERIOD - 300;  


void setup() {
    errE err;
    loraDbg = true;         // Set to 'true' to enable RN2483 TX/RX traces
    bool storeConfig = true; // Set to 'false' if persistend config already in place    
    
    ledYellowOneLight(LOW); // turn the LED off
    ledYellowTwoLight(HIGH); // turn the LED on
    delay(500);   
    Serial.begin(115200);

    lora.begin();
    // Waiting for the USB serial connection
    while (!Serial) {
        ;
    }


    Serial.print("FW Version :");
    Serial.println(lora.sysGetVersion());

    /* NOTICE: Current Keys configuration can be skipped if already stored
     *          with the store config Example
     */
    if (storeConfig) {
         // Write HwEUI
        Serial.println("Writing Hw EUI in DEV EUI ...");
        lora.macSetDevEUICmd(lora.sysGetHwEUI());

        if (err != RN_OK) {
            Serial.println("\nFailed writing Dev EUI");
        }
        
        Serial.println("Writing APP EUI ...");
        err = lora.macSetAppEUICmd("0000000000000001");
        if (err != RN_OK) {
            Serial.println("\nFailed writing APP EUI");
        }
        
        Serial.println("Writing Application Key ...");
        lora.macSetAppKeyCmd("ffffffffffffffffffffffffffff0000");
        if (err != RN_OK) {
            Serial.println("\nFailed writing raw APP Key"); 
        }
        
        Serial.println("Writing Device Address ...");
        err = lora.macSetDevAddrCmd(lora.sendRawCmdAndAnswer("mac get devaddr"));
        if (err != RN_OK) {
            Serial.println("\nFailed writing Dev Address");
        }
        
        Serial.println("Setting ADR ON ...");
        err = lora.macSetAdrOn();
        if (err != RN_OK) {
            Serial.println("\nFailed setting ADR");
        }
    }
    /* NOTICE End: Key Configuration */
    
    Serial.println("Setting Automatic Reply ON ...");
    err = lora.macSetArOn();
    if (err != RN_OK) {
        Serial.println("\nFailed setting automatic reply");
    }
    
    Serial.println("Setting Trasmission Power to Max ...");
    lora.macPause();
    err = lora.radioSetPwr(14);
    if (err != RN_OK) {
        Serial.println("\nFailed Setting the power to max power");
    }
    lora.macResume();

    delay(5000);
    ledYellowTwoLight(LOW); // turn the LED off 
}


void loop() {
  
    static int loop_cnt = 0;
    static int tx_cnt = 0;
    static bool joined = false;
    static uint32_t state;
    
    if (Serial.available()) {
        c = Serial.read();    
        Serial.write(c); 
        buff[i++] = c;
        if (c == '\n') {
            Serial.print(lora.sendRawCmdAndAnswer(buff));
            i = 0;
            memset(buff, 0, sizeof(buff));
        }
    }
    
    if (lora.available()) {
        //Unexpected data received from Lora Module;
        Serial.print("\nRx> ");  
        Serial.print(lora.read());
    }

    if (!(loop_cnt % LOOP_PERIOD)) { //5 minutes
       state = lora.macGetStatus();
       Serial.println(state, HEX);
       
       // Check If network is still joined
       if (MAC_JOINED(state)) {
           if (!joined) {
                Serial.println("\nOTA Network JOINED! ");
                joined = true;
           }
           // Sending String to the Lora Module towards the gateway
           ledYellowTwoLight(HIGH); // turn the LED on
           tx_cnt++;
       
           // Sending different data at any cycle
           if (tx_cnt == 1) {
               Serial.println("Sending Confirmed String ...");
               lora.macTxCmd(String("0123"), 1, TX_ACK); // Confirmed tx 
           } else if (tx_cnt == 2) {
               Serial.println("Sending Unconfirmed String ...");
               lora.macTxCmd("456");          // Unconfirmed tx String
           } else {
               const char tx_size = 3;
               char tx[tx_size] = {0x37,0x38,0x39};
               Serial.println("Sending Unconfirmed Buffer ...");
               lora.macTxCmd(tx, tx_size);   // Unconfirmed tx buffer
               tx_cnt = 0;
           }
           ledYellowTwoLight(LOW); // turn the LED off 
       } else {
           joined = false;
           lora.macJoinCmd(OTAA);
           delay(100);
       }
      loop_cnt = 0; 
    }
    loop_cnt++;
    delay(10);
}


