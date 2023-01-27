#pragma once 
#include "pch.h" 
using namespace std;
static flatbuffers::FlatBufferBuilder builder(1024);

class Packet_C2S_HEARTBEAT : public Packet
{
public:
    int id;
    string dummy;

public:
    Packet_C2S_HEARTBEAT()
    {
        SetPacketType(MESSAGE_ID::MESSAGE_ID_C2S_HEARTBEAT);  
    }

    virtual uint8_t* Encode(int& packetLength) override
    {
        auto dummyString = builder.CreateString(dummy);
        auto makePacket = CreateC2S_HEARTBEAT(builder, id, dummyString);
        auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_C2S_HEARTBEAT, makePacket.Union());
        builder.Finish(packet);
        packetLength = builder.GetSize();

        auto data = builder.GetBufferPointer();
        builder.Clear();
        return data;
    }

    virtual void Decode(const Message *packet) override
    {
        auto data = static_cast<const C2S_HEARTBEAT*>(packet->packet());

        id = data->id();
        dummy = data->dummy()->c_str();
    }
};

class Packet_S2C_HEARTBEAT : public Packet
{
public:
    int id;
    string dummy;

public:
    Packet_S2C_HEARTBEAT()
    {
        SetPacketType(MESSAGE_ID::MESSAGE_ID_S2C_HEARTBEAT);  
    }

    virtual uint8_t* Encode(int& packetLength) override
    {
        auto dummyString = builder.CreateString(dummy);
        auto makePacket = CreateS2C_HEARTBEAT(builder, id, dummyString);
        auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_HEARTBEAT, makePacket.Union());
        builder.Finish(packet);
        packetLength = builder.GetSize();

        auto data = builder.GetBufferPointer();
        builder.Clear();
        return data;
    }

    virtual void Decode(const Message *packet) override
    {
        auto data = static_cast<const S2C_HEARTBEAT*>(packet->packet());

        id = data->id();
        dummy = data->dummy()->c_str();
    }
};