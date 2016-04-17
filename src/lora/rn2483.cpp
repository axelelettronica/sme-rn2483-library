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
    if (swVer[0] == 0) {
        // maybe to remove because new constructor
        prepareAnswer(swVer, SW_VER_LEN);

        // send request
        rawData(SYS_GET_VER);

        // remain till buffer is completed
        while (!lora.hasAnswer()) {
            delay(10);
        };
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

const char* RN2483::setUserEEprom(char address, char data){
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
    while (!lora.hasAnswer()) {
        delay(10);
    };

    memset(answer,0,3);
    memcpy(answer, lora.getLastAnswer(), 2);

    return answer;
}

int RN2483::getPower(void) {
    int ret;

    prepareAnswer(answer, SMALL_ANSWER_DATA_LEN);
    // send request
    rawData(SYS_GET_VDD);

    // remain till buffer is completed
    while (!lora.hasAnswer()) {
        delay(10);
    };

    sscanf(answer, "%4d", &ret);
    return ret;
}



RN2483 lora;
