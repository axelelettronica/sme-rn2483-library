/* SmartEverything RN2483 Library
 * Copyright (C) 2017 by IOTEAM
 *
 * This file is part of the SmartEverything Arduino RN2483 Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SmartEverything RN2483 Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *  Design:      seve@ioteam.it
 */

#include "../rn2483.h"
#include "rn2483Model.h"

volatile bool loraDbg = false; 
 
typedef struct answer_t {
    uint8_t id;
    char *str;
} answerCodesType;


const char asw_1_str[]  = "ok";
const char asw_2_str[]  = "invalid_param";
const char asw_3_str[]  = "not_joined";
const char asw_4_str[]  = "no_free_ch";
const char asw_5_str[]  = "silent";
const char asw_6_str[]  = "frame_counter_err_rejoin_needed";
const char asw_7_str[]  = "busy";
const char asw_8_str[]  = "mac_paused";
const char asw_9_str[]  = "invalid_data_len";
const char asw_10_str[]  = "keys_not_init";
const char asw_11_str[]  = "denied";
const char asw_12_str[]  = "accepted";
const char asw_13_str[]  = "radio_err";
const char asw_14_str[]  = "radio_tx_ok";
const char asw_15_str[]  = "mac_tx_ok";
const char asw_16_str[]  = "mac_err";
const char asw_17_str[]  = "";

const char asw_part_1_str[]  = "mac_rx ";

const char * answerCodes[ASW_CODES_NUM]  = {
    asw_1_str,
    asw_2_str,
    asw_3_str,
    asw_4_str,
    asw_5_str,
    asw_6_str,
    asw_7_str,
    asw_8_str,
    asw_9_str,
    asw_10_str,
    asw_11_str,
    asw_12_str,
    asw_13_str,
    asw_14_str,
    asw_15_str,
    asw_16_str,
    asw_17_str
};


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


inline bool LORA_NTW_JOINED( uint32_t status)
{
    return (status & 0x1);
}

// Convert binary data sequence [beginIt, endIt) to hexadecimal string
void
RN2483::dataToHexString(const char *const beginIt, const char *const endIt, String& str)
{
    str.reserve((endIt - beginIt) * 2);
    for(const char* it(beginIt); it != endIt; ++it) {
        str += s_hexTable[(const unsigned char)*it];
    }
}


void RN2483::rawData(String stream) {
    stream.concat("\r\n");
    comm->print(stream);
    if (loraDbg) {
        SerialUSB.print("-> ");
        SerialUSB.print((char *)stream.c_str());            
    }
}


uint8_t RN2483::checkAnswer(const char *answer)
{
    for (uint8_t i = 0; i < ASW_CODES_NUM; ++i) {
        if (!strncmp(answer, (answerCodes[i]), strlen((answerCodes[i])))) {
            return i;
        }
    }

    return ASW_STR;
}


errE RN2483::sendData(char *data, uint16_t dataLen, int16_t portId, txModeE type)
{
    String msgStr(MAC_TX_CMD);
    uint16_t cmdPort = ((portId == L_CONFIGURED_PORT) ? port : portId);
    String dataStr;
    volatile uint8_t answ = ASW_ERR;
     
    if(!data) return RN_ERR;
    
    msgStr.concat((type == TX_NOACK) ? "uncnf " : "cnf ");
    msgStr.concat(cmdPort);
    msgStr.concat(" ");
    dataToHexString((const char *const)data, (const char *const)(data+dataLen), dataStr);
    msgStr.concat(dataStr);

    if (ASW_OK == sendCmd(msgStr)) {
        if (type == TX_NOACK) {
            return RN_OK;
        }
        // wait for confirmation from the server;
        answ = waitAnswer();
        if (answ == ASW_TX_OK) {
            return RN_OK;
        } else {
            return RN_ERR;
        }
    }
    return RN_ERR;
        
}
     
uint8_t RN2483::sendCmd(String stream)
{
    rawData(stream);
    waitAnswer();
    return checkAnswer(getLastAnswer());
}


