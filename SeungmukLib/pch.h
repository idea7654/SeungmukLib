// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define _HAS_STD_BYTE 0
#define QUERY_H_RESTRICTION_PERMISSIVE 1

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <wchar.h>
#include <algorithm>
#include <synchapi.h>
#include <thread>
#include <timeapi.h>
#include <functional>
#include <iostream>
#include <variant>
#include <mysql.h>
#include <hiredis.h>

#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

#define MAX_QUEUE_LENGTH 100
#define MAX_BUFFER_LENGTH 1024
#define SOCKET_BUF_SIZE 1024
#define OBJECTPOOL_SIZE 100
#define MAX_CLIENT	5000
#define WORKER_THREAD_SIZE 10

#include "Types.h"
#include "RandWell.h"
#include "Logger.h"
#include "Lock.h"
#include "Json.h"
#include "CSVParser.h"
#include "ThreadManager.h"
#include "Packet/PacketDefine_generated.h"

#include "Packet.h"
#include "ContentsPacket.hpp"

#include "CircularQueue.h"

#include "MemoryPool.h"
#include "Network.h"
#include "IOCP.h"
#include "UdpNetwork.h"

#include "MySQLDB.h"
#include "RedisDB.h"
#include "QueryReturn.h"

#endif //PCH_H
