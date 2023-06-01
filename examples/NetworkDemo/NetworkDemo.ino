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
int count = 0;

void setup()
{
	Serial.begin(9600);
	// Initialize the dobot network with 1 dobot
	DobotNet::Init(dobots, 2);
	// Send movement speed and acceleration parameters to the dobot
	send_movement_parameters();
}

void send_movement_parameters()
{
	for(auto & dobot : dobots)
	{
		dobot.SetPTPCoordinateParams(100, 100, 80, 80, true); // XYZ moving speed
		dobot.SetPTPCommonParams(50, 50, true); // Ratios
	}
	// Submit movement packets, without callback to be performed
	DobotNet::Tick(nullptr);
}

void loop()
{
	float x;
	if(count++ % 2) x = 200;
	else x = 300;

	// Move to a given point using the linear moving mode
	for(auto & dobot: dobots) dobot.MoveTo(MOVL_XYZ, x, 0, 50, 0);

	// Send all previously requested packets
	DobotNet::Tick(nullptr);

	// Wait 3 seconds
	delay(3000);
}
