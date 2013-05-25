#ifndef __PLAYERS_LIST_H__
#define __PLAYERS_LIST_H__

#include <vector>
#include <string>
#include <stdexcept>
#include "mutex.h"

class ServerWorker;

struct PlayerRecord {
		std::string Nick;
		bool Free;
		bool Ready;
		ServerWorker * Worker;
		Mutex Starter;
};

class PlayersList {
	public:
		PlayersList ();
		virtual ~PlayersList ();
		void Init (int plrsCount) throw (std::runtime_error);
		int RegisterNick (const std::string & nick) throw (std::runtime_error);
		void Unregister (int id) throw (std::runtime_error); 

		void SetReadyFlag (int id) throw (std::runtime_error);
		bool AllPlayersReady () throw (std::runtime_error);
		void SetServerWorker (int id, ServerWorker & wrkr) throw (std::runtime_error);
		void AwaitUnlocking (int id) throw (std::runtime_error);

		void BroadcastStartMessage () throw (std::runtime_error);

	private:
		std::vector<PlayerRecord> Records;
		Mutex Mute;
		bool Ready;

		bool CheckNickAvail (const std::string & nick) throw (std::runtime_error);
		bool HasFreeSlot () throw (std::runtime_error);
};

#endif
