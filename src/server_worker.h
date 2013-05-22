#ifndef __SERVER_WORKER_H__
#define __SERVER_WORKER_H__

#include <stdexcept>
#include "socket.h"
#include "thread.h"
#include "shared_config.h"
#include "server_shared.h"

class ServerWorker : public Thread {
	public:
		ServerWorker (ServerShared & shrd) throw (std::runtime_error);
		ServerWorker (const ServerWorker & oth) throw (std::runtime_error);
		virtual ~ServerWorker ();

		void Serve (int sockfd) throw (std::runtime_error);
		void * Main (void * arg) throw (std::runtime_error);
		void Run (int sockfd) throw (std::runtime_error);
		void Stop ();

		
		void CheckProtocol () throw (std::runtime_error);
		void GetRequest () throw (std::runtime_error);
		void SendResponse () throw (std::runtime_error);

	protected:
		int GivenSockFd;
	private:
		ServerShared & Shared;
		bool Active;
		Client Clnt;
		RequestType LastReq;
		std::string ReqArg;
};

#endif
