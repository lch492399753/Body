/*
*********************************************************************************************************
* Copyright (c) 2016, XXXX��˾
* All rights reserved.
*
* �ļ�����: Posture.h
* �ļ���ʶ:
* ժ    Ҫ: ����������� ��������
*
* ��    ��: Ver 1.00
* ԭ �� ��:	Iwen.Liu
* �������: 2016.12.16
*
* ��    ��:
* ��    ��:
* �������:
* ����˵��:
*
*********************************************************************************************************
*/
#include "sys.h"
#include "stmflash.h"
#include "ADC.h"
#include "motor.h"
#include "delay.h"
#include "stdio.h"
#include "Ring_buffer.h"
#include "delay.h"
#include "string.h"
#define CONFIG_POSTURE_TIMEOUT_MS	2000

#define HIGH_SPEED 550
#define LOW_SPEED  600
#define STOP_SPEED 719

#define DELTA 5
#define CHECK_DELTA 50

#define ANGEL_2_AD_K  11

#define HEAD_ANGEL_ERR 	-1
#define WAIST_ANGEL_ERR -2
#define LARM_ANGEL_ERR 	-3
#define RARM_ANGEL_ERR 	-4


#define HEAD_ADC_CH				3		//����ͷ��ADCͨ��
#define WAIST_ADC_CH			0		//��������ADCͨ��
#define LARM_ADC_CH				2		//������첲ADCͨ��
#define RARM_ADC_CH				1		//�����и첲ADCͨ��

#define GetHeadADValue()		After_filter[HEAD_ADC_CH]
#define GetLArmADValue()		After_filter[LARM_ADC_CH]
#define GetRArmADValue()		After_filter[RARM_ADC_CH]
#define GetWaistADValue()		After_filter[WAIST_ADC_CH]



#define IS_LEFT_EQU_INCR_HEAD	1		//ͷ����תADC��ֵ����   ����Ϊ1 ����Ϊ0			
#define IS_LEFT_EQU_INCR_WAIST	0		//������תADC��ֵ����   ����Ϊ1 ����Ϊ0
#define IS_UP_EQU_INCR_LARM		0		//��첲��̧ADC��ֵ���� ����Ϊ1 ����Ϊ0
#define IS_UP_EQU_INCR_RARM		1		//�Ҹ첲��̧ADC��ֵ���� ����Ϊ1 ����Ϊ0

#define HEAD_TURN_STOP_FLG		1		//ͷ��ֹͣ
#define HEAD_TURN_LEFT_FLG		2		//ͷ����ת
#define HEAD_TURN_RIGHT_FLG		3		//ͷ����ת

#define WAIST_TURN_STOP_FLG		1		//����ֹͣ
#define WAIST_TURN_LEFT_FLG		2		//������ת
#define WAIST_TURN_RIGHT_FLG	3		//������ת

#define LARM_TURN_STOP_FLG		1		//���ֹͣ
#define LARM_TURN_UP_FLG		2		//���̧��
#define LARM_TURN_DOWN_FLG		3		//��۷���

#define RARM_TURN_STOP_FLG		1		//�ұ�ֹͣ
#define RARM_TURN_UP_FLG		2		//�ұ�̧��
#define RARM_TURN_DOWN_FLG		3		//�ұ۷���

/*�ؽ���Ŷ���*/
#define AR_HEAD					1
#define AR_WAIST 				2
#define AR_LARM 				3
#define AR_RARM 				4
#define AR_ALL					5

#define UP_LIMIT 				0
#define DOWN_LIMIT				1
#define DEF_POS 				2

#define FLASH_BASE_ADDR  		0X0803F800     //����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define LIMT_BASE_ADDR FLASH_BASE_ADDR
 
/*******����λ�ô洢��ַ******/
#define HEAD_DEF_VAL_ADDR	(LIMT_BASE_ADDR+2)
#define HEAD_L_LIMT_ADDR 	(LIMT_BASE_ADDR+4)
#define HEAD_R_LIMT_ADDR 	(LIMT_BASE_ADDR+6)

