#include "DobotNet.h"
#include <SoftwareSerial.h>

// Emulate Serial port on pins RX: 12 and TX: 13
SoftwareSerial Serial5{12, 13};
// Serial ports on which the Dobots will be connected
HardwareSerialWrapper dobotS1{&Serial1};
SoftwareSerialWrapper dobotS2{&Serial5};

// Instantiate two Dobot instances
DobotInstance dobots[2] = {{&dobotS1, 0}, {&dobotS2, 1}};

// Loop count
int loopCount = 0;
// Connected dobot count
int connectedDobots = 0;


void HandleDobotRx(uint8_t dobotId, Message* msg)
{ // Called when a response is received from a dobot
	if(msg->id == ProtocolGetPose) ++connectedDobots;
}
// List containing callback functions for each dobot
// Can be different for each dobot
DobotResponseCallback callbacks[2] = {HandleDobotRx, HandleDobotRx};

void setup()
{
	Serial.begin(9600);
	// Initialize the dobot network with 2 dobots
	DobotNet::Init(dobots, 2);
}

void SendMovePacket()
{
	float x;

	if(loopCount++ % 2) x = 200;
	else x = 300;

	// Move to a given point using the linear moving mode
	for(auto & dobot: dobots) dobot.MoveTo(MOVL_XYZ, x, 0, 50, 0);
}

void loop()
{
	if(connectedDobots == 2) SendMovePacket();

	connectedDobots = 0;
	// Send a packet to check if both dobots respond upon receiving it
	for(auto & dobot: dobots) dobot.SendGetterProtocol(ProtocolGetPose, true);

	// Flush all packets to every dobot connected to the board (here, two dobots)
	// If both dobots are connected, both the Move packet and GetPose packet will be sent
	// Otherwise, only the GetPose packet will be passed
	DobotNet::Tick(callbacks);

	// Wait 3 seconds
	delay(3000);
}