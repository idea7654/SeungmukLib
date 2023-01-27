# How to Use

# Network

- IOCP

  - 'IOCompletionPort'のサブクラスを作ります。

  - ```c++
    class NetworkContents : public IOCompletionPort
    {
    	virtual void ProcessPacket() override;
    }
    ```

  - ProcessPacket()はパケットの受信後、処理を担当する仮想関数です。

  - パケットはメンバー変数であるm_ReadQueueに自動的に入ります。

  - ProcessPacket()はマルチスレッド環境です。（ロックが必要)

  - ```c++
    void NetworkContents::ProcessPacket()
    {
        while (!m_ReadQueue.IsEmpty())
        {
            auto packet = m_ReadQueue.Pop(); //Queueはスレッドセーフ
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

  - コンテンツパケットは"Packet Generator"にて作れます。

  - ”Packet Generator”は.fbs(flatbuffers extends)からC++クラスを作ります。

  - 1. /cd PacketGenerator
    2. Change path in "PacketGenerator.js" 
    3. node PacketGenerator.js

  - ```c++
    //ContentsQueryInput.hpp
    //Example
    //PacketGeneratorによって、こんな形に自動生成されます。
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
    ```

  - 使い方

  - ```c++
    class NetworkContents : public IOCompletionPort
    {
        virtual void ProcessPacket() override;
    };

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
              	auto id = std::move(newPacket.id);
              	audo dummy = std::move(newPacket.dummy);
                break;
            }
            //add case for other packet...
            default:
                std::cout << "Packet Not Defined!!" << std::endl; //Log...
            }
        }
    }

    int main()
    {
    	NetworkContents* network = new NetworkContents();
    	if (!network->Initialize("config.json"))
            return 0;
      
      	auto dummyClient = new SOCKETINFO(); //dummy Client -> 実際使用の時は後からのセッションにあるセッションベクトルから利用が効率的
        Packet_S2C_HEARTBEAT dummyPacket; //Create Packet Interface
        dummyPacket.dummy = "dummy"; //Fill Packet Parameter
        dummyPacket.id = 3; //Fill Packet Parameter
        auto packet = dummyPacket.Encode(); //make packet
        network->Send(packet, dummyClient); //send
    }
    ```

  - config.json構造

  - ```json
    {
      "port": 9800
    }
    ```

  - SOCKETINFO構造

  - ```c++
    struct SOCKETINFO
    {
    	WSAOVERLAPPED	overlapeed;
    	WSABUF			dataBuf;
    	SOCKET			socket;
    	char			messageBuffer[MAX_BUFFER_LENGTH];
    	int				recvBytes;
    	int				sendBytes;
    };
    ```

- UDP

  - IOCPと全くインタフェースが同じなので使い方は同一。
  - ただ、Send()のインタフェースが少し違う。


  - ```c++
    Send(unsigned char* sendMsg, int packetSize, UDP_QUEUE_DATA sendData);
    ```

  - UDP_QUEUE_DATA構造

  - ```c++
    struct UDP_QUEUE_DATA
    {
    	char*	packetData;
    	char*	ipAddress;
    	short	port;
    	int		packetLength;
    };
    ```

- HTTP

  - 'HttpNetwork'のサブクラスを作成

  - ```c++
    class HTTP : public HttpNetwork
    {
        
    };
    ```

  - 仮想関数の'SetAPI'を定義。(API)

  - ```c++
    class HTTP : public HttpNetwork
    {
        virtual void SetAPI() override
        {
            m_Server.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
                res.set_content("Http Server Response!", "text/plain");
                });
        }
    };
    ```

  - メインで使用

  - ```c++
    int main()
    {
    	HTTP* server = new HTTP();
        server->Initialize("httpConfig.json");

        delete server;
    }
    ```

  - httpConfig.json構造

  - ```c++
    {
      "ip": "127.0.0.1",
      "port": 3000
    }
    ```



# Database



# Utils

