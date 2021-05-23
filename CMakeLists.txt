cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra")

add_library(crc32 crc32.c)
add_library(client-to-server client-to-server.cpp)
add_library(random random.cpp)

add_library(event-parser event-parser.cpp)
target_link_libraries(event-parser crc32)

add_library(game-state game-state.cpp)
target_link_libraries(game-state client-to-server)
target_link_libraries(game-state event-parser)
target_link_libraries(game-state random)

add_library(connection-manager connection-manager.cpp)
target_link_libraries(connection-manager client-to-server)
target_link_libraries(connection-manager game-state)

add_executable(screen-worms-server screen-worms-server.cpp)
target_link_libraries(screen-worms-server client-to-server)
target_link_libraries(screen-worms-server server-to-client)

install(TARGETS DESTINATION .)
