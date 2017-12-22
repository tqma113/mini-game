cmake_minimum_required(VERSION 3.9)
project(Game C)

set(CMAKE_C_STANDARD 99)

add_executable(Game game.c Webserver/webserver.c Webserver/webserver.h Webserver/init_server.c Webserver/init_server.h Webserver/init_log.c Webserver/init_log.h Webserver/http_session.c Webserver/http_session.h Websockets/websocket_server.c Websockets/websocket_server.h GameServer/game_server.c GameServer/game_server.h Webserver/data_struct.h game.h)