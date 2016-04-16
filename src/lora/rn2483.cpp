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
        char buffer[50];
        // maybe to remove because new contructor
        prepareAnswer(buffer, sizeof(buffer));
        
        // send request
        lora.rawData(SYS_GET_VER);
        
        // remain till buffer is completed
        while (!lora.hasAnswer()) {
            delay(10);
        };
        memcpy(swVer, lora.getLastAnswer(), strlen(lora.getLastAnswer()));
    }
    return swVer;
}

RN2483 lora;