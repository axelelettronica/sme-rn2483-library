#ifndef RN2483_MODEL_H
#define RN2483_MODEL_H

#define SMALL_ANSWER_DATA_LEN   20 // tuned on "sys get hweui" answer

#define SYS_GET_VER         "sys get ver"
#define SW_VER_LEN          50

#define SYS_FACTORY_RESET   "sys factoryRESET"
#define SYS_RESET           "sys reset"

#define SYS_GET_NVM         "sys get nvm 3"
#define GET_NVM_LEN         13
#define SYS_SET_NVM         "sys set nvm 3"
#define SET_NVM_LEN         18

#define SYS_GET_VDD         "sys get vdd"

#define SYS_GET_HWEUI       "sys get hweui"




// Radio command
#define RADIO_GET_MODE      "radio get mod"
#define RADIO_SET_MODE      "radio set mod "
#define RADIO_MODE_LEN      18
#define LORA_MODE           "lora"
#define FSK_MODE            "fsk"
#endif
