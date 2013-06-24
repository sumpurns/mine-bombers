#ifndef __CLIENT_WORKER_H__
#define __CLIENT_WORKER_H__

#include <stdexcept>
#include <string>

#include "socket.h"

//! A class implementing high-level game protocol routines
class ClientWorker {
	public:
		//!Constructor (does almost nothing)
		ClientWorker () throw (std::runtime_error);
		//!Destructor
		virtual ~ClientWorker ();

		//!Connect to a remote server
		void Connect () throw (std::runtime_error);
		//!Send a special disconnect message to server
		void Disconnect () throw (std::runtime_error);

		//!Send a nickname to server, trying to pick this nickname 
		void Login () throw (std::runtime_error);

		//!Check the protocol versions at the start of session
		void CheckProtocol () throw (std::runtime_error);
		//!Gets the game XML configuration from server
		void GetGameConfig () throw (std::runtime_error);
		//!Gets game map
		void GetGameMap () throw (std::runtime_error);
		//!Get other resources (like textures, for example)
		/*! \param name The name of resource (typicaly path to the file on server)*/
		void GetGameFile (const std::string & name) throw (std::runtime_error);

		//!Routine that uses GetGameConfig();, GetGameMap(); and GetGameFile(); in right order
		void GetAllResources () throw (std::runtime_error);

	protected: 
	private:
		//!Client socket connected to server
		Client Clnt;
};


#endif
