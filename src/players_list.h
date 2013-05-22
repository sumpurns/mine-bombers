#ifndef __PLAYERS_LIST_H__
#define __PLAYERS_LIST_H__

#include <vector>
#include <string>
#include <stdexcept>
#include "mutex.h"

struct PlayerRecord {
		std::string Nick;
		bool Free;
};

class PlayersList {
	public:
		PlayersList ();
		virtual ~PlayersList ();
		void Init (int plrsCount) throw (std::runtime_error);
		int RegisterNick (const std::string & nick) throw (std::runtime_error);
		void Unregister (int id) throw (std::runtime_error); 

	private:
		std::vector<PlayerRecord> Records;
		bool Ready;
		Mutex Mute;

		bool CheckNickAvail (const std::string & nick) throw (std::runtime_error);
		bool HasFreeSlot () throw (std::runtime_error);
};

#endif
