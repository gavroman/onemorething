cmake_minimum_required(VERSION 3.10)
project(onemorething)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED network audio graphics window system)

include_directories(${SFML_INCLUDE_DIR})

include_directories(SYSTEM ./Map ./Character ./Player ./tinyxml2 ./Game)

add_executable(onemorething main.cpp Map/Map.cpp Map/Map_cell_processoring.cpp tinyxml2/tinyxml2.cpp Game/Game.cpp Character/Character.cpp Character/Character_methods.cpp)

target_link_libraries(onemorething ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})