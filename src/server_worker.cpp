#include "server_worker.h"

#include <iostream>
#include <cstring>
#include "shared_config.h"

ServerWorker::ServerWorker () throw (std::runtime_error) {
	Active = false;
}

ServerWorker::~ServerWorker () {
	Stop ();
}

void ServerWorker::Stop () {
	if (Active) {
	}
}

void * ServerWorker::Main (void * arg) throw (std::runtime_error) {
	Clnt.Init (GivenSockFd);
	Active = true;

	try {
		CheckProtocol ();
	} catch (std::runtime_error & err) {
		std::cout << err.what() << ", finishing worker" << std::endl;
		return NULL;
	}

	Clnt.SendFile("res/game.xml");

	return NULL;
}

void ServerWorker::Serve (int sock) throw (std::runtime_error) {
	GivenSockFd = sock;
	Start(NULL);
}
void ServerWorker::CheckProtocol () throw (std::runtime_error) {
	char buf[20];
	Clnt.Recv (buf, strlen(SERVER_GREETING) + 1);
	if (strcmp(buf, SERVER_GREETING)) {
		throw std::runtime_error("Wrong protocol");
	}
	std::cout << buf << std::endl;
	Clnt.Send (CLIENT_GREETING, strlen(CLIENT_GREETING) + 1);
}
