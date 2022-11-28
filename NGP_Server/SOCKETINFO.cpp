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
		// �÷��̾� �̵� ��Ŷ
	{
		const int MAX_PLAYER = 2;

		S2C_PLAYER_MOVE_PACKET packet;
		packet.type = (char)(SERVER_PACKET_INFO::PLAYER_MOVE);

		PLAYERINFO pInfo[MAX_PLAYER];

		for (int i = 0; i < MAX_PLAYER; ++i) {
			// �������� �����ϴ� �÷��̾��� ������ ��Ƽ� ������ ��
			if (m_pScene) {
				pInfo[i].p_dir = 0;		
				pInfo[i].p_id = m_Id;
				pInfo[i].p_pos[0] = m_pScene->GetPlayerPosition(i);
				pInfo[i].p_pos[1] = m_pScene->GetPlayerPosition(i + 1);
			}
			else {
				// �׽�Ʈ��
				pInfo[i].p_dir = 0;
				pInfo[i].p_id = m_Id;
				pInfo[i].p_pos[0].x = 387.5f;		// �׽�Ʈ�� ���� ��ǥ
				pInfo[i].p_pos[0].y = 1175.0f;
				pInfo[i].p_pos[1].x = 187.5f;		// �׽�Ʈ�� ���� ��ǥ
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
		// �� ���� ��Ŷ
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
		// Ŭ���̾�Ʈ���� ���� ������ �־��ֱ�
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
	// remove ��Ŷ ������ �ʿ䰡 ����.-- > ���� ������ 
	/*S2C_REMOVE_PACKET p;
	p.type = ....*/
	
	closesocket(m_sock);
}
