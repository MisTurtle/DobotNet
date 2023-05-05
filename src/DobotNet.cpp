//
// Created by mtr1c on 07/04/2023.
//
#include "DobotNet.h"
#include "FlexiTimer2.h"

int Serial_putc( char c, struct __file * )
{
	Serial.write( c );
	return c;
}
void printf_begin()
{
	fdevopen( &Serial_putc, 0 );
}

bool DobotNet::initiated = false;
DobotInstance* DobotNet::net = nullptr;
uint8_t DobotNet::netCount = 0;

void DobotNet::Init(DobotInstance *dobots, uint8_t count)
{
	if(DobotNet::initiated) return;

	// For Debug
	printf_begin();

	DobotNet::initiated = true;
	DobotNet::net = dobots;
	DobotNet::netCount = count;

	FlexiTimer2::set(100, DobotNet::SerialRead);
	FlexiTimer2::start();
}

DobotInstance *DobotNet::GetDobot(uint8_t id)
{
	if(id >= netCount) return nullptr;
	return DobotNet::net + id;
}

void DobotNet::SerialRead()
{
	if(!DobotNet::initiated) return;
	for(int i = 0; i < DobotNet::netCount; ++i)
	{
		auto bot = DobotNet::net + i;
		SerialInterface* port = bot->GetSerialPort();
		while(port->available())
		{
			uint8_t data = port->read();
			if(!RingBufferIsFull(&bot->GetProtocolHandler()->rxRawByteQueue))
				RingBufferEnqueue(&bot->GetProtocolHandler()->rxRawByteQueue, &data);
		}
	}
}

void DobotNet::Tick(DobotResponseCallback *cbs)
{
	// Process packets to be [sent to / received from] dobots
	for(int i = 0; i < DobotNet::netCount; ++i)
	{
		DobotInstance* _bot = DobotNet::GetDobot(i);
		if(_bot != nullptr)
			_bot->SerialProcess(cbs == nullptr ? nullptr : cbs[i]);
	}
}
