cmake_minimum_required(VERSION 3.16.3)
project (opcuatest)
add_executable(opcuatest src/main.c src/tank.c src/valve.c src/util.c)
target_link_libraries(opcuatest open62541)
