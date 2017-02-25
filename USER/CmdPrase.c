#include "CmdPrase.h"
#include "Posture.h"
#include "usart.h"
#include "stmflash.h"
#include "usart.h"
#include "eye.h"
#include "UltrasonicWave.h"
CMDBlkTypeDef CMDBlk;
CMDBlkTypeDef TxCMDBlk;

uint8_t g_u8ErrID = 0;
uint8_t g_u8ForceID = 0;
uint8_t g_u8LimitID = 0;

uint8_t GetCheckSum(uint8_t *uBuff, uint8_t uBuffLen) 
{ 
	uint8_t  i, uSum = 0; 

	for(i = 0; i < uBuffLen; i++) 
	{ 
		uSum = uSum + uBuff[i]; 
	} 
	uSum = (~uSum) + 1; 

	return uSum; 
} 

void CMDActSet(uint8_t AR, uint8_t Dir, uint8_t Angel, uint16_t Speed)
{
	AR_CMD_DATA _data;
//	memset( _data, 0, sizeof(_data) );
	_data.AR = AR;
	_data.Delta = Angel;
	_data.Dir = Dir;
	_data.Speed =  Speed;
	if(AR == 0)
	{
		return;
	}
	
	switch(AR)
	{
		case AR_HEAD:
		{
			RingBuffer_Insert ( &g_tCMDHeadRingBuf, &_data );
			GetRunTime(AR);
			break;
		}
		case AR_WAIST:
		{
			RingBuffer_Insert ( &g_tCMDWaistRingBuf, &_data );
			GetRunTime(AR);
			break;
		}
		case AR_LARM:
		{
			RingBuffer_Insert ( &g_tCMDLArmRingBuf, &_data );
			break;
		}
		case AR_RARM:
		{
			RingBuffer_Insert ( &g_tCMDRArmRingBuf, &_data );
			GetRunTime(AR);
			break;
		}
		default:
			break;
	}
}

void CMDLimitSet(uint8_t _cmd)
{
	switch(_cmd)
	{
		case CMD_SET_HEAD_DEF:
		{
			STMFLASH_Write(HEAD_DEF_VAL_ADDR,(After_filter+HEAD_ADC_CH),1);
			break;
		}
		case CMD_SET_HEAD_L_LIMIT:
		{
			STMFLASH_Write(HEAD_L_LIMT_ADDR,(After_filter+HEAD_ADC_CH),1);
			break;
		}
		case CMD_SET_HEAD_R_LIMIT:
		{
			STMFLASH_Write(HEAD_R_LIMT_ADDR,(After_filter+HEAD_ADC_CH),1);
			break;
		}
		case CMD_SET_WAIST_DEF:
		{
			STMFLASH_Write(LARM_DEF_VAL_ADDR,(After_filter+AR_WAIST),1);
			break;
		}
		case CMD_SET_WAIST_L_LIMIT:
		{
			STMFLASH_Write(WAIST_L_LIMT_ADDR,(After_filter+AR_WAIST),1);
			break;
		}
		case CMD_SET_WAIST_R_LIMIT:
		{
			STMFLASH_Write(WAIST_R_LIMT_ADDR,(After_filter+AR_WAIST),1);
			break;
		}
		case CMD_SET_LARM_DEF:
		{
			STMFLASH_Write(LARM_DEF_VAL_ADDR,(After_filter+AR_LARM),1);
			break;
		}
		case CMD_SET_LARM_U_LIMIT:
		{
			STMFLASH_Write(LARM_UP_LIMT_ADDR,(After_filter+AR_LARM),1);
			break;
		}
		case CMD_SET_LARM_D_LIMIT:
		{
			STMFLASH_Write(LARM_DOWN_LIMT_ADDR,(After_filter+AR_LARM),1);
			break;
		}
		case CMD_SET_RARM_DEF:
		{
			STMFLASH_Write(LARM_DEF_VAL_ADDR,(After_filter+AR_RARM),1);
			break;
		}
		case CMD_SET_RARM_U_LIMIT:
		{
			STMFLASH_Write(RARM_UP_LIMT_ADDR,(After_filter+AR_RARM),1);
			break;
		}
		case CMD_SET_RARM_D_LIMIT:
		{
			STMFLASH_Write(RARM_DOWN_LIMT_ADDR,(After_filter+AR_RARM),1);
			break;
		}
		default:
			break;
	}
}

