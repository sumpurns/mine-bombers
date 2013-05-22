#include "players_list.h"


PlayersList::PlayersList () {
	Ready = false;
}

PlayersList::~PlayersList () {
}

void PlayersList::Init (int plrsCount) throw (std::runtime_error) {
	Records.resize(4);
	Ready = true;
}

bool PlayersList::CheckNickAvail (const std::string & nick) throw (std::runtime_error) {
	if (!Ready) {
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (!Records[id].Free) {
			if (Records[id].Nick == nick) {
				return false;
			}
		}
	}
	return true;
}

bool PlayersList::HasFreeSlot () throw (std::runtime_error) {
	if (!Ready) {
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (Records[id].Free) {
			return true;
		}
	}
	return false;
}

int PlayersList::RegisterNick (const std::string & nick) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		throw std::runtime_error("PlayersList is not ready for action");
	}
	if (!CheckNickAvail(nick)) {
		return -2;
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (Records[id].Free) {
			Records[id].Nick = nick;
			Records[id].Free = false;
			return id;
		}
	}
	Mute.Unlock();
	return -1;
}

void PlayersList::Unregister (int id) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		throw std::runtime_error("PlayersList is not ready for action");
	}
	Records[id].Free = true;
	Mute.Unlock();
}
