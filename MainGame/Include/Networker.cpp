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
	//closesocket(m_clientSocket);
	//WSACleanup();
}

bool Networker::ConnectTo(const char* ipAddr)
{
	// connect to ipAddr
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ipAddr);
	serveraddr.sin_port = htons(SERVERPORT);
	int val = connect(m_clientSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (val == SOCKET_ERROR) return false;

	// if login ok
	// get client id here
	return true;
}

bool Networker::WaitForSessionStart()
{
	char temp;
	int val = recv(m_clientSocket, &temp, sizeof(char), MSG_WAITALL);
	return true;
}

void Networker::UpdateSendPacket(uint8_t dir)
{
}

bool Networker::SendPlayerPacket()
{

	return true;
	//return val != SOCKET_ERROR;
}

bool Networker::GetPackets()
{
	int val;
	// synchronize other players

	// synchronize moving steps
	return true;
}
