#ifndef __CMDPRASE_H
#define __CMDPRASE_H

#include "sys.h"
#include "ring_buffer.h"

#define CMD_COM COM1

enum _cmd_fun
{
	CMD_ACT = 1,
	CMD_LIMIT,
	CMD_OBST,
	CMD_EXPRESS,
};

enum _cmd_obst_id
{
	EN_ALL = 1,
	DISEN_ALL,
	DISEN_CH1,
	DISEN_CH2,
	DISEN_CH3,
	SET_DIS,
};

enum _act_cmd
{
	CMD_HEAD_TURN_LEFT = 1,
	CMD_HEAD_TURN_RIGHT,
	CMD_HEAD_STOP,
	
	CMD_WAIST_TURN_LEFT,
	CMD_WAIST_TURN_RIGHT,
	CMD_WAIST_STOP,
	
	CMD_LARM_TURN_UP,
	CMD_LARM_TURN_DOWN,
	CMD_LARM_STOP,
	
	CMD_RARM_TURN_UP,
	CMD_RARM_TURN_DOWN,
	CMD_RARM_STOP,
	
	CMD_ALL_BCAK,
};

enum _limit_cmd
{
	CMD_SET_HEAD_DEF = 1,
	CMD_SET_HEAD_L_LIMIT,
	CMD_SET_HEAD_R_LIMIT,
	
	CMD_SET_WAIST_DEF,
	CMD_SET_WAIST_L_LIMIT,
	CMD_SET_WAIST_R_LIMIT,
	
	CMD_SET_LARM_DEF,
	CMD_SET_LARM_U_LIMIT,
	CMD_SET_LARM_D_LIMIT,
	
	CMD_SET_RARM_DEF,
	CMD_SET_RARM_U_LIMIT,
	CMD_SET_RARM_D_LIMIT,
};

enum _express_cmd
{
	CMD_EXPRESS1 = 1,
	CMD_EXPRESS2,
	CMD_EXPRESS3,
	
	CMD_EXPRESS4,
	CMD_EXPRESS5,
	CMD_EXPRESS6,
	
	CMD_EXPRESS7,
	CMD_EXPRESS8,
	CMD_EXPRESS9,
	
	CMD_EXPRESS10,
	CMD_EXPRESS11,
	CMD_EXPRESS12,
};

typedef __packed struct
{
	uint8_t u8HeadFlag;		//帧头		
	uint8_t u8CMDId;		//命令ID
	uint8_t u8LimitFlag;	//极限设置命令
	uint8_t u8ObstFlag;		//障碍设置命令
	uint8_t u8ExpressFlag;	//表情选择命令
	uint8_t u8ARFlag;		//关节选择
	uint8_t u8DirFlag;		//运动方向设置
	uint8_t u8Angel;		//角度高位
	uint8_t u8SpeedH;		//速度高位
	uint8_t u8SpeedL;		//速度低位
	uint8_t u8ObstDis0;		//障碍0
	uint8_t u8ObstDis1;		//障碍1
	uint8_t u8ObstDis2;		//障碍2
	uint8_t u8ErrMsg;		//保留位
	uint8_t u8ForceMsg;		//保留位
	uint8_t u8CheckSum;		//校验和
	uint8_t u8TailFlag;		//帧尾
} CMDTypeDef; 

typedef union
{
	CMDTypeDef CMDBodyBlk;
	uint8_t u8CMDBodyByte[17];
} CMDBlkTypeDef;

/**用于发送**/
enum _cmd_id_tx
{
	OBST_MSG = 1,
	ERR_MSG,
	LIMIT_MSG,
	FROCE_MSG,
};

enum _err_msg
{
	ERR_NONE = 0,
	
};

enum _force_msg
{
	FORCE_NONE = 0,
};

extern CMDBlkTypeDef CMDBlk;

extern uint8_t g_u8ErrID;
extern uint8_t g_u8ForceID;
extern uint8_t g_u8LimitMsg ;

extern uint8_t GetCheckSum(uint8_t *uBuff, uint8_t uBuffLen) ;

void CMDPrase(void);
void SendMsg(void);
#endif
