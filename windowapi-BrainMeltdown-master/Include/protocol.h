#pragma once

struct LOGIN_DATA {
    uint8_t id;
    FPOINT p0_pos;
    FPOINT p1_pos;
};

struct CS_MOVE_PACKET
{
    int c_id;
    uint8_t input_key;
};

struct SC_LOGIN_PACKET {
    uint8_t my_id;
    LOGIN_DATA All_Players_Data[3];
};

struct SC_MOVE_PACKET
{
    int c_id;
    uint8_t input_key;
    FPOINT p_pos[2];
};

struct SC_ROLLERCOASTER_PACKET
{
    int rc_idx;
    FPOINT rcPos;
};

//for (auto c : clients) {
//    do_send_packet(c.m_id, )
//}
