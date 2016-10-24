#ifndef RN2483_H
#define RN2483_H

#include <Arduino.h>
#include "lora/rn2483Model.h"

#define HWEUI_LEN 8
const int16_t  L_DEFAULT_PORT = 1;
const int16_t  L_CONFIGURED_PORT = 0xFFFF;

typedef enum {
    UnknownRadio,
    LoRa,
    FSK
}radioModeE;

typedef enum {
    TX_ACK,
    TX_NOACK,
}txModeE;

typedef struct {
    char sw:1;
    char hwEUI:1;
    char macAppEUI:1;
    radioModeE radioMode;
}RN2483InitS;

typedef enum {
    RN_OK,
    RN_ERR,
} errE;

typedef enum {
    OTAA,
    ABP,
} joinModeE;

typedef enum {
    BAND_868,
    BAND_433,
} bandE;

#define RN_BUFFER_LEN   100

#define TX      1
#define RX      0


typedef struct {
    uint8_t idx;
    uint8_t data[RN_BUFFER_LEN];
} rnMsgT;


#define RN_MAC_EUI_LEN  16

class RN2483 {
private:
    Uart *comm;
    int answerLen;
    char *bufferAnswer;
    int bufferAnswerLen;
    char swVer[SW_VER_LEN+1];
    char answer[SMALL_ANSWER_DATA_LEN+1];
    char hwEUI[RN_MAC_EUI_LEN+1];
    char macAppEUI[RN_MAC_EUI_LEN+1];
    //char macDevEui[RN_MAC_EUI_LEN+1];
    
    RN2483InitS initField;
    uint16_t port;
    
    rnMsgT rx, tx;
    //uint8_t cur_dir;
private:

    void rawData(String stream);
    boolean hasAnswer(void);
    bool checkAnswer(const char *answer);
    void dataToHexString(const char*const beginIt, const char*const endIt, String& str);
    errE sendCmd(String stream);
public:

    RN2483(){initField.hwEUI=0;initField.sw=0;initField.radioMode=UnknownRadio;};
    void begin(long speed = 57600, Uart *serial=&iotAntenna);
    bool available();
    const char* read(int *len);
    
    void handleRxData(uint8_t inChar);
    char* getRxData(void);
    bool rxDataReady(void);
    
    errE sendRawCmd(String stream);
    errE sendData(char *data, uint16_t dataLen, int16_t portId, txModeE type);

    inline void prepareAnswer(char *buffer, int bufferLen){
        bufferAnswer = buffer;
        bufferAnswerLen = bufferLen;
    };
    inline const char* getLastAnswer(void) {return bufferAnswer;};

    // MAC COmmands
    errE macResetCmd(bandE band = BAND_868); 
    errE macTxCmd(String stream, int16_t portId = L_CONFIGURED_PORT, txModeE type = TX_NOACK);
    errE macJoinCmd(joinModeE  mode = OTAA); 
    const char* getMacAppEUI(void);
    errE macSetDevEUICmd(String stream);
    errE macSetAppEUICmd(String stream);    
    errE macSetNtwSessKeyCmd(String stream);
    errE macSetAppSessKeyCmd(String stream);
    errE macSetAppKeyCmd(String stream);
    
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
     * Response: true if the parameters (address and data) are valid
     *           false if the parameters (address and data) are not valid
     */
    bool setUserEEprom(char address, char data);

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
    //const char* 
    const char * getHwEUI(void);


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
     *  Response: true if the modulation is valid
     *            false if the modulation is not valid
     */
    bool setRadioMode (char *radioMode);
};

// external variable used by the sketches
extern RN2483  lora;
extern bool loraDbg;
#endif
