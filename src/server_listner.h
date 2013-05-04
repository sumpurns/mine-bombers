#ifndef __SERVER_LISTNER_H__
#define __SERVER_LISTNER_H__

#include <string>
#include <list>
#include <stdexcept>
#include "socket.h"
#include "server_worker.h"

class ServerListner {
	public:
		ServerListner () throw (std::runtime_error);
		virtual ~ServerListner ();

		void Load (const std::string & configFile) throw (std::runtime_error);
		void Unload ();

		void Start () throw (std::runtime_error);
		void AcceptConnection (int sockfd) throw (std::runtime_error);

		void StartRound () throw (std::runtime_error);
	protected:
	private:
		bool Loaded;
		bool Started;
		Server Serv;
		std::list<ServerWorker> WorkersList;
};

#endif