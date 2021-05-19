cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_FLAGS "-g -Wall")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
  _add_executable(${ARGV})
  if (TARGET ${_name})
    target_link_libraries(${_name} err)
  endif()
endmacro()

add_library(err err.c)
add_library(random random.cpp)
add_library(player player.cpp)
add_library(game-state game-state.cpp)
add_library(client-to-server client-to-server.cpp)
add_executable(screen-worms-server screen-worms-server.cpp)

install(TARGETS DESTINATION .)
