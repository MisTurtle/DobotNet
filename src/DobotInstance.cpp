//
// Created by mtr1c on 07/04/2023.
//

#include "DobotInstance.h"
#include "Protocol.h"

DobotInstance::DobotInstance(SerialInterface *sPort, uint8_t id)
{
	this->_port = sPort;
	this->_port->begin(115200);

	this->dobotId = id;
	ProtocolInit(this->GetProtocolHandler());
}

// GETTERS //
SerialInterface *DobotInstance::GetSerialPort()
{
	return this->_port;
}

ProtocolHandler *DobotInstance::GetProtocolHandler()
{
	return &this->_protocol;
}

uint8_t DobotInstance::GetDobotId() const
{
	return this->dobotId;
}

uint64_t *DobotInstance::GetCmdIndex()
{
	return &this->_gQueuedCmdIndex;
}
// END : GETTERS //

// ACTIONS //
void DobotInstance::SerialProcess(DobotResponseCallback cb)
{
	ProtocolProcess(
			this->GetDobotId(),
			this->GetSerialPort(),
			this->GetProtocolHandler(),
			cb
	);
}
// END: ACTIONS //


// PROTOCOL //
void DobotInstance::SendGetterProtocol(uint8_t pid, bool isQueued)
{
	CreateMessageAndSendGetter(
			this->GetProtocolHandler(),
			pid,
			isQueued
	);
}

void DobotInstance::SetEndEffectorParams(float xBias, float yBias, float zBias, bool isQueued)
{
	EndEffectorParams cmd{xBias, yBias, zBias};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolEndEffectorParams,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(EndEffectorParams)
	);
}

void DobotInstance::SetEndEffectorSuctionCup(bool ctrlEnabled, bool sucked, bool isQueued)
{
	uint8_t cmd[2] = {ctrlEnabled, sucked};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolEndEffectorSuctionCup,
			isQueued,
			cmd,
			2
	);
}

void DobotInstance::SetSucking(bool sucking, bool isQueued)
{
	this->SetEndEffectorSuctionCup(sucking, sucking, isQueued);
}

void DobotInstance::SetEndEffectorGripper(bool ctrlEnabled, bool gripped, bool isQueued)
{
	uint8_t cmd[2] = {ctrlEnabled, gripped};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolEndEffectorGripper,
			isQueued,
			cmd,
			2
	);
}

void DobotInstance::SetGripping(bool grip, bool isQueued)
{
	this->SetEndEffectorGripper(true, grip, isQueued);
}

void DobotInstance::DisableGripper(bool isQueued)
{
	this->SetEndEffectorGripper(false, false, isQueued);
}

// Movement
void DobotInstance::MoveTo(uint8_t mode, float x, float y, float z, float r)
{
	this->SetPTPCmd(mode, x, y, z, r);
}

// PTP Settings
void DobotInstance::SetPTPJointParams(const float v[4], const float a[4], bool isQueued)
{
	PTPJointParams cmd{
		v[0], v[1], v[2], v[3],
		a[0], a[1], a[2], a[3]
	};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolPTPJointParams,
			isQueued,
			(uint8_t*) &cmd,
			sizeof(PTPJointParams)
	);
}
void DobotInstance::SetPTPCoordinateParams(float xyzVelocity, float rVelocity, float xyzAcceleration, float rAcceleration, bool isQueued)
{
	PTPCoordinateParams cmd{xyzVelocity, rVelocity, xyzAcceleration, rAcceleration};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolPTPCoordinateParams,
			isQueued,
			(uint8_t*) &cmd,
			sizeof(PTPCoordinateParams)
	);
}
void DobotInstance::SetPTPJumpParams(float jumpHeight, float maxJumpHeight, bool isQueued)
{
	PTPJumpParams cmd{jumpHeight, maxJumpHeight};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolPTPJumpParams,
			isQueued,
			(uint8_t*) &cmd,
			sizeof(PTPJumpParams)
	);
}

void DobotInstance::SetPTPCommonParams(float velocityRatio, float accelerationRatio, bool isQueued)
{
	PTPCommonParams cmd{velocityRatio, accelerationRatio};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolPTPCommonParams,
			isQueued,
			(uint8_t*) &cmd,
			sizeof(PTPCommonParams)
	);
}

void DobotInstance::SetPTPCmd(uint8_t mode, float x, float y, float z, float r)
{
	PTPCmd cmd{mode, x, y, z, r};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolPTPCmd,
			true,
			(uint8_t *) &cmd,
			sizeof(PTPCmd)
	);
}

// JOG Settings

// Multiplexing
void DobotInstance::SetIOMultiplexing(uint8_t address, uint8_t multiplex, bool isQueued)
{
	IOMultiplexing cmd{address, multiplex};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolIOMultiplexing,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(IOMultiplexing)
	);
}

void DobotInstance::SetIODO(uint8_t address, uint8_t level, bool isQueued)
{
	IODO cmd{address, level};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolIODO,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(IODO)
	);
}

// Home
void DobotInstance::SetHomeParams(float x, float y, float z, float r, bool isQueued)
{
	HOMEParams cmd{x, y, z, r};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolHOMEParams,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(HOMEParams)
	);
}

void DobotInstance::DoHomeProcedure(bool isQueued)
{
	HOMECmd cmd{0};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolHOMECmd,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(HOMECmd)

	);
}

void DobotInstance::Delay(uint32_t timeout, bool isQueued)
{
	WAITCmd cmd{timeout};
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolWAITCmd,
			isQueued,
			(uint8_t *) &cmd,
			sizeof(WAITCmd)
		);
}

// Alarm
void DobotInstance::ClearAlarmState()
{
	CreateMessageAndSendAction(
			this->GetProtocolHandler(),
			ProtocolFunctionALARMBase + 1,
			false,
			nullptr,
			0
	);
}
void DobotInstance::ClearAlarm()
{
	this->ClearAlarmState();
}
// END : PROTOCOL //

// DOBOT ARDUINO PARAMETERS //
// Set of 8 booleans to help keep track of a dobot's current state
bool DobotInstance::GetParam(uint8_t n) const
{
	return (this->arduinoParams >> n) & 0x01;
}
bool DobotInstance::SetParam(uint8_t n, bool value)
{
	if(value)
		this->arduinoParams |= 1UL << n;
	else this->arduinoParams &= ~(1UL << n);
}
uint8_t DobotInstance::GetAllParams() const
{
	return this->arduinoParams;
}
// END: DOBOT ARDUINO PARAMETERS