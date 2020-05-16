#ifndef  SI4432_REG_H
#define  SI4432_REG_H

#include "rf_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif
  
 
#define DEF_CARRIER_FRE         470     //default carrier frequency
#define DEF_CINT                0       //RF晶振负载电容12.13偏离微调数
#define DEF_RF_FRE_STEP         100     //Frequency hopping step：100 KHz
#define DEF_RF_HOP              9      //Frequency channel：0~19  Default:0,470.00MHZ
#define RF_RX_GPIO              GPIO0Configuration      
#define RF_TX_GPIO              GPIO1Configuration     
#define RF_NULL_GPIO            GPIO2Configuration
#define GPIO_TO_GND             0x14 
#define DEF_TLDC                3       //休眠唤醒周期，以秒为单位，限制在1-100，0表示关闭周期唤醒
#define DEF_RF_PWR              7       //0-1 1-2 2-5 3-8 4-11 5-14 6-17 7-20dBm;default:3->8dBm
#define DEF_RF_DR               2       //Data rate:0~6  Default:9.6k
#define DEF_TX_COUNT            1       //默认单包数据发射次数
#define RF_FIFO_LENGTH          64      //Radio收发FIFO长度
#define DEF_TIMEOUT             20000    //默认发射超时：1uS*DEF_TIMEOUT


