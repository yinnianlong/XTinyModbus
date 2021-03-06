/**
* @file 		MD_RTU_Fun.h
* @brief		无
* @details	无
* @author		zspace
* @date		2020/3/23
* @version	A001
* @par History:  无       
*/
#ifndef _MD_RTU_FUN_H__
#define _MD_RTU_FUN_H__

#include "MD_RTU_Queue.h"
#include "MD_RTU_RegCoil.h"
#include "MD_RTU_Type.h"
#include "MD_RTU_Tool.h"

//typedef uint8 (*MDS_RTU_ReadDataProcessFunction)(uint16 reg,uint16 regNum,uint8 funCode);
//typedef uint8 (*MDS_RTU_WriteDataProcessFunction)(uint16 reg,uint16 regNum,uint8 funCode,void* data,uint8 byteCount);


#define 	REG_COIL_ITEM_NUM 	20

#define 	MDS_RTU_CMD_SIZE		256			/*单条指令长度*/
typedef struct{	
	ModbusBase											modbusBase;													/*继承modbusBase*/
	MDSqQueue 											mdSqQueue;													/*数据接收队列*/
	MDSqQueue												mdMsgSqQueue;												/*消息处理队列*/
	uint8														salveAddr;													/*从机地址*/
	
	uint8														serialReadCache[MDS_RTU_CMD_SIZE];	/*单指令读取队列*/
	uint16													serialReadCount;										/*指令的长度*/

	PRegCoilItem 										pRegCoilList[REG_COIL_ITEM_NUM];					/*寄存器注册表*/

	/*上次接收的时间,0xFFFFFFF表示未起开始检测帧*/
	uint32 lastTimesTick;
	/*当前的实时时间单位100US*/
	uint32 timesTick;
	
	/*帧间隔时间3.5T*/
	uint16 frameIntervalTime;
	
	/*接收的CRC16*/
	uint16 CRC16Update;
	
	/*发送的CRC16*/
	uint16 CRC16SendUpdate;
}*PModbusS_RTU,ModbusS_RTU;


#define MDS_RTU_FUN_CODE(a)					(((PModbusS_RTU)(a))->serialReadCache[1])
#define MDS_RTU_CRC16(a)						(((((PModbusS_RTU)(a))->serialReadCache[a->serialReadCount-1])<<8)|\
(((PModbusS_RTU)(a))->serialReadCache[a->serialReadCount]))

#define MDS_RTU_START_REG(a)				(((((PModbusS_RTU)(a))->serialReadCache[2])<<8)|\
(((PModbusS_RTU)(a))->serialReadCache[3]))
#define MDS_RTU_REGS_NUM(a)					(((((PModbusS_RTU)(a))->serialReadCache[4])<<8)|\
(((PModbusS_RTU)(a))->serialReadCache[5]))



/*下面宏用来发送modbus数据,并同时计算校验*/
#define MSD_START_SEND(a)		(a)->CRC16SendUpdate=0xFFFF
#define MSD_SEND_BYTE(a,b)	(a)->CRC16SendUpdate=crc16_update((a)->CRC16SendUpdate,(b));\
MDS_RTU_SendByte(a,b)
#define MSD_SEND_END(a)			(TO_MDBase(a))->mdRTUSendBytesFunction((uint8*)(&((a)->CRC16SendUpdate)),2)


void MDS_RTU_Init(PModbusS_RTU pModbusRTU,MD_RTU_SerialInit mdRTUSerialInitFun,uint8 salveAddr,uint32 baud,uint8 dataBits,uint8 stopBit,uint8 parity);
void MDS_RTU_Process(PModbusS_RTU pModbus_RTU);



#endif
