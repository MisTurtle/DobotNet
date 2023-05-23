/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Message.cpp
** Latest modified Date:2016-06-01
** Latest Version:      V2.0.0
** Descriptions:        Message API
**
**--------------------------------------------------------------------------------------------------------
** Created by:
** Created date:        2016-06-01
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "Message.h"
#include "Packet.h"
#include <Arduino.h>
/*********************************************************************************************************
** Function name:       MessageRead
** Descriptions:        Read a message from rx packet queue
** Input parameters:    protocolHandler and message
** Output parameters:   None
** Returned value:      ProtocolResult
*********************************************************************************************************/
ProtocolResult MessageRead(ProtocolHandler *protocolHandler, Message *message)
{
    RingBuffer *rxPacketQueue = &protocolHandler->rxPacketQueue;
    Packet *packet = &protocolHandler->rxAppPacket;

    if (RingBufferIsEmpty(rxPacketQueue)) {
        return ProtocolReadMessageQueueEmpty;
    }
    // Read a packet
    RingBufferDequeue(rxPacketQueue, packet);
    // Transform to message
    message->id = packet->payload.id;
    message->rw = packet->payload.ctrl & 0x01;
    message->isQueued = (packet->payload.ctrl >> 1) & 0x01;
    message->paramsLen = packet->header.payloadLen - 2;
    if (message->paramsLen) {
        memcpy(&message->params[0], &packet->payload.params[0], message->paramsLen);
    }
    return ProtocolNoError;
}

/*********************************************************************************************************
** Function name:       MessageWrite
** Descriptions:        Write a message to tx packet queue
** Input parameters:    protocolHandler and message
** Output parameters:   None
** Returned value:      ProtocolResult
*********************************************************************************************************/
ProtocolResult MessageWrite(ProtocolHandler *protocolHandler, const Message *message)
{
    RingBuffer *txPacketQueue = &protocolHandler->txPacketQueue;
    Packet *packet = &protocolHandler->txAppPacket;

    if (RingBufferIsFull(txPacketQueue))
	{
        return ProtocolWritePacketQueueFull;
    }
    // Header
    packet->header.payloadLen = message->paramsLen + 2;

    // Payload
    packet->payload.id = message->id;
    packet->payload.ctrl = 0;
    packet->payload.ctrl |= message->rw & 0x01;
    packet->payload.ctrl |= (message->isQueued << 1) & 0x02;
    if (message->paramsLen) {
        memcpy(&packet->payload.params[0], &message->params[0], message->paramsLen);
    }
    RingBufferEnqueue(txPacketQueue, packet);

    return ProtocolNoError;
}

/**
 * Create a message and write it to the given protocol handler
 * ! For getters only !
 */
void CreateMessageAndSendGetter(ProtocolHandler* handler, uint8_t pid, bool isQueued)
{
	Message msg;
	MessageFillGetter(&msg, pid, isQueued);
	MessageWrite(handler, &msg);
}
/**
 * Create a message and write it to the given protocol handler
 * ! For actions only !
 */
void CreateMessageAndSendAction(ProtocolHandler* handler, uint8_t pid, bool isQueued, const void *src, uint8_t size)
{
	Message msg;
	MessageFillAction(&msg, pid, isQueued, src, size);
	MessageWrite(handler, &msg);
}
/**
 * Fill a Message with basic informations
 * ! For getters only !
 */
void MessageFillGetter(Message* msg, uint8_t pid, bool isQueued)
{
	memset(msg, 0, sizeof(Message));
	msg->id = pid;
	msg->rw = false;
	msg->isQueued = isQueued;
	msg->paramsLen = 0;
}

/**
 * Fill a Message for an action protocol
 * ! For actions only !
 */
void MessageFillAction(Message* msg, uint8_t pid, bool isQueued, const void* src, uint8_t size)
{
	memset(msg, 0, sizeof(Message));
	msg->id = pid;
	msg->rw = true;
	msg->isQueued = isQueued;
	msg->paramsLen = size;
	if(size != 0 and src != nullptr) memcpy(msg->params, src, size);
}

/*********************************************************************************************************
** Function name:       MessageProcess
** Descriptions:
** Input parameters:    protocolHandler and message
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void MessageProcess(ProtocolHandler *protocolHandler)
{
    PacketProcess(protocolHandler);
}

