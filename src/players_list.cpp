#include "players_list.h"


PlayersList::PlayersList () {
	Ready = false;
}

PlayersList::~PlayersList () {
}

void PlayersList::Init (int plrsCount) throw (std::runtime_error) {
	Records.resize(plrsCount);
	for (int id = 0; id < plrsCount; id++) {
		Records[id].Free = true;
		Records[id].Ready = false;
		Records[id].Worker = NULL;
		Records[id].Starter.Lock();
	}
	Mute.Unlock();
	Ready = true;
}

bool PlayersList::CheckNickAvail (const std::string & nick) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (!Records[id].Free) {
			if (Records[id].Nick == nick) {
				Mute.Unlock();
				return false;
			}
		}
	}
	Mute.Unlock();
	return true;
}

bool PlayersList::HasFreeSlot () throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (Records[id].Free) {
			Mute.Unlock();
			return true;
		}
	}
	Mute.Unlock();
	return false;
}

int PlayersList::RegisterNick (const std::string & nick) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	if (!CheckNickAvail(nick)) {
		Mute.Unlock();
		return -2;
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		if (Records[id].Free) {
			Records[id].Nick = nick;
			Records[id].Free = false;
			Mute.Unlock();
			return id;
		}
	}
	Mute.Unlock();
	return -1;
}

void PlayersList::Unregister (int id) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	Records[id].Free = true;
	Records[id].Ready  = false;
	Records[id].Worker = NULL;
	//Records[id].Starter.Lock();
	Mute.Unlock();
}

void PlayersList::SetReadyFlag (int cId) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	if (!Records[cId].Free) {
		Records[cId].Ready = true;
	} else {
		Mute.Unlock();
		throw std::runtime_error("Trying to set ready flag on non-active worker");
	}
	Mute.Unlock();
}

bool PlayersList::AllPlayersReady () throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int i = 0; i < (int)Records.size(); i++) {
		if (Records[i].Free || !Records[i].Ready) {
			Mute.Unlock();
			return false;
		}
	}
	Mute.Unlock();
	return true;
}

void PlayersList::SetServerWorker (int id, ServerWorker & wrkr) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	if (!Records[id].Free) {
		Records[id].Worker = &wrkr;
	} else {
		Mute.Unlock();
		throw std::runtime_error("Setting server worker on non-active player record");
	}
	Mute.Unlock();
}

void PlayersList::BroadcastStartMessage () throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	for (int id = 0; id < (int)Records.size(); id++) {
		Records[id].Starter.Unlock();
	}
	Mute.Unlock();
}

void PlayersList::AwaitUnlocking (int id) throw (std::runtime_error) {
	Mute.Lock();
	if (!Ready) {
		Mute.Unlock();
		throw std::runtime_error("PlayersList is not ready for action");
	}
	if (!Records[id].Free && Records[id].Ready) {
		Mute.Unlock();
		Records[id].Starter.Lock();
	} else {
		Mute.Unlock();
		throw std::runtime_error("Can't set non-active worker to awaiting state");
	}
}
