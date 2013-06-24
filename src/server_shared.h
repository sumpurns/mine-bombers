#ifndef __SERVER_SHARED_H__
#define __SERVER_SHARED_H__

#include "players_list.h"

//! Global and shared objects of server
struct ServerShared {
	//! List of registered players
	PlayersList Players;
};

#endif
