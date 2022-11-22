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

	// 클라이언트에게 type에 해당하는 패킷을 생성해 송신한다
	void ServerDoSend(char type);
	void ServerDoSendLoginPacket(bool isSuccess);
	// 클라이언트로 부터 온 패킷을 수신한다
	bool ServerDoRecv();

	// 수신한 패킷의 종류를 파악하고 처리한다
	void ProcessPacket(char* data);
	void Disconnect();
};



