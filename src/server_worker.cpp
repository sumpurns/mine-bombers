#include "server_worker.h"

#include <iostream>
#include <cstring>

ServerWorker::ServerWorker (ServerShared & shrd) throw (std::runtime_error) 
: Shared(shrd) {
	Active = false;
}

ServerWorker::ServerWorker (const ServerWorker & oth) throw (std::runtime_error) 
: Shared(oth.Shared) {
	Active = oth.Active;
	GivenSockFd = oth.GivenSockFd;
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

	bool finished = false;
	while (!finished) {
		try {
			GetRequest();
		} catch (std::runtime_error & err) {
			std::cout << err.what() << ". Finishing the worker cause of GetRequest exception" << std::endl;
			finished = true;
			break;
		}
		if (LastReq == REQ_FINISH) {
			finished = true;
			std::cout << "Recieved REQ_FINISH, quiting the worker" << std::endl;
			break;
		}
		try {
			SendResponse();
		} catch (std::runtime_error & err) {
			std::cout << err.what() << ". Finishing the worker cause of SendResponse exception" << std::endl;
			finished = true;
			break;
		}
	}

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

void ServerWorker::GetRequest () throw (std::runtime_error) {
	if (0 == Clnt.Recv(reinterpret_cast<char*>(&LastReq), sizeof(RequestType))) {
		throw std::runtime_error ("Client disconnected suddenly");
	}
	switch (LastReq) {
		case REQ_GAME_FILE:
			Clnt.RecvString (ReqArg);
			std::cout << "Requested file \"" << ReqArg << "\"" << std::endl;
			break;
		case REQ_GAME_CONF:
			std::cout << "Requested game.xml" << std::endl;
			break;
		case REQ_GAME_MAP:
			std::cout << "Requested game map" << std::endl;
			break;
		case REQ_LOGIN:
			Clnt.RecvString (ReqArg);
			std::cout << "Player logged in with a nickname: " << ReqArg << std::endl;
			break;
		default:
			break;
	}
}

void ServerWorker::SendResponse () throw (std::runtime_error) {
	int cId = -3;
	size_t snt = 1;
	switch (LastReq) {
		case REQ_GAME_FILE:
			Clnt.SendFile(ReqArg);
			break;
		case REQ_GAME_CONF:
			Clnt.SendFile("res/game.xml");
			break;
		case REQ_GAME_MAP:
			Clnt.SendFile("res/1.map");
			break;
		case REQ_LOGIN:
			cId = Shared.Players.RegisterNick(ReqArg);
			snt = Clnt.Send(reinterpret_cast<char*>(&cId), sizeof(int));
			break;
		default:
			throw std::runtime_error ("Unsupported request");
			break;
	}
	if (snt == 0) {
		throw std::runtime_error("Client died [Send returneed 0]");
	}
}
