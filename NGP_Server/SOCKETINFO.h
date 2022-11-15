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

	// 클라이언트에게 type에 해당하는 패킷을 생성해 송신한다
	void ServerDoSend(char type);

	// 클라이언트로 부터 온 패킷을 수신한다
	bool ServerDoRecv();

	// 수신한 패킷의 종류를 파악하고 처리한다
	void ProcessPacket(char* data);
};

