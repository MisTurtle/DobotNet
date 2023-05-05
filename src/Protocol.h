/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           Protocol.h
** Latest modified Date:
** Latest Version:      V1.0.0
** Descriptions:        Protocol interface
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Liu Zhufu
** Created date:        2016-03-14
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "SerialInterface.h"
#include "ProtocolDef.h"
#include "DobotInstance.h"

enum
{
	JUMP_XYZ,
	MOVJ_XYZ,
	MOVL_XYZ,
	JUMP_ANGLE,
	MOVJ_ANGLE,
	MOVL_ANGLE,
	MOVJ_INC,
	MOVL_INC,
};

enum {
	IDEL,       //Invalid status
	AP_DOWN,    // X+/Joint1+
	AN_DOWN,    // X-/Joint1-
	BP_DOWN,    // Y+/Joint2+
	BN_DOWN,    // Y-/Joint2-
	CP_DOWN,    // Z+/Joint3+
	CN_DOWN,    // Z-/Joint3-
	DP_DOWN,    // R+/Joint4+
	DN_DOWN     // R-/Joint4-
};

enum {
	UNSET,
	REG_OUTPUT,
	PWM_OUTPUT,
	REG_INPUT,
	AD_INPUT
};

enum{
	COORDINATE_MODEL,
	JOINT_MODEL
};
typedef struct tagEndEffectorParams {
	float xBias;
	float yBias;
	float zBias;
}EndEffectorParams;

typedef struct tagJOGJointParams {
	float velocity[4];
	float acceleration[4];
}JOGJointParams;

typedef struct tagJOGCoordinateParams {
	float velocity[4];
	float acceleration[4];
}JOGCoordinateParams;

typedef struct tagJOGCommonParams {
	float velocityRatio;
	float accelerationRatio;
}JOGCommonParams;

typedef struct tagJOGCmd {
	uint8_t isJoint;
	uint8_t cmd;
}JOGCmd;

typedef struct tagPTPJointParams {
	float velocity[4];
	float acceleration[4];
}PTPJointParams;

typedef struct tagPTPCoordinateParams {
	float xyzVelocity;
	float rVelocity;
	float xyzAcceleration;
	float rAcceleration;
}PTPCoordinateParams;

typedef struct tagPTPJumpParams {
	float jumpHeight;
	float maxJumpHeight;
}PTPJumpParams;

typedef struct tagPTPCommonParams {
	float velocityRatio;
	float accelerationRatio;
}PTPCommonParams;

typedef struct tagPTPCmd {
	uint8_t ptpMode;
	float x;
	float y;
	float z;
	float r;
}PTPCmd;

typedef struct tagIOMultiplexing {
	uint8_t address; // EIO addressing（Value range 1~20）
	uint8_t multiplex; // EIO function
	/**
	 * IO Functions
	 * Input 3.3V 		: 3
	 * Input AD 		: 4
	 * Output 3.3V-12V 	: 1
	 * Output PWM		: 2
	 */
} IOMultiplexing;
typedef struct tagIODO {
	uint8_t address; // EIO addressing(Value range 1~20)
	uint8_t level; // Level output 0-Low level 1-High level
} IODO;
typedef struct tagPOCmd{
	uint8_t ratio; // Percentage of movement completion
	uint16_t address; // EIO addressing(Value range 1~20)
	uint8_t level; // Output state
}POCmd;

typedef struct tagHOMECmd {
	uint32_t reserved; // Reserved for future use
} HOMECmd;

typedef struct tagHOMEParams {
	float x;
	float y;
	float z;
	float r;
} HOMEParams;

typedef struct tagPose {
	float x;
	float y;
	float z;
	float r;
	float jointAngle[4];
} Pose;

typedef void(*DobotResponseCallback)(uint8_t, Message*);

extern void ProtocolInit(ProtocolHandler* handler);

extern void ProtocolProcess(uint8_t dobotId, SerialInterface* port, ProtocolHandler* handler, DobotResponseCallback cb);

#endif