#define WAIST_DEF_VAL_ADDR	(LIMT_BASE_ADDR+8)
#define WAIST_L_LIMT_ADDR 	(LIMT_BASE_ADDR+10)
#define WAIST_R_LIMT_ADDR 	(LIMT_BASE_ADDR+12)

#define LARM_DEF_VAL_ADDR	(LIMT_BASE_ADDR+14)
#define LARM_UP_LIMT_ADDR 	(LIMT_BASE_ADDR+16)
#define LARM_DOWN_LIMT_ADDR (LIMT_BASE_ADDR+18)

#define RARM_DEF_VAL_ADDR	(LIMT_BASE_ADDR+20)
#define RARM_UP_LIMT_ADDR 	(LIMT_BASE_ADDR+22)
#define RARM_DOWN_LIMT_ADDR (LIMT_BASE_ADDR+24)

#define OBST0_DIS_ADDR		(LIMT_BASE_ADDR+26)
#define OBST1_DIS_ADDR		(LIMT_BASE_ADDR+28)
#define OBST2_DIS_ADDR		(LIMT_BASE_ADDR+30)

#define OBST_BIT_CHECK_ADDR		(LIMT_BASE_ADDR+32)

#define ANGEL_ERR 0x01
#define FLASH_READ_ERR	0x02

/*ָ����������*/
#define QUEUE_MAX_SIZE 32

typedef unsigned char qdata;
typedef unsigned short qsize;
typedef struct
{
	int16_t DefVal;				//�ؽ�Ĭ�ϽǶ�
	int16_t  StopVal;				//�ؽ���һ�����Ƕ�ֵ
	int16_t  StopVal1;
	int16_t  CurrVal;				//�ؽڵ�ǰ�Ƕ�ֵ
	int16_t  CurrValPre;				//�ؽڵ�ǰ�Ƕ�ֵ
	int16_t  TargetVal;				//Ŀ��Ƕ�ֵ
	uint16_t DeltaVal;				//��ǰ�Ƕ�ֵ��Ŀ��Ƕ�ֵ��ƫ��ֵ
	int16_t LimitUpVal;			//�ؽڽǶ�����
	int16_t LimitDownVal;			//�ؽڽǶ�����
	uint16_t Speed;					//�˶��ٶ�
	uint8_t  ArriveFlg;				//�ؽڵ�λ��ʶ
	uint8_t  DirFlg;					//�ؽ��˶������ʶλ
	uint16_t StopFlg;	
	uint8_t  RunFlg;
	uint32_t StartTicks;	
	uint8_t ErrFlg;
}ANGEL_TAP_DEF;

typedef struct AR_Act_Cmd_Node
{
	uint8_t AR;
	uint8_t Dir;
	uint8_t Delta;
	uint8_t Speed;
}AR_CMD_DATA;



extern ANGEL_TAP_DEF ArmLeftAngel;
extern ANGEL_TAP_DEF ArmRightAngel;
extern ANGEL_TAP_DEF HeadAngel;
extern ANGEL_TAP_DEF WaistAngel;

extern AR_CMD_DATA g_CMDHeadBuffer[QUEUE_MAX_SIZE];
extern AR_CMD_DATA g_CMDWaistBuffer[QUEUE_MAX_SIZE];
extern AR_CMD_DATA g_CMDLArmBuffer[QUEUE_MAX_SIZE];
extern AR_CMD_DATA g_CMDRArmBuffer[QUEUE_MAX_SIZE];

extern RINGBUFF_T g_tCMDHeadRingBuf;
extern RINGBUFF_T g_tCMDWaistRingBuf;
extern RINGBUFF_T g_tCMDLArmRingBuf;
extern RINGBUFF_T g_tCMDRArmRingBuf;


void GetAngelLimit(void);
void SetAngelLimit(uint8_t AR);
void SetDefPos(char AR);
void GetStartAngel(void);
void SetARTargetPos(char AR,char dir, uint16_t delta);
void ARRun(void);
void GetCurrVal(void);

void GetRunTime(uint8_t _ar);
void HeadRun(void);
void WaistRun(void);
void LArmRun(void);
void RArmRun(void);
void PostureInit(void);
void AngelFLASHCheckAndGet(void);
void AngelCheck(void);
void ARStop(char ar);
/**********************************End of File *********************************************************/

