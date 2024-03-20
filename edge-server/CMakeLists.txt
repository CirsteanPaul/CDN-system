cmake_minimum_required(VERSION 3.26)
project(edge_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(edge_server edge-server/main.cpp
        edge-server/Client/Client.cpp
        edge-server/Client/Client.h
        edge-server/Client/Client.h
        Shared/Logger/Logger.cpp
        Shared/Logger/Logger.h
        Shared/Logger/LoggerMessage.h
        Shared/Server/Server.cpp
        Shared/Server/Server.h
        edge-server/Thread/Thread.cpp
        edge-server/Thread/Thread.h
        edge-server/ThreadPool/ThreadPool.cpp
        edge-server/ThreadPool/ThreadPool.h
        Shared/Result.h
        edge-server/CdnThread/CdnThread.cpp
        edge-server/CdnThread/CdnThread.h
        edge-server/Contracts/IResponse.h
        edge-server/Contracts/IResponse.h
        edge-server/CdnManager/CdnManager.cpp
        edge-server/CdnManager/CdnManager.h
        Shared/Location/Location.cpp
        Shared/Location/Location.h)
