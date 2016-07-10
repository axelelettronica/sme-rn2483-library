#include <Arduino.h>
#include <Uart.h>
#include "../rn2483.h"
#include "rn2483Model.h"

bool loraDbg = false;  

static const char* s_hexTable[256] =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", "10", "11",
    "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", "30", "31", "32", "33", "34", "35",
    "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
    "5a", "5b", "5c", "5d", "5e", "5f", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
    "6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d",
    "7e", "7f", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", "a0", "a1",
    "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
    "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", "c0", "c1", "c2", "c3", "c4", "c5",
    "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
    "ea", "eb", "ec", "ed", "ee", "ef", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
    "fc", "fd", "fe", "ff"
};


// Convert binary data sequence [beginIt, endIt) to hexadecimal string
void
RN2483::dataToHexString(const char *const beginIt, const char *const endIt, String& str)
{
    //str.clear();
    str.reserve((endIt - beginIt) * 2);
    for(const char* it(beginIt); it != endIt; ++it) {
        str += s_hexTable[*it];
    }
}



void RN2483::begin() {
    iotAntenna.begin(57600);
    port = L_DEFAULT_PORT;
    memset (&rx, 0, sizeof(rnMsgT));
        //cur_dir = RX;
}

void RN2483::rawData(String stream) {
    answerLen =0; // reset Answer Counter
    stream.concat("\r\n");
    iotAntenna.print(stream);
    if (loraDbg) {
        SerialUSB.print(" ");
        SerialUSB.print((char *)stream.c_str());            
    }
}

 
errE RN2483::sendCmd(String stream)
{
    prepareAnswer((char *)tx.data, RN_BUFFER_LEN);
    rawData(stream);
    
    // remain till buffer is completed
    while (!hasAnswer()) {
        delay(10);
    };
    
    errE err = (checkAnswer(getLastAnswer()) ? RN_OK : RN_ERR);
    memset(&tx, 0, sizeof(tx));
    return err;
}

errE RN2483::sendRawCmd(String stream)
{
    return sendCmd(stream);
}
    
errE RN2483::sendData(char *data, uint16_t dataLen, int8_t portId, txModeE type)
{
    String msgStr(MAC_TX_CMD);
    uint8_t cmdPort = ((portId == L_CONFIGURED_PORT) ? port : portId);
    String dataStr;
 
    if(!data) return RN_ERR;
    
    msgStr.concat((type == TX_NOACK) ? "uncnf " : "cnf ");
    msgStr.concat(cmdPort);
    msgStr.concat(" ");
    dataToHexString((const char *const)data, (const char *const)(data+dataLen), dataStr);
    msgStr.concat(dataStr);
    //msgStr.concat("\r\n");
    rawData(msgStr);
    
    // remain till buffer is completed
    while (!hasAnswer()) {
        delay(10);
    };

    return (checkAnswer(getLastAnswer()) ? RN_OK : RN_ERR);
}
    
boolean RN2483::hasAnswer(void) {
    bool flush = true;

    while (iotAntenna.available()) {

        bufferAnswer[answerLen] = iotAntenna.read();
        if (flush && 
            ((bufferAnswer[answerLen] == '\r') || 
             (bufferAnswer[answerLen] == '\n'))) {
            continue;
        } else   
          flush = false;

        // the answer ends with the \n char
        if ((answerLen >1) && 
            (bufferAnswer[answerLen-1] == '\r') &&
            (bufferAnswer[answerLen] == '\n')) {
             if (loraDbg) {
                 SerialUSB.print("\nAnsw> ");
                 SerialUSB.print((char *)bufferAnswer);            
             }       
             delay(10);
             if (!iotAntenna.available()) {
                 bufferAnswer[answerLen-1] = '\0';
                 return true;
             }
        }
        answerLen++;

        // cannot receive more data than the allocated buffer
        if (answerLen >= bufferAnswerLen) {
            return true;
        }        
    }

    // nothing ready at the moment
    return false;
}