typedef enum _RF_REG_MAP				//revV2&B1
{
  DeviceType 						= 0x00,
  DeviceVersion						= 0x01,
  /* 当前运行状态                        */ 
  DeviceStatus 						= 0x02,
  /* 中断状态标志寄存器                  */ 
  InterruptStatus1 					= 0x03,
  InterruptStatus2 					= 0x04,
  /* 中断使能寄存器控制                  */ 
  InterruptEnable1 					= 0x05,          
  InterruptEnable2 					= 0x06,   
  /* 状态控制寄存器                      */       
  OperatingFunctionControl1 				= 0x07,
  OperatingFunctionControl2 				= 0x08,
  /* 负载电容配置寄存器                  */ 
  CrystalOscillatorLoadCapacitance 			= 0x09,

  MicrocontrollerOutputClock 				= 0x0A,
  GPIO0Configuration 					= 0x0B,
  GPIO1Configuration 					= 0x0C,         
  GPIO2Configuration					= 0x0D,
  IOPortConfiguration					= 0x0E,
  ADCConfiguration					= 0x0F,
  ADCSensorAmplifierOffset				= 0x10,
  ADCValue						= 0x11,
  TemperatureSensorControl				= 0x12,
  TemperatureValueOffset				= 0x13,
  WakeUpTimerPeriod1 					= 0x14,          
  WakeUpTimerPeriod2 					= 0x15,         
  WakeUpTimerPeriod3 					= 0x16,         
  WakeUpTimerValue1					= 0x17,
  WakeUpTimerValue2					= 0x18,
  LowDutyCycleModeDuration 				= 0x19,       
  LowBatteryDetectorThreshold  				= 0x1A,
  BatteryVoltageLevel 					= 0x1B,                          
  IFFilterBandwidth  					= 0x1C,                           
  AFCLoopGearshiftOverride				= 0x1D,
  AFCTimingControl 					= 0x1E,                              
  ClockRecoveryGearshiftOverride 			= 0x1F,              
  ClockRecoveryOversamplingRatio 			= 0x20,              
  ClockRecoveryOffset2 					= 0x21,                       
  ClockRecoveryOffset1 					= 0x22,                       
  ClockRecoveryOffset0 					= 0x23,                     
  ClockRecoveryTimingLoopGain1 				= 0x24,              
  ClockRecoveryTimingLoopGain0 				= 0x25,             
  ReceivedSignalStrengthIndicator 			= 0x26,          
  RSSIThresholdForClearChannelIndicator 	        = 0x27,   
  AntennaDiversityRegister1				= 0x28,
  AntennaDiversityRegister2				= 0x29,
  AFCLimiter						= 0x2A,
  DataAccessControl 					= 0x30,                          
  EZmacStatus 						= 0x31,                                  
  HeaderControl1 					= 0x32,                               
  HeaderControl2 					= 0x33,                              
  PreambleLength 					= 0x34,                               
  PreambleDetectionControl 				= 0x35,     
  /*  同步码              */               
  SyncWord3 						= 0x36,                                   
  SyncWord2 						= 0x37,                                   
  SyncWord1 						= 0x38,                               
  SyncWord0 						= 0x39,    
                              
  TransmitHeader3					= 0x3A,                       
  TransmitHeader2 					= 0x3B,                             
  TransmitHeader1 					= 0x3C,                              
  TransmitHeader0 					= 0x3D,  
  /* 发送包长度            */                             
  TransmitPacketLength 					= 0x3E, 
   /* 包头校验     */                        
  CheckHeader3 						= 0x3F,                                
  CheckHeader2 						= 0x40,                              
  CheckHeader1 						= 0x41,                             
  CheckHeader0 						= 0x42,  
  /* 包头使能，发位为单位           */                            
  HeaderEnable3 					= 0x43,                               
  HeaderEnable2 					= 0x44,                                 
  HeaderEnable1 					= 0x45,                                
  HeaderEnable0 					= 0x46,  
                              
  ReceivedHeader3 					= 0x47,                          
  ReceivedHeader2 					= 0x48,                         
  ReceivedHeader1 					= 0x49,                           
  ReceivedHeader0 					= 0x4A,   
  /* 接收包长度            */                          
  ReceivedPacketLength					= 0x4B,

  AnalogTestBus 					= 0x50,                              
  DigitalTestBus 					= 0x51,                          
  TXRampControl 					= 0x52,                             
  PLLTuneTime 						= 0x53,                            
  CalibrationControl 					= 0x55,                     
  ModemTest 						= 0x56,                               
  ChargepumpTest 					= 0x57,                    
  ChargepumpCurrentTrimming_Override 		        = 0x58,         
  DividerCurrentTrimming				= 0x59,    
  VCOCurrentTrimming 				        = 0x5A,                           
  VCOCalibration_Override 			        = 0x5B,                    
  SynthesizerTest 					= 0x5C,                              
  BlockEnableOverride1 					= 0x5D,                        
  BlockEnableOverride2 					= 0x5E,                      
  BlockEnableOverride3 					= 0x5F,                       
  ChannelFilterCoefficientAddress 			= 0x60,             
  ChannelFilterCoefficientValue 			= 0x61,            
  CrystalOscillator_ControlTest 			= 0x62,               
  RCOscillatorCoarseCalibration_Override 	        = 0x63,    
  RCOscillatorFineCalibration_Override 		        = 0x64,      
  LDOControlOverride 					= 0x65,                          
  DeltasigmaADCTuning1			 		= 0x67,
  DeltasigmaADCTuning2			 		= 0x68,
  AGCOverride1					 	= 0x69,
  AGCOverride2 						= 0x6A,
  GFSKFIRFilterCoefficientAddress 			= 0x6B,            
  GFSKFIRFilterCoefficientValue 			= 0x6C, 
  /* 发射功率                           */              
  TXPower 						= 0x6D,  
  /* 数据速率设定寄存器                 */                                  
  TXDataRate1 						= 0x6E,                            
  TXDataRate0 						= 0x6F, 
                               
  ModulationModeControl1 					= 0x70,                   
  ModulationModeControl2 					= 0x71,
  /* 若AFC使能，则处在接收状态时，设定了允许最大的频率移动  */                    
  FrequencyDeviation 						= 0x72,   
  /* 若AFC使能，则TX与RX间的频率偏差保存在0x73和0x74中  */                         
  FrequencyOffset 						= 0x73,                            
  FrequencyChannelControl					= 0x74,
  FrequencyBandSelect 						= 0x75,                        
  NominalCarrierFrequency1	 				= 0x76,                    
  NominalCarrierFrequency0 					= 0x77,                    
  FrequencyHoppingChannelSelect 			        = 0x79,
  FrequencyHoppingStepSize 					= 0x7A,                     
  TXFIFOControl1 						= 0x7C,                        
  TXFIFOControl2 						= 0x7D,    
  RXFIFOControl 						= 0x7E,                               
  FIFOAccess							= 0x7F, 
}RF_REG_MAP;

typedef struct               
{
  rf_uint8  tLdc;                    //休眠周期
  rf_uint8  nHop;                    //射频频点
  rf_uint8  nPower;                  //射频功率
  rf_uint8  nBaud;                   //射频速率
  rf_uint8  tCount;                  //发射计数
  rf_uint8  rIdle;                   //空闲计时
}RF_CFG_T;

#ifdef __cplusplus
}
#endif

#endif
