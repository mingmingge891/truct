


/*˵����
	DMA��DMA_BufferSize  ָ����ת�����������ַbuf��С
	ADC�п���ʹ�ö�ͨ�����ߵ�ͨ��
	ͨ��ADC_ScanConvMode �����Ƿ�ʹ��
	ADC_RegularChannelConfig ����˵����
	���е�Rank�ܹؼ�ָ�����ڶ�ͨ����ת���Ĵ��� �����ʹ�ö�ͨ����ʱ�����ͨ��ֻ����һ��
	���Ǵ�ʱʹ��Rank=2 ��˼�Ƕ�ȡ��2��ͨ�������� ��ʱ�ͻ�û������
*/

/**
  * @brief  DMA
  * @param  None
  * @retval : None
*/
void ADC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);
	
	/*ת������Դ��ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	/*ת������Ŀ�ĵ�ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	/*DMA�����ݴ�С*/
	DMA_InitStructure.DMA_BufferSize = 1;
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
}

/**
  * @brief  ADC ����
  * @param  None
  * @retval : None
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ADC_DMA_Config();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); 
	//ADC_TempSensorVrefintCmd(ENABLE);//ʹ��Ƭ���¶ȴ�����

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //PCLK 6��Ƶ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);//ͨ����ת������(����ͨ����ת����ֵ��DMA�Ĵ���)��ת��ʱ��
	
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);//��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ADC�����ת����������
	
}