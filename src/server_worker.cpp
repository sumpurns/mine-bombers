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

void ServerWorker::Start (int sockfd) throw (std::runtime_error) {
	char buf[300];
	Clnt.Init (sockfd);
	Active = true;
	Clnt.Recv (buf, strlen(SERVER_GREETING) + 1);
	if (strcmp(buf, SERVER_GREETING)) {
		throw std::runtime_error((std::string("Wrong protocol: ") + buf).c_str());
	}
	std::cout << buf << std::endl;
}

void ServerWorker::Stop () {
	if (Active) {
	}
}
