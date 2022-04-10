#ifndef TMC_2660_STEPPER_H_
#define TMC_2660_STEPPER_H_
#include "Types.h"
#include "TMC2660_Constants.h"
#include "stm32f4xx.h"

/*Define TMC2660 object type*/
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
    // void (*WriteRead)(uint8_t *wData, uint16_t wSize, uint8_t *rData, uint16_t rSize);
    // void (*ChipSelect)(bool cs); //Chip select signal
    // //same as void ChipSelect(bool cs)
    // void (*StartStop)(bool ss);  //Start and stop operation function
    // void (*Direct)(bool dir);   //Direction operation function
    // void (*Enable)(bool enn);   //Enable operation function
    bool cs;
    bool ss;
    bool dir;
    bool enn;
    CurveObjectType curve; //Motor speed regulation curve

} TMC2660ObjectType;

//NOTE:
//(*bar). is the same as bar->
// typedef void (*TMC2660WriteReadType)(uint8_t *wData,uint16_t wSize,uint8_t *rData,uint16_t rSize);
// typedef void (*ChipSelect)(bool cs); //Chip select signal
// typedef void (*StartStop)(bool ss); //Start and stop operation function
// typedef void (*Direct)(bool dir); //Direction operation function
// typedef void (*Enable)(bool enn); //Enable operation function

// typedef struct TMC26660Reg
// {
//     DRVCTRL;
//     CHOPCONF;
//     SMARTEN;
//     SGCSCONF;
//     DRVCONF;
// } TMC26660RegType;
/*Initialize TMC2660 object*/
extern void Tmc2660Initialization(TMC2660ObjectType *tmc, bool interface, int microStep, uint16_t Power, uint16_t stepAngle, uint16_t *pStartStop, uint16_t *pDirection, uint16_t *pRotateSet, uint16_t *pMotorState);

extern void WriteReadTmc2660Register(TMC2660ObjectType *tmc, uint8_t reg);
void WriteRead(uint8_t * wData, uint16_t wSize, uint8_t *rData, uint16_t rSize);
void ChipSelect(TMC2660ObjectType *tmc);
void TMC_CSN_ENABLE();
void TMC_CSN_DISABLE();
//static void StartStop(TMC2660ObjectType *tmc);
//static void Direct(TMC2660ObjectType *tmc);
//void TMC_DIR_ENABLE();
//void TMC_DIR_DISABLE();
//static void Enable(TMC2660ObjectType *tmc);
//void TMC_ENN_ENABLE();
//void TMC_ENN_DISABLE();
// void spi_motor_test(TMC2660ObjectType *tmc);

void spi_motor_zero(TMC2660ObjectType *tmc);

void max_coil_a(TMC2660ObjectType *tmc);

void max_coil_b(TMC2660ObjectType *tmc);

#endif /* TMC_2660_STEPPER_H_*/
