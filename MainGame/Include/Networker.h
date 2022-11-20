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

	// �����κ��� �α��� ��Ŷ�� �� ������ ��ٸ���.
	//bool WaitLoginPacket();
	bool IsClientLogin(bool);

	// �Է��� ��ȭ�� ����� �۽��Ѵ�
	bool ClientDoSendMovePacket(uint8_t dir);

	// send packet to server
	void ProcessPacket(char* packet);

	// receive packet from server
	bool ClientDoRecv();

	void Disconnect();
};

