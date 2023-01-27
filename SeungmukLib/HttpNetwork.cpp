#include "pch.h"
#include "HttpNetwork.h"

bool HttpNetwork::Initialize(const char* configFile)
{
    Json config(configFile);

    m_IP = const_cast<char*>(config.GetValue<std::string>("ip").c_str());
    m_Port = config.GetValue<int>("port");
    
    SetAPI();
    Listen();

    return true;
}

void HttpNetwork::Listen()
{
    m_Server.listen(m_IP, m_Port);
    cout << "HTTP Server Listen Start!" << endl;
    g_Log.debug("HTTP Server Listen Start!\n");
}
