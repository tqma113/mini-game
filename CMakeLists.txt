cmake_minimum_required(VERSION 3.9)
project(Game C)

set(CMAKE_C_STANDARD 99)

add_executable(Game game.c Webserver/webserver.c Webserver/webserver.h Webserver/init_web_server.c Webserver/init_web_server.h Webserver/init_log.c Webserver/init_log.h Webserver/http_session.c Webserver/http_session.h Websockets/websocket_server.c Websockets/websocket_server.h GameServer/game_server.c GameServer/game_server.h data_struct.h game.h Websockets/init_websocket_server.c Websockets/init_websocket_server.h Websockets/init_ws_log.c Websockets/init_ws_log.h Websockets/websocket_session_init.c Websockets/websocket_session_init.h Websockets/websocket_session.c Websockets/websocket_session.h Websockets/base64.c Websockets/base64.h Websockets/sha1.c Websockets/sha1.h GameServer/init_info.c GameServer/init_info.h)