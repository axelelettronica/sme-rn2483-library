#ifndef RN2483_H
#define RN2483_H

#include <Arduino.h>
#include "lora\rn2483Model.h"


class RN2483 {
private:
    int answerLen;
    char *bufferAnswer;
    int bufferAnswerLen;
    char swVer[SW_VER_LEN];
    char nvm[NVM_DATA_LEN];

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

    //SYS specific command
    /*
     * Returns the information on hardware platform,
     * firmware version, release date
     */
    const char* getVersion(void);

    /*
     * Resets the RN2483 module’s configuration data and user EEPROM
     * to factory default values and restarts the RN2483 module.
     */
    inline void factoryReset(void){rawData(SYS_FACTORY_RESET);};

    /*
     * Resets and restarts the RN2483 module.
     */
    inline void reset(void) {rawData(SYS_RESET);};

    /*
     * Returns data from the requested user EEPROM <address>.
     *   from  300 to 3FF
     *
     * <address>: hexadecimal number EEPROM base
     *             from  00 to FF
     *
     * Response: 00 – FF if the address is valid
     *          invalid_param if the address is not valid
     */
    const char* getUserEEprom(char address);
    
    /*
     * Write data to the requested user EEPROM <address>.
     *   from  300 to 3FF
     *
     * <address>: hexadecimal number EEPROM base
     *             from  00 to FF
     * <data>: hexadecimal number representing data, from 00 to FF
     * 
     * Response: ok if the parameters (address and data) are valid
     *           invalid_param if the parameters (address and data) are not valid
     */
    const char* setUserEEprom(char address, char data);
};

// external variable used by the sketches
extern RN2483  lora;
#endif
