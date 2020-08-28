cmake_minimum_required(VERSION 3.17)
project(redes_tcp_ip C)

set(CMAKE_C_STANDARD 11)

add_executable(server
        server.c)

add_executable(client
        client.c)