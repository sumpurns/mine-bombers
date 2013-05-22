#include "socket.h"

#include <vector>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "shared_config.h"

Socket::Socket () throw (std::runtime_error) {
	Active = false;
	Binded = false;
	Connected = false;
}

Socket::~Socket () {
	Fini ();
}

void Socket::Init () throw (std::runtime_error) {
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock == -1) {
		throw std::runtime_error("Init: socket(); failed");
	}
	Active = true;
}

void Socket::Init (int sockfd) throw (std::runtime_error) {
	Sock = sockfd;
	Active = true;
	Connected = true;
}

void Socket::Fini () {
	if (Active) {
		close(Sock);
	}
}

size_t Socket::Send (const char * data, size_t size) throw (std::runtime_error) {
	if (!Active || !Connected) {
		throw std::runtime_error("Not ready to send");
	}
	int ret = send(Sock, reinterpret_cast<const void*>(data), size, 0);
	if (ret == -1) {
		throw std::runtime_error("Error while Send();");
	}
	return (size_t)ret;
}

size_t Socket::Recv (char * data, size_t size) throw (std::runtime_error) {
	if (!Active || !Connected) {
		throw std::runtime_error("Not ready to recv");
	}
	int ret = recv(Sock, reinterpret_cast<void*>(data), size, 0);
	if (ret == -1) {
		throw std::runtime_error("Error while Recv();");
	}
	return (size_t)ret;
}

void Socket::SendFile (const std::string & path) throw (std::runtime_error) {
	std::ifstream file;
	file.open(path.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Can't open file for sending via socket");
	}
	
	file.seekg(0, std::fstream::end);
	size_t fileSize = file.tellg();
	size_t snt = Send(reinterpret_cast<char*>(&fileSize), sizeof(size_t));
	if (snt == 0) {
		throw std::runtime_error("Disconnected suddenly while sending a file");
	}

	file.seekg(0, std::fstream::beg);
	size_t readen, sent;
	char buf[NET_BLOCK];
	for (sent = 0; sent < fileSize; sent += readen) {
		file.read (buf, readen = std::min(fileSize - sent, (size_t)NET_BLOCK));
		if (file.fail()) {
			throw (std::runtime_error("Error while trying to read from file"));
		}
		size_t stBlock = 0;
		while (stBlock < readen) {
			stBlock += (snt = Send(buf + stBlock, readen - stBlock));
			if (snt == 0) {
				throw std::runtime_error("Disconnected suddenly while sending a file");
			}
		}
	}
	file.close();
}

void Socket::RecvFile (const std::string & path) throw (std::runtime_error) {
	std::ofstream file;
	file.open(path.c_str(), std::ofstream::out | std::ofstream::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Can't open a file to recieve the data");
	}

	size_t fileSize, recvd, written = 0;
	char buf[NET_BLOCK];
	size_t rcv = Recv(reinterpret_cast<char*>(&fileSize), sizeof(size_t));
	if (rcv == 0) {
		throw std::runtime_error("Disconnected suddenly while recieving a file");
	}
	while (written < fileSize) {
		recvd = Recv(buf, std::min(fileSize - written, (size_t)NET_BLOCK));
		if (recvd == 0) {
			throw std::runtime_error("Disconnected suddenly while recieving a file");
		}
		file.write(buf, recvd);
		written += recvd;
	}
	file.close();
}

void Socket::SendString (const std::string & str) throw (std::runtime_error) {
	size_t len = str.length() + 1;
	size_t ret = Send (reinterpret_cast<char*>(&len), sizeof(size_t));
	if (ret == 0) {
		throw std::runtime_error("Disconnected suddenly while sending a string");
	}
	ret = Send (reinterpret_cast<const char*>(str.data()), len);
	if (ret == 0) {
		throw std::runtime_error("Disconnected suddenly while sending a string");
	}
}

void Socket::RecvString (std::string & str) throw (std::runtime_error) {
	size_t len, ret;
	ret = Recv (reinterpret_cast<char*>(&len), sizeof(size_t));
	if (ret == 0) {
		throw std::runtime_error("Disconnected suddenly while recieving a string");
	}
	std::vector<char> tstr;
	tstr.resize(len);
	ret = Recv (reinterpret_cast<char*>(tstr.data()), len);
	if (ret == 0) {
		throw std::runtime_error("Disconnected suddenly while recieving a string");
	}
	str.assign(tstr.begin(), tstr.end() - 1);
}

Client::Client () throw (std::runtime_error) {
}

Client::~Client () {
}

bool Client::Connect (const std::string & host, const std::string & port) throw (std::runtime_error) {
	if (!Active) {
		throw std::runtime_error("Not ready to connect");
	}
	addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (0 != getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) {
		throw std::runtime_error("Can't get addr info");
	}

	if (0 != connect(Sock, res->ai_addr, res->ai_addrlen)) {
		return false;
	}
	Connected = true;
	return true;
}

Server::Server () {
}

Server::~Server () {
}

void Server::Bind (const std::string & port) throw (std::runtime_error) {
	if (!Active) {
		throw std::runtime_error("Not ready to bind");
	}
	addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; 

	if (0 != getaddrinfo(NULL, port.c_str(), &hints, &res)) {
		throw std::runtime_error("Can't get addr info");
	}

	if (0 != bind(Sock, res->ai_addr, res->ai_addrlen)) {
		throw std::runtime_error("Can't bind this port");
	}
	Binded = true;
}

void Server::Listen () throw (std::runtime_error) {
	if (!Active || !Binded) {
		throw std::runtime_error("Not ready for listening");
	}
	if (0 != listen(Sock, 20)) {
		throw std::runtime_error("Can't listen");
	}
}

int Server::Accept () {
	if (!Active || !Binded) {
		throw std::runtime_error("Not ready for accepting");
	}
	int ret = accept(Sock, NULL, NULL);
	if (-1 == ret) {
		throw std::runtime_error("Error while accepting the connection");
	}
	return ret;
}
