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
	static Scene* m_pScene;

	static void SetScene(Scene* pScene) { m_pScene = pScene; }

	SOCKETINFO();
	SOCKETINFO(int id, SOCKET s);

	~SOCKETINFO();

	// Ŭ���̾�Ʈ���� type�� �ش��ϴ� ��Ŷ�� ������ �۽��Ѵ�
	void ServerDoSend(char type);
	void ServerDoSendLoginPacket(bool isSuccess);

	// Ŭ���̾�Ʈ�� ���� �� ��Ŷ�� �����Ѵ�
	bool ServerDoRecv();

	// ������ ��Ŷ�� ������ �ľ��ϰ� ó���Ѵ�
	void ProcessPacket(char* data);
	void Disconnect();
};



