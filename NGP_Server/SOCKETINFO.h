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

public:
	// 이전 입력, 위치와 비교하기 위한 변수를 설정
	static PLAYERINFO m_befPlayersInfo[MAX_PLAYERS];
	static PLAYERINFO m_PlayersInfo[MAX_PLAYERS];
	static bool m_Updated[MAX_PLAYERS * 2];

public:
	static void SetScene(Scene* pScene) { m_pScene = pScene; }

	// 플레이어 패킷의 정보를 갱신
	static void UpdatePlayerInfo();

	static void UpdateBeforeInfo();

	static bool IsUpdated();

public:
	// 클라이언트에게 type에 해당하는 패킷을 생성해 송신한다
	int ServerDoSend(char type);
	int ServerDoSendLoginPacket(bool isSuccess);

	// 클라이언트로 부터 온 패킷을 수신한다
	bool ServerDoRecv();

	// 수신한 패킷의 종류를 파악하고 처리한다
	void ProcessPacket(char* data);
};



