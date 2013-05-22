#include <iostream>
//#include "socket.h"
#include "server_listner.h"
#include "server_worker.h"
#include "thread.h"


#include <sys/sem.h>

class PlayerRecord {
	public:
		std::string Nick;
		bool Free;
	protected:
	private:
};

class PlayersList {
	public:
		std::vector<PlayerRecord> Records;
};

class ServerMain : public Thread {
	public:
		ServerMain () throw (std::runtime_error);
		~ServerMain ();
		
		void * Main (void * arg) throw (std::runtime_error);
		
		int RegisterPlayer (const & std::string & nick) throw (std::runtime_error);

		void Init (ServerListner & srvLstner);
		void HandleWorkers () throw (std::runtime_error);
	protected:
	private:
};

int main (int argc, char ** argv) {
	std::cout << "MineBombers server. You are welcome." << std::endl;
	ServerListner srv;
	srv.Load("res/game.xml");
	srv.Start();
	
	return 0;
}
