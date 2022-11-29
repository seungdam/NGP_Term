#pragma once
#include <winsock2.h> 
#include <iostream>

using namespace std;

class Scene;

class SOCKETINFO
{
private:
	int m_Id;
	SOCKET m_sock;
	char m_type;
	unsigned char m_dir;

public:
	SOCKETINFO();
	SOCKETINFO(int id, SOCKET s);

	~SOCKETINFO();

private:
	static Scene* m_pScene;
	static PLAYERINFO m_PlayersInfo[MAX_PLAYERS];

public:
	static void SetScene(Scene* pScene) { m_pScene = pScene; }

	// 플레이어 패킷의 정보를 갱신
	static void UpdatePlayerInfo();

public:
	// 클라이언트에게 type에 해당하는 패킷을 생성해 송신한다
	void ServerDoSend(char type);
	void ServerDoSendLoginPacket(bool isSuccess);

	// 클라이언트로 부터 온 패킷을 수신한다
	bool ServerDoRecv();

	// 수신한 패킷의 종류를 파악하고 처리한다
	void ProcessPacket(char* data);
	void Disconnect();
};



