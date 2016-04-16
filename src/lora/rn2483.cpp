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
        char buffer[SW_VER_LEN];
        // maybe to remove because new contructor
        memset(buffer,0,SW_VER_LEN);
        prepareAnswer(buffer, SW_VER_LEN);

        // send request
        rawData(SYS_GET_VER);

        // remain till buffer is completed
        while (!lora.hasAnswer()) {
            delay(10);
        };
        memcpy(swVer, lora.getLastAnswer(), strlen(lora.getLastAnswer()));
    }
    return swVer;
}


const char* RN2483::getUserEEprom(char address){
    char addPos = strlen(SYS_GET_NVM);
    char bufferT[GET_NVM_LEN];

    prepareAnswer(nvm, NVM_DATA_LEN);
    strcpy(bufferT,SYS_GET_NVM);

    itoa(address>>4, &bufferT[addPos], 16);
    itoa(address&0x0F, &bufferT[addPos+1], 16);

    // send request
    rawData(bufferT);

    // remain till buffer is completed
    while (!lora.hasAnswer()) {
        delay(10);
    };

    return nvm;
}

const char* RN2483::setUserEEprom(char address, char data){
    char addPos = strlen(SYS_SET_NVM);
    char bufferT[SET_NVM_LEN];

    prepareAnswer(nvm, NVM_DATA_LEN);
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

    memset(nvm,0,3);
    memcpy(nvm, lora.getLastAnswer(), 2);

    return nvm;
}


RN2483 lora;
