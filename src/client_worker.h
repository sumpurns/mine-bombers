#ifndef __CLIENT_WORKER_H__
#define __CLIENT_WORKER_H__

#include <stdexcept>
#include <string>
#include "socket.h"

class ClientWorker {
	public:
		ClientWorker () throw (std::runtime_error);
		virtual ~ClientWorker ();

		void Connect () throw (std::runtime_error);
		void Disconnect () throw (std::runtime_error);

		void Login (const std::string & nick) throw (std::runtime_error);

		void CheckProtocol () throw (std::runtime_error);
		void GetGameConfig () throw (std::runtime_error);
		void GetGameMap () throw (std::runtime_error);
		void GetGameFile (const std::string & name) throw (std::runtime_error);

		void GetAllResources () throw (std::runtime_error);

	protected: 
	private:
		Client Clnt;
};


#endif
