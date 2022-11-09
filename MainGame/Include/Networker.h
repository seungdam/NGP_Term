#pragma once

#include "Game.h"

// when it is created, init wsa, when it's deleted, cleanup wsa
class Networker
{
	int m_iClientID = -1;
	SOCKET m_clientSocket;

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

