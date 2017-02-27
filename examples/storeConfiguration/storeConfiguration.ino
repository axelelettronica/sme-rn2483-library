/*
    SmarteEveryting Lion RN2483 Library - storeConfigurtion

    This example shows how to prepare the RN2483 for the next examples.
    In particular, it stores :
    - the keys for the OTA join (you can modify it to store the ABP required 
      keys as well).
    - some user data.
    - Mac configuration paameters
 
    created 25 Feb 2017
    by Seve (seve@ioteam.it)

    This example is in the public domain
    https://github.com/axelelettronica/sme-rn2483-library

    More information on RN2483 available here:
    http://www.microchip.com/wwwproducts/en/RN2483
    
 */

 
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
 */
 
#include <Arduino.h>
#include <rn2483.h>

char buffer[100];

bool otaConfig = true;

const char *APP_EUI   = "0000000000000001";
const char *APP_KEY   = "01020304050607080102030";

const char *DEV_ADDR  = "001a2a9e";
const char *NTW_S_KEY = "01020304050607080102030sawdwscef";
const char *APP_S_KEY = "01020304050607080102030sawdwscef";

void setup() {

    loraDbg = false;      // Set to 'true' to enable RN2483 TX/RX traces
     
    Serial.begin(115200);
    lora.begin();
    delay(100);

    while (!Serial) {
        ;
    }
}


void loop() {
    errE ok;
    const char *s;
    char answer;
    
    Serial.println("Current RN2483 persistent configuration will be replaced");
    Serial.println("do you want to proceed [Y/N] ?");
    do {
        delay(100);

        if (Serial.available()) {
            answer = Serial.read();
        }
    } while ((answer != 'Y') && (answer != 'y')&& 
             (answer != 'N') && (answer != 'n'));

    if (('N' == answer) || ('n' == answer)) {
      return;
    }  

    lora.sysFactoryReset();
    
    // Setting Dev EUI
    Serial.print("\nHW EUI          : ");
    Serial.println(lora.sysGetHwEUI());
        

    Serial.print("MAC DEV-EUI     : ");
    Serial.println(lora.sendRawCmdAndAnswer("mac get deveui"));
    Serial.println("Using Hw Eui as Dev eui: ...");    
    lora.macSetDevEUICmd(lora.sysGetHwEUI());
    s = lora.sendRawCmdAndAnswer("mac get deveui");
    Serial.print("NEW MAC DEV-EUI : ");
    Serial.println(s);

    // Setting App EUI
    lora.macSetAppEUICmd(APP_EUI);
    Serial.print("MAC APP-EUI     : ");
    Serial.println(lora.sendRawCmdAndAnswer("mac get appeui"));

    // Setting App Key
    lora.macSetAppKeyCmd(APP_KEY);
    Serial.print("MAC APP-KEY     : ");
    Serial.println(APP_KEY);

    if (!otaConfig) {   

        // Setting Dev Address
        lora.macSetDevAddrCmd(DEV_ADDR);
        Serial.print("MAC DEV-ADDR     : ");
        Serial.println(DEV_ADDR);
        
        // Setting App Session Key 
        lora.macSetAppSessKeyCmd(APP_S_KEY);
        Serial.print("MAC APP_SES_KEY  : ");
        Serial.println(APP_S_KEY);
        
        // Setting App Networks Session Key 
        lora.macSetNtwSessKeyCmd(NTW_S_KEY);
        Serial.print("MAC NTW_S_KEY: ");
        Serial.println(NTW_S_KEY);
    }
    lora.macSetAdrOn();   
    s = lora.sendRawCmdAndAnswer("mac get adr");
    Serial.print("Setting MAC Adaptive DR : ");
    Serial.println(s);
    
    // Storing char the EEprom
    Serial.println("\nYou can store User Information in the RN2483 internal EEPROM:\n");
    Serial.println("Writing 0x01 into address 0x300");    
    lora.sysSetUserEEprom(0x300, 0x01);
    Serial.println("Writing 0x00 into address 0x31A"); 
    lora.sysSetUserEEprom(0x31A, 00);
    Serial.println("Writing 0x7E into address 0x3FF"); 
    lora.sysSetUserEEprom(0x3FF, 0x7E);
    Serial.print("Reading EEProm Address 0x300   : ");
    Serial.println(lora.sysGetUserEEprom(0x300), HEX);
    Serial.print("Reading EEProm Address 0x31A   : ");
    Serial.println(lora.sysGetUserEEprom(0x31A), HEX);
    Serial.print("Reading EEProm Address 0x3FF   : ");
    Serial.println(lora.sysGetUserEEprom(0x3FF), HEX);



    lora.macSave();
    Serial.println("\nNew configuration Stored!!!");
        
    //endless loop
    while(1){
        ;
    }
}


