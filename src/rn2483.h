#ifndef RN2483_H
#define RN2483_H

#include <Arduino.h>
#include "lora/rn2483Model.h"

#define HWEUI_LEN 8
const int16_t  L_DEFAULT_PORT = 1;
const int16_t  L_CONFIGURED_PORT = 0xFFFF;

typedef enum  answerCodes_t_  {
    ASW_OK,
    ASW_INVALID_PARAM,
    ASW_NOT_JOINED,
    ASW__NO_FREE_CHAN,
    ASW_SILENT,
    ASW_REJOIN_NEEDED,
    ASW_BUSY,
    ASW_PAUSED,
    ASW_INVALID_LEN,
    ASW_KEYS_NOT_INIT,
    ASW_JOIN_DENIED,
    ASW_JOIN_ACCEPTED,
    ASW_RADIO_ERR,
    ASW_RADIO_TX_OK,
    ASW_TX_OK,
    ASW_ERR,
    ASW_STR,
    ASW_CODES_NUM
} answerCodes_t;

typedef enum {
    UnknownRadio,
    LoRa,
    FSK
}radioModeE;

typedef enum {
    TX_ACK,
    TX_NOACK,
}txModeE;


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


class rnMsgT {
public:
    uint8_t len;
    uint8_t idx;
    uint8_t data[RN_BUFFER_LEN];
public:
    rnMsgT() {len = RN_BUFFER_LEN; init();}
    ~rnMsgT() {}  
    void init(void) {memset(data,0,RN_BUFFER_LEN); idx = 0;}
    void prepare(void) { init();}    
};


#define RN_MAC_EUI_LEN  16

class RN2483 {
private:
    Uart *comm;
/*    int answerLen;
    char *bufferAnswer;
    int bufferAnswerLen;
    char swVer[SW_VER_LEN+1];
    char answer[SMALL_ANSWER_DATA_LEN+1];
    char hwEUI[RN_MAC_EUI_LEN+1];
    char macAppEUI[RN_MAC_EUI_LEN+1];
    //char macDevEui[RN_MAC_EUI_LEN+1];*/
    
    uint16_t port;
    
    rnMsgT rx, tx;
    //uint8_t cur_dir;
private:

    void dataToHexString(const char*const beginIt, const char*const endIt, String& str);
    void rawData(String stream);
    uint8_t checkAnswer(const char *answer);
    errE sendData(char *data, uint16_t dataLen, int16_t portId, txModeE type);
    // boolean hasAnswer(void);
    uint8_t sendCmd(String stream);
    uint8_t waitAnswer(void);    
    char* getRxData(void);
    void handleRxData(uint8_t inChar);
    bool rxDataReady(void);
 
    #define SEND_CMN_CMD(msgStr) {return ((sendCmd(msgStr) == ASW_OK)? RN_OK : RN_ERR);} 
    
public:

    RN2483(); //: comm(NULL) {/*initField.hwEUI=0;initField.sw=0;initField.radioMode=UnknownRadio;*/};
    void begin(long speed = 57600, Uart *serial=&iotAntenna);
    bool available();
    const char* read(int *len);
    const char* read(void);
       
    const char* getLastAnswer(void);    
    errE sendRawCmd(String stream);
    const char *sendRawCmdAndAnswer(String stream);


    /******* MAC Commands *******/
    
    errE macSetDevAddrCmd(String stream);
    errE macSetDevEUICmd(String stream);
    errE macSetAppEUICmd(String stream);
    errE macSetNtwSessKeyCmd(String stream);
    errE macSetAppSessKeyCmd(String stream);
    errE macSetAppKeyCmd(String stream);
    
    const char* macGetStatus(void);      
    const char* getMacAppEUI(void);

    errE macResetCmd(bandE band = BAND_868); 
    errE macTxCmd(String stream, int16_t portId = L_CONFIGURED_PORT, txModeE type = TX_NOACK);
    errE macTxCmd(char *data, int16_t len, int16_t portId = L_CONFIGURED_PORT, txModeE type = TX_NOACK);
    errE macJoinCmd(joinModeE  mode = OTAA); 
    errE macSetDataRate(uint8_t dataRate); // 0..7 
        
    inline errE macSave(void) {SEND_CMN_CMD(MAC_SAVE_CMD);};
    const char * macPause(void);
    inline errE macResume(void) {SEND_CMN_CMD(MAC_RESUME_CMD);};


   
    // Setting the Adaptive Data Rate
    inline errE macSetAdrOn(void) {SEND_CMN_CMD(MAC_SET_ADR_ON_CMD);};    
    inline errE macSetAdrOff(void) {SEND_CMN_CMD(MAC_SET_ADR_OFF_CMD);};
    // Setting the Automatic Reply
    inline errE macSetArOn(void) {SEND_CMN_CMD(MAC_SET_ADR_ON_CMD);};    
    inline errE macSetArOff(void) {SEND_CMN_CMD(MAC_SET_ADR_OFF_CMD);};   

    
    
    /******* SYS Commands *******/
    
    errE sysSleepCmd(uint32_t msec);
    
    //errE sysSetSleep(String stream);
 
    /*
     * Returns the information on hardware platform,
     * firmware version, release date
     */
    const char* sysGetVersion(void);

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
    char sysGetUserEEprom(char address);

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
    bool sysSetUserEEprom(char address, char data);


    /*
     * This command informs the RN2483 module to do an ADC conversion on the VDD.
     * The measurement is converted and returned as a voltage (mV).
     *
     * Response: 0–3600 (string value from 0 to 3600)
     */
    const char * sysGetVdd(void);

    /*
     * This command reads the preprogrammed EUI node address from the RN2483 module.
     * The value returned by this command is a globally unique number provided by Microchip.
     *
     * Response: hexadecimal number representing the preprogrammed EUI node address
     *
     */
    const char * sysGetHwEUI(void);
        
            
    //errE sysWakeUp(void);
    
    /*
     * Resets and restarts the RN2483 module.
     */
    const char * sysReset(void);
    

    /*
     * Resets the RN2483 module’s configuration data and user EEPROM
     * to factory default values and restarts the RN2483 module.
     */
    const char * sysFactoryReset(void);





    /******* RADIO Commands *******/

    
    errE radioSetSync(uint8_t sync);
    errE radioSetPwr(uint8_t pwr);    
    const char * radioGetPwr();    
    /*
     * This command reads back the current mode of operation of the module.
     *      Default: lora
     *
     * Response: string representing the current mode of operation of the module,
     *              either lora or fsk.
     *
     */
    radioModeE radioGetMode();

    /*
     *
     * This command changes the modulation method being used by the module. 
     * Altering the mode of operation does not affect previously set parameters, 
     *      variables or registers.
     * FSK mode also allows GFSK transmissions when data shaping is enabled.
     * 
     *  <mode>:     LoRa,     FSK 
     *
     *  Response: true if the modulation is valid
     *            false if the modulation is not valid
     */

     errE radioSetMode (radioModeE radioMode);
};

// external variable used by the sketches
extern RN2483  lora;
extern volatile bool loraDbg;
#endif
