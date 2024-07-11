#include "pch.h"
#include "Networker.h"
#include "Scene/Scene.h"

Networker::Networker()
{
	// was start
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "WSA START ERROR!!" << endl;

	// socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) 
		cout << "SOCKET INIT ERROR!!" << endl;

}

Networker::~Networker()
{
	closesocket(m_sock);
	WSACleanup();
}

bool Networker::DoConnect(const char* ipAddr)
{
	// connect to ipAddr
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddr);
	serverAddr.sin_port = htons(SERVERPORT);
	int val = connect(m_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (val == SOCKET_ERROR) return false;

	// login ok
	return true;
}

void Networker::IsClientLogin(bool isSuccess)
{
	m_isLogin = isSuccess;
}

bool Networker::DoSend(uint8_t dir)
{
	static uint8_t befDir = 0;

	// 만약 이전 입력과 다르다면
	if (befDir != dir) {
		befDir = dir;

		// 플레이어 이동 패킷을 보낸다.
		C2S_MOVE_PACKET packet;
		packet.type = (char)CLIENT_PACKET_INFO::MOVE;
		packet.from_c_id = m_cid;
		packet.direction = dir;

		int retval = send(m_sock, (char*)&packet, sizeof(C2S_MOVE_PACKET), 0);

		if (retval == SOCKET_ERROR) {
			cout << "송신 실패" << endl;
			return false;
		}

		// 테스트 코드
		
		
	}
	return true;
}

void Networker::ProcessPacket(char* packet)
{
	switch ((SERVER_PACKET_INFO)packet[0]) {
	case SERVER_PACKET_INFO::LOGIN:
	{
		S2C_LOGIN_PACKET* llp = (S2C_LOGIN_PACKET*)packet;
		m_cid = llp->c_id;
		IsClientLogin(llp->b_success);
		break;
	}
	case SERVER_PACKET_INFO::PLAYER_MOVE:
	{
		S2C_PLAYER_MOVE_PACKET* pmp = (S2C_PLAYER_MOVE_PACKET*)packet;
		// 여기서 플레이어의 업데이트된 정보를 Scene에다가 덮어써준다.
		if (m_pScene) {

			m_pScene->SetPlayerData(pmp);
		}
	}
		break;

	case SERVER_PACKET_INFO::SCENE_CHANGE: 
	{
		m_pScene = nullptr;
		S2C_SCENE_CHANGE_PACKET* pmp = (S2C_SCENE_CHANGE_PACKET*)packet;
		GameManager::GetInst().OnSceneChangePacket(pmp->next_scene_num);
	}
		break;
	case SERVER_PACKET_INFO::GAME_END:
	{
		m_pScene = nullptr;
		S2C_END_GAME_PACKET* pmp = (S2C_END_GAME_PACKET*)packet;
		m_most_high_score_id = pmp->most_high_score_id;
	}
	break;
	}
}

bool Networker::DoRecv()
{
	int retval = 0;
	char buff[512];
	retval = recv(m_sock, buff, sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		cout << "수신 에러" << endl;
		return false;
	}

	int packetSize = GetS2CSize(buff[0]);

	retval = recv(m_sock, buff + sizeof(char), packetSize - sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		cout << "수신 에러" << endl;
		return false;
	}

	ProcessPacket(buff);
	return true;
}

void Networker::Disconnect()
{
	closesocket(m_sock);
}

void Networker::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}


