#include "my_controller.h"

static char flag1;
static char flag2;

static char i=0;
uint8_t usart1_buffer[BlutTooth_num];

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if(flag2){
			usart1_buffer[i++] = USART_ReceiveData(USART1);
		}
		if(USART_ReceiveData(USART1) == BlueTooth_head_1){
			flag1=1;
			usart1_buffer[i++] = USART_ReceiveData(USART1);
		}
		if(USART_ReceiveData(USART1) == BlueTooth_head_2 && flag1==1){
			flag2=1;
			usart1_buffer[i++] = USART_ReceiveData(USART1);
		}
		if(i==BlutTooth_num){
			i=0;
			flag1=0;
			flag2=0;
			robot.BlueTooth.straight = usart1_buffer[2];
			robot.BlueTooth.spin = usart1_buffer[3];			
		}
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){		
		USART_ClearFlag(USART1, USART_FLAG_IDLE);
	}
}