uint8_t RN2483::waitAnswer(void) 
{
    volatile unsigned int c = 0;
    
    tx.init();
    
    while (!comm->available())
        ;
       
    do {
           // cannot receive more data than the allocated buffer
        if (tx.data[tx.idx] >= tx.len) {
            return RN_ERR;
        }
        if (comm->available()) {
            c = comm->read();
            tx.data[tx.idx++] = c;
        } else {
            delay(10);
        }
    } while ((c != '\n'));

    if (loraDbg) {
        SerialUSB.print("<- ");
        SerialUSB.print((char *)tx.data);
    }
    if (tx.idx > 2) {
        tx.data[tx.idx-2] = '\0'; // remove \r
        tx.data[tx.idx-1] = '\0'; // remove \n
    }
    return checkAnswer(getLastAnswer());
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

/*****************************************************************************/
/***************************       Public APIs      **************************/
/*****************************************************************************/
 
 /***************************** Generic Commands *****************************/
 const char*
 RN2483::read(int *len)
 {
     String msgStr;
     msgStr.concat(getRxData());
     memset(&rx, 0, sizeof(rx));
     *len = msgStr.length();
     return msgStr.c_str();
 }

 const char*
 RN2483::read(void)
 {
     String msgStr;
     msgStr.concat(getRxData());
     memset(&rx, 0, sizeof(rx));
     return msgStr.c_str();
 }

 RN2483::RN2483(void)
 {
    comm = NULL;
    port = L_DEFAULT_PORT;
    rx.init();
    tx.init();
 }

void RN2483::begin(long speed, Uart *serial) {
    this->comm = serial;
    comm->begin(speed);


}
 
bool
RN2483::available(void)
{

     while (comm->available()) {
         // get the new byte
         char inChar = (char)comm->read();
         handleRxData(inChar);
         
         if (rxDataReady()) {
             return true;
         }
     }
     return false;
 }
const char* RN2483::getLastAnswer(void)
{
    return (const char *)tx.data;
}


errE RN2483::sendRawCmd(String stream)
{
    uint8_t ret = sendCmd(stream);
    
    switch (ret) {       
    case ASW_OK:
    case ASW_STR:
    case ASW_JOIN_ACCEPTED:
    case ASW_RADIO_TX_OK:
    case ASW_TX_OK:
        return RN_OK;
        
    case ASW_RADIO_ERR:        
    case ASW_INVALID_PARAM:
    case ASW_NOT_JOINED:
    case ASW__NO_FREE_CHAN:
    case ASW_SILENT:
    case ASW_REJOIN_NEEDED:
    case ASW_BUSY:
    case ASW_PAUSED:
    case ASW_INVALID_LEN:
    case ASW_KEYS_NOT_INIT:
    case ASW_JOIN_DENIED:
    case ASW_ERR:
    default:
        return RN_ERR;
    }    
}

const char * RN2483::sendRawCmdAndAnswer(String stream)
{
    //if (ASW_STR == sendCmd(stream)) {
    //    return getLastAnswer();
    //}
    //return "ERR";
    sendCmd(stream);
    return getLastAnswer();
}




/*****************************   MAC Commands *******************************/

errE 
RN2483::macSetDevEUICmd(String stream)
{
    String msgStr(MAC_SET_DEV_EUI);
    msgStr.concat(stream);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    } 
}

errE
RN2483::macSetDevAddrCmd(String stream)
{
    String msgStr(MAC_SET_DEV_ADDR);
    msgStr.concat(stream);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
        } else {
        return RN_ERR;
    }
}

errE
RN2483::macSetAppEUICmd(String stream)
{
    String msgStr(MAC_SET_APP_EUI);
    msgStr.concat(stream);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }
}
  
errE
RN2483::macSetNtwSessKeyCmd(String stream)
{
    String msgStr(MAC_SET_NTW_SESS_KEY);
    msgStr.concat(stream);
    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }
}

errE
RN2483::macSetAppSessKeyCmd(String stream)
{
    String msgStr(MAC_SET_APP_SESS_KEY);
    msgStr.concat(stream);
    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }    
}

errE
RN2483::macSetAppKeyCmd(String stream)
{
    String msgStr(MAC_SET_APP_KEY);
    msgStr.concat(stream);
    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }
}
  

const char*
RN2483::getMacAppEUI(void)
{
   if (ASW_STR == sendCmd(MAC_GET_APP_EUI)) {
        return getLastAnswer();
    }
    return "ERR";
}

const char* RN2483::macGetStatusStr(void)
{
    // send request
    if (ASW_STR == sendCmd(MAC_GET_STATUS)) {
        return getLastAnswer();
    }
    return "ERR";
}

uint32_t RN2483::macGetStatus(void)
{
    uint32_t status = 0;
    const char *s = macGetStatusStr();
    if (s[0] == 'E') {
        return status;
    }
    
    status = (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3]; 

    return status;
}

const char* RN2483::macPause(void)
{
    // send request
    if (ASW_STR == sendCmd(MAC_PAUSE_CMD)) {
        return getLastAnswer();
    }
    return "ERR";
}

errE RN2483::macResetCmd(bandE band)
{
    String msgStr(MAC_RESET_CMD);
    msgStr.concat(((band == BAND_868) ? "868" : "433"));

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }      
}


errE RN2483::macTxCmd(String stream, int16_t portId, txModeE type)
{
    String msgStr(MAC_TX_CMD);
    uint16_t cmdPort = ((portId == L_CONFIGURED_PORT) ? port : portId);
    String dataStr;
    
    msgStr.concat((type == TX_NOACK) ? "uncnf " : "cnf ");
    msgStr.concat(cmdPort);
    msgStr.concat(" ");
    dataToHexString(stream.begin(), stream.end(), dataStr);
    msgStr.concat(dataStr);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }
}

