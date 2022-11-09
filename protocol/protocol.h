#pragma once

struct PLAYERINFO
{
	int p_id;
	int p_pos;
	int p_dir;
};

struct S2C_LOGIN_PACKET {
	char type;
	bool b_success;
	char c_id;
};

struct S2C_PLAYER_MOVE_PACKET
{
	char type;
	PLAYERINFO p_data[3];
};

struct S2C_SCENE_CHANGE__PACKET
{
	char type;
	char next_scene_num;
};

struct C2S_MOVE_PACKET
{
	char type;
	char from_c_id;
	char direction;
};
