#include "../Inc/2660_stepper.h"

#include "../Inc/2660_REG_bitfields.h"
#include "../Inc/TMC2660_Constants.h"
#include "../Inc/TMC2660_Fields.h"
#include "../Inc/TMC2660_Register.h"
//#include "../Inc/Types.h"
//#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_spi.h"

//SPI_HandleTypeDef hspi3;
//UART_HandleTypeDef huart2;
//TIM_HandleTypeDef htim1;
//HAL_StatusTypeDef hal_status;
int j;

uint32_t tmc2660_defaultRegisterResetState[TMC2660_REGISTER_COUNT] =
{
	0x00000000,  // 0: DRVCTRL      000
    0x00000000,  // 1: UNUSED       001
	0x00000000,  // 2: UNUSED       010
	0x00000000,  // 3: UNUSED       011
    0x00091935,  // 4: CHOPCONF     100
	0x000A0000,  // 5: SMARTEN      101
	0x000D0505,  // 6: SGCSCONF     110
	0x000EF040,  // 7: DRVCONF      111
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
                       uint16_t *pMotorState, //motor state
                       SPI_HandleTypeDef *hspi3,
                       UART_HandleTypeDef *huart1
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

   //tmc->pStartStop=pStartStop;
   //tmc->pDirection=pDirection;
   //tmc->pRotateSet=pRotateSet;
   //tmc->pMotorState=pMotorState;

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

   WriteReadTmc2660Register(tmc,REG_CHOPCONF, hspi3, huart1);
   WriteReadTmc2660Register(tmc,REG_SGCSCONF, hspi3, huart1);
   if(interface==TMC2660_SPI)
   {
        //int j;
        //char MSG[100] = {'\0'};
        //uint32_t drvconf_reg_val = tmc->Register[REG_DRVCONF];
        //j = sprintf(MSG, "VAL BEFORE:%lu\r\n", (long int)drvconf_reg_val);
        //HAL_UART_Transmit(&(*huart1), (uint8_t *)MSG, sizeof(MSG), HAL_MAX_DELAY);
        tmc->Register[REG_DRVCONF]=tmc->Register[REG_DRVCONF]|0xA190;
        WriteReadTmc2660Register(tmc,REG_DRVCONF, hspi3, huart1);
        //uint32_t drvconf_reg_val_after = tmc->Register[REG_DRVCONF];
        //j += sprintf(MSG+j, "VAL AFTER:%lu\r\n", (long int)drvconf_reg_val_after);
        //HAL_UART_Transmit(&(*huart1), (uint8_t *)MSG, sizeof(MSG), HAL_MAX_DELAY);
        HAL_Delay(100);
   }
   else
   {
		//int j;
	    char MSG[100] = {'\0'};
		//uint32_t drvconf_reg_val = tmc->Register[REG_DRVCONF];
		j += sprintf(MSG+j, "VAL IN INIT BEFORE:%lu\r\n",tmc->Register[REG_DRVCONF]);
        tmc->Register[REG_DRVCONF]=tmc->Register[REG_DRVCONF]|0xA140;
        WriteReadTmc2660Register(tmc,REG_DRVCONF, hspi3, huart1);
        j+= sprintf(MSG+j, "VAL IN INIT AFTER:%lu\r\n",tmc->Register[REG_DRVCONF]);
        tmc->Register[REG_DRVCTRL]=tmc->Register[REG_DRVCTRL]|0x100|MicroStep[microStep];
        WriteReadTmc2660Register(tmc,REG_DRVCTRL, hspi3, huart1);
        j+= sprintf(MSG+j, "DRV CTRL AFTER:%lu\r\n", tmc->Register[REG_DRVCTRL]);
        HAL_UART_Transmit(&(*huart1), (uint8_t *)MSG, sizeof(MSG), HAL_MAX_DELAY);
   }

   WriteReadTmc2660Register(tmc, REG_SMARTEN, hspi3, huart1);

   //SetMotorPower(tmc,Power);
}

// typedef void (*TMC2660WriteReadType)(uint8_t *wData,uint16_t wSize,uint8_t *rData,uint16_t rSize);
// typedef void (*TMC2660ChipSelcetType)(TMC2660CSType cs); //Chip select signal
// typedef void (*TMC2660StartStopType)(TMC2660SSType ss); //Start and stop operation function
// typedef void (*TMC2660DirectType)(TMC2660DIRType dir); //Direction operation function
// typedef void (*TMC2660EnableType)(TMC2660ENNType enn); //Enable operation function

void spi_motor_zero(TMC2660ObjectType *tmc, SPI_HandleTypeDef *hspi3, UART_HandleTypeDef *huart1){
    tmc->Register[REG_DRVCTRL]= 0x00000;
    WriteReadTmc2660Register(tmc,REG_DRVCTRL, hspi3, huart1);
}


void max_coil_a(TMC2660ObjectType *tmc, SPI_HandleTypeDef *hspi3, UART_HandleTypeDef *huart1){
	//int i;
	char MSG[500] = {'\0'};
	j += sprintf(MSG+j, "VAL DRVCTRL BEFORE:%lx\r\n", tmc->Register[REG_DRVCTRL]);
    tmc->Register[REG_DRVCTRL] = 0x0003FE00;
    j += sprintf(MSG+j, "VAL DRVCTRL AFTER:0x%08lX\r\n", (uint32_t)tmcRegister[REG_DRVCTRL]);
    j += sprintf(MSG+j, "VAL REG_CHOPCONF:0x%08lX\r\n",  (uint32_t)tmc->Register[REG_CHOPCONF]);
    j += sprintf(MSG+j, "VAL REG_SMARTEN:0x%08lX\r\n",  (uint32_t)tmc->Register[REG_SMARTEN]);
    j += sprintf(MSG+j, "VAL REG_SGCSCONF:0x%08lX\r\n", (uint32_t)tmc->Register[REG_SGCSCONF]);
    j += sprintf(MSG+j, "VAL REG_DRVCONF:0x%08lX\r\n", (uint32_t)tmc->Register[REG_DRVCONF]);
    tmc.Register[REG_DRVCTRL] = 0x0003FE00;
    tmc->Register[REG_CHOPCONF] = 0x0003FE00;
    tmc->Register[REG_SMARTEN] = 0x0003FE00;
    tmc->Register[REG_SGCSCONF] = 0x0003FE00;
    tmc->Register[REG_DRVCONF] = 0x0003FE00;
    j += sprintf(MSG+j, "VAL DRVCTRL AFTER:0x%08lX\r\n", (uint32_t)tmc->Register[REG_DRVCTRL]);
    j += sprintf(MSG+j, "VAL REG_CHOPCONF AFTER:0x%08lX\r\n",  (uint32_t)tmc->Register[REG_CHOPCONF]);
    j += sprintf(MSG+j, "VAL REG_SMARTEN AFTER:0x%08lX\r\n",  (uint32_t)tmc->Register[REG_SMARTEN]);
    j += sprintf(MSG+j, "VAL REG_SGCSCONF AFTER:0x%08lX\r\n", (uint32_t)tmc->Register[REG_SGCSCONF]);
    j += sprintf(MSG+j, "VAL REG_DRVCONF AFTER:0x%08lX\r\n", (uint32_t)tmc->Register[REG_DRVCONF]);
    HAL_UART_Transmit(&(*huart1), (uint8_t *)MSG, sizeof(MSG), HAL_MAX_DELAY);
    WriteReadTmc2660Register(tmc,REG_DRVCTRL, hspi3, huart1);
    //uint32_t drvconf_reg = tmc->Register[REG_DRVCTRL];

    //HAL_UART_Transmit(&(*huart1), (uint8_t *)MSG, sizeof(MSG), HAL_MAX_DELAY);
//    for(i=0;i<j;i++){
//    	MSG[i] = '\0';
//    }
}


