#ifndef DOBOTNET_DOBOTNET_H
#define DOBOTNET_DOBOTNET_H

#include "DobotInstance.h"

class DobotNet {
public:
	static bool initiated;
	static DobotInstance* net;
	static uint8_t netCount;

	static void Init(DobotInstance* dobots, uint8_t count);
	static DobotInstance* GetDobot(uint8_t id);
	static void Tick(DobotResponseCallback* cbs);

	static void SerialRead();
};

#endif
