#include "server_listner.h"
#include "server_worker.h"
#include "xml.h"

ServerListner::ServerListner (ServerShared & shrd) throw (std::runtime_error) 
: Shared(shrd) {
	Loaded = false;
	Started = false;
	Shared.Players.Init(2);
}

ServerListner::~ServerListner () {
	Unload();
}

//Completed. Load the XML, bind the socket.
void ServerListner::Load (const std::string & configFile) throw (std::runtime_error) {
	XML xml;
	xml.Load (configFile);
	xml.SelectSection ("server");
	xml.SelectNode ("network");
	std::string port = xml.GetAttribute ("port");
	Serv.Init ();
	Serv.Bind (port);
}

void ServerListner::Unload () {
	if (Loaded) {
	}
}

//Run the listen/accept cycle
void ServerListner::Start () throw (std::runtime_error) {
	while (true) {
		Serv.Listen();
		AcceptConnection(Serv.Accept());
	}
}


//Accept the incoming connection
void ServerListner::AcceptConnection (int sockfd) throw (std::runtime_error) {
	WorkersList.push_back(ServerWorker(Shared));
	WorkersList.back().Serve(sockfd);
}


//Currently not needed
void ServerListner::StartRound () throw (std::runtime_error) {
}
