#include <iostream>
#include <vector>
//#include "socket.h"
#include "server_listner.h"
#include "server_worker.h"
#include "thread.h"
#include "server_shared.h"




class ServerMain : public Thread {
	public:
		ServerMain () throw (std::runtime_error);
		~ServerMain ();
		
		void * Main (void * arg) throw (std::runtime_error);
		
		int RegisterPlayer (const std::string & nick) throw (std::runtime_error);

		void Init (ServerListner & srvLstner);
		void HandleWorkers () throw (std::runtime_error);
	protected:
	private:
};


int main (int argc, char ** argv) {
	std::cout << "MineBombers server. You are welcome." << std::endl;

	ServerShared shared;
	shared.Players.Init(2);

	ServerListner srv(shared);
	srv.Load("res/game.xml");
	srv.Start();
	
	return 0;
}
