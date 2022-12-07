#pragma once

#include "Game.h"

class Scene;

// when it is created, init wsa, when it's deleted, cleanup wsa
class Networker
{
	int m_iClientID = -1;
	SOCKET m_clientSocket;
	Scene* m_pScene;
	
public:
	bool m_isLogin = false;
	int m_most_high_score_id = -1;
	Networker();
	~Networker();

	// returns false when fails
	bool ConnectTo(const char* ipAddr);

	// �����κ��� �α��� ��Ŷ�� �� ������ ��ٸ���.
	//bool WaitLoginPacket();
	void IsClientLogin(bool);

	// �Է��� ��ȭ�� ����� �۽��Ѵ�
	bool ClientDoSendMovePacket(uint8_t dir);

	// send packet to server
	void ProcessPacket(char* packet);

	// receive packet from server
	bool ClientDoRecv();

	void Disconnect();

	void SetScene(Scene* pScene);

	int GetID() const { return m_iClientID; }
};

