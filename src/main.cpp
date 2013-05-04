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


class Map {
	public:
		Map () throw (std::runtime_error);
		virtual ~Map ();

		void Load (const std::string & file, TerrainDefaults & tDefs) throw (std::runtime_error);
		void Draw (SDL_Surface * screen, TextureRegistry & tReg) throw (std::runtime_error);
	private:
		std::vector< std::vector <TerrainElement> > Field;
		int Width;
		int Height;
		bool Loaded;
};

Map::Map () throw (std::runtime_error) {
	Loaded = false;
}

Map::~Map () {
}

void Map::Load (const std::string & file, TerrainDefaults & tDefs) throw (std::runtime_error) {
	std::ifstream f;
	f.open(file.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!f.is_open()) {
		throw std::runtime_error(("Can't open file " + file).c_str());
	}
	std::vector<unsigned char> tmp;
	f.read(reinterpret_cast<char*>(&Width), sizeof(int));
	f.read(reinterpret_cast<char*>(&Height), sizeof(int));
	Field.resize(Height);
	tmp.resize(Width);
	for (int y = 0; y < Height; y++) {
		Field[y].resize(Width);
		if (!f.read(reinterpret_cast<char*>(tmp.data()), (size_t)Width).good()) {
			throw std::runtime_error ("Error while reading map file");
		}
		for (int x = 0; x < Width; x++) {
			Field[y][x] = tDefs.GetElementByMapId((int)tmp[x]);
		}
	}
	f.close();
}

void Map::Draw (SDL_Surface * screen, TextureRegistry & tReg)  throw(std::runtime_error) {
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			tReg[Field[y][x].GetTextureId()].GridDraw(screen, x, y, 0, 0);
		}
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

	Client clnt;
	clnt.Init();
	clnt.Connect("localhost", "12345");
	clnt.Send(SERVER_GREETING, strlen(SERVER_GREETING) + 1);

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
			/*for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					gnd.GridDraw(Screen, x, y, 0, 0);
				}
			}*/
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

