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
	// ���� �Է�, ��ġ�� ���ϱ� ���� ������ ����
	static PLAYERINFO m_befPlayersInfo[MAX_PLAYERS];
	static PLAYERINFO m_PlayersInfo[MAX_PLAYERS];
	static bool m_Updated[MAX_PLAYERS * 2];

public:
	static void SetScene(Scene* pScene) { m_pScene = pScene; }

	// �÷��̾� ��Ŷ�� ������ ����
	static void UpdatePlayerInfo();

	static void UpdateBeforeInfo();

	static bool IsUpdated();

public:
	// Ŭ���̾�Ʈ���� type�� �ش��ϴ� ��Ŷ�� ������ �۽��Ѵ�
	int ServerDoSend(char type);
	int ServerDoSendLoginPacket(bool isSuccess);

	// Ŭ���̾�Ʈ�� ���� �� ��Ŷ�� �����Ѵ�
	bool ServerDoRecv();

	// ������ ��Ŷ�� ������ �ľ��ϰ� ó���Ѵ�
	void ProcessPacket(char* data);
};



