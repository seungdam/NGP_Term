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

	// 서버로부터 로그인 패킷이 올 때까지 기다린다.
	//bool WaitLoginPacket();
	void IsClientLogin(bool);

	// 입력의 변화가 생기면 송신한다
	bool ClientDoSendMovePacket(uint8_t dir);

	// send packet to server
	void ProcessPacket(char* packet);

	// receive packet from server
	bool ClientDoRecv();

	void Disconnect();

	void SetScene(Scene* pScene);

	int GetID() const { return m_iClientID; }
};

