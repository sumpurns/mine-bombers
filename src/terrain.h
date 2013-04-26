#ifndef __TERRAIN_ELEMENT_H__
#define __TERRAIN_ELEMENT_H__

#include <string>
#include "helpers.h"
#include "textures_reg.h"
#include "xml.h"

class TerrainElement {
	public:
		TerrainElement () throw (std::runtime_error);
		virtual ~TerrainElement ();

		void Load(XML & xml, TextureRegistry & texReg) throw (std::runtime_error);
		int GetTextureId () throw (std::runtime_error);
		int GetMapId () throw (std::runtime_error);
		float GetDamagePercent () throw (std::runtime_error);
	protected:
	private:
		bool Loaded;
		int MapId;
		int MaxDurability;
		int Durability;
		int TextureId;
		bool Empty;
		bool Diggable;
		bool Destroyable;
};

#endif
