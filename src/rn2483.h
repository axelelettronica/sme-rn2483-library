#ifndef RN2483_H
#define RN2483_H

#include <Arduino.h>
#include "lora\rn2483Model.h"

#define HWEUI_LEN 8

typedef enum {
    UnknownRadio,
    LoRa,
    FSK
}radioModeE;

typedef struct {
    char sw:1;
    char hwEUI:1;
    radioModeE radioMode;
}RN2483InitS;


class RN2483 {
private:
    int answerLen;
    char *bufferAnswer;
    int bufferAnswerLen;
    char swVer[SW_VER_LEN];
    char answer[SMALL_ANSWER_DATA_LEN];
    char hwEUI[HWEUI_LEN];
    RN2483InitS initField;

private:
    bool checkAnswer(char *answer);

public:
    RN2483(){initField.hwEUI=0;initField.sw=0;initField.radioMode=UnknownRadio;};
    void init();
    void rawData(String stream);
    inline void prepareAnswer(char *buffer, int bufferLen){
        bufferAnswer = buffer;
        bufferAnswerLen = bufferLen;
    };
    boolean hasAnswer(void);
    inline const char* getLastAnswer(void) {return bufferAnswer;};



    //SYS command
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
    char getUserEEprom(char address);

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

    /*
     * This command informs the RN2483 module to do an ADC conversion on the VDD.
     * The measurement is converted and returned as a voltage (mV).
     *
     * Response: 0–3600 (decimal value from 0 to 3600)
     */
    int getPower(void);

    /*
     * This command reads the preprogrammed EUI node address from the RN2483 module.
     * The value returned by this command is a globally unique number provided by Microchip.
     *
     * Response: hexadecimal number representing the preprogrammed EUI node address
     *
     */
    const char* getHwEUI(void);


    //RADIO command

    /*
     * This command reads back the current mode of operation of the module.
     *      Default: lora
     *
     * Response: string representing the current mode of operation of the module,
     *              either lora or fsk.
     *
     */
    radioModeE getRadioMode();

    /*
     *
     * This command changes the modulation method being used by the module. 
     * Altering the mode of operation does not affect previously set parameters, 
     *      variables or registers.
     * FSK mode also allows GFSK transmissions when data shaping is enabled.
     * 
     *  <mode>: string representing the modulation method, 
     *              either lora or fsk.
     *
     *  Response: ok if the modulation is valid
     *            invalid_param if the modulation is not valid
     */
    bool setRadioMode (char *radioMode);
};

// external variable used by the sketches
extern RN2483  lora;
#endif
