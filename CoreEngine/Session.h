#pragma once


// Session에 관한 인터페이스를 제공한다.
class Session
{
	virtual int32 DoRecv() = 0;
	virtual int32 DoSend() = 0;
	virtual void Disconnect() = 0;
	virtual int32 DoConnect(const char* ipAddr) = 0;
};
