# How to Use

# Network

- IOCP

  - Make Child Class From 'IOCompletionPort'

  - ```c++
    class NetworkContents : public IOCompletionPort
    {
    	virtual void ProcessPacket() override;
    }
    ```

  - ProcessPacket() is virtual function for receive packet.

  - Packet will put in member variable, m_ReadQueue(protected)

  - ProcessPacket() is Multi-threaded Environment(need Lock)

  - ```c++
    void NetworkContents::ProcessPacket()
    {
        while (!m_ReadQueue.IsEmpty())
        {
            auto packet = m_ReadQueue.Pop();
            auto message = GetMessage(packet.packetData);
            auto protocol = message->packet_type();

            switch (protocol)
            {
            case MESSAGE_ID::MESSAGE_ID_C2S_HEARTBEAT:
            {
                Packet_C2S_HEARTBEAT newPacket;
                newPacket.Decode(message);
                //Process for Packet_C2S_HEARTBEAT
                break;
            }
            }
        }
    }
    ```

  - Contents Packet can create by "Packet Generator"

  - "Packet Generator" create C++ Interface Class from .fbs(flatbuffers extends)

  - 1. /cd PacketGenerator
    2. Change path in "PacketGenerator.js" 

- UDP