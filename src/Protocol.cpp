/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Protocol.cpp
** Latest modified Date:2016-06-01
** Latest Version:      V1.0.0
** Descriptions:        Protocol interface
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Liu Zhufu
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "Protocol.h"
#include "Message.h"
#include <stdio.h>
#include <string.h>

/*********************************************************************************************************
** Function name:       ProtocolInit
** Descriptions:        Init the protocol buffer etc.
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void ProtocolInit(ProtocolHandler* handler)
{
	// Init Serial protocol
	RingBufferInit(&handler->txRawByteQueue, handler->gSerialTXRawByteBuffer, RAW_BYTE_BUFFER_SIZE, sizeof(uint8_t));
	RingBufferInit(&handler->rxRawByteQueue, handler->gSerialRXRawByteBuffer, RAW_BYTE_BUFFER_SIZE, sizeof(uint8_t));
	RingBufferInit(&handler->txPacketQueue, handler->gSerialTXPacketBuffer, PACKET_BUFFER_SIZE, sizeof(Packet));
	RingBufferInit(&handler->rxPacketQueue, handler->gSerialRXPacketBuffer, PACKET_BUFFER_SIZE, sizeof(Packet));
}

void ProtocolProcess(uint8_t dobotId, SerialInterface* port, ProtocolHandler* handler, DobotResponseCallback cb)
{
	Message message;
	MessageProcess(handler);

	// Write commands to the serial port
	if(RingBufferGetCount(&handler->txRawByteQueue))
	{
		uint8_t data;
		while (!RingBufferIsEmpty(&handler->txRawByteQueue))
		{
			RingBufferDequeue(&handler->txRawByteQueue, &data);
			port->write(data);
		}
	}
	// Read responses from the serial port
	while(MessageRead(handler, &message) == ProtocolNoError)
	{
		// Debug
		printf("Rx message: ");
		printf("message id:%d, rw:%d, isQueued:%d, paramsLen:%d\r\n",
		       message.id, message.rw, message.isQueued, message.paramsLen);
		printf("params: ");
		for(int i=0; i<message.paramsLen; i++)
		{
			printf("%02x ", message.params[i]);
		}
		printf("\r\n");

		// Handle responses
		if(cb != nullptr) cb(dobotId, &message);
	}
}
