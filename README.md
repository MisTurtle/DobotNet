![Logo](https://images2.imgbox.com/49/26/ztFAQqU5_o.png)
# DobotNet

## Description
**DobotNet** is an Arduino library that enables the control of several __Dobot Magician__ instances from a single _Arduino Mega Board_.

It was developped for a school project during the first semester of 2023, with the goal of showcasing the possibilities behind the collaboration between robots, even educational ones.


## Features

- Easy-to-use API
- Supports both **Hardware** and **Software** serials
- Includes 8 bits to manage an instance's state without additional code
- Callback system for an easy handling of a dobot's responses


## Installation

You can easily add the __DobotNet__ library right now to your Arduino project by *downloading a zipped version of this repository*. Once it is done, you can hope in your Arduino IDE and go to `Sketch > Include Library > Add .zip Library` and double-click on the `DobotNet.zip` file in your `Downloads` folder.

## Usage

*The following code snippets (and more) are available in the `examples` folder of this repository*

### 1. Control one dobot instance with *DobotNet*

The following program controls one *Dobot Magician* device whose TX and RX communication pins are respectively connected to the RX1 and TX1 pins on the Arduino Board

```cpp
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
```

### 2. Control two or more dobot instances with *DobotNet*

Similarly, the following program performs the same action as the one before, but on two Dobots at the same time. One should be plugged to the RX1 and TX1 pins, while the other should be on pins 12 and 13 of the Mega board.

```cpp
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
	// Initialize the dobot network with 2 dobots
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

	DobotNet::Tick(nullptr);
}

void loop()
{
	float x;

	if(count++ % 2) x = 200;
	else x = 300;

	// Move to a given point using the linear moving mode
	for(auto & dobot: dobots) dobot.MoveTo(MOVL_XYZ, x, 0, 50, 0);

	// Flush all packets to every dobot connected to the board (here, two dobots)
	// `nullptr` indicates that no callback function needs to be performed when receiving a dobot's answer
	DobotNet::Tick(nullptr);

	// Wait 3 seconds
	delay(3000);
}
```

### 3. Using callbacks

In the context of this library, callbacks can be used to retrieve and treat informations provided by a dobot instance.
Whether it is to check if a dobot is still connected, to get its position, its alarm state etc..., callbacks are the way to go here.

The following example will make the two dobots move only if they are both connected. If at any point a dobot gets disconnected from the board, the move action will not be sent.

```cpp
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
```

## Example Project Repository

To take a look at the final repository for a project made using this library (Python, C++), [click here](https://github.com/MisTurtle/DobotCityBuilding)