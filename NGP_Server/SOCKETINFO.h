#pragma once
#include <winsock2.h> 
#include <iostream>

using namespace std;


class SOCKETINFO
{
private:
	int m_Id;
	SOCKET m_sock;
	char m_type;
	char m_dir;

public:
	SOCKETINFO();
	SOCKETINFO(int id, SOCKET s);

	~SOCKETINFO() { closesocket(m_sock); };

	// Ŭ���̾�Ʈ���� type�� �ش��ϴ� ��Ŷ�� ������ �۽��Ѵ�
	void ServerDoSend(char type);
	void ServerDoSendLoginPacket(bool isSuccess);
	// Ŭ���̾�Ʈ�� ���� �� ��Ŷ�� �����Ѵ�
	bool ServerDoRecv();

	// ������ ��Ŷ�� ������ �ľ��ϰ� ó���Ѵ�
	void ProcessPacket(char* data);
	void Disconnect();
};



