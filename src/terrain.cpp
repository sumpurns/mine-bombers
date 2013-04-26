#include "terrain.h"


int TerrainElement::GetTextureId () throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("TerrainElement is not loaded, can't get texture ID");
	}
	return TextureId;
}

int TerrainElement::GetMapId () throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("TerrainElement is not loaded, can't get texture ID");
	}
	return MapId;
}

float TerrainElement::GetDamagePercent () throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("TerrainElement is not loaded yet, can't get the damage percent");
	}
	return (float)Durability/(float)MaxDurability;
}

TerrainElement::TerrainElement () throw (std::runtime_error) {
	Loaded = false;
}

TerrainElement::~TerrainElement () {
}


void TerrainElement::Load (XML & xml, TextureRegistry & texReg) throw (std::runtime_error) {
	TextureId = texReg.GetTextureId(xml.GetAttribute("texture"));
	MapId = Helper::GetInt(xml.GetAttribute("mapId"));
	MaxDurability = Helper::GetInt(xml.GetAttribute("durability"));
	Durability = MaxDurability;
	Destroyable = Helper::GetBool(xml.GetAttribute("destroyable"));
	Diggable = Helper::GetBool(xml.GetAttribute("diggable"));
	Empty = Helper::GetBool(xml.GetAttribute("empty"));
	Loaded = true;
}

