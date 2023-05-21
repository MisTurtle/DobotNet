#include "DobotNet.h"

// Serial port on which the Dobot will be connected
HardwareSerialWrapper dobotSerial{&Serial1};
// Instantiate a Dobot instance of ID 0 that uses Serial 1
DobotInstance dobot{&dobotSerial, 0};

void Dobot_Callback(uint8_t dobotId, Message* msg)
{
	// Called when the dobot sends a response packet via the RX port
	if(msg->id == ProtocolPTPCmd)
		DobotNet::GetDobot(dobotId)->SetParam(0, true);
}

DobotResponseCallback cb[1] = {Dobot_Callback};

void setup()
{
	// Initialize the dobot network with 1 dobot
	DobotNet::Init(&dobot, 1);
	// Keep track of the dobot's movements state using param 0
	// Param 0 = true => Dobot is free to move
	dobot.SetParam(0, true);
}

void loop()
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
	DobotNet::Tick(cb);
	// Wait 100 ms
	delay(100);
}
