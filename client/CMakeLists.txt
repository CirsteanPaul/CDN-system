cmake_minimum_required(VERSION 3.26)
project(client)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
add_executable(client client/main.cpp
        client/Client/Client.cpp
        client/Client/Client.h
        Shared/Logger/Logger.cpp
        Shared/Logger/Logger.h
        Shared/Logger/LoggerMessage.h
        client/Contracts/IRequest.h
        client/Contracts/IRequest.h
        client/Contracts/ImageShower.h)

target_link_libraries(client ${OpenCV_LIBS})
