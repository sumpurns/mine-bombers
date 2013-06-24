#ifndef __SERVER_LISTNER_H__
#define __SERVER_LISTNER_H__

#include <string>
#include <list>
#include <stdexcept>
#include "socket.h"
#include "server_worker.h"
#include "server_shared.h"

//! A main server socket, waiting for a clients to connect
class ServerListner {
	public:
		//! Constructor
		/*! \paaram shrd The ServerShared structure, containing the data structures that is actual for entire server*/
		ServerListner (ServerShared & shrd) throw (std::runtime_error);
		//! Destructor
		virtual ~ServerListner ();

		//! Loads the server configuration
		/*! \param configFile XML configuration file, describing the whole server, from binded port to used textures and terrain presets*/
		void Load (const std::string & configFile) throw (std::runtime_error);
		//! Unloads the server (real destructor, I think)
		void Unload ();

		//! Starts the server main listen/accept loop
		void Start () throw (std::runtime_error);
		//! The routine for accepting connections, generates ServerWorker on each connect
		/*! \param sockfd The system's socket descriptor connected to the new client
		 *  \sa ServerWorker*/
		void AcceptConnection (int sockfd) throw (std::runtime_error);

		//! When we got enough connections the listen/accept loop stops and we start a round of a game
		void StartRound () throw (std::runtime_error);
	protected:
	private:
		//! The link to server global data
		ServerShared & Shared;

		//! Set to true when Load(); is called
		bool Loaded;
		//! Set to true when Start(); is called
		bool Started;
		//! Server socket that is listening for a new connections
		Server Serv;
		//! List of a ServerWorker s created by AcceptConnection(); call 
		std::list<ServerWorker> WorkersList;
};

#endif
