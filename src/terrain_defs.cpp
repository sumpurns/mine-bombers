#include "terrain_defs.h"

TerrainDefaults::TerrainDefaults () throw (std::runtime_error) {
	Loaded  = false;
}

TerrainDefaults::~TerrainDefaults () {
}

int TerrainDefaults::GetDefaultId () throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("TerrainDefaults is not loaded yet. Can't say the default TerrainElement Id");
	}
	return DefaultId;
}

int TerrainDefaults::GetId (const std::string & name) throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("TerrainDefaults is not loaded, can't get terrain id");
	}
	for (int id = 0; id <  (int) Elements.size(); id++) {
		if (name == Elements[id].Name) {
			return id;
		}
	}
	throw std::runtime_error (("Terrain element with such name is not found: " + name).c_str());
	return -1;
}

const TerrainElement & TerrainDefaults::operator[] (int index) throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("Terrain defaults is not loaded, can't get one of terrain types");
	}
	if (index < 0 || index >= (int)Elements.size()) {
		throw std::runtime_error ("Index is out of range, no such terrain default");
	}
	return Elements[index].Element;
}

void TerrainDefaults::Load (const std::string & file, TextureRegistry & texReg) throw (std::runtime_error) {
	XML xml;
	xml.Load(file);
	xml.SelectSection("map");
	std::string defName = xml.GetValue("default");
	DefaultId = texReg.GetTextureId(defName);
	xml.SelectNode("terrain-types");
	for (bool res = xml.SelectNode("ttype"); res; res = xml.SelectNextNode ("ttype")) {
		Elements.push_back(Record());
		Elements.back().Name = xml.GetAttribute("name");
		Elements.back().Element.Load(xml, texReg);
	}
	Loaded = true;
}

const TerrainElement & TerrainDefaults::GetElementByMapId (int mid) throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error ("TerrainDefaults is not loaded yet, so getting elements by map id is deprecated");
	}
	for (int id = 0; id < (int) Elements.size(); id++) {
		if (mid == Elements[id].Element.GetMapId()) {
			return Elements[id].Element;
		}
	}
	throw std::runtime_error("No default terrain element with such map id");
}

