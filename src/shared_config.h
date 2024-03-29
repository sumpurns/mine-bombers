#ifndef __SHARED_CONFIG_H__
#define __SHARED_CONFIG_H__

#define DEFAULT_SERVER_ADDR "localhost"
//#define DEFAULT_SERVER_ADDR "minebombers.servegame.com"

#define SERVER_GREETING "MBSRV1.0"
#define CLIENT_GREETING "PROTO_OK"

enum RequestType {REQ_GAME_CONF, REQ_GAME_MAP, REQ_GAME_FILE, REQ_LOGIN, REQ_MOVE, REQ_PUT,
	REQ_REMOTE, REQ_USE, REQ_TAKE, REQ_FINISH};

#define NET_BLOCK 1024

#define TICK_INTERVAL 40

#define LOC_GAME_XML "conf.xml"
#define LOC_GAME_MAP "conf.map"

#endif
