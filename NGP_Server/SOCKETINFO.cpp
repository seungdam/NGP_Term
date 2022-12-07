#include "Game.h"
#include "Scene/Scene.h"
#include "SOCKETINFO.h"


Scene* SOCKETINFO::m_pScene = nullptr;
PLAYERINFO SOCKETINFO::m_PlayersInfo[MAX_PLAYERS] = {};
PLAYERINFO SOCKETINFO::m_befPlayersInfo[MAX_PLAYERS] = {};
bool SOCKETINFO::m_Updated[MAX_PLAYERS * 2] = {};
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

	// updated를 false로
	memset(m_Updated, 0, sizeof(bool) * MAX_PLAYERS * 2);
}

bool SOCKETINFO::IsUpdated()
{
	// 입력도 캐릭터 마다 비교
	unsigned char input[2] = {
		0b00001111,
		0b11110000
	};

	bool bUpdated = false;
	for (int i = 0; i < MAX_PLAYERS * 2; ++i) {
		// 좌표 변경 비교
		if (!(m_befPlayersInfo[i / 2].p_pos[i % 2] == m_PlayersInfo[i / 2].p_pos[i % 2]) ||
			!((m_befPlayersInfo[i / 2].p_dir & input[i % 2]) == (m_PlayersInfo[i / 2].p_dir & input[i % 2]))) {
			bUpdated = true;
			m_Updated[i] = true;
		}
	}

	return bUpdated;
}

// Processing Scene change packet and move packet
int SOCKETINFO::ServerDoSend(char type, int i)
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

		// 움직인 플레이어들에 대해서만 보낸다
		for (int i = 0; i < MAX_PLAYERS * 2; ++i) {
			// 좌표가 바뀐 캐릭터에 대해서만 송신
			if (!m_Updated[i]) continue;

			packet.p_id = i / 2;
			packet.is_purple = i % 2;
			packet.x = m_PlayersInfo[packet.p_id].p_pos[packet.is_purple].x;
			packet.y = m_PlayersInfo[packet.p_id].p_pos[packet.is_purple].y;
			packet.p_dir = m_PlayersInfo[packet.p_id].p_dir;
			 
			retval = send(m_sock, (char*)&packet, sizeof(S2C_PLAYER_MOVE_PACKET), 0);
			if (SOCKET_ERROR == retval) {
				cout << "[" << m_Id << "]" <<  "player move error" << endl;
			}
		}
	}
		break;
	case SERVER_PACKET_INFO::SCENE_CHANGE:
		// 씬 변경 패킷
	{
		S2C_SCENE_CHANGE_PACKET packet;
		packet.type = (char)(SERVER_PACKET_INFO::SCENE_CHANGE);
		packet.next_scene_num = i;
		retval = send(m_sock, (char*)&packet, sizeof(S2C_SCENE_CHANGE_PACKET), 0);

		if (SOCKET_ERROR == retval) {
			cout <<"[" << m_Id <<"]" << "scene change error" << endl;
		}
	}
		break;
	case SERVER_PACKET_INFO::GAME_END:
	{
		S2C_END_GAME_PACKET packet;
		packet.type = (char)(SERVER_PACKET_INFO::GAME_END);
		packet.most_high_score_id = i;
		retval = send(m_sock, (char*)&packet, sizeof(S2C_END_GAME_PACKET), 0);
		if (SOCKET_ERROR == retval) {
			cout << "[" << m_Id << "]" << "end game error" << endl;
		}
	}
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



int SOCKETINFO::ServerDoRecv()
{
	int retval;
	char buff[512];
	//C2S_MOVE_PACKET packet;

	retval = recv(m_sock, buff, sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR || retval == 0) {
		cout << "recv error1 " << endl;
		return retval;
	}
	retval = recv(m_sock, buff + sizeof(char), sizeof(C2S_MOVE_PACKET) - sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR || retval == 0) 
		return retval;

	ProcessPacket(buff);

	return retval;
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
