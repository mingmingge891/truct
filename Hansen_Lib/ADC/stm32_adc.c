


/*说明：
	DMA的DMA_BufferSize  指的是转换接收区域地址buf的小
	ADC中可以使用多通道或者单通道
	通过ADC_ScanConvMode 配置是否使能
	ADC_RegularChannelConfig 函数说明：
	其中的Rank很关键指的是在多通道中转换的次序 如果在使用多通道的时候如果通道只开了一个
	但是此时使用Rank=2 意思是读取第2个通道的数据 此时就会没有数据
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
	
	/*转换数据源地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	/*转换数据目的地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	/*DMA的数据大小*/
	DMA_InitStructure.DMA_BufferSize = 1;
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
}

/**
  * @brief  ADC 配置
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
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//多通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件启动转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数目
	ADC_Init(ADC1, &ADC_InitStructure); 
	//ADC_TempSensorVrefintCmd(ENABLE);//使能片内温度传感器

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //PCLK 6分频
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);//通道，转换次序(就是通道的转换的值到DMA的次序)，转换时间
	
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能ADC的软件转换启动功能
	
}