#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <SDL/SDL.h>

#include "helpers.h"
#include "config.h"
#include "shared_config.h"
#include "texture.h"
#include "textures_reg.h"
#include "terrain.h"
#include "terrain_defs.h"
#include "socket.h"
#include "xml.h"
#include "map.h"

static SDL_Surface * Screen;

static void Slock() {
	if ( SDL_MUSTLOCK(Screen) ) {
		if ( SDL_LockSurface(Screen) < 0 ) {
			return;
		}
	}
}

static void Sulock() {
	if ( SDL_MUSTLOCK(Screen) ) {
		SDL_UnlockSurface(Screen);
	}
}

class ClientWorker {
	public:
		ClientWorker () throw (std::runtime_error);
		virtual ~ClientWorker ();

		void Connect () throw (std::runtime_error);
		void Disconnect () throw (std::runtime_error);

		void CheckProtocol () throw (std::runtime_error);
		void GetGameConfig () throw (std::runtime_error);
		void GetGameMap () throw (std::runtime_error);
		void GetGameFile (const std::string & name) throw (std::runtime_error);

	protected: 
	private:
		Client Clnt;
};

ClientWorker::ClientWorker () throw (std::runtime_error) {
}

ClientWorker::~ClientWorker () {
}

void ClientWorker::Connect () throw (std::runtime_error) {
	Clnt.Init();
	Clnt.Connect("localhost", "12345");
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

int main (int argc, char ** argv) {
	if (0 > SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Can not init SDL library" << std::endl;
		return 1;
	}
	Screen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (!Screen) {
		std::cout << "Can't init video mode" << std::endl;
		return 1;
	}

	ClientWorker ClntWrk;
	try {
		ClntWrk.Connect();
		ClntWrk.CheckProtocol();
	} catch (std::runtime_error & err) {
		std::cout << err.what() << ", finishing client" << std::endl;
		return 1;
	}
	//ClntWrk.GetGameConfig();
	ClntWrk.GetGameMap();
	ClntWrk.GetGameFile("res/art/ground.bmp");
	ClntWrk.Disconnect();

	TextureRegistry texReg;
	texReg.Load("res/game.xml");
	//int tId = texReg.GetTextureId("ground");
	//const Texture & gnd = texReg[tId];

	TerrainDefaults tDefs;
	tDefs.Load("res/game.xml", texReg);

	Map map;
	map.Load("res/1.map", tDefs);

	SDL_Event event;
	bool Done = false;
	while (!Done) {
		Slock();
			map.Draw(Screen, texReg);
			SDL_Flip(Screen);
		Sulock();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				Done = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					Done = true;
				}
			}
		}
	}
	SDL_Quit();
	return 0;
}

