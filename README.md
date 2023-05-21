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
```

## Project Repository

To take a look at the final repository for our project (Python, C++), [click here](https://github.com/) (Coming soon...)