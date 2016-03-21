#include "../rn2483.h"


void RN2483::init() {
    Serial1.begin(57600);
}

void RN2483::rawData(String stream) {
    answerLen =0; // reset Answer Counter
    stream.concat("\r\n");
    Serial1.print(stream);
}

boolean RN2483::hasAnswer(void) {
    if (Serial1.available()) {
        bufferAnswer[answerLen] = Serial1.read();
        
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
RN2483 lora;