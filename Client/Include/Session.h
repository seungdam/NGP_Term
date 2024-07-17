#pragma once
#define MAX_BUFFER_SIZE 512
class Scene;

// when it is created, init wsa, when it's deleted, cleanup wsa
class Session
{
	__int32 m_cid = -1;
	__int8 m_recv_buff[MAX_BUFFER_SIZE];

	SOCKET m_sock;
	Scene* m_pScene;
	
public:
	bool m_isLogin = false;
	int m_most_high_score_id = -1;
	
	Session();
	~Session();

	// returns false when fails
	bool DoConnect(const char* ipAddr);

	// 서버로부터 로그인 패킷이 올 때까지 기다린다.
	void IsClientLogin(bool);

	// 입력의 변화가 생기면 송신한다
	__int32 DoSend(uint8_t dir);

	// send packet to server
	void ProcessPacket(char* packet);

	// receive packet from server
	__int32 DoRecv();

	void Disconnect();

	void SetScene(Scene* pScene);

	int GetID() const { return m_cid; }
};

