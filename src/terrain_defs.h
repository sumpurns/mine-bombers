#ifndef __TERRAIN_DEFAULTS_H__
#define __TERRAIN_DEFAULTS_H__

#include <vector>
#include "terrain.h"

class TerrainDefaults {
	public:
		TerrainDefaults () throw (std::runtime_error);
		virtual ~TerrainDefaults ();

		void Load (const std::string & file, TextureRegistry & texReg) throw (std::runtime_error);
		void Unload ();

		int GetDefaultId () throw (std::runtime_error);
		int GetId (const std::string & name) throw (std::runtime_error);
		const TerrainElement & GetElementByMapId (int mid) throw (std::runtime_error);
		const TerrainElement & operator[] (int index) throw (std::runtime_error);
	protected:
	private:
		struct Record {
			TerrainElement Element;
			std::string Name;
		};
		bool Loaded;
		std::vector<Record> Elements;
		int DefaultId;
};


#endif
