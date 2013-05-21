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
	int sockfd = *reinterpret_cast<int*>(arg);
	char buf[300];
	Clnt.Init (sockfd);
	Active = true;
	Clnt.Recv (buf, strlen(SERVER_GREETING) + 1);
	if (strcmp(buf, SERVER_GREETING)) {
		throw std::runtime_error((std::string("Wrong protocol: ") + buf).c_str());
	}
	std::cout << buf << std::endl;
	return NULL;
}

void ServerWorker::Serve (int sock) throw (std::runtime_error) {
	Start(&sock);
}