void CMDObstSet(uint8_t _CmdID, uint16_t ObstDis0,uint16_t ObstDis1,uint16_t ObstDis2)
{
	switch(_CmdID)
	{
		case EN_ALL:
		{
			g_u8ObstCheckFlg = 0x07;
			WriteObstCheckToFlash((uint16_t)g_u8ObstCheckFlg);
			
			break;
		}
		case DISEN_ALL:
		{
			g_u8ObstCheckFlg = 0x00;
			WriteObstCheckToFlash((uint16_t)g_u8ObstCheckFlg);
			break;
		}
		case DISEN_CH1:
		{
			g_u8ObstCheckFlg = 0x06;
			WriteObstCheckToFlash((uint16_t)g_u8ObstCheckFlg);
			//WriteObstDisToFlash(ObstDis0, ObstDis1, ObstDis2);
			break;
		}
		case DISEN_CH2:
		{
			g_u8ObstCheckFlg = 0x05;
			WriteObstCheckToFlash((uint16_t)g_u8ObstCheckFlg);
			break;
		}
		case DISEN_CH3:
		{
			g_u8ObstCheckFlg = 0x03;
			WriteObstCheckToFlash((uint16_t)g_u8ObstCheckFlg);
			break;
		}
		case SET_DIS:
		{
			SetObstDis(ObstDis0, ObstDis1, ObstDis2);
			WriteObstDisToFlash(ObstDis0, ObstDis1, ObstDis2);
			break;
		}
		default:
			break;
	}
}

void CMDExpressSet(uint8_t ExpressID)
{
	g_u8ExpressID = ExpressID;
}


void CMDPrase()
{
	uint8_t u8CMDCheckSum = 0;
	uint8_t u8Angel = 0;
	uint16_t u16Speed = 0;
	uint8_t u8CmdCnt = 0;

	u8CmdCnt = RingBuffer_GetCount( &g_tUSART4RxRingBuf );
	if(u8CmdCnt < 17)
	{
		//RingBuffer_Flush(&g_tUSART1RxRingBuf);
		return;
	}
	
	RingBuffer_PopMult(&g_tUSART4RxRingBuf, CMDBlk.u8CMDBodyByte, 17);
		
	u8CMDCheckSum = GetCheckSum(CMDBlk.u8CMDBodyByte, 17);
		
	if( (CMDBlk.CMDBodyBlk.u8HeadFlag == 0x02) &&
		(CMDBlk.CMDBodyBlk.u8TailFlag == 0x03) /*&&
		(CMDBlk.CMDBodyBlk.u8CheckSum == u8CMDCheckSum)*/ )
	{
			
		switch(CMDBlk.CMDBodyBlk.u8CMDId)
		{
			case CMD_ACT:
			{
				u8Angel = (uint8_t) CMDBlk.CMDBodyBlk.u8Angel;
				u16Speed = (uint16_t)((CMDBlk.CMDBodyBlk.u8SpeedH << 8) | (CMDBlk.CMDBodyBlk.u8SpeedL));
				CMDActSet(CMDBlk.CMDBodyBlk.u8ARFlag, CMDBlk.CMDBodyBlk.u8DirFlag, u8Angel, u16Speed);
				break;
			}
			case CMD_LIMIT:
			{
				CMDLimitSet(CMDBlk.CMDBodyBlk.u8LimitFlag);
				break;
			}
			case CMD_OBST:
			{
				CMDObstSet( CMDBlk.CMDBodyBlk.u8ObstFlag, CMDBlk.CMDBodyBlk.u8ObstDis0,
							CMDBlk.CMDBodyBlk.u8ObstDis1, CMDBlk.CMDBodyBlk.u8ObstDis2 );
				break;
				
			}
			case CMD_EXPRESS:
			{
				CMDExpressSet(CMDBlk.CMDBodyBlk.u8ExpressFlag);
				break;
			}
			default:
				break;
		}
	
	}
	else
	{
		RingBuffer_Flush(&g_tUSART4RxRingBuf);
		return;
	}
}

void SendMsg()
{
	
	TxCMDBlk.CMDBodyBlk.u8HeadFlag = 0x02;
	TxCMDBlk.CMDBodyBlk .u8ObstDis0 = g_u8ObstFlg;
	TxCMDBlk.CMDBodyBlk .u8ObstDis1 = g_sonar[1].distance;
	TxCMDBlk.CMDBodyBlk .u8ObstDis2 = g_sonar[2].distance;
	TxCMDBlk.CMDBodyBlk.u8ErrMsg = g_u8ErrID;
	TxCMDBlk.CMDBodyBlk.u8ForceMsg = g_u8ForceID;
	TxCMDBlk.CMDBodyBlk.u8LimitFlag = g_u8LimitID;
	
	TxCMDBlk.CMDBodyBlk.u8TailFlag = 0x03;
	TxCMDBlk.CMDBodyBlk.u8CheckSum = GetCheckSum(TxCMDBlk.u8CMDBodyByte, 17);
	
	USART4_SendStrings(TxCMDBlk.u8CMDBodyByte, sizeof(TxCMDBlk.u8CMDBodyByte));
	//USART1_SendStrings(aa,sizeof(aa));
}


