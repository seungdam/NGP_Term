#pragma once
class Scene;

class SSerssion
{
private:
	int32 m_sid;
	SOCKET m_sock;
	int8 m_type;
	uint8 m_dir;
	int32 m_score = 0;
public:
	SSerssion();
	SSerssion(int id, SOCKET s);

	~SSerssion();

private:
	static Scene* m_pScene;

public:
	// 이전 입력, 위치와 비교하기 위한 변수를 설정
	static PLAYERINFO m_befPlayersInfo[MAX_PLAYERS];
	static PLAYERINFO m_PlayersInfo[MAX_PLAYERS];
	static bool m_Updated[MAX_PLAYERS * 2];

public:
	static void SetScene(Scene* pScene) 
	{ 
		m_pScene = pScene; 
	}

	// 플레이어 패킷의 정보를 갱신
	static void UpdatePlayerInfo();
	static void UpdateBeforeInfo();
	static bool IsUpdated();

public:
	// 클라이언트에게 type에 해당하는 패킷을 생성해 송신한다
	int DoSend(char packet_type, int parameter = 0);
	int DoSendLoginPacket(bool isSuccess);

	// 클라이언트로 부터 온 패킷을 수신한다
	int DoRecv();

	void AddScore() 
	{
		++m_score;
		std::cout << "[" << m_sid << "] Score: " << m_score << std::endl;
	}
	int GetScore() { return m_score; }
	// 수신한 패킷의 종류를 파악하고 처리한다
	void ProcessPacket(char* data);
};



