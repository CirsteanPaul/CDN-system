cmake_minimum_required(VERSION 3.26)
project(cdn_node)

set(CMAKE_CXX_STANDARD 17)

add_executable(cdn_node cdn-node/main.cpp
        Shared/Result.h
        cdn-node/ThreadPool/ThreadPool.h
        cdn-node/Thread/Thread.h
        Shared/Server/Server.h
        Shared/Logger/Logger.h
        Shared/Logger/LoggerMessage.h
        Shared/Logger/Logger.cpp
        Shared/Server/Server.cpp
        cdn-node/Thread/Thread.cpp
        cdn-node/ThreadPool/ThreadPool.cpp
        cdn-node/Client/Client.cpp
        cdn-node/Client/Client.h
        cdn-node/SyncThread/SyncThread.cpp
        cdn-node/SyncThread/SyncThread.h
        cdn-node/Contracts/IResponse.h
        cdn-node/Contracts/IRequest.h
        cdn-node/Cache/Cache.cpp
        cdn-node/Cache/Cache.h
        ../Shared/IO/IO.h
)
