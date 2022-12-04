#include "Game.h"
#include "Scene/Scene.h"
#include "SOCKETINFO.h"


Scene* SOCKETINFO::m_pScene = nullptr;
PLAYERINFO SOCKETINFO::m_PlayersInfo[MAX_PLAYERS] = {};
PLAYERINFO SOCKETINFO::m_befPlayersInfo[MAX_PLAYERS] = {};

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

void SOCKETINFO::UpdatePlayerInfo()
{
	// 씬에서 플레이어 정보를 가져와 업데이트 한다
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		m_PlayersInfo[i].p_dir = m_pScene->GetPlayerInput(i);				// 방향 업데이트 필요
		m_PlayersInfo[i].p_id = i;
		m_PlayersInfo[i].p_pos[0] = m_pScene->GetPlayerPosition(i * 2);
		m_PlayersInfo[i].p_pos[1] = m_pScene->GetPlayerPosition(i * 2 + 1);
	}
}

void SOCKETINFO::UpdateBeforeInfo()
{
	// m_befPlayersInfo를 갱신한다
	memcpy(m_befPlayersInfo, m_PlayersInfo, sizeof(PLAYERINFO) * MAX_PLAYERS);
}

bool SOCKETINFO::IsUpdated()
{
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		// 입력 비교
		// 입력을 아주 잠깐만 눌렀다 땠을 때 땟다는 정보를 서버가 보내지 않는 점 확인,
		// 이를 수정하기 위해 단순히 떨어지고 있다 || 움직이고 있다 뿐만 아닌 입력 변화도 체크해준다
		if (m_befPlayersInfo[i].p_dir != m_PlayersInfo[i].p_dir) return true;
	}

	// 모든 플레이어들에 대해 떨어지는중 || 움직이는중을 확인
	if (m_pScene->IsPlayersUpdated()) return true;

	return false;
}

// Processing Scene change packet and move packet
int SOCKETINFO::ServerDoSend(char type)
{
	int retval = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;

	switch ((SERVER_PACKET_INFO)type) {
	case SERVER_PACKET_INFO::PLAYER_MOVE:
		// 플레이어 이동 패킷
	{
		S2C_PLAYER_MOVE_PACKET packet;
		packet.type = (char)(SERVER_PACKET_INFO::PLAYER_MOVE);

		// cpy
		memcpy(packet.p_data, m_PlayersInfo, sizeof(PLAYERINFO) * MAX_PLAYERS);

		
		retval = send(m_sock, (char*)&packet, sizeof(S2C_PLAYER_MOVE_PACKET), 0);
		if (SOCKET_ERROR == retval) {
			cout << "player move error" << endl;
		}

		//cout << "send id: " << m_Id << " dir: " << (unsigned int)m_dir << endl;
	}
		break;
	case SERVER_PACKET_INFO::SCENE_CHANGE:
		// 씬 변경 패킷
		break;
	}
	return retval;
}

int SOCKETINFO::ServerDoSendLoginPacket(bool isSuccess)
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
	return retval;
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
		//cout << "recv id: " << m_Id << " dir: " << (unsigned int)m_dir << endl;

		if (m_pScene) {
			m_pScene->SetPlayerInput(m_Id, m_dir);
		}
	}
		break;
	}
	
}
