#ifndef __SERVER_WORKER_H__
#define __SERVER_WORKER_H__

#include <stdexcept>
#include "socket.h"
#include "thread.h"
#include "shared_config.h"
#include "server_shared.h"

//! This is a thread object designed to interact with assigned client
class ServerWorker : public Thread {
	public:
		//! Constructor
		/*! \param shrd Global data structures of server is accesseable from a worker*/
		ServerWorker (ServerShared & shrd) throw (std::runtime_error);
		//! Copy constructor, nothing interesting
		ServerWorker (const ServerWorker & oth) throw (std::runtime_error);
		//! Destructor
		virtual ~ServerWorker ();

		//! This is the interface for an entry point of a worker instance
		/*! \param sockfd System socket file descriptor of a connected client, it calls own Thread::Start() function*/
		void Serve (int sockfd) throw (std::runtime_error);
		//! Function dirived and redefined from class Thread, It is real entry point of a thread
		/*! \param arg Unused
		 *  \return Unused
		 *  \sa Serve()*/
		void * Main (void * arg) throw (std::runtime_error);
		//! Stops the worker. Does nothing for real
		void Stop ();

		//! Checks the version of protocol (server greeting)
		void CheckProtocol () throw (std::runtime_error);
		//! Gets the request in standart form
		void GetRequest () throw (std::runtime_error);
		//! Forms and sends the response corresponding to request
		void SendResponse () throw (std::runtime_error);

	protected:
		//! Socket file descriptor. We don't need this field for all the time, we have Clnt member instead.
		int GivenSockFd;
	private:
		//! A link to server shared data
		ServerShared & Shared;
		//! Set when the worker is active
		bool Active;
		//! Client socket used for interaction with a client
		Client Clnt;
		//! Cuurent request type
		RequestType LastReq;
		//! Current request argument (if any)
		std::string ReqArg;
};

#endif
