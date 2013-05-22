#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <stdexcept>

class Socket {
	public:
		Socket () throw (std::runtime_error);
		virtual ~Socket ();

		void Init () throw (std::runtime_error);
		void Init (int sockfd) throw (std::runtime_error);
		void Fini ();

		size_t Send (const char * data, size_t size) throw (std::runtime_error);
		size_t Recv (char * data, size_t size) throw (std::runtime_error);

		void SendFile (const std::string & path) throw (std::runtime_error);
		void RecvFile (const std::string & path) throw (std::runtime_error);

		void SendString (const std::string & str) throw (std::runtime_error);
		void RecvString (std::string & str) throw (std::runtime_error);
	protected:
		int Sock;
		bool Active;
		bool Connected;
		bool Binded;
	private:
};

class Client : public Socket {
	public:
		Client () throw (std::runtime_error);
		virtual ~Client ();

		bool Connect (const std::string & host, const std::string & port) throw (std::runtime_error);
	protected:
	private:
};

class Server : public Socket {
	public:
		Server ();
		virtual ~Server ();

		void Bind (const std::string & port) throw (std::runtime_error);
		void Listen () throw (std::runtime_error);
		int Accept ();
	protected:
	private:
};

#endif