const char* RN2483::getVersion(void)
{
    if (initField.sw == 0) {
        // maybe to remove because new constructor
        prepareAnswer(swVer, SW_VER_LEN);

        // send request
        rawData(SYS_GET_VER);

        // remain till buffer is completed
        while (!hasAnswer()) {
            delay(10);
        };

        initField.sw = 1; //initialized
    }
    return swVer;
}


char RN2483::getUserEEprom(char address){
    int ret;
    char addPos = strlen(SYS_GET_NVM);
    char bufferT[GET_NVM_LEN];

    strcpy(bufferT,SYS_GET_NVM);

    itoa(address>>4, &bufferT[addPos], 16);
    itoa(address&0x0F, &bufferT[addPos+1], 16);

    prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);
    // send request
    rawData(bufferT);

    // remain till buffer is completed
    while (!lora.hasAnswer()) {
        delay(10);
    };

    // clean for 0xd & 0xa
    answer[2]=0;
    answer[3]=0;
    sscanf(answer, "%x", &ret);

    return ret;
}

bool RN2483::setUserEEprom(char address, char data){
    char addPos = strlen(SYS_SET_NVM);
    char bufferT[SET_NVM_LEN];

    prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);
    strcpy(bufferT,SYS_SET_NVM);

    itoa(address>>4, &bufferT[addPos], 16);
    itoa(address&0x0F, &bufferT[addPos+1], 16);
    bufferT[addPos+2]=' ';
    itoa(data>>4, &bufferT[addPos+3], 16);
    itoa(data&0x0F, &bufferT[addPos+4], 16);
    // send request
    rawData(bufferT);

    // remain till buffer is completed
    while (!hasAnswer()) {
        delay(10);
    };

    return checkAnswer(getLastAnswer());
}

int RN2483::getPower(void) {
    int ret;

    prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);
    // send request
    rawData(SYS_GET_VDD);

    // remain till buffer is completed
    while (!hasAnswer()) {
        delay(10);
    };

    sscanf(answer, "%4d", &ret);
    return ret;
}

const char * RN2483::getHwEUI(void)
{
    if (initField.hwEUI == 0) {
        // maybe to remove because new constructor
        prepareAnswer(hwEUI, RN_MAC_EUI_LEN);

        // send request
        rawData(SYS_GET_HWEUI);

        // remain till buffer is completed
        while (!hasAnswer()) {
            delay(10);
        };      
        initField.hwEUI = 1; //initialized
    }
    //String rtnStr(hwEUI);
    return hwEUI;
}



radioModeE RN2483::getRadioMode(void) {
    if (initField.radioMode==UnknownRadio) {
        // maybe to remove because new constructor
        prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);

        // send request
        rawData(RADIO_GET_MODE);

        // remain till buffer is completed
        while (!hasAnswer()) {
            delay(10);
        };

        if (answer[0] =='l') {
            initField.radioMode = LoRa;
        } else if (answer[0] =='f') {
            initField.radioMode = FSK;
        }
    }

    return initField.radioMode;
}

bool RN2483::setRadioMode (char *radioMode){

    //char addPos = strlen(SYS_SET_NVM);
    char bufferT[RADIO_MODE_LEN];
    strcpy(bufferT,RADIO_SET_MODE);

    memcpy(&bufferT[strlen(RADIO_SET_MODE)], radioMode, strlen(radioMode));

    prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);
    // send request
    rawData(bufferT);

    // remain till buffer is completed
    while (!hasAnswer()) {
        delay(10);
    };

    if (checkAnswer(answer)) {        
        // new conf reset initialion status
        initField.radioMode = UnknownRadio;
        return true;
    } else {
        return false;
    }
}

bool RN2483::checkAnswer(const char *answer){
    return (('o'==answer[0]) && ('k'==answer[1]));
}

/****************************************************************************/

