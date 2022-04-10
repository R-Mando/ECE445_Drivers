/*
 * TMC429 library definitions
 *
 * This file contains all macro and function definitions of the TMC429 library.
*/

#ifndef TMC_IC_TMC429_H_
#define TMC_IC_TMC429_H_

	#include "TMC429_Register.h"
	#include "Types.h"
	#include "stm32f4xx.h"

	typedef struct TMC429Object{
	union{
		uint32_t val;
		struct {
			uint32_t rrs: 1;
			uint32_t smda: 2;
			uint32_t address: 4;
			uint32_t rw: 1;
			uint32_t data : 24;
		}bitfield;
		
	};
	} TMC429ObjectType;
	
	// TMC429 library functions
	void Init429(void);
	void ReadWrite429(uint8_t *Read, uint8_t *Write);
	void Write429Zero(uint8_t Address);
	void Write429Bytes(uint8_t Address, uint8_t *Bytes);
	void Write429Datagram(uint8_t Address, uint8_t HighByte, uint8_t MidByte, uint8_t LowByte);
	void Write429Short(uint8_t Address, int32_t Value);
	void Write429Int(uint8_t Address, int32_t Value);
	uint8_t Read429Status(void);
	uint8_t Read429Bytes(uint8_t Address, uint8_t *Bytes);
	uint8_t Read429SingleByte(uint8_t Address, uint8_t Index);
	int32_t Read429Short(uint8_t Address);
	int32_t Read429Int(uint8_t Address);
	void Set429RampMode(uint8_t Axis, uint8_t RampMode);
	void Set429SwitchMode(uint8_t Axis, uint8_t SwitchMode);
	uint8_t SetAMax(uint8_t Motor, uint32_t AMax);
	void HardStop(uint32_t Motor);

	uint8_t ReadWriteSPI(SPI_HandleTypeDef *hspi1, uint8_t byte, bool last_byte);
	void enableChipSelect();
	void disableChipSelect();
	
#endif /* TMC_IC_TMC429_H_ */
