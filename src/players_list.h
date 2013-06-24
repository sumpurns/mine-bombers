#ifndef __PLAYERS_LIST_H__
#define __PLAYERS_LIST_H__

#include <vector>
#include <string>
#include <stdexcept>
#include "mutex.h"

//! Describes a server-side representation of player instance slot
/*! The quantity of slots (read: maximum players) is limited by server configuration*/
struct PlayerRecord {
		//! Nickname
		std::string Nick;
		//! Is this slot free
		bool Free;
};

//! Handles the registration of players and related staff on a server side
class PlayersList {
	public:
		//! Constructor does almost nothing
		PlayersList ();
		//! Destructor
		virtual ~PlayersList ();
		//! Initializes the structure
		/*! \param plrsCount The maximum amount of players on server*/
		void Init (int plrsCount) throw (std::runtime_error);
		//! Try to register a player with a nickname given
		/*! \param nick Nickname
		 *  \return The ID of player (client will recieve this handle by net). Return value is less then 0, on error*/
		int RegisterNick (const std::string & nick) throw (std::runtime_error);
		//! Unregister a player with an ID given
		/*! This routine is being called when a connected client calls ClientWorker::Disconnect(); or terminates suddenly
		 *  \param id The ID of a client*/
		void Unregister (int id) throw (std::runtime_error); 

	private:
		//! List of player records itself
		std::vector<PlayerRecord> Records;
		//! Set, if Init(); had been already called
		bool Ready;
		//! A mutex to prevent data corruption on simultaniouse data write access
		Mutex Mute;

		//! Check, if nickname is not registered yet
		bool CheckNickAvail (const std::string & nick) throw (std::runtime_error);
		//! Check, if we still have free place for a player
		bool HasFreeSlot () throw (std::runtime_error);
};

#endif
