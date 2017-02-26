/* SmartEverything RN2483 Library
 * Copyright (C) 2017 by IOTEAM
 *
 * This file is part of the SmartEverything Arduino RN2483 Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SmartEverything RN2483 Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *  Design:      seve@ioteam.it
 */
#ifndef _RN2483_MODEL_H_
#define _RN2483_MODEL_H_

#define SMALL_ANSWER_DATA_LEN    20 // tuned on "sys get hweui" answer
#define SW_VER_LEN               50


#define JOIN_OTA_MODE            "otaa"
#define JOIN_ABP_MODE            "abp"

// SYS Commands
#define SYS_SLEEP                "sys sleep "
#define SYS_GET_VER              "sys get ver"
#define SYS_FACTORY_RESET        "sys factoryRESET"
#define SYS_RESET                "sys reset"

#define SYS_GET_NVM              "sys get nvm 3"
#define GET_NVM_LEN              13
#define SYS_SET_NVM              "sys set nvm 3"
#define SET_NVM_LEN              18
#define SYS_GET_VDD              "sys get vdd"
#define SYS_GET_HWEUI            "sys get hweui"

// MAC Commands
#define MAC_GET_APP_EUI          "mac get appeui"
#define MAC_GET_STATUS           "mac get status"
#define MAC_RESET_CMD            "mac reset "
#define MAC_TX_CMD               "mac tx "
#define MAC_JOIN_CMD             "mac join "
#define MAC_SAVE_CMD             "mac save"
#define MAC_PAUSE_CMD            "mac pause"
#define MAC_RESUME_CMD           "mac resume"

#define MAC_SET_DEV_ADDR         "mac set devaddr "
#define MAC_SET_DEV_EUI          "mac set deveui "
#define MAC_SET_APP_EUI          "mac set appeui "
#define MAC_SET_NTW_SESS_KEY     "mac set nwkskey "
#define MAC_SET_APP_SESS_KEY     "mac set appskey "
#define MAC_SET_APP_KEY          "mac set appkey "
#define MAC_GET_STATUS           "mac get status"
#define MAC_SET_DATARATE         "mac set dr "
#define MAC_SET_ADR_ON_CMD       "mac set adr on"
#define MAC_SET_ADR_OFF_CMD      "mac set adr off"
#define MAC_SET_AR_ON_CMD       "mac set ar on"
#define MAC_SET_AR_OFF_CMD      "mac set ar off"

// Radio command
#define RADIO_GET_MODE           "radio get mod"
#define RADIO_SET_MODE           "radio set mod "
#define RADIO_MODE_LEN           18
#define RADIO_SET_PWR            "radio set pwr "
#define RADIO_GET_PWR            "radio get pwr"
#define RADIO_SET_SYNC           "radio set sync "
#define LORA_MODE                "lora"
#define FSK_MODE                 "fsk"



#endif
