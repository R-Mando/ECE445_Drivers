#ifndef TMC_2660_STEPPER_H_
#define TMC_2660_STEPPER_H_
#include "Types.h"


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
    void (*WriteRead)(uint8_t *wData, uint16_t wSize, uint8_t *rData, uint16_t rSize);
    void (*ChipSelect)(bool cs); //Chip select signal
    //same as void ChipSelect(bool cs)
    void (*StartStop)(bool ss);  //Start and stop operation function
    void (*Direct)(bool dir);   //Direction operation function
    void (*Enable)(bool enn);   //Enable operation function

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
void Tmc2660Initialization(TMC2660ObjectType *tmc,       //The TMC object variable to be initialized
                        bool interface, //Driver interface type
                        int microStep, //Microstep setting
                        uint16_t Power, //current range
                        uint16_t stepAngle, //Inherent step angle
                        uint16_t *pStartStop, //Start and stop operation command
                        uint16_t *pDirection, //direction control
                        uint16_t *pRotateSet, //Rotate speed setting
                        uint16_t *pMotorState //motor state
                        //TMC2660WriteReadType writeRead, //Read and write function pointer
                        //TMC2660ChipSelectType cs, //Chip select operation function pointer
                        //TMC2660StartStopType startStop, //Start and stop operation function pointer
                        //TMC2660DirectType direct, //Direction setting function pointer
                        //TMC2660EnableType enable, //Enable control function pointer
                       );

static void WriteReadTmc2660Register(TMC2660ObjectType *tmc, uint8_t reg);
//void WriteRead(uint8_t * wData, uint16_t wSize, uint8_t *rData, uint16_t rSize);
//static void ChipSelect(bool cs);
void TMC_CSN_ENABLE();
void TMC_CSN_DISABLE();
static void StartStop(bool ss);
static void Direct(bool dir);
void TMC_DIR_ENABLE();
void TMC_DIR_DISABLE();
static void Enable(bool enn);
void TMC_ENN_ENABLE();
void TMC_ENN_DISABLE();
#endif /* TMC_2660_STEPPER_H_*/
