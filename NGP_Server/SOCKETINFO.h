#pragma once
#include <winsock2.h> 


class SOCKETINFO
{
private:
	int m_Id;
	SOCKET m_sock;

public:
	SOCKETINFO();
	SOCKETINFO(int id, SOCKET s);

	~SOCKETINFO();

	// Ŭ���̾�Ʈ���� type�� �ش��ϴ� ��Ŷ�� ������ �۽��Ѵ�
	void ServerDoSend(char type);

	// Ŭ���̾�Ʈ�� ���� �� ��Ŷ�� �����Ѵ�
	bool ServerDoRecv();

	// ������ ��Ŷ�� ������ �ľ��ϰ� ó���Ѵ�
	void ProcessPacket(char* data);
};

