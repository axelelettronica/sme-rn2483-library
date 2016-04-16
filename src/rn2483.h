#ifndef RN2483_H
#define RN2483_H

#include <Arduino.h>


class RN2483 {
    private:
        int answerLen;
        char *bufferAnswer;
        int bufferAnswerLen;
        char swVer[50];

    public:
    RN2483(){swVer[0]=0;};
    void init();
    void rawData(String stream);
    inline void prepareAnswer(char *buffer, int bufferLen){
        bufferAnswer = buffer;
        bufferAnswerLen = bufferLen;
    };        
    boolean hasAnswer(void);
    inline const char* getLastAnswer(void) {return bufferAnswer;};
    const char* getVersion(void);
};

// external variable used by the sketches
extern RN2483  lora;
#endif