#include "client_worker.h"
#include <iostream>
#include <cstring>
#include "xml.h"
#include "shared_config.h"

ClientWorker::ClientWorker () throw (std::runtime_error) {
}

ClientWorker::~ClientWorker () {
}

void ClientWorker::Connect () throw (std::runtime_error) {
	Clnt.Init();
	if (!Clnt.Connect(DEFAULT_SERVER_ADDR, "12345")) {
		throw std::runtime_error("Can't connect to server");
	}
}

void ClientWorker::Disconnect () throw (std::runtime_error) {
	RequestType rt = REQ_FINISH;
	Clnt.Send (reinterpret_cast<char*>(&rt), sizeof(RequestType));
}

void ClientWorker::CheckProtocol () throw (std::runtime_error) {
	Clnt.Send(SERVER_GREETING, strlen(SERVER_GREETING) + 1);
	char buf[20];
	Clnt.Recv(buf, strlen(CLIENT_GREETING) + 1);
	if (strcmp(CLIENT_GREETING, buf)) {
		throw std::runtime_error("Wrong server reply");
	}
	std::cout << buf << std::endl;
}

void ClientWorker::Login (const std::string & nick) throw (std::runtime_error) {
	RequestType rt = REQ_LOGIN;
	Clnt.Send (reinterpret_cast<char*>(&rt), sizeof(RequestType));
	Clnt.SendString(nick);
	int playerId;
	Clnt.Recv(reinterpret_cast<char*>(&playerId), sizeof(int));
	if (playerId < 0) {
		throw std::runtime_error("Server returned invalid playerId");
	}
	std::cout << "Player ID: " << playerId << std::endl;
}

void ClientWorker::GetGameConfig () throw (std::runtime_error) {
	RequestType rt = REQ_GAME_CONF;
	Clnt.Send (reinterpret_cast<char*>(&rt), sizeof(RequestType));
	Clnt.RecvFile (LOC_GAME_XML);
}

void ClientWorker::GetGameMap () throw (std::runtime_error) {
	RequestType rt = REQ_GAME_MAP;
	Clnt.Send (reinterpret_cast<char*>(&rt), sizeof(RequestType));
	Clnt.RecvFile (LOC_GAME_MAP);
}

void ClientWorker::GetGameFile (const std::string & name) throw (std::runtime_error) {
	RequestType rt = REQ_GAME_FILE;
	Clnt.Send (reinterpret_cast<char*>(&rt), sizeof(RequestType));
	Clnt.SendString (name);
	Clnt.RecvFile ("tmp/" + name);
}

void ClientWorker::GetAllResources () throw (std::runtime_error) {
	GetGameConfig();
	GetGameMap();
	XML xml;
	xml.Load(LOC_GAME_XML);
	xml.SelectSection("textures");
	std::string resName;
	for (bool b = xml.SelectNode("texture"); b; b = xml.SelectNextNode("texture")) {
		resName = xml.GetAttribute("path");
		GetGameFile(resName);
	}
}
