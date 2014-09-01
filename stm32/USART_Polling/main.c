/**
  ******************************************************************************
  * @file    main.c
  * @author  J.Shankarappa
  * @version V0.0.1
  * @date    13-Mar3h-2012
  * @brief   USART3 Program for STM32L-Discovery Board
  ******************************************************************************
  */

#include "stm32l1xx.h"
#include <discover_board.h>

USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

int main(void)
{

	uint8_t ch;

  /* Enable GPIO clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

   /* Enable USART3 clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);
   GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);

   /* Configure USART3 Rx & Tx as alternate function */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   /* GPIOC --> USART3 Rx & Tx */
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

   USART_InitStructure.USART_BaudRate = 9600;
   USART_InitStructure.USART_WordLength = USART_WordLength_9b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   /* USART configuration */
   USART_Init(USART3, &USART_InitStructure);

   /* Enable USART */
   USART_Cmd(USART3, ENABLE);

   do {
	   USART_ReceiveData(USART3);
   } while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET);


   while(1) {
	   /* wait for character to arrive */
	   while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
		   ;

	   GPIO_TOGGLE(GPIOB,GPIO_Pin_6);

	   /* read the available data */
	   ch = USART_ReceiveData(USART3);
	   ch++;
          
	   /* Wait while TX is full */
	   while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
    	   ;

	   /*transmit data only when TX is empty */
	   USART_SendData(USART3, ch);
	   GPIO_TOGGLE(GPIOB, GPIO_Pin_7);
   }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
