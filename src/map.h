#ifndef __MAP_H__
#define __MAP_H__

#include "terrain.h"
#include "terrain_defs.h"

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

#endif
