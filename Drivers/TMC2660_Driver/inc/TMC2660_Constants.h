/*
 * TMC2660_Constants.h
 *
 *  Created on: 20.08.2018
 *      Author: tmctest
 */

#ifndef TMC_IC_TMC2660_TMC2660_CONSTANTS_H_
#define TMC_IC_TMC2660_TMC2660_CONSTANTS_H_

#include "2660_api/inc/Constants.h"

#define TMC2660_REGISTER_COUNT   8
#define TMC2660_MOTORS           1
#define TMC2660_WRITE_BIT        0x08
#define TMC2660_ADDRESS_MASK     0x07
#define TMC2660_ADDRESS_SHIFT    20
#define TMC2660_VALUE_MASK       0xFFFFF
#define TMC2660_VALUE_SHIFT      0
#define TMC2660_MAX_VELOCITY     s32_MAX
#define TMC2660_MAX_ACCELERATION u24_MAX

// read addressescorrespondes with RDSEL, added latest to access latest data of common lower 7 bits
#define TMC2660_RESPONSE0        0
#define TMC2660_RESPONSE1        1
#define TMC2660_RESPONSE2        2
#define TMC2660_RESPONSE_LATEST  3

#define TMC2660CS_Enable 1
#define TMC2660CS_Disable 0 
#define TMC2660SS_Start 1
#define TMC2660DIR_CCW 0
#define TMC2660ENN_Enable 1
#define TMC2660_SPI 1

#endif /* TMC_IC_TMC2660_TMC2660_CONSTANTS_H_ */
