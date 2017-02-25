/*
*********************************************************************************************************
* Copyright (c) 2016, XXXX公司
* All rights reserved.
*
* 文件名称: Posture.c
* 文件标识:
* 摘    要: 底盘驱动电机 驱动程序
*
* 版    本: Ver 1.00
* 原 作 者:	Iwen.Liu
* 完成日期: 2016.12.16
*
* 版    本:
* 作    者:
* 完成日期:
* 更改说明:
*
*********************************************************************************************************
*/
#include "Posture.h"

ANGEL_TAP_DEF ArmLeftAngel;
ANGEL_TAP_DEF ArmRightAngel;
ANGEL_TAP_DEF HeadAngel;
ANGEL_TAP_DEF WaistAngel;

AR_CMD_DATA g_CMDHeadBuffer[QUEUE_MAX_SIZE];
AR_CMD_DATA g_CMDWaistBuffer[QUEUE_MAX_SIZE];
AR_CMD_DATA g_CMDLArmBuffer[QUEUE_MAX_SIZE];
AR_CMD_DATA g_CMDRArmBuffer[QUEUE_MAX_SIZE];

RINGBUFF_T g_tCMDHeadRingBuf;
RINGBUFF_T g_tCMDWaistRingBuf;
RINGBUFF_T g_tCMDLArmRingBuf;
RINGBUFF_T g_tCMDRArmRingBuf;


void PostureInit()
{
	RingBuffer_Init( &g_tCMDHeadRingBuf,  g_CMDHeadBuffer, 4, QUEUE_MAX_SIZE );
	RingBuffer_Init( &g_tCMDWaistRingBuf, g_CMDWaistBuffer, 4, QUEUE_MAX_SIZE );
	RingBuffer_Init( &g_tCMDLArmRingBuf,  g_CMDLArmBuffer,  4, QUEUE_MAX_SIZE );
	RingBuffer_Init( &g_tCMDRArmRingBuf,  g_CMDRArmBuffer,  4, QUEUE_MAX_SIZE );
//	HeadAngel.CurrVal = 
//	HeadAngel.CurrValPre = 
//	HeadAngel.DeltaVal = 
//	HeadAngel.OrgVal = 
//	HeadAngel.

//	ArmLeftAngel.CurrVal = 
//	ArmLeftAngel.CurrValPre = 
//	ArmLeftAngel.DeltaVal = 
//	ArmLeftAngel.OrgVal = 
}



/*读取各个关节极限位置的角度值*/

