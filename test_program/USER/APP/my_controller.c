#include "my_controller.h"

static char flag;
static char i;
static uint8_t usart1_buffer[6];

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{

		usart1_buffer[i++] = USART_ReceiveData(USART1);
		if(i == BlutTooth_num) {
			i = 0;
		}
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		
		for(int i=0, j=1; i<BlutTooth_num; i++, j++){
			if(usart1_buffer[i] == BlueTooth_head_1 && usart1_buffer[++i] == BlueTooth_head_2){
				
				break;
			}
		}
		
		USART_ClearFlag(USART1, USART_FLAG_IDLE);
	}
} 


