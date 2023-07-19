#include "DobotNet.h"

// Serial port on which the Dobot will be connected
HardwareSerialWrapper dobotSerial{&Serial1};
// Instantiate a Dobot instance of ID 0 that uses Serial 1
DobotInstance dobot{&dobotSerial, 0};
// Loop count
int count = 0;

void setup()
{
	Serial.begin(9600);
	// Initialize the dobot network with 1 dobot
	DobotNet::Init(&dobot, 1);
	// Send movement speed and acceleration parameters to the dobot
	send_movement_parameters();
}

void send_movement_parameters()
{
	dobot.SetPTPCoordinateParams(100, 100, 80, 80, true); // XYZ moving speed
	dobot.SetPTPCommonParams(50, 50, true); // Ratios

	DobotNet::Tick(nullptr);
}

void loop()
{
	float x;

	if(count++ % 2) x = 200;
	else x = 300;

	// Move to a given point using the linear moving mode
	dobot.MoveTo(MOVL_XYZ, x, 0, 50, 0);

	// Flush all packets to every dobot connected to the board (here, one dobot)
	// `nullptr` indicates that no callback function needs to be performed when receiving the dobot's answer
	DobotNet::Tick(nullptr);

	// Wait 3 seconds
	delay(3000);
}