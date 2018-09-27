// Proj : PWM Controlled GPIO
// Doc  : myPWM.c
// Name : Kenosha Vaz
// Date : 22 March, 2018

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static TIM_HandleTypeDef tim1;
int16_t str,count=0;
uint8_t state,clr;

/* Initialise the GPIO pins */

ParserReturnVal_t CmdPWM_Init(int action)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  uint16_t rc,value;
  
  /* char *sec; */

  /* int bc,sr; */

  /* rc = fetch_uint16_arg(&pin); */
  /* if(rc) */
  /*   { */
  /*     printf("Must supply a Pin Number!\n\n"); */
  /*     return CmdReturnBadParameter1; */
  /*   } */

  /* bc=0x0001; */

  /* sr=0x0001; */

  /* if(pin<=15){ */
  /*   sr=bc<<pin; */
  /* }else{ */
  /*   printf("Must be between 0-15 ONLY!\n"); */
  /* } */


  /* if(action!=CMD_INTERACTIVE) return CmdReturnOk; */

  /* rc = fetch_string_arg(&sec); */
  /* if(rc) */
  /*   { */
  /*     printf("Must supply a Section Value!\n\n"); */
  /*     return CmdReturnBadParameter1; */
  /*   }   */

  /* switch(*sec){ */
  /* case 'A': */
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = 6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  /*   break; */
  /* case 'B': */
  /*   __HAL_RCC_GPIOB_CLK_ENABLE(); */
  /*   GPIO_InitStruct.Pin = sr; */
  /*   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; */
  /*   GPIO_InitStruct.Pull = GPIO_NOPULL; */
  /*   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; */
  /*   GPIO_InitStruct.Alternate = 0; */
  /*   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); */
  /*   break; */
  /* case 'C': */
  /*   __HAL_RCC_GPIOC_CLK_ENABLE(); */
  /*   GPIO_InitStruct.Pin = sr; */
  /*   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; */
  /*   GPIO_InitStruct.Pull = GPIO_NOPULL; */
  /*   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; */
  /*   GPIO_InitStruct.Alternate = 0; */
  /*   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); */
  /*   break; */
  /* } */

  rc = fetch_uint16_arg(&value);
  if(rc)
    {
      printf("\nMust supply a Period Value!\n\n");
      return CmdReturnBadParameter1;
    }  

  /*Initialise the Timer Delay*/
  
  __HAL_RCC_TIM1_CLK_ENABLE();

  /* Initialize PWM */
  
  tim1.Instance = TIM1;
  tim1.Init.Prescaler     = HAL_RCC_GetPCLK2Freq() / 1000000;
  tim1.Init.CounterMode   = TIM_COUNTERMODE_UP;
  tim1.Init.Period        = value;
  tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  tim1.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&tim1);

  HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

  HAL_TIM_Base_Start(&tim1);

  
  TIM1->DIER |= 0x01;
  
  /* Configure Output */

  TIM_OC_InitTypeDef sConfig;

  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.Pulse        = 0;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_LOW;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  /* For Channel 1 */

  HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig, TIM_CHANNEL_1);

  HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_1); /* Start PWM Output */

  /* For Channel 2 */

  HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig, TIM_CHANNEL_2);

  HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_2); /* Start PWM Output */

  /* For Channel 3 */

  HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig, TIM_CHANNEL_3);

  HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_3); /* Start PWM Output */
 
  return CmdReturnOk;
}
ADD_CMD("pwminit",CmdPWM_Init,"                Initialise PWM Output");

void TIM1_UP_TIM16_IRQHandler(void){

  TIM1->SR &= ~0x01;

  if(clr==1){
    
    switch(state){
    case 1:
      if(count<=10000){
	count++;
	TIM1->CCR1=0;
      }else{
	state++;
      }    
      break;
    case 2:
      if(count<=5000){
	count++;
	TIM1->CCR1=str;
      }else{
	state++;
      }
      break;
    case 3:
      if(count<=2500){
	count++;
	TIM1->CCR1=str/2;
      }else{
	state=0;
      }
      break;
    }
  }else if(clr==2){
    switch(state){
    case 1:
      if(count<=10000){
	count++;
	TIM1->CCR2=0;
      }else{
	state++;
      }    
      break;
    case 2:
      if(count<=5000){
	count++;
	TIM1->CCR2=str;
      }else{
	state++;
      }
      break;
    case 3:
      if(count<=2500){
        count++;
	TIM1->CCR2=str/2;
      }else{
	state=0;
      }
      break;
    }
  }else{
    switch(state){
    case 1:
      if(count<=10000){
	count++;
	TIM1->CCR3=0;
      }else{
	state++;
      }    
      break;
    case 2:
      if(count<=5000){
	count++;
	TIM1->CCR3=str;
      }else{
	state++;
      }
      break;
    case 3:
      if(count<=2500){
	count++;
	TIM1->CCR3=str/2;
      }else{
	state=0;
      }
      break;
    }
  }
}

ParserReturnVal_t CmdPWM(int action)
{
  if(action!=CMD_INTERACTIVE) return CmdReturnOk;

  uint16_t rc, chn, val;

  rc = fetch_uint16_arg(&chn);
  if(rc)
    {
      printf("\nMust supply a Channel Number!\n\n");
      return CmdReturnBadParameter1;
    }

  rc = fetch_uint16_arg(&val);
  if(rc)
    {
      printf("\nMust supply an Output Value!\n\n");
      return CmdReturnBadParameter1;
    }

  state=0;

  if(chn==1){
    TIM1->CCR1=val;
  }else if(chn==2){
    TIM1->CCR2=val;
  }else if(chn==3){
    TIM1->CCR3=val;
  }else{
    printf("\nMust be 1|2|3 for desired Channel!\n\n");
    return CmdReturnBadParameter1;
  }
  return CmdReturnOk;
}
ADD_CMD("pwm",CmdPWM,"<Channel> <Val> Enable PWM Output");

ParserReturnVal_t CmdPulse(int action)
{
  if(action!=CMD_INTERACTIVE) return CmdReturnOk;

  uint16_t rc, chn;

  rc = fetch_uint16_arg(&chn);
  if(rc)
    {
      printf("\nMust supply a Channel Number!\n\n");
      return CmdReturnBadParameter1;
    }

  state=1;
  clr=chn;
  str=100;
  
  return CmdReturnOk;
}
ADD_CMD("pulse",CmdPulse,"<Channel>       Enable 'Heartbeat' Pulse");
