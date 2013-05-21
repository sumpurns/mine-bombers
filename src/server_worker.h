#ifndef __SERVER_WORKER_H__
#define __SERVER_WORKER_H__

#include <stdexcept>
#include "socket.h"
#include "thread.h"

class ServerWorker : public Thread {
	public:
		ServerWorker () throw (std::runtime_error);
		virtual ~ServerWorker ();

		void Serve (int sockfd) throw (std::runtime_error);
		void * Main (void * arg) throw (std::runtime_error);
		void Run (int sockfd) throw (std::runtime_error);
		void Stop ();

	protected:
	private:
		bool Active;
		Client Clnt;
};

#endif
