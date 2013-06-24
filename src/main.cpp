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
#include "client_worker.h"

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
	try {
		ClntWrk.Login();
	} catch (std::runtime_error & err) {
		std::cout << "Nickname is not accepted" << std::endl;
		ClntWrk.Disconnect();
		return 1;
	}
	ClntWrk.GetAllResources();
	ClntWrk.Disconnect();

	TextureRegistry texReg;
	texReg.Load(LOC_GAME_XML);
	//int tId = texReg.GetTextureId("ground");
	//const Texture & gnd = texReg[tId];

	TerrainDefaults tDefs;
	tDefs.Load(LOC_GAME_XML, texReg);

	Map map;
	map.Load(LOC_GAME_MAP, tDefs);

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

