#pragma once
#include "pch.h"

class Packet
{
public:
	MESSAGE_ID m_PacketType;

public:
	void SetPacketType(MESSAGE_ID packetType)
	{
		m_PacketType = packetType;
	}

	virtual uint8_t* Encode(int &packetLength) = 0; // Encode -> StreamWrite
	virtual void Decode(const Message *packet) = 0; // Decode -> StreamRead
};