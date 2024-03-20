cmake_minimum_required(VERSION 3.26)
project(origin_server)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenCV REQUIRED)
add_executable(origin_server origin-server/main.cpp
        origin-server/Contracts/IRequest.h
        origin-server/Contracts/IResponse.h
        origin-server/Contracts/TextRequest.cpp
        origin-server/Contracts/ImageRequest.h
        origin-server/Contracts/TextRequest.h
        Shared/Result.h
        Shared/Logger/Logger.cpp
        Shared/IO/IO.h
        Shared/Logger/Logger.h
        Shared/Logger/LoggerMessage.h
        Shared/Server/Server.cpp
        Shared/Server/Server.h
        origin-server/ThreadPool/ThreadPool.cpp
        origin-server/ThreadPool/ThreadPool.h
        origin-server/Thread/Thread.cpp
        origin-server/Thread/Thread.h
)
target_link_libraries(origin_server ${OpenCV_LIBS})
