#include "DobotNet.h"
#include <SoftwareSerial.h>

// Emulate Serial port on pins RX: 12 and TX: 13
SoftwareSerial Serial5{12, 13};
// Serial ports on which the Dobots will be connected
HardwareSerialWrapper dobotS1{&Serial1};
SoftwareSerialWrapper dobotS2{&Serial5};

// Instantiate two Dobot instances
DobotInstance dobots[2] = {{&dobotS1, 0}, {&dobotS2, 1}};

void Dobot_Callback(uint8_t dobotId, Message* msg)
{
	// Called when a dobot sends a response packet via the RX port
	if(msg->id == ProtocolPTPCmd)
		DobotNet::GetDobot(dobotId)->SetParam(0, true);
}

DobotResponseCallback cbs[2] = {Dobot_Callback, Dobot_Callback};

void setup()
{
	// Initialize the dobot network with 1 dobot
	DobotNet::Init(dobots, 2);
}

void loop()
{
	for(auto & dobot: dobots)
	{
		if(dobot.GetParam(0))
		{
			// Toggle param 1 to know whether to move to x = 200 or x = 300
			dobot.SetParam(1, !dobot.GetParam(1));

			float x;
			if(dobot.GetParam(1)) x = 200;
			else x = 300;

			// Call on the DobotInstance::MoveTo(mode, x, y, z, r) function
			dobot.MoveTo(
					MOVL_XYZ, // Movement mode
					x, // x coordinate
					0, // y coordinate
					50, // z coordinate
					0 // r end-effector rotation
			);
			// Param 0 = false => Dobot is occupied
			dobot.SetParam(0, false);
		}
	}
	DobotNet::Tick(cbs);
	// Wait 100 ms
	delay(100);
}
