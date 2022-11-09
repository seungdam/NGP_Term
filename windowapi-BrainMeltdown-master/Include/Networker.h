#pragma once
#include "Game.h"

// when it is created, init wsa, when it's deleted, cleanup wsa
class Networker
{
	int m_iClientID;
	SOCKET m_clientSocket;
	CS_MOVE_PACKET m_playerMovePacket;
	SC_MOVE_PACKET m_otherPlayerPacket[MAX_PLAYERS];
	SC_ROLLERCOASTER_PACKET m_movingStepPacket[MAX_MOVING_STEPS];

public:
	Networker();
	~Networker();

	// returns false when fails
	bool ConnectTo(const char* ipAddr);

	// waits until server send start packet
	bool WaitForSessionStart();

	// set player moving dir
	void UpdateSendPacket(uint8_t dir);

	// send packet to server
	bool SendPlayerPacket();

	// receive packet from server
	bool GetPackets();

};

