#include "pch.h"
#include "Scene/Scene.h"
#include "SSession.h"

using namespace std;

Scene* SSerssion::m_pScene = nullptr;
PLAYERINFO SSerssion::m_PlayersInfo[MAX_PLAYERS] = {};
PLAYERINFO SSerssion::m_befPlayersInfo[MAX_PLAYERS] = {};
bool SSerssion::m_Updated[MAX_PLAYERS * 2] = {};

SSerssion::SSerssion()
{
}

SSerssion::SSerssion(int id, SOCKET s) :
	m_sid{ id },
	m_sock{ s }
{
}

SSerssion::~SSerssion()
{
	closesocket(m_sock);
}

void SSerssion::UpdatePlayerInfo()
{
	// 씬에서 플레이어 정보를 가져와 업데이트 한다
	for (int i = 0; i < MAX_PLAYERS; ++i) 
	{
		m_PlayersInfo[i].dir = m_pScene->GetPlayerInput(i);				// 방향 업데이트 필요
		m_PlayersInfo[i].pid = i;
		m_PlayersInfo[i].pos[0] = m_pScene->GetPlayerPosition(i * 2);
		m_PlayersInfo[i].pos[1] = m_pScene->GetPlayerPosition(i * 2 + 1);
	}
}

void SSerssion::UpdateBeforeInfo()
{
	// m_befPlayersInfo를 갱신한다
	memcpy(m_befPlayersInfo, m_PlayersInfo, sizeof(PLAYERINFO) * MAX_PLAYERS);

	// updated를 false로
	memset(m_Updated, 0, sizeof(bool) * MAX_PLAYERS * 2);
}

bool SSerssion::IsUpdated()
{
	// 입력도 캐릭터 마다 비교
	unsigned char input[2] = 
	{
		0b00001111,
		0b11110000
	};

	bool bUpdated = false;
	for (int i = 0; i < MAX_PLAYERS * 2; ++i) 
	{
		// 좌표 변경 비교
		if (!(m_befPlayersInfo[i / 2].pos[i % 2] == m_PlayersInfo[i / 2].pos[i % 2]) ||
			!((m_befPlayersInfo[i / 2].dir & input[i % 2]) == (m_PlayersInfo[i / 2].dir & input[i % 2]))) 
		{
			bUpdated = true;
			m_Updated[i] = true;
		}
	}

	return bUpdated;
}

// Processing Scene change packet and move packet
int SSerssion::DoSend(char type, int parameter)
{
	int retval = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;

	switch ((SERVER_PACKET_INFO)type) 
	{
		case SERVER_PACKET_INFO::PLAYER_MOVE:
			// 플레이어 이동 패킷
		{
			S2C_PLAYER_MOVE_PACKET packet;
			packet.type = (char)(SERVER_PACKET_INFO::PLAYER_MOVE);

			// 움직인 플레이어들에 대해서만 보낸다
			for (int i = 0; i < MAX_PLAYERS * 2; ++i) {
				// 좌표가 바뀐 캐릭터에 대해서만 송신
				if (!m_Updated[i]) continue;

				packet.pid = i / 2;
				packet.is_purple = i % 2;
				packet.x = m_PlayersInfo[packet.pid].pos[packet.is_purple].x;
				packet.y = m_PlayersInfo[packet.pid].pos[packet.is_purple].y;
				packet.dir = m_PlayersInfo[packet.pid].dir;
				 
				retval = send(m_sock, (char*)&packet, sizeof(S2C_PLAYER_MOVE_PACKET), 0);
				if (SOCKET_ERROR == retval) 
				{
					cout << "[" << m_sid << "]" <<  "player move error" << endl;
				}
			}
		}
			break;
		case SERVER_PACKET_INFO::SCENE_CHANGE:
			// 씬 변경 패킷
		{
			S2C_SCENE_CHANGE_PACKET packet;
			packet.type = (char)(SERVER_PACKET_INFO::SCENE_CHANGE);
			packet.next_scene_num = parameter;
			retval = send(m_sock, (char*)&packet, sizeof(S2C_SCENE_CHANGE_PACKET), 0);

			if (SOCKET_ERROR == retval) 
			{
				cout <<"[" << m_sid <<"]" << "scene change error" << endl;
			}
		}
			break;
		case SERVER_PACKET_INFO::GAME_END:
		{
			S2C_END_GAME_PACKET packet;
			packet.type = (char)(SERVER_PACKET_INFO::GAME_END);
			packet.most_high_score_id = parameter;
			retval = send(m_sock, (char*)&packet, sizeof(S2C_END_GAME_PACKET), 0);
			if (SOCKET_ERROR == retval) 
			{
				cout << "[" << m_sid << "]" << "end game error" << endl;
			}
		}
			break;
		case SERVER_PACKET_INFO::LOGIN:
		{
			S2C_LOGIN_PACKET login_packet;
			login_packet.type = (char)SERVER_PACKET_INFO::LOGIN;
			login_packet.b_success = parameter;
			login_packet.c_id = m_sid;

			int retval;
			retval = send(m_sock, (char*)&login_packet, sizeof(S2C_LOGIN_PACKET), 0);
			if (SOCKET_ERROR == retval)
			{
				cout << "err" << endl;
			}
			return retval;
		}
		break;
	}
	return retval;
}

int SSerssion::DoRecv()
{
	int retval;
	char buff[512];
	
	retval = recv(m_sock, buff, sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR || retval == 0) 
	{
		cout << "recv error1 " << endl;
		return retval;
	}
	retval = recv(m_sock, buff + sizeof(char), sizeof(C2S_MOVE_PACKET) - sizeof(char), MSG_WAITALL);
	if (retval == SOCKET_ERROR || retval == 0) 
		return retval;

	ProcessPacket(buff);

	return retval;
}

void SSerssion::ProcessPacket(char* data)
{
	switch ((CLIENT_PACKET_INFO)data[0]) 
	{
		case CLIENT_PACKET_INFO::MOVE:
		{
			// 클라이언트에서 받은 데이터 넣어주기
			C2S_MOVE_PACKET* info = (C2S_MOVE_PACKET*)data;
			m_type = info->type;
			m_sid = info->cid;
			m_dir = info->direction;
			//cout << "recv id: " << m_sid << " dir: " << (unsigned int)m_dir << endl;

			if (m_pScene) 
			{
				m_pScene->SetPlayerInput(m_sid, m_dir);
			}
		}
			break;
	}
	
}
