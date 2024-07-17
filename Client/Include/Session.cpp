#include "pch.h"
#include "Session.h"
#include "Scene/Scene.h"

Session::Session()
{
	
	// socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		cout << "SOCKET INIT ERROR!!" << endl;
	}
}

Session::~Session()
{
	closesocket(m_sock);
	
}

bool Session::DoConnect(const char* ipAddr)
{
	// connect to ipAddr
	SOCKADDR_IN server_addr;
	::ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET,ipAddr, &server_addr.sin_addr);
	server_addr.sin_port = htons(SERVERPORT);
	int val = connect(m_sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (val == SOCKET_ERROR) return false;

	// login ok
	return true;
}

void Session::IsClientLogin(bool isSuccess)
{
	m_isLogin = isSuccess;
}

__int32 Session::DoSend(uint8_t new_dir)
{
	static uint8_t cur_dir = 0;

	// 만약 이전 입력과 다르다면
	if (cur_dir != new_dir) 
	{
		cur_dir = new_dir;

		// 플레이어 이동 패킷을 보낸다.
		C2S_MOVE_PACKET packet;
		packet.type = (char)CLIENT_PACKET_INFO::MOVE;
		packet.cid = m_cid;
		packet.direction = new_dir;

		int retval = send(m_sock, (char*)&packet, sizeof(C2S_MOVE_PACKET), 0);

		if (retval == SOCKET_ERROR) 
		{
			cout << "송신 실패" << endl;
			return false;
		}

		// 테스트 코드
		
		
	}
	return true;
}

void Session::ProcessPacket(char* packet)
{
	switch ((SERVER_PACKET_INFO)packet[0]) 
	{
		case SERVER_PACKET_INFO::LOGIN:
		{
			S2C_LOGIN_PACKET* llp = reinterpret_cast<S2C_LOGIN_PACKET*>(packet);
			m_cid = llp->c_id;
			IsClientLogin(llp->b_success);
			break;
		}
		case SERVER_PACKET_INFO::PLAYER_MOVE:
		{
			S2C_PLAYER_MOVE_PACKET* pmp = reinterpret_cast<S2C_PLAYER_MOVE_PACKET*>(packet);
			// 여기서 플레이어의 업데이트된 정보를 Scene에다가 덮어써준다.
			if (m_pScene) {

				m_pScene->SetPlayerData(pmp);
			}
		}
			break;

		case SERVER_PACKET_INFO::SCENE_CHANGE: 
		{
			m_pScene = nullptr;
			S2C_SCENE_CHANGE_PACKET* pmp = reinterpret_cast<S2C_SCENE_CHANGE_PACKET*>(packet);
			GameManager::GetInst().OnSceneChangePacket(pmp->next_scene_num);
		}
			break;
		case SERVER_PACKET_INFO::GAME_END:
		{
			m_pScene = nullptr;
			S2C_END_GAME_PACKET* pmp = reinterpret_cast<S2C_END_GAME_PACKET*>(packet);
			m_most_high_score_id = pmp->most_high_score_id;
		}
		break;
	}
}

__int32 Session::DoRecv()
{
	__int32 retval = 0;
	::ZeroMemory(m_recv_buff, MAX_BUFFER_SIZE);

	// Get Packet Type
	retval = recv(m_sock, m_recv_buff, sizeof(__int8), MSG_WAITALL);
	if (retval == SOCKET_ERROR) 
	{
		return retval;
	}

	// Get Remain Data
	__int32 remain_size = GetPacketSize(m_recv_buff[0]) - sizeof(char);

	retval = recv(m_sock, m_recv_buff + sizeof(__int8), remain_size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) 
	{
		return retval;
	}

	ProcessPacket(m_recv_buff);
	return retval;
}

void Session::Disconnect()
{
	closesocket(m_sock);
}

void Session::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}


