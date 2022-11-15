#include "SOCKETINFO.h"
#include "../protocol/protocol.h"
#include <iostream>

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

	// 잘 오는지 테스트
	char type;
	C2S_MOVE_PACKET packet;
	retval = recv(m_sock, &type, sizeof(char), MSG_WAITALL);
	retval = recv(m_sock, (char*)&packet + sizeof(char), sizeof(C2S_MOVE_PACKET) - sizeof(char), MSG_WAITALL);

	printf("id: %d, 수신내용: %d\n", m_Id, (unsigned char)packet.direction);

	if (retval == SOCKET_ERROR) 
		return false;

	//ProcessPacket();

	return true;
}

void SOCKETINFO::ProcessPacket(char* data)
{

}