//   Receiving Data
static bool _rx_ready = false;


char *
RN2483::getRxData(void)
{
    _rx_ready = false;
    return (char*)rx.data;
}

void
RN2483::handleRxData(uint8_t inChar)
{   
    if (rx.idx>= (RN_BUFFER_LEN-1)) return;
    
    if (/*cur_dir == RX && */!_rx_ready) {
       rx.data[rx.idx] = inChar;
       if ((rx.idx>1) && 
           (rx.data[rx.idx-1]  == '\r') &&
           (rx.data[rx.idx]  == '\n')) {
          
           _rx_ready = true;
           return; // rewind
       }
       rx.idx++;
    }
}

 bool
 RN2483::rxDataReady(void)
 {
     // read if rx data is ready
     return _rx_ready;
 }
 
bool
RN2483::available(void)
 {

     while (iotAntenna.available()) {
         // get the new byte
         char inChar = (char)iotAntenna.read();
         handleRxData(inChar);
         
         if (rxDataReady()) {
             return true;
         }
     }
     return false;
}
 
 const char*
 RN2483::read(void)
 {
     String msgStr;
     msgStr.concat(getRxData());
     memset(&rx, 0, sizeof(rx));
     return msgStr.c_str();
 }

/****************************************************************************/
/*****************************   MAC Commands *******************************/

errE 
RN2483::macSetDevEUICmd(String stream)
{
    String msgStr(MAC_SET_DEV_EUI);
    msgStr.concat(stream);
    initField.hwEUI = 0;
    return sendCmd(msgStr); 
}

errE
RN2483::macSetAppEUICmd(String stream)
{
    String msgStr(MAC_SET_APP_EUI);
    msgStr.concat(stream);
    initField.macAppEUI = 0;
    return sendCmd(msgStr);
}
  
errE
RN2483::macSetNtwSessKeyCmd(String stream)
{
    String msgStr(MAC_SET_NTW_SESS_KEY);
    msgStr.concat(stream);
    return sendCmd(msgStr);
}

errE
RN2483::macSetAppSessKeyCmd(String stream)
{
    String msgStr(MAC_SET_APP_SESS_KEY);
    msgStr.concat(stream);
    return sendCmd(msgStr);
}

errE
RN2483::macSetAppKeyCmd(String stream)
{
    String msgStr(MAC_SET_APP_KEY);
    msgStr.concat(stream);
    return sendCmd(msgStr);
}
  

const char*
RN2483::getMacAppEUI(void)
{
    if (initField.macAppEUI == 0) {
        // maybe to remove because new constructor
        prepareAnswer(macAppEUI, RN_MAC_EUI_LEN);

        // send request
        rawData(MAC_GET_APP_EUI);

        // remain till buffer is completed
        while (!hasAnswer()) {
            delay(1);
        };
        initField.macAppEUI = 1; //initialized
    }

    return macAppEUI;
}


errE RN2483::macResetCmd(bandE band)
{
    String msgStr(MAC_RESET_CMD);
    msgStr.concat(((band == BAND_868) ? "868" : "433"));

    return sendCmd(msgStr);       
}


errE RN2483::macTxCmd(String stream, int8_t portId, txModeE type)
{
    String msgStr(MAC_TX_CMD);
    uint8_t cmdPort = ((portId == L_CONFIGURED_PORT) ? port : portId);
    String dataStr;
    
    msgStr.concat((type == TX_NOACK) ? "uncnf " : "cnf ");
    msgStr.concat(cmdPort);
    msgStr.concat(" ");
    dataToHexString(stream.begin(), stream.end(), dataStr);
    msgStr.concat(dataStr);

    return sendCmd(msgStr);
}

errE RN2483::macJoinCmd(joinModeE mode)
{
    String msgStr(MAC_JOIN_CMD);
    msgStr.concat(((mode == OTAA) ? "otaa" : "abp"));

    return sendCmd(msgStr);
}


RN2483 lora;