errE RN2483::macTxCmd(char *data, int16_t len, int16_t portId, txModeE type)
{
    String msgStr(MAC_TX_CMD);
    uint16_t cmdPort = ((portId == L_CONFIGURED_PORT) ? port : portId);
    String dataStr;
    
    msgStr.concat((type == TX_NOACK) ? "uncnf " : "cnf ");
    msgStr.concat(cmdPort);
    msgStr.concat(" ");
    dataToHexString(data, data+len, dataStr);
    msgStr.concat(dataStr);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }
}


errE RN2483::macJoinCmd(joinModeE mode)
{
    String msgStr(MAC_JOIN_CMD);
    msgStr.concat(((mode == OTAA) ? JOIN_OTA_MODE : JOIN_ABP_MODE));

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    } 
}

errE RN2483::macSetDataRate(uint8_t dataRate)
{
    String msgStr(MAC_SET_DATARATE);
    String dataStr;
    
    msgStr.concat(dataRate);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }    
}


/*****************************   SYS Commands *******************************/

errE RN2483::sysSleepCmd (uint32_t msec)
{
    String msgStr(SYS_SLEEP);
    String dataStr;
    
    msgStr.concat(msec);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
        } else {
        return RN_ERR;
    }
}

const char* RN2483::sysGetVersion(void)
{
    // send request
    if (ASW_STR == sendCmd(SYS_GET_VER)) {
        return getLastAnswer();
    }
    return "ERR";
}

const char* RN2483::sysReset(void)
{
    // send request
    if (ASW_STR == sendCmd(SYS_RESET)) {
        return getLastAnswer();
    }
    return "ERR";
}

const char* RN2483::sysFactoryReset(void)
{
    // send request
    if (ASW_STR == sendCmd(SYS_FACTORY_RESET)) {
        return getLastAnswer();
    }
    return "ERR";
}


char RN2483::sysGetUserEEprom(char address){
    int ret;
    unsigned char addPos = strlen(SYS_GET_NVM);
    char bufferT[GET_NVM_LEN];

    strcpy(bufferT,SYS_GET_NVM);

    itoa(address>>4, &bufferT[addPos], 16);
    itoa(address&0x0F, &bufferT[addPos+1], 16);

    sendCmd(bufferT);

    // clean for 0xd & 0xa
    const char *pbuf = getLastAnswer();

    sscanf(pbuf, "%x", &ret);

    return ret;
}

bool RN2483::sysSetUserEEprom(char address, char data){
    unsigned char addPos = strlen(SYS_SET_NVM);
    char bufferT[SET_NVM_LEN];

    strcpy(bufferT,SYS_SET_NVM);

    itoa(address>>4, &bufferT[addPos], 16);
    itoa(address&0x0F, &bufferT[addPos+1], 16);
    bufferT[addPos+2]=' ';
    itoa(data>>4, &bufferT[addPos+3], 16);
    itoa(data&0x0F, &bufferT[addPos+4], 16);

    return sendCmd(bufferT);
}

const char * RN2483::sysGetVdd(void) 
{    
    if (ASW_STR == sendCmd(SYS_GET_VDD)) {
        return getLastAnswer();
    } else
        return "ERR";
}

const char * RN2483::sysGetHwEUI(void)
{
    if (ASW_STR == sendCmd(SYS_GET_HWEUI)) {
        return getLastAnswer();
    }
    return "ERR";
}

/*****************************  Radio Commands ******************************/


errE RN2483::radioSetSync(uint8_t sync)
{
    String msgStr(RADIO_SET_SYNC);
    String dataStr;
    
    msgStr.concat(sync);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
        } else {
        return RN_ERR;
    }
}
errE RN2483::radioSetPwr(uint8_t pwr)
{
    String msgStr(RADIO_SET_PWR);
    String dataStr;
    
    msgStr.concat(pwr);

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
        } else {
        return RN_ERR;
    }
}


const char*
RN2483::radioGetPwr(void)
{
    if (ASW_STR == sendCmd(RADIO_GET_PWR)) {
        return getLastAnswer();
    }
    return "ERR";
}



radioModeE RN2483::radioGetMode(void)
{
    const char *answ;
    if (ASW_STR == sendCmd(RADIO_GET_MODE)) {
        answ = getLastAnswer();
        if (*answ =='l') {
            return LoRa;
            } else if (*answ =='f') {
            return FSK;
        }
    }

    return UnknownRadio;
}

errE RN2483::radioSetMode (radioModeE radioMode)
{
    String msgStr(RADIO_SET_MODE);
    msgStr.concat(((radioMode == LoRa) ? "lora" : "fsk"));

    if (sendCmd(msgStr) == ASW_OK) {
        return RN_OK;
    } else {
        return RN_ERR;
    }

}
    
RN2483 lora;
