#include "MD_RTU_Serial.h"

#include "MD_RTU_Fun.h"
#include "usart.h"
#include "tim3.h"

/*��ǰ���ڵ�Modbus*/
PModbusS_RTU _pModbus_RTU=NULL;

void MDSSerialSendBytes(uint8 *bytes,uint16 num);
void MDSSerialSWRecv_Send(uint8 mode);
void MDSTimeHandler100US(uint32 times);

/*Ӳ����ʼ�����������ڴ˳�ʼ������*/
void MDSInitSerial(void* obj,uint32 baud,uint8 dataBits,uint8 stopBit,uint8 parity){
	PModbusS_RTU pModbus_RTU=obj;
	if(pModbus_RTU==NULL){return ;}
	_pModbus_RTU=pModbus_RTU;
	
	_pModbus_RTU->mdsRTUSendBytesFunction=MDSSerialSendBytes;
	_pModbus_RTU->mdsRTURecSendConv=MDSSerialSWRecv_Send;
}

/*bsp���жϽ��յ����������*/
void MDSSerialRecvByte(uint8 byte){
	if(_pModbus_RTU==NULL){return;}
	_pModbus_RTU->mdsRTURecByteFunction(_pModbus_RTU , byte);
}

/*�л����գ����߷���*/
void MDSSerialSWRecv_Send(uint8 mode){
	/*��ʱû��ʵ�ָù���*/
}
/*���ͺ���*/
void MDSSerialSendBytes(uint8 *bytes,uint16 num){
	/*���������bsp�ķ��ͺ���*/
	uart_send_bytes(bytes,num);
}
/*��ʱ���е��øú���*/
void MDSTimeHandler100US(uint32 times){
	if(_pModbus_RTU==NULL){return;}
	_pModbus_RTU->mdRTUTimeHandlerFunction(_pModbus_RTU ,times);
}