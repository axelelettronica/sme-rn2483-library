#include "../rn2483.h"
#include "rn2483Model.h"

void RN2483::init() {
    SigFox.begin(57600);
}

void RN2483::rawData(String stream) {
    answerLen =0; // reset Answer Counter
    stream.concat("\r\n");
    SigFox.print(stream);
}

boolean RN2483::hasAnswer(void) {
    if (SigFox.available()) {
        bufferAnswer[answerLen] = SigFox.read();

        // the answer ends with the \n char
        if (bufferAnswer[answerLen] == 0x0a) {
            return true;
        }
        answerLen++;

        // cannot receive more data than the allocated buffer
        if (answerLen > bufferAnswerLen)
            return true;
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

uint32_t test;
const char* RN2483::getHwEUI(void)
{
    if (initField.hwEUI == 0) {
        // maybe to remove because new constructor
        prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);

        // send request
        rawData(SYS_GET_HWEUI);

        // remain till buffer is completed
        while (!hasAnswer()) {
            delay(10);
        };

        int i=0;
        test = strtol(answer, NULL, 16);
        for (;i<16;i++){
            test = atoi(&answer[i]);
        }
        initField.hwEUI = 1; //initialized
    }
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

    char addPos = strlen(SYS_SET_NVM);
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

RN2483 lora;
