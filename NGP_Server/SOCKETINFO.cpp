#include "SOCKETINFO.h"
#include "../protocol/protocol.h"


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

void SOCKETINFO::ServerDoSend(char type)
{
	switch ((SERVER_PACKET_INFO)type) {
	case SERVER_PACKET_INFO::LOGIN:
		// 로그인 패킷
		break;

	case SERVER_PACKET_INFO::PLAYER_MOVE:
		// 플레이어 이동 패킷
		break;

	case SERVER_PACKET_INFO::SCENE_CHANGE:
		// 씬 변경 패킷
		break;
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
		// 클라이언트에서 받은 
		C2S_MOVE_PACKET* info = (C2S_MOVE_PACKET*)data;
		m_type = info->type;
		m_Id = info->from_c_id;
		m_dir = info->direction;
		cout << "id: " << m_Id << " dir: " << (int)m_dir << endl;
	}
		break;
	}
	
}
