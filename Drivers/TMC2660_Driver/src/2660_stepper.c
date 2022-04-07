#include "../Inc/2660_stepper.h"

#include "../Inc/2660_REG_bitfields.h"
#include "../Inc/TMC2660_Constants.h"
#include "../Inc/TMC2660_Fields.h"
#include "../Inc/TMC2660_Register.h"
//#include "../Inc/Types.h"
//#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx.h"

SPI_HandleTypeDef hspi3;
//UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim1;
HAL_StatusTypeDef hal_status;

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
                       )
{
    
    uint16_t MicroStepNum[9]={1,2,4,8,16,32,64,128,256};
    uint32_t MicroStep[9] = {0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
    
    if((tmc==NULL))//||(writeRead==NULL)||(cs==NULL)||(enable==NULL))
    {
        return;
    }
   
    //tmc->WriteRead=writeRead;
    //tmc->ChipSelect=cs;
    //tmc->StartStop=startStop;
    //tmc->Direct=direct;
    //tmc->Enable=enable;
   
    tmc->pStartStop=pStartStop;
    tmc->pDirection=pDirection;
    tmc->pRotateSet=pRotateSet;
    tmc->pMotorState=pMotorState;
   
    tmc->microStep=MicroStepNum[microStep];
    tmc->stepAngle=(float)stepAngle/10.0;

    tmc->curve.stepSpeed=0.02;
    tmc->curve.currentSpeed=0;
    tmc->curve.startSpeed=0;
    tmc->curve.speedMax=300;
    tmc->curve.speedMin=1.0;
    tmc->curve.curveMode=0;//CURVE_SPTA ?;
    tmc->curve.flexible=10.0;


    tmc->Register[REG_DRVCTRL]=tmc2660_defaultRegisterResetState[REG_DRVCTRL];
    tmc->Register[REG_CHOPCONF]=tmc2660_defaultRegisterResetState[REG_CHOPCONF];
    tmc->Register[REG_SMARTEN]=tmc2660_defaultRegisterResetState[REG_SMARTEN];
    tmc->Register[REG_SGCSCONF]=tmc2660_defaultRegisterResetState[REG_SGCSCONF];
    tmc->Register[REG_DRVCONF]=tmc2660_defaultRegisterResetState[REG_DRVCONF];
   
    tmc->Register[REG_CHOPCONF]=tmc->Register[REG_CHOPCONF]|0x1B1;
    tmc->Register[REG_SMARTEN]=tmc->Register[REG_SMARTEN]|0x202;
    tmc->Register[REG_SGCSCONF]=tmc->Register[REG_SGCSCONF]|0x10000;
   
    WriteReadTmc2660Register(tmc,REG_CHOPCONF);
    WriteReadTmc2660Register(tmc,REG_SGCSCONF);
   
    if(interface==TMC2660_SPI)
    {
        tmc->Register[REG_DRVCONF]=tmc->Register[REG_DRVCONF]|0xA190;
        WriteReadTmc2660Register(tmc,REG_DRVCONF);
    }
    else
    {
        tmc->Register[REG_DRVCONF]=tmc->Register[REG_DRVCONF]|0xA140;
        WriteReadTmc2660Register(tmc,REG_DRVCONF);
       
        tmc->Register[REG_DRVCTRL]=tmc->Register[REG_DRVCTRL]|0x100|MicroStep[microStep];
        WriteReadTmc2660Register(tmc,REG_DRVCTRL);
    }
  
    WriteReadTmc2660Register(tmc,REG_SMARTEN);
   
    //SetMotorPower(tmc,Power);
}

// typedef void (*TMC2660WriteReadType)(uint8_t *wData,uint16_t wSize,uint8_t *rData,uint16_t rSize);
// typedef void (*TMC2660ChipSelcetType)(TMC2660CSType cs); //Chip select signal
// typedef void (*TMC2660StartStopType)(TMC2660SSType ss); //Start and stop operation function
// typedef void (*TMC2660DirectType)(TMC2660DIRType dir); //Direction operation function
// typedef void (*TMC2660EnableType)(TMC2660ENNType enn); //Enable operation function

/*Read and write registers* TMC2660RegType reg */
static void WriteReadTmc2660Register(TMC2660ObjectType *tmc, uint8_t reg)
{
    uint8_t wData[3];
    uint8_t rData[3];
    uint32_t status=0;
    uint32_t regValue;
   
    tmc->ChipSelect(TMC2660CS_Enable);
   
    regValue=tmc->Register[reg]&0xFFFFF;
    wData[0]=(uint8_t)(regValue>>16);
    wData[1]=(uint8_t)(regValue>>8);
    wData[2]=(uint8_t)regValue;
   
    tmc->WriteRead(wData,3,rData,3);
   
    status=rData[0];
    status=(status<<8)+rData[1];
    status=(status<<8)+rData[2];
   
    tmc->status= status;
    printf("PRINTING STATUS: ");
    printf("%lu", (long int)tmc->status);

    tmc->ChipSelect(TMC2660CS_Disable);
}

void WriteRead(uint8_t * wData, uint16_t wSize, uint8_t *rData, uint16_t rSize){
    //char uart_buf[50];
    //int uart_buf_len;
    // CS pin should default high - but already set earlier
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    // Say something
    // uart_buf_len = sprintf(uart_buf, "SPI Test\r\n");
    // HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

    hal_status = HAL_SPI_TransmitReceive(&hspi3, wData, rData, wSize, 100);
    HAL_Delay(100);

    // Print out status register
    // uart_buf_len = sprintf(uart_buf, "Status: 0xx\r\n", (unsigned int)spi_buf[0]);
    // HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

    // if (hal_status == HAL_OK)
    // {
    //     *byte = rx_data[1];    // response is in the second byte
    // }
    //return hal_status;

}

/*TMC2660 chip select operation function*/
static void ChipSelect(bool cs)
{
    if(cs==TMC2660CS_Enable)
    {
        TMC_CSN_ENABLE();
    }
    else
    {
        TMC_CSN_DISABLE();
    }
}

void TMC_CSN_ENABLE(){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

void TMC_CSN_DISABLE(){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

/*Start and stop operation function*/
static void StartStop(bool ss)
{
    if(ss==TMC2660SS_Start)
    {
        if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
        {
            return;
        }
    }
    else
    {
        if(HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3) != HAL_OK)
        {
            return;
        }
    }
}

// /*Direction operation function*/
static void Direct(bool dir)
{
    if(dir==TMC2660DIR_CCW)
    {
        TMC_DIR_DISABLE();
    }
    else
    {
        TMC_DIR_ENABLE();
    }
}

void TMC_DIR_ENABLE(){
    return;
}
void TMC_DIR_DISABLE(){
    return;
}

// /*Enable operation function*/
static void Enable(bool enn)
{
    if(enn==TMC2660ENN_Enable)
    {
        TMC_ENN_ENABLE();
    }
    else
    {
        TMC_ENN_DISABLE();
    }
}

void TMC_ENN_ENABLE(){
    return;
}

void TMC_ENN_DISABLE(){ 
    return;
}

// /* Read and write data through the SPI2 port */
// static void WriteReadBySPI2(uint8_t *wData,uint16_t wSize,uint8_t *rData,uint16_t rSize)
// {
//     HAL_SPI_TransmitReceive (&hspi2, wData, rData, wSize, 1000);
// }

// void SM42Tmc2660Driver(void)
// {
//     float temp=0;
       
//     if(aPara.phyPara.sm42RotateSet<=0)
//     {
//         //return;
//       aPara.phyPara.sm42StartStop=0;
//     }
   
//     Tmc2660ControlBySD(&tmc);
   
//     SpeedSet(tmc.frequency);
   
//     temp=tmc.frequency*((float)aPara.phyPara.sm42StepAngle);
//     temp=temp/((float)aPara.phyPara.sm42MicroStep);
//     aPara.phyPara.sm42RotateSpeed=(uint16_t)(temp*100/30.0);
// }

// static void SpeedSet(float freq)
// {
//     uint16_t period=0;
//     float temp=24000000;
   
//     if(freq>0)
//     {
//         temp=temp/freq;
//         period=(uint16_t)temp;
       
//         if((2<=period)&&(period<65535))
//         {
//             __HAL_TIM_SET_AUTORELOAD(&htim1,period-1);
//             __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,period/2-1);
//         }
//     }
//     else
//     {
//         __HAL_TIM_SET_AUTORELOAD(&htim1,0);
//         __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);
//     }
// }
