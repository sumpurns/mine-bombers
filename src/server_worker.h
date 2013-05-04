#ifndef __SERVER_WORKER_H__
#define __SERVER_WORKER_H__

#include <stdexcept>
#include "socket.h"

class ServerWorker {
	public:
		ServerWorker () throw (std::runtime_error);
		virtual ~ServerWorker ();

		void Start (int sockfd) throw (std::runtime_error);
		void Stop ();

	protected:
	private:
		bool Active;
		Client Clnt;
};

#endif
