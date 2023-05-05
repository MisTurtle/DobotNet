//
// Created by mtr1c on 07/04/2023.
//

#ifndef DOBOTNET_DOBOTINSTANCE_H
#define DOBOTNET_DOBOTINSTANCE_H


#include "SerialInterface.h"
#include "ProtocolDef.h"
#include "Protocol.h"
#include "Message.h"
#include "ProtocolID.h"

class DobotInstance {
public:
	DobotInstance(SerialInterface* sPort, uint8_t id);

	// GETTERS //
	SerialInterface* GetSerialPort();
	ProtocolHandler* GetProtocolHandler();

	uint8_t GetDobotId() const;
	uint64_t* GetCmdIndex();
	// END: GETTERS //

	// COMMUNICATION ACTIONS //
	void SerialProcess(void(*cb)(uint8_t, Message*));
	// END : COMMUNICATION ACTIONS //

	// DOBOT PROTOCOL //
	void SendGetterProtocol(uint8_t pid, bool isQueued);
	// End Effectors
	void SetEndEffectorParams(float xBias, float yBias, float zBias, bool isQueued);
	void SetEndEffectorSuctionCup(bool ctrlEnabled, bool sucked, bool isQueued);
	void SetSucking(bool sucking, bool isQueued);
	void SetEndEffectorGripper(bool ctrlEnabled, bool gripped, bool isQueued);
	void SetGripping(bool grip, bool isQueued);
	void DisableGripper(bool isQueued);

	// Movement
	void MoveTo(uint8_t mode, float x, float y, float z, float r);
	// PTP Settings
	void SetPTPJointParams(const float velocity[4], const float acceleration[4], bool isQueued);
	void SetPTPCoordinateParams(float xyzVelocity, float rVelocity, float xyzAcceleration, float rAcceleration, bool isQueued);
	void SetPTPJumpParams(float jumpHeight, float maxJumpHeight, bool isQueued);
	void SetPTPCommonParams(float velocityRatio, float accelerationRatio, bool isQueued);
	void SetPTPCmd(uint8_t mode, float x, float y, float z, float r);
	// JOG Settings

	// Multiplexing
	void SetIOMultiplexing(uint8_t address, uint8_t multiplex, bool isQueued);
	void SetIODO(uint8_t address, uint8_t level, bool isQueued);

	// Home
	void SetHomeParams(float x, float y, float z, float r, bool isQueued);
	void DoHomeProcedure(bool isQueued);
	// END: DOBOT ACTIONS //

	// DOBOT ARDUINO PARAMETERS //
	// Set of 8 booleans to help keep track of a dobot's current state
	bool GetParam(uint8_t n) const;
	bool SetParam(uint8_t n, bool value);
	uint8_t GetAllParams() const;
	// END: DOBOT ARDUINO PARAMETERS

private:
	SerialInterface* _port;
	ProtocolHandler _protocol{};

	uint8_t dobotId;
	uint64_t _gQueuedCmdIndex = 0;

	uint8_t arduinoParams = 0; // Param 0 is LSB, on the right
};


#endif //DOBOTNET_DOBOTINSTANCE_H
