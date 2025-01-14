#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "SV.h"
#include "gait.h"
#include "Basic_cpg_controller.h"
#include "Pcpg_controller.h"
#include "Delay_line.h"
#include "main.h"

#define DELAYSIZE 96
#define DELAYRF 0
#define DELAYRH 24
#define DELAYLF 48
#define DELAYLH 72
#define DELAYPN 0
#define DELAYPP 14
#define DELAYSS 2

TIM_HandleTypeDef TIM5_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM7_Handler;      //定时器句柄 
TIM_OC_InitTypeDef TIM2_CH2Handler;
u8 mesg[4]={0X03,0X02,0XD0,0X12};	
u8 canbuf1[8]={0};
u8 canbuf2[8]={0};
u8 canbuf3[4]={0};
//struct Basic_cpg_controller cpg;
//struct Pcpg_controller pcpg_1;
//struct Pcpg_controller pcpg_2;
//struct Pcpg_controller pcpg_pneumatic_pos;
//struct Pcpg_controller pcpg_pneumatic_neg;
//struct Delay_line pneumatic_neg_delay;
//struct Delay_line pneumatic_pos_delay;
//struct Delay_line joint23_delay;
//struct Delay_line joint1_delay;


void TIM7_Init(u16 arr,u16 psc)
{  
    TIM7_Handler.Instance=TIM7;                          //通用定时器3
    TIM7_Handler.Init.Prescaler=psc;                     //分频
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM7_Handler.Init.Period=arr;                        //自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
   
		HAL_TIM_Base_Init(&TIM7_Handler);    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
}

void TIM5_Init(u16 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //通用定时器3
    TIM5_Handler.Init.Prescaler=psc;                     //分频
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM5_Handler.Init.Period=arr;                        //自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
   
		HAL_TIM_Base_Init(&TIM5_Handler);    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
}

void TIM2_Init(u16 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=psc;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
   
		HAL_TIM_Base_Init(&TIM2_Handler);    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器6和定时器6更新中断：TIM_IT_UPDATE  
  
		HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//开启PWM通道2
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟

		HAL_NVIC_SetPriority(TIM5_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM3中断   
	}  
	   if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM5时钟

		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	} 
		if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM5时钟

		HAL_NVIC_SetPriority(TIM7_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //�  
	} 	
}

//定时器5中断服务函数
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
}

void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

//定时器5中断服务函数调用
float s=0;
float delta_t=0.05;
float times = 0;

u8 temp_movement_flag = 0; // tripod gait
u8 movement_flag = 0; // tripod gait
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM5_Handler))
    {			
		//	Read_All_Ad();	
		//	air_control_trot();
		// air_control_tripod();
		//	movement_trot();	
		//	movement_tripod();
      
			cpg_run(1);
			
			s=s+delta_t;
			times++;
		}
		if(htim==(&TIM2_Handler))
    {			
			Read_All_Ad();			
			battery_indicatior();
		}
		if(htim==(&TIM7_Handler))
    {	
			
		}
}
//if(z_H<450) // right
//			{
//					temp_movement_flag = 1;  // change to trot gait
//			}
//			if(z_H>600)  // left
//			{
//					temp_movement_flag = 0;  // change to tripod gait
//			}
//			
//			if(x_L < 480)  // keep pushing the joystick to move
//			{
//				if(s==0||s>=T||times == 1)  // when 
//					{
//					L_onestep=44;
//					H_onestep=46;
//					movement_flag = temp_movement_flag;	
//					}	
//					
//				if(movement_flag==1)  // trot gait mode
//				{
//	//			if(s>0&&s<T)
//	//				{
//	//				L_onestep=45;
//	//				H_onestep=44;
//	//				times = 1;
//	//				}
//					if(s==0||s>=T||times == 1)
//					{
//	//				StartAngleInit1();
//	//				InitRobotPosion();
//					L_onestep=44;
//					H_onestep=46;
//									
//					}	
//					movement_trot_tilt();			
//					s=s+delta_t;			
//				}
//				if(movement_flag == 0)
//				{
//	//				if(s>0&&s<T)
//	//				{
//	//				L_onestep=45;
//	//				H_onestep=44;
//	//				times = 1;
//	//				}
//					
//							
//					movement_tilt();		
//					s=s+delta_t;				
//				}
//			}
//			
//			if(x_L>660)
//			{
//				L_onestep=0;
//				H_onestep=0;
//			}
////			if(x_L>660)
////			{
////				if(s==0||s>=T)
////				{
////				L_onestep=0;
////				H_onestep=0;
////				}	
////				StartAngleInit1();
////				InitRobotPosion();			