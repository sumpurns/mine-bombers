#include "map.h"

#include <fstream>

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
