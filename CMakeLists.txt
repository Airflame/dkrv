cmake_minimum_required(VERSION 3.16)
project(dkrv)

set(CMAKE_CXX_STANDARD 14)

set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
if(SFML_FOUND)
    add_executable(dkrv main.cpp src/GameServer.cpp include/GameServer.h src/GameClient.cpp include/GameClient.h src/Player.cpp include/Player.h)
    target_link_libraries(dkrv sfml-graphics sfml-window sfml-system sfml-network)
endif()
