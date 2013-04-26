#include <iostream>
#include "socket.h"

int main (int argc, char ** argv) {
	std::cout << "MineBombers server. You are welcome." << std::endl;
	char buf[300];
	Server srv;
	Client tlk;
	srv.Init();
	srv.Bind("12345");
	srv.Listen();
	tlk.Init(srv.Accept());
	tlk.Recv(buf, 300);
	std::cout << buf << std::endl;
	return 0;
}
