#ifndef TMC_IC_TMC2660_H_
#define TMC_IC_TMC2660_H_
#include "Types.h"

/*Define TMC2660 object type*/
typedef struct TMC2660Object
{
    float microStep;       //Microstep setting
    float stepAngle;       //Inherent step angle
    float frequency;       //running frequency
    uint16_t *pStartStop;  //Start and stop operation command
    uint16_t *pDirection;  //Direction control
    uint16_t *pRotateSet;  //Rotate speed setting
    uint16_t *pMotorState; //motor state

    uint32_t status;      //TMC communication return status
    uint32_t Register[5]; //register
    void (*WriteRead)(uint8_t *wData, uint16_t wSize, uint8_t *rData, uint16_t rSize);
    void (*ChipSelect)(bool cs); //Chip select signal
    void (*StartStop)(bool ss);  //Start and stop operation function
    void (*Direct)(bool dir);   //Direction operation function
    void (*Enable)(bool enn);   //Enable operation function

    CurveObjectType curve; //Motor speed regulation curve
    tmc->ChipSelect(cs_value)
} TMC2660ObjectType;


typedef struct CurveObject
{
    float stepSpeed;
    float currentSpeed;
    float startSpeed;
    float speedMax;
    float speedMin;
    float curveMode;
    float flexible;

} CurveObjectType;

const int32_t tmc2660_defaultRegisterResetState[TMC2660_REGISTER_COUNT] =
{
	0x00000000,  // 0: DRVCTRL
	0x00091935,  // 1: CHOPCONF
	0x000A0000,  // 2: SMARTEN
	0x000D0505,  // 3: SGCSCONF
	0x000EF040,  // 4: DRVCONF
    0x00000000,  // 5: UNUSED
	0x00000000,  // 6: UNUSED
	0x00000000   // 7: UNUSED
};


// typedef struct TMC26660Reg
// {
//     DRVCTRL;
//     CHOPCONF;
//     SMARTEN;
//     SGCSCONF;
//     DRVCONF;
// } TMC26660RegType;

#endif /* TMC_IC_TMC2660_H_ */