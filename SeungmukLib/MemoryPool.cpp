#pragma once
#include "MemoryPool.h"
#include "pch.h"

//int main()
//{
//	char RecvPacket[1024];
//
//	auto packetType = GetMessage(RecvPacket)->packet_type();
//	
//	switch (packetType)
//	{
//	case MESSAGE_ID::MESSAGE_ID_C2S_HEARTBEAT:
//	{
//		Packet_C2S_HEARTBEAT *packetClass = new Packet_C2S_HEARTBEAT();
//		packetClass->Decode(GetMessage(RecvPacket));
//		packetClass->id = 3;
//		break;
//	}
//	}
//
//	Packet_C2S_HEARTBEAT packet;
//	packet.id = 3;
//	packet.dummy = "hi";
//	int packetLength = 0;
//	auto sendPacket = packet.Encode(packetLength);
//}