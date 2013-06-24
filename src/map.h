#ifndef __MAP_H__
#define __MAP_H__

#include "terrain.h"
#include "terrain_defs.h"

//!Represents a gaming field on both (client andd server) sides
class Map {
	public:
		//!Does almost nothing
		Map () throw (std::runtime_error);
		//!Destructor
		virtual ~Map ();

		//!Loads map from a file
		/*! \param file File containing the array of texels
		 *  \param tDefs A table containing default texel states for filling up the field*/
		void Load (const std::string & file, TerrainDefaults & tDefs) throw (std::runtime_error);
		//!Draws the field on the screen
		/*! \param screen The SDL_Surface structer to draw on it
		 *  \param tReg The array of loaded textures*/
		void Draw (SDL_Surface * screen, TextureRegistry & tReg) throw (std::runtime_error);
	private:
		//!The field itself
		std::vector< std::vector <TerrainElement> > Field;
		//!Widht of field
		int Width;
		//!Height of field
		int Height;
		//!Set to true if the field is alredy loaded
		bool Loaded;
};

#endif
