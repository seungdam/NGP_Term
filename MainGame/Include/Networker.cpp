#include "Networker.h"
#include "Scene/Scene.h"

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
		m_iClientID = llp->c_id;
		IsClientLogin(llp->b_success);
		break;
	}
	case SERVER_PACKET_INFO::PLAYER_MOVE:
	{
		S2C_PLAYER_MOVE_PACKET* pmp = (S2C_PLAYER_MOVE_PACKET*)packet;
		PLAYERINFO pData = pmp->p_data[0];
		// 테스트, 잘 오는지 확인 용
		//printf("PLAYER_MOVE---------------\n");
		//printf("ID: %d\nP1_POS: %.2f, %.2f\nP2_POS: %.2f, %.2f\n\n", pData.p_id, pData.p_pos[0].x, pData.p_pos[0].y, pData.p_pos[1].x, pData.p_pos[1].y);

		// 여기서 플레이어의 업데이트된 정보를 Scene에다가 덮어써준다.
		if (m_pScene) {
			for (int i = 0; i < _countof(pmp->p_data); ++i) {
				m_pScene->SetPlayerData(i, pmp->p_data[i]);
			}
		}
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

	int packetSize = GetS2CSize(buff[0]);

	retval = recv(m_clientSocket, buff + sizeof(char), packetSize - sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR)
		return false;

	ProcessPacket(buff);
	return true;
}

void Networker::Disconnect()
{
	closesocket(m_clientSocket);
}

void Networker::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}


