#include "usart.h"
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif

#include "const.h"
#include "robot.h"

void usart1_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
		
}

void usart2_Init(uint8_t* rx1_buf, uint8_t* rx2_buf, uint16_t dma_buf_num)
{
    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// | RCC_AHB1Periph_DMA1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);//RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PB11  usart3 rx
    /* -------------- Configure GPIO ---------------------------------------*/
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_100MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        USART_DeInit(USART2);

        USART_InitStructure.USART_BaudRate = 100000;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_Mode = USART_Mode_Rx;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART2, &USART_InitStructure);

        USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

        USART_ClearFlag(USART2, USART_FLAG_IDLE);
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        USART_Cmd(USART2, ENABLE);
    }

    /* -------------- Configure NVIC ---------------------------------------*/
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RC_NVIC;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }


    /* -------------- Configure DMA -----------------------------------------*/
    {
        DMA_InitTypeDef DMA_InitStructure;
        DMA_DeInit(DMA1_Stream5);
        DMA_InitStructure.DMA_Channel = DMA_Channel_4;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->DR);
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
        DMA_InitStructure.DMA_BufferSize = dma_buf_num;//
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMA1_Stream5, &DMA_InitStructure);
        DMA_DoubleBufferModeConfig(DMA1_Stream5, (uint32_t)rx2_buf, DMA_Memory_0);
        DMA_DoubleBufferModeCmd(DMA1_Stream5, ENABLE);
        DMA_Cmd(DMA1_Stream5, DISABLE); //Add a disable  DMA_Cmd(DMA1_Stream1, DISABLE)
        DMA_Cmd(DMA1_Stream5, ENABLE);//   DMA_Cmd(DMA1_Stream1, ENABLE)
    }
}

void usart6_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);

	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
//	
//	{
//        DMA_InitTypeDef DMA_InitStructure;
//        DMA_DeInit(DMA2_Stream1);
//        DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//        DMA_InitStructure.DMA_BufferSize = 10;	//
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // ѭ��ģʽ
//        DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMA2_Stream1, &DMA_InitStructure);
//        DMA_DoubleBufferModeConfig(DMA2_Stream1, (uint32_t)rx2_buf, DMA_Memory_0);
//        DMA_DoubleBufferModeCmd(DMA2_Stream1, ENABLE);
//        DMA_Cmd(DMA2_Stream1, DISABLE); //Add a disable  DMA_Cmd(DMA1_Stream1, DISABLE)
//        DMA_Cmd(DMA2_Stream1, ENABLE);//   DMA_Cmd(DMA1_Stream1, ENABLE)
//    }
	
}

void RC_unable(void)
{
    USART_Cmd(USART2, DISABLE);
}
void RC_restart(uint16_t dma_buf_num)
{
    USART_Cmd(USART2, DISABLE);
    DMA_Cmd(DMA1_Stream5, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, dma_buf_num);

    USART_ClearFlag(USART2, USART_FLAG_IDLE);

    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF4);
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF4);
    DMA_Cmd(DMA1_Stream5, ENABLE);
    USART_Cmd(USART2, ENABLE);
}


