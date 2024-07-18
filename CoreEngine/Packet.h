#pragma once
enum class SERVER_PACKET_INFO
{
	LOGIN = 0,
	PLAYER_MOVE,
	SCENE_CHANGE,
	GAME_END
};

enum class CLIENT_PACKET_INFO
{
	MOVE = 0,
};

struct PLAYERINFO
{
	int pid;
	FPOINT pos[2];
	unsigned char dir;
};

struct S2C_LOGIN_PACKET 
{
	char type;
	bool b_success;
	char c_id;
};

struct S2C_PLAYER_MOVE_PACKET
{
	char type;
	char pid;
	char is_purple;
	unsigned char dir;
	short x, y;
};

struct S2C_SCENE_CHANGE_PACKET
{
	char type;
	char next_scene_num;
};

struct S2C_END_GAME_PACKET 
{
	char type;
	char most_high_score_id;
};

struct C2S_MOVE_PACKET
{
	char type;
	char cid;
	unsigned char direction;
};



inline int GetPacketSize(char p)
{
	switch ((SERVER_PACKET_INFO)p)
	{
	case SERVER_PACKET_INFO::LOGIN:
		return sizeof(S2C_LOGIN_PACKET);

	case SERVER_PACKET_INFO::PLAYER_MOVE:
		return sizeof(S2C_PLAYER_MOVE_PACKET);

	case SERVER_PACKET_INFO::SCENE_CHANGE:
		return sizeof(S2C_SCENE_CHANGE_PACKET);

	case SERVER_PACKET_INFO::GAME_END:
		return sizeof(S2C_END_GAME_PACKET);

	default:
		// error
		return -1;
	}
}



