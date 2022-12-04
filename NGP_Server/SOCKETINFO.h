#pragma once
#include <winsock2.h> 
#include <iostream>

using namespace std;

class Scene;

class SOCKETINFO
{
private:
	int m_id;
	SOCKET m_sock;
public:
	SOCKETINFO();
	SOCKETINFO(int id, SOCKET s);

	~SOCKETINFO();

private:
	static Scene* m_pScene;

public:
	static PLAYERINFO m_PlayersInfo[MAX_PLAYERS];

public:
	static void SetScene(Scene* pScene) { m_pScene = pScene; }

	// �÷��̾� ��Ŷ�� ������ ����
	static void UpdatePlayerInfo();

	static void UpdateBeforeInfo();

	static void IsUpdated();

public:
	// Ŭ���̾�Ʈ���� type�� �ش��ϴ� ��Ŷ�� ������ �۽��Ѵ�
	int ServerDoSend(char type);
	int ServerDoSendLoginPacket(bool isSuccess);
	int ServerDoSendMovePacket(int);
	// Ŭ���̾�Ʈ�� ���� �� ��Ŷ�� �����Ѵ�
	bool ServerDoRecv();

	// ������ ��Ŷ�� ������ �ľ��ϰ� ó���Ѵ�
	void ProcessPacket(char* data);
};