void max_coil_b(TMC2660ObjectType *tmc,  SPI_HandleTypeDef *hspi3, UART_HandleTypeDef *huart1){
    tmc->Register[REG_DRVCTRL]= 0x3FEFF;
    WriteReadTmc2660Register(tmc,REG_DRVCTRL, hspi3, huart1);
}

/*Read and write registers* TMC2660RegType reg */
void WriteReadTmc2660Register(TMC2660ObjectType *tmc, uint8_t reg, SPI_HandleTypeDef *hspi3, UART_HandleTypeDef *huart1)
{
//	uint8_t MSG[35] = {'\0'};
    uint8_t wData[3];
    uint8_t rData[3];
    uint32_t status=0;
    uint32_t regValue;
    uint32_t datagram;
   
    //tmc->ChipSelect(TMC2660CS_Enable);
    tmc->cs = TMC2660CS_Enable;
    ChipSelect(tmc, huart1);
    regValue = tmc->Register[reg]&TMC2660_DRVCTRL_VAL_MASK; 
    datagram = (TMC2660_DATAGRAM(reg, regValue) & TMC2660_DATAGRAM_MASK);
    wData[0]=(uint8_t)(datagram>>16);
    wData[1]=(uint8_t)(datagram>>8);
    wData[2]=(uint8_t)datagram;
   
    WriteRead(wData,3,rData,3, hspi3, huart1);
   
    status=rData[0];
    status=(status<<8)+rData[1];
    status=(status<<8)+rData[2];
   
    tmc->status= status;
    printf("PRINTING STATUS: ");
    printf("%lu", (long int)tmc->status);

    //tmc->ChipSelect(TMC2660CS_Disable);
    tmc->cs = TMC2660CS_Disable;
    ChipSelect(tmc, huart1);
}

void WriteRead(uint8_t * wData, uint16_t wSize, uint8_t *rData, uint16_t rSize, SPI_HandleTypeDef *hspi3, UART_HandleTypeDef *huart1){
    //char uart_buf[50];
    //int uart_buf_len;
    // CS pin should default high - but already set earlier
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    // Say something
    // uart_buf_len = sprintf(uart_buf, "SPI Test\r\n");
    // HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
	// SPI_HandleTypeDef hspi3;
    HAL_SPI_TransmitReceive(&(*hspi3), wData, rData, wSize, 100);
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
void ChipSelect(TMC2660ObjectType *tmc, UART_HandleTypeDef *huart1)
{
    if(tmc->cs==TMC2660CS_Enable)
    {
        TMC_CSN_ENABLE();
    }
    else
    {
        TMC_CSN_DISABLE();
    }
}

void TMC_CSN_ENABLE(){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
}

void TMC_CSN_DISABLE(){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

/*Start and stop operation function*/
//static void StartStop(TMC2660ObjectType *tmc)
//{
//    if(tmc->ss==TMC2660SS_Start)
//    {
////        if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
////        {
//            return;
//        //}
//    }
//    else
//    {
////        if(HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3) != HAL_OK)
////        {
//            return;
//        //}
//    }
//}
//
//// /*Direction operation function*/
//static void Direct(TMC2660ObjectType *tmc)
//{
//    if(tmc->dir==TMC2660DIR_CCW)
//    {
//        TMC_DIR_DISABLE();
//    }
//    else
//    {
//        TMC_DIR_ENABLE();
//    }
//}
//
//void TMC_DIR_ENABLE(){
//    return;
//}
//void TMC_DIR_DISABLE(){
//    return;
//}
//
//// /*Enable operation function*/
//static void Enable(TMC2660ObjectType *tmc)
//{
//    if(tmc->enn==TMC2660ENN_Enable)
//    {
//        TMC_ENN_ENABLE();
//    }
//    else
//    {
//        TMC_ENN_DISABLE();
//    }
//}
//
//void TMC_ENN_ENABLE(){
//    return;
//}
//
//void TMC_ENN_DISABLE(){
//    return;
//}

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
