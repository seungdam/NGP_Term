#include "SOCKETINFO.h"
#include "Game.h"
#include "Scene/Scene.h"


Scene* SOCKETINFO::m_pScene = nullptr;

SOCKETINFO::SOCKETINFO()
{
}

SOCKETINFO::SOCKETINFO(int id, SOCKET s) :
	m_Id{ id },
	m_sock{ s }
{
}

SOCKETINFO::~SOCKETINFO()
{
	closesocket(m_sock);
}

// Processing Scene change packet and move packet
void SOCKETINFO::ServerDoSend(char type)
{
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	switch ((SERVER_PACKET_INFO)type) {
	case SERVER_PACKET_INFO::PLAYER_MOVE:
		// 플레이어 이동 패킷
	{
		const int MAX_PLAYER = 2;

		S2C_PLAYER_MOVE_PACKET packet;
		packet.type = (char)(SERVER_PACKET_INFO::PLAYER_MOVE);

		PLAYERINFO pInfo[MAX_PLAYER];

		for (int i = 0; i < MAX_PLAYER; ++i) {
			// 서버에서 유지하는 플레이어의 정보를 담아서 보내줄 것
			if (m_pScene) {
				pInfo[i].p_dir = 0;		
				pInfo[i].p_id = m_Id;
				pInfo[i].p_pos[0] = m_pScene->GetPlayerPosition(i);
				pInfo[i].p_pos[1] = m_pScene->GetPlayerPosition(i + 1);
			}
			else {
				// 테스트용
				pInfo[i].p_dir = 0;
				pInfo[i].p_id = m_Id;
				pInfo[i].p_pos[0].x = 387.5f;		// 테스트용 시작 좌표
				pInfo[i].p_pos[0].y = 1175.0f;
				pInfo[i].p_pos[1].x = 187.5f;		// 테스트용 시작 좌표
				pInfo[i].p_pos[1].y = 1175.0f;
			}
		}
		
		memcpy(packet.p_data, pInfo, sizeof(PLAYERINFO) * MAX_PLAYER);

		int retval;
		retval = send(m_sock, (char*)&packet, sizeof(S2C_PLAYER_MOVE_PACKET), 0);
		if (SOCKET_ERROR == retval) {
			cout << "player move error" << endl;
		}
	}
		break;
	case SERVER_PACKET_INFO::SCENE_CHANGE:
		// 씬 변경 패킷
		break;
	}
}

void SOCKETINFO::ServerDoSendLoginPacket(bool isSuccess)
{
	S2C_LOGIN_PACKET login_packet;
	login_packet.type = (char)SERVER_PACKET_INFO::LOGIN;
	login_packet.b_success = isSuccess;
	login_packet.c_id = m_Id;
	
	int retval;
	retval = send(m_sock, (char*)&login_packet, sizeof(S2C_LOGIN_PACKET), 0);
	if (SOCKET_ERROR == retval) {
		cout << "err" << endl;
	}

}



bool SOCKETINFO::ServerDoRecv()
{
	int retval;
	char buff[512];
	//C2S_MOVE_PACKET packet;

	retval = recv(m_sock, buff, sizeof(char), MSG_WAITALL);
	retval = recv(m_sock, buff + sizeof(char), sizeof(C2S_MOVE_PACKET) - sizeof(char), MSG_WAITALL);

	if (retval == SOCKET_ERROR) 
		return false;

	ProcessPacket(buff);

	return true;
}

void SOCKETINFO::ProcessPacket(char* data)
{
	switch ((CLIENT_PACKET_INFO)data[0]) {
	case CLIENT_PACKET_INFO::MOVE:
	{
		// 클라이언트에서 받은 데이터 넣어주기
		C2S_MOVE_PACKET* info = (C2S_MOVE_PACKET*)data;
		m_type = info->type;
		m_Id = info->from_c_id;
		m_dir = info->direction;
		cout << "id: " << m_Id << " dir: " << (unsigned int)m_dir << endl;

		if (m_pScene) {
			m_pScene->SetPlayerInput(m_Id, m_dir);
		}
	}
		break;
	}
	
}

void SOCKETINFO::Disconnect()
{
	// remove 패킷 전송할 필요가 잇음.-- > 게임 접속이 
	/*S2C_REMOVE_PACKET p;
	p.type = ....*/
	
	closesocket(m_sock);
}
