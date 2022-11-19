#include "Networker.h"

Networker::Networker()
{
	// was start
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "WSA START ERROR!!" << endl;

	// socket
	m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_clientSocket == INVALID_SOCKET) 
		cout << "SOCKET INIT ERROR!!" << endl;

}

Networker::~Networker()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

bool Networker::ConnectTo(const char* ipAddr)
{
	// connect to ipAddr
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddr);
	serverAddr.sin_port = htons(SERVERPORT);
	int val = connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (val == SOCKET_ERROR) return false;

	// login ok
	return true;
}

//bool Networker::WaitLoginPacket()
//{
//
//	return true;
//}

bool Networker::IsClientLogin(bool isSuccess)
{
	return isSuccess;
}

bool Networker::ClientDoSendMovePacket(uint8_t dir)
{
	static uint8_t befDir = 0;

	// 만약 이전 입력과 다르다면
	if (befDir != dir) {
		befDir = dir;

		// 플레이어 이동 패킷을 보낸다.
		C2S_MOVE_PACKET packet;
		packet.type = (char)CLIENT_PACKET_INFO::MOVE;
		packet.from_c_id = m_iClientID;
		packet.direction = dir;

		int retval = send(m_clientSocket, (char*)&packet, sizeof(C2S_MOVE_PACKET), 0);

		if (retval == SOCKET_ERROR)
			return false;

		// 테스트 코드
		
		printf("송신: %d\n", packet.type);
		
	}
	return true;
}

void Networker::ProcessPacket(char* packet)
{
	switch ((SERVER_PACKET_INFO)packet[0]) {
	case SERVER_PACKET_INFO::LOGIN:
	{
		S2C_LOGIN_PACKET* llp = (S2C_LOGIN_PACKET*)packet;
		if (llp->b_success) IsClientLogin(llp->b_success);
		break;
	}
	case SERVER_PACKET_INFO::PLAYER_MOVE:
	{
		S2C_PLAYER_MOVE_PACKET* pmp = (S2C_PLAYER_MOVE_PACKET*)packet;
	}
		break;
	}
}

bool Networker::ClientDoRecv()
{
	int retval;
	char buff[512];
	retval = recv(m_clientSocket, buff, sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		return false;
	}
	retval = recv(m_clientSocket, buff + sizeof(char), sizeof(S2C_LOGIN_PACKET) - sizeof(char), MSG_WAITALL);

	if (retval == SOCKET_ERROR)
		return false;

	ProcessPacket(buff);
	return true;
}