void AngelFLASHCheckAndGet()
{
	if( (STMFLASH_ReadHalfWord(HEAD_DEF_VAL_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(HEAD_L_LIMT_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(HEAD_R_LIMT_ADDR) < 4096) )
	{
		HeadAngel.ErrFlg &= ~(FLASH_READ_ERR);
		HeadAngel.DefVal = STMFLASH_ReadHalfWord(HEAD_DEF_VAL_ADDR);
		HeadAngel.LimitUpVal = STMFLASH_ReadHalfWord(HEAD_L_LIMT_ADDR);
		HeadAngel.LimitDownVal = STMFLASH_ReadHalfWord(HEAD_R_LIMT_ADDR);
	}
	else
	{
		HeadAngel.ErrFlg |= (FLASH_READ_ERR);
	}
	
	if( (STMFLASH_ReadHalfWord(WAIST_DEF_VAL_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(WAIST_L_LIMT_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(WAIST_R_LIMT_ADDR) < 4096) )
	{
		WaistAngel.ErrFlg &= ~(FLASH_READ_ERR);
		WaistAngel.DefVal = STMFLASH_ReadHalfWord(WAIST_DEF_VAL_ADDR);
		WaistAngel.LimitUpVal = STMFLASH_ReadHalfWord(WAIST_L_LIMT_ADDR);
		WaistAngel.LimitDownVal = STMFLASH_ReadHalfWord(WAIST_R_LIMT_ADDR);
	}
	else
	{
		WaistAngel.ErrFlg |= (FLASH_READ_ERR);
	}
	
	if( (STMFLASH_ReadHalfWord(LARM_DEF_VAL_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(LARM_UP_LIMT_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(LARM_DOWN_LIMT_ADDR) < 4096) )
	{
		ArmLeftAngel.ErrFlg &= ~(FLASH_READ_ERR);
		ArmLeftAngel.DefVal = STMFLASH_ReadHalfWord(LARM_DEF_VAL_ADDR);
		ArmLeftAngel.LimitUpVal = STMFLASH_ReadHalfWord(LARM_UP_LIMT_ADDR);
		ArmLeftAngel.LimitDownVal = STMFLASH_ReadHalfWord(LARM_UP_LIMT_ADDR);
	}
	else
	{
		ArmLeftAngel.ErrFlg |= (FLASH_READ_ERR);
	}
	
	if( (STMFLASH_ReadHalfWord(RARM_DEF_VAL_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(RARM_UP_LIMT_ADDR) < 4096) && 
		(STMFLASH_ReadHalfWord(RARM_DOWN_LIMT_ADDR) < 4096) )
	{
		ArmRightAngel.ErrFlg &= ~(FLASH_READ_ERR);
		ArmRightAngel.DefVal = STMFLASH_ReadHalfWord(RARM_DEF_VAL_ADDR);
		ArmRightAngel.LimitUpVal = STMFLASH_ReadHalfWord(RARM_UP_LIMT_ADDR);
		ArmRightAngel.LimitDownVal = STMFLASH_ReadHalfWord(RARM_UP_LIMT_ADDR);
	}
	else
	{
		ArmRightAngel.ErrFlg |= (FLASH_READ_ERR);
	}
		
}

void GetCurrAngel(uint8_t AR)
{
	filter();
	
	ArmRightAngel.CurrVal = After_filter[1];
//	switch(AR)
//	{
//		case AR_HEAD:
//		{
//			HeadAngel.CurrVal = After_filter[1];
//			break;
//		}
//		case AR_WAIST:
//		{
//			WaistAngel.CurrVal = After_filter[3];
//			break;
//		}
//		case AR_LARM:
//		{
//			ArmLeftAngel.CurrVal = After_filter[0];
//			break;
//		}
//		case AR_RARM:
//		{
//			ArmRightAngel.CurrVal = After_filter[2];
//			break;
//		}
//		default:
//			break;
	//}
	
}

/*获取上电时 各个关节状态*/
void GetStartAngel()
{
	filter();
	
	
	HeadAngel.StopVal1 = GetHeadADValue();
	HeadAngel.CurrVal =  GetHeadADValue();
	HeadAngel.StopVal =  GetHeadADValue();
	HeadAngel.TargetVal = GetHeadADValue();
	
	WaistAngel.StopVal1 = GetWaistADValue();
	WaistAngel.CurrVal = GetWaistADValue();
	WaistAngel.StopVal = GetWaistADValue();
	WaistAngel.TargetVal = GetWaistADValue();
	
	ArmLeftAngel.StopVal1 = GetLArmADValue();
	ArmLeftAngel.CurrVal = GetLArmADValue();
	ArmLeftAngel.StopVal = GetLArmADValue();
	ArmLeftAngel.TargetVal = GetLArmADValue();
	
	ArmRightAngel.StopVal1 = GetRArmADValue();
	ArmRightAngel.CurrVal = GetRArmADValue();
	ArmRightAngel.StopVal = GetRArmADValue();
	ArmRightAngel.TargetVal = GetRArmADValue();
	
}

void GetCurrVal()
{
	filter();
	
	HeadAngel.CurrVal = GetHeadADValue();
	
	WaistAngel.CurrVal = GetWaistADValue();
	
	ArmLeftAngel.CurrVal = GetLArmADValue();
	
	ArmRightAngel.CurrVal = GetRArmADValue();
}

/*获取关节启动时 时间戳*/
void GetRunTime(uint8_t _ar)
{
	switch(_ar)
	{
		case AR_HEAD:
		{
			HeadAngel.StartTicks = getms();
			break;
		}
		case AR_WAIST:
		{
			WaistAngel.StartTicks = getms();
			break;
		}
		case AR_LARM:
		{
			ArmLeftAngel.StartTicks = getms();
			break;
		}
		case AR_RARM:
		{
			ArmRightAngel.StartTicks = getms();
			break;
		}
		default:
			break;
	}
}

void IsARStop()
{
	
}

void ARStop(char ar)
{
	switch(ar)
	{
		case AR_HEAD:
		{
			HEAD_SPEED(STOP_SPEED);
			HEAD_STOP();
		}
		case AR_WAIST:
		{
			WAIST_SPEED(STOP_SPEED);
			WAIST_STOP();
		}
		case AR_LARM:
		{
			L_ARM_SPEED(STOP_SPEED);
			L_ARM_STOP();
		}
		case AR_RARM:
		{
			R_ARM_SPEED(STOP_SPEED);
			R_ARM_STOP();
			ArmRightAngel.StopFlg = 1;
		}
		case AR_ALL:
		{
			HEAD_SPEED(STOP_SPEED);
			HEAD_STOP();
			WAIST_SPEED(STOP_SPEED);
			WAIST_STOP();
			L_ARM_SPEED(STOP_SPEED);
			L_ARM_STOP();
			R_ARM_SPEED(STOP_SPEED);
			R_ARM_STOP();
			HeadAngel.StopFlg = 1;
			WaistAngel.StopFlg = 1;
			ArmLeftAngel.StopFlg = 1;
			ArmRightAngel.StopFlg = 1;
			
		}
		default:
			break;
	}
}

void SetARSpeed(char ar, uint16_t speed)
{
	switch(ar)
	{
		case AR_HEAD:
		{
			HEAD_SPEED(speed);
		}
		case AR_WAIST:
		{
			WAIST_SPEED(speed);
		}
		case AR_LARM:
		{
			L_ARM_SPEED(speed);
		}
		case AR_RARM:
		{
			R_ARM_SPEED(speed);
		}
		default:
			break;
	}
}

void HeadRun()
{
	if( (HeadAngel.ErrFlg & ANGEL_ERR) == 1)
	{
		return;
	}
	
	if(HeadAngel.DirFlg == HEAD_TURN_STOP_FLG)
	{
		ARStop(AR_HEAD);
	}
	
	if( (HeadAngel.CurrVal > HeadAngel.TargetVal- DELTA) && (HeadAngel.CurrVal < HeadAngel.TargetVal+DELTA) )
	{
		//ArmRightAngel.StartTicks = getms();
		HeadAngel.Speed = STOP_SPEED;
		//ArmRightAngel.DirFlg = RARM_TURN_STOP_FLG;
		ARStop(AR_HEAD);
		HeadAngel.StopVal = HeadAngel.CurrVal;
		HeadAngel.StopVal1 = HeadAngel.CurrVal;
		HeadAngel.ArriveFlg = 1;
		HeadAngel.StopFlg = 1;
	}
	else if(HeadAngel.CurrVal <= (HeadAngel.TargetVal- DELTA) )
	{
		if( (HeadAngel.TargetVal - HeadAngel.CurrVal) < 100)
		{
			HeadAngel.Speed = LOW_SPEED;
		}
		else
		{
			HeadAngel.Speed = HIGH_SPEED;
		}
		
		if( HeadAngel.DirFlg == HEAD_TURN_LEFT_FLG)
		{
			HEAD_TURN_LEFT();
		}
		else if(HeadAngel.DirFlg == HEAD_TURN_RIGHT_FLG)
		{
			HEAD_TURN_RIGHT();
		}
		HeadAngel.StopFlg =0;
	}
	else
	{
		if( (HeadAngel.CurrVal - HeadAngel.TargetVal ) < 100)
		{
			HeadAngel.Speed = LOW_SPEED;
		}
		else
		{
			HeadAngel.Speed = HIGH_SPEED;
		}
		
		if( HeadAngel.DirFlg == HEAD_TURN_LEFT_FLG)
		{
			HEAD_TURN_LEFT();
		}
		else if(HeadAngel.DirFlg == HEAD_TURN_RIGHT_FLG)
		{
			HEAD_TURN_RIGHT();
		}
		HeadAngel.StopFlg =0;
	}
	
	SetARSpeed( AR_HEAD, HeadAngel.Speed);
}

void WaistRun()
{
	if((WaistAngel.ErrFlg & ANGEL_ERR) == 1)
	{
		return;
	}
	
	if(WaistAngel.DirFlg == WAIST_TURN_STOP_FLG)
	{
		ARStop(AR_WAIST);
	}
	
	if( (WaistAngel.CurrVal > WaistAngel.TargetVal- DELTA) && (WaistAngel.CurrVal < WaistAngel.TargetVal+DELTA) )
	{
		//ArmRightAngel.StartTicks = getms();
		WaistAngel.Speed = STOP_SPEED;
		//ArmRightAngel.DirFlg = RARM_TURN_STOP_FLG;
		ARStop(AR_WAIST);
		WaistAngel.StopVal = WaistAngel.CurrVal;
		WaistAngel.StopVal1 = WaistAngel.CurrVal;
		WaistAngel.ArriveFlg = 1;
		WaistAngel.StopFlg = 1;
	}
	else if(WaistAngel.CurrVal <= (WaistAngel.TargetVal- DELTA) )
	{
		if( (WaistAngel.TargetVal - WaistAngel.CurrVal) < 100)
		{
			WaistAngel.Speed = LOW_SPEED;
		}
		else
		{
			WaistAngel.Speed = HIGH_SPEED;
		}
		
		if( WaistAngel.DirFlg == WAIST_TURN_LEFT_FLG)
		{
			WAIST_TURN_LEFT();
		}
		else if(WaistAngel.DirFlg == WAIST_TURN_RIGHT_FLG)
		{
			WAIST_TURN_RIGHT();
		}
		WaistAngel.StopFlg =0;
	}
	else
	{
		if( (WaistAngel.CurrVal - WaistAngel.TargetVal ) < 100)
		{
			WaistAngel.Speed = LOW_SPEED;
		}
		else
		{
			WaistAngel.Speed = HIGH_SPEED;
		}
		
		if( WaistAngel.DirFlg == WAIST_TURN_LEFT_FLG)
		{
			WAIST_TURN_LEFT();
		}
		else if(WaistAngel.DirFlg == WAIST_TURN_RIGHT_FLG)
		{
			WAIST_TURN_RIGHT();
		}
		WaistAngel.StopFlg =0;
	}
	
	SetARSpeed( AR_WAIST, WaistAngel.Speed);
}

void LArmRun()
{
	
	if( (ArmLeftAngel.ErrFlg & ANGEL_ERR) == 1)
	{
		return;
	}
	
	if(ArmLeftAngel.DirFlg == RARM_TURN_STOP_FLG)
	{
		ARStop(AR_LARM);
	}
	
	if( (ArmLeftAngel.CurrVal > ArmLeftAngel.TargetVal- DELTA) && (ArmLeftAngel.CurrVal < ArmLeftAngel.TargetVal+DELTA) )
	{
		//ArmRightAngel.StartTicks = getms();
		ArmLeftAngel.Speed = STOP_SPEED;
		//ArmRightAngel.DirFlg = RARM_TURN_STOP_FLG;
		ARStop(AR_LARM);
		ArmLeftAngel.StopVal = ArmLeftAngel.CurrVal;
		ArmLeftAngel.StopVal1 = ArmLeftAngel.CurrVal;
		ArmLeftAngel.ArriveFlg = 1;
		ArmLeftAngel.StopFlg = 1;
	}
	else if(ArmLeftAngel.CurrVal <= (ArmLeftAngel.TargetVal- DELTA) )
	{
		if( (ArmLeftAngel.TargetVal - ArmLeftAngel.CurrVal) < 100)
		{
			ArmLeftAngel.Speed = LOW_SPEED;
		}
		else
		{
			ArmLeftAngel.Speed = HIGH_SPEED;
		}
		
		if( ArmLeftAngel.DirFlg == LARM_TURN_UP_FLG)
		{
			L_ARM_TURN_UP();
		}
		else if(ArmLeftAngel.DirFlg == LARM_TURN_DOWN_FLG)
		{
			L_ARM_TURN_DOWN();
		}
		ArmLeftAngel.StopFlg =0;
	}
	else
	{
		if( (ArmLeftAngel.CurrVal - ArmLeftAngel.TargetVal ) < 100)
		{
			ArmLeftAngel.Speed = LOW_SPEED;
		}
		else
		{
			ArmLeftAngel.Speed = HIGH_SPEED;
		}
		
		if( ArmLeftAngel.DirFlg == LARM_TURN_UP_FLG)
		{
			L_ARM_TURN_UP();
		}
		else if(ArmLeftAngel.DirFlg == LARM_TURN_DOWN_FLG)
		{
			L_ARM_TURN_DOWN();
		}
		ArmLeftAngel.StopFlg =0;
	}
	
	SetARSpeed( AR_LARM, ArmLeftAngel.Speed);
}



void AngelCheck()
{

	if( ArmRightAngel.StopFlg == 0)
	{
		if( ( ArmRightAngel.CurrVal > ( ArmRightAngel.CurrValPre - CHECK_DELTA)) && ( ArmRightAngel.CurrVal < ( ArmRightAngel.CurrValPre + CHECK_DELTA)))
		{
			ArmRightAngel.ErrFlg |= ANGEL_ERR;
			ARStop(AR_RARM);
		}
		else
		{
			ArmRightAngel.ErrFlg &= ~(ANGEL_ERR);
		}
		ArmRightAngel.CurrValPre = ArmRightAngel.CurrVal;
	}
	
	if( ArmLeftAngel.StopFlg == 0)
	{
		if( ( ArmLeftAngel.CurrVal > ( ArmLeftAngel.CurrValPre - CHECK_DELTA)) && ( ArmLeftAngel.CurrVal < ( ArmLeftAngel.CurrValPre + CHECK_DELTA)))
		{
			ArmLeftAngel.ErrFlg |= ANGEL_ERR;
			ARStop(AR_LARM);
		}
		else
		{
			ArmLeftAngel.ErrFlg &= ~(ANGEL_ERR);
		}
		ArmLeftAngel.CurrValPre = ArmLeftAngel.CurrVal;
	}
	
	if( HeadAngel.StopFlg == 0)
	{
		if( ( HeadAngel.CurrVal > ( HeadAngel.CurrValPre - CHECK_DELTA)) && ( HeadAngel.CurrVal < ( HeadAngel.CurrValPre + CHECK_DELTA)))
		{
			HeadAngel.ErrFlg |= ANGEL_ERR;
			ARStop(AR_HEAD);
		}
		else
		{
			HeadAngel.ErrFlg &= ~(ANGEL_ERR);
		}
		HeadAngel.CurrValPre = HeadAngel.CurrVal;
	}
	
	if( WaistAngel.StopFlg == 0)
	{
		if( (WaistAngel.CurrVal > ( WaistAngel.CurrValPre - CHECK_DELTA)) && ( WaistAngel.CurrVal < ( WaistAngel.CurrValPre + CHECK_DELTA)))
		{
			WaistAngel.ErrFlg |= ANGEL_ERR;
			ARStop(AR_RARM);
		}
		else
		{
			WaistAngel.ErrFlg &= ~(ANGEL_ERR);
		}
		WaistAngel.CurrValPre = WaistAngel.CurrVal;
	}
	
}

void RArmRun()
{
	
	if((ArmRightAngel.ErrFlg & ANGEL_ERR) == 1)
	{
		return;
	}
	
	if(ArmRightAngel.DirFlg == RARM_TURN_STOP_FLG)
	{
		ARStop(AR_RARM);
	}
	
	if( (ArmRightAngel.CurrVal > ArmRightAngel.TargetVal- DELTA) && (ArmRightAngel.CurrVal < ArmRightAngel.TargetVal+DELTA) )
	{
		//ArmRightAngel.StartTicks = getms();
		ArmRightAngel.Speed = STOP_SPEED;
		//ArmRightAngel.DirFlg = RARM_TURN_STOP_FLG;
		ARStop(AR_RARM);
		ArmRightAngel.StopVal = ArmRightAngel.CurrVal;
		ArmRightAngel.StopVal1 = ArmRightAngel.CurrVal;
		ArmRightAngel.ArriveFlg = 1;
		ArmRightAngel.StopFlg = 1;
	}
	else if(ArmRightAngel.CurrVal <= (ArmRightAngel.TargetVal- DELTA) )
	{
		if( (ArmRightAngel.TargetVal - ArmRightAngel.CurrVal) < 100)
		{
			ArmRightAngel.Speed = LOW_SPEED;
		}
		else
		{
			ArmRightAngel.Speed = HIGH_SPEED;
		}
		
		if( ArmRightAngel.DirFlg == RARM_TURN_UP_FLG)
		{
			R_ARM_TURN_UP();
		}
		else if(ArmRightAngel.DirFlg == RARM_TURN_DOWN_FLG)
		{
			R_ARM_TURN_DOWN();
		}
		ArmRightAngel.StopFlg =0;
	}
	else
	{
		if( (ArmRightAngel.CurrVal - ArmRightAngel.TargetVal ) < 100)
		{
			ArmRightAngel.Speed = LOW_SPEED;
		}
		else
		{
			ArmRightAngel.Speed = HIGH_SPEED;
		}
		
		if( ArmRightAngel.DirFlg == RARM_TURN_UP_FLG)
		{
			R_ARM_TURN_UP();
		}
		else if(ArmRightAngel.DirFlg == RARM_TURN_DOWN_FLG)
		{
			R_ARM_TURN_DOWN();
		}
		ArmRightAngel.StopFlg =0;
	}
	
	SetARSpeed( AR_RARM, ArmRightAngel.Speed);
}


void ARRun()
{
	AR_CMD_DATA _data;
	memset(&_data, 0, sizeof(_data));
	GetCurrVal();
	
	if(WaistAngel.StopFlg == 1)
	{
		if(RingBuffer_GetCount(&g_tCMDWaistRingBuf) > 0)
		{
			if( RingBuffer_Pop( &g_tCMDWaistRingBuf,&_data) )
			{
				WaistAngel.ArriveFlg = 0;
			}
			//ArmRightAngel.DeltaVal = _data.Delta;
			
		}
	}
	if(WaistAngel.ArriveFlg  == 0)
	{
		SetARTargetPos(_data.AR, _data.Dir ,_data.Delta * ANGEL_2_AD_K);
		WaistRun();
		memset(&_data, 0, sizeof(_data));
	}
	
	if(HeadAngel.StopFlg == 1)
	{
		if(RingBuffer_GetCount(&g_tCMDHeadRingBuf) > 0)
		{
			if( RingBuffer_Pop( &g_tCMDHeadRingBuf,&_data) )
			{
				HeadAngel.ArriveFlg = 0;
			}
			//ArmRightAngel.DeltaVal = _data.Delta;
			
		}
	}
	if(HeadAngel.ArriveFlg  == 0)
	{
		SetARTargetPos(_data.AR, _data.Dir ,_data.Delta * ANGEL_2_AD_K);
		HeadRun();
		memset(&_data, 0, sizeof(_data));
	}
	
	if(ArmLeftAngel.StopFlg == 1)
	{
		if(RingBuffer_GetCount(&g_tCMDLArmRingBuf) > 0)
		{
			if( RingBuffer_Pop( &g_tCMDLArmRingBuf,&_data) )
			{
				ArmLeftAngel.ArriveFlg = 0;
			}
			//ArmRightAngel.DeltaVal = _data.Delta;
			
		}
	}
	if(ArmLeftAngel.ArriveFlg  == 0)
	{
		SetARTargetPos(_data.AR, _data.Dir ,_data.Delta * ANGEL_2_AD_K);
		LArmRun();
		memset(&_data, 0, sizeof(_data));
	}
	
	if(ArmRightAngel.StopFlg == 1)
	{
		if(RingBuffer_GetCount(&g_tCMDRArmRingBuf) > 0)
		{
			if( RingBuffer_Pop( &g_tCMDRArmRingBuf,&_data) )
			{
				ArmRightAngel.ArriveFlg = 0;
			}
			//ArmRightAngel.DeltaVal = _data.Delta;
			
		}
	}
	if(ArmRightAngel.ArriveFlg  == 0)
	{
		SetARTargetPos(_data.AR, _data.Dir ,_data.Delta * ANGEL_2_AD_K);
		RArmRun();
		memset(&_data, 0, sizeof(_data));
	}
	
}

void HeadMotorCtl(char Dir, uint16_t DelAngel, uint16_t Speed)
{
//	switch(Dir)
//	{
//		case HEAD_TURN_LEFT:
//		{	
//			if()
//			break;
//		}
//		case HEAD_TURN_RIGHT:
//		{
//			if()
//			break;
//		}
//		case HEAD_TURN_STOP:
//		{
//			if()
//			break;
//		}
//		default:
//			break;
//	}
}

void AdjustMode()
{
	
}

void SetARTargetPos(char AR,char dir,uint16_t delta)
{
	int16_t s16Temp = 0;
	switch(AR)
	{
		
		case AR_HEAD:
		{
			if(IS_LEFT_EQU_INCR_HEAD)
			{
				if(dir == HEAD_TURN_LEFT_FLG)
				{
					s16Temp = HeadAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						HeadAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						HeadAngel.TargetVal = s16Temp;
					}
				}
				if(dir == HEAD_TURN_RIGHT_FLG)
				{
					s16Temp = HeadAngel.StopVal - delta;
					if( s16Temp <0)
					{
						HeadAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						HeadAngel.TargetVal = s16Temp;
					}
				}
			}
			else
			{
				if(dir == HEAD_TURN_RIGHT_FLG)
				{
					s16Temp = HeadAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						HeadAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						HeadAngel.TargetVal = s16Temp;
					}
				}
				if(dir == HEAD_TURN_LEFT_FLG)
				{
					s16Temp = HeadAngel.StopVal - delta;
					if( s16Temp <0)
					{
						HeadAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						HeadAngel.TargetVal = s16Temp;
					}
				}
			}
			break;
		}
		
		case AR_WAIST:
		{
			if(IS_LEFT_EQU_INCR_WAIST)
			{
				if(dir == WAIST_TURN_LEFT_FLG)
				{
					s16Temp = WaistAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						WaistAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						WaistAngel.TargetVal = s16Temp;
					}
				}
				if(dir == WAIST_TURN_RIGHT_FLG)
				{
					s16Temp = WaistAngel.StopVal - delta;
					if( s16Temp <0)
					{
						WaistAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						WaistAngel.TargetVal = s16Temp;
					}
				}
			}
			else
			{
				if(dir == WAIST_TURN_RIGHT_FLG)
				{
					s16Temp = WaistAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						WaistAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						WaistAngel.TargetVal = s16Temp;
					}
				}
				if(dir == WAIST_TURN_LEFT_FLG)
				{
					s16Temp = WaistAngel.StopVal - delta;
					if( s16Temp <0)
					{
						WaistAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						WaistAngel.TargetVal = s16Temp;
					}
				}
			}
			break;
		}
		case AR_LARM:
		{
			if(IS_UP_EQU_INCR_LARM)
			{
				if(dir == LARM_TURN_UP_FLG)
				{
					s16Temp = ArmLeftAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						ArmLeftAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						ArmLeftAngel.TargetVal = s16Temp;
					}
				}
				if(dir == LARM_TURN_DOWN_FLG)
				{
					s16Temp = ArmLeftAngel.StopVal - delta;
					if( s16Temp <0)
					{
						ArmLeftAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						ArmLeftAngel.TargetVal = s16Temp;
					}
				}
			}
			else
			{
				if(dir == LARM_TURN_DOWN_FLG)
				{
					s16Temp = ArmLeftAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						ArmLeftAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						ArmLeftAngel.TargetVal = s16Temp;
					}
				}
				if(dir == LARM_TURN_UP_FLG)
				{
					s16Temp = ArmLeftAngel.StopVal - delta;
					if( s16Temp <0)
					{
						ArmLeftAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						ArmLeftAngel.TargetVal = s16Temp;
					}
				}
			}
			break;
		}
		
		case AR_RARM:
		{
			ArmRightAngel.DirFlg = dir;
			
			if(IS_UP_EQU_INCR_RARM)
			{
				if(dir == RARM_TURN_UP_FLG)
				{
					s16Temp = ArmRightAngel.StopVal + delta;
					
					if( s16Temp > 4095)
					{
						
						ArmRightAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						
						ArmRightAngel.TargetVal = s16Temp;
					}
				}
				if(dir == RARM_TURN_DOWN_FLG)
				{
					//ArmRightAngel.DirFlg = RARM_TURN_DOWN_FLG;
					s16Temp = ArmRightAngel.StopVal - delta;
					if( s16Temp <0)
					{
						
						ArmRightAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						
						ArmRightAngel.TargetVal = s16Temp;
					}
				}
			}
			else
			{
				if(dir == RARM_TURN_DOWN_FLG)
				{
					//ArmRightAngel.DirFlg = RARM_TURN_DOWN_FLG;
					s16Temp = ArmRightAngel.StopVal + delta;
					if( s16Temp > 4095)
					{
						ArmRightAngel.TargetVal = s16Temp - 4095;
					}
					else
					{
						ArmRightAngel.TargetVal = s16Temp;
					}
				}
				if(dir == RARM_TURN_UP_FLG)
				{
					//ArmRightAngel.DirFlg = RARM_TURN_UP_FLG;
					s16Temp = ArmRightAngel.StopVal - delta;
					if( s16Temp <0)
					{
						ArmRightAngel.TargetVal = s16Temp + 4095;
					}
					else
					{
						ArmRightAngel.TargetVal = s16Temp;
					}
				}
			}
			break;
		}
		default:
			break;
	}
}
/**********************************End of File *********************************************************/
