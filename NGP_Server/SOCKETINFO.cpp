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
	// ������ �÷��̾� ������ ������ ������Ʈ �Ѵ�
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		m_PlayersInfo[i].p_dir = m_pScene->GetPlayerInput(i);				// ���� ������Ʈ �ʿ�
		m_PlayersInfo[i].p_id = i;
		m_PlayersInfo[i].p_pos[0] = m_pScene->GetPlayerPosition(i * 2);
		m_PlayersInfo[i].p_pos[1] = m_pScene->GetPlayerPosition(i * 2 + 1);
	}
}

void SOCKETINFO::UpdateBeforeInfo()
{
	// m_befPlayersInfo�� �����Ѵ�
	memcpy(m_befPlayersInfo, m_PlayersInfo, sizeof(PLAYERINFO) * MAX_PLAYERS);
}

bool SOCKETINFO::IsUpdated()
{
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		// �Է� ��
		// �Է��� ���� ��� ������ ���� �� ���ٴ� ������ ������ ������ �ʴ� �� Ȯ��,
		// �̸� �����ϱ� ���� �ܼ��� �������� �ִ� || �����̰� �ִ� �Ӹ� �ƴ� �Է� ��ȭ�� üũ���ش�
		if (m_befPlayersInfo[i].p_dir != m_PlayersInfo[i].p_dir) return true;
	}

	// ��� �÷��̾�鿡 ���� ���������� || �����̴����� Ȯ��
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
		// �÷��̾� �̵� ��Ŷ
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
		// �� ���� ��Ŷ
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
		// Ŭ���̾�Ʈ���� ���� ������ �־��ֱ�
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
