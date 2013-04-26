#include "textures_reg.h"

#include <vector>

TextureRegistry::TextureRegistry () throw(std::runtime_error) {
	Loaded = false;
}

TextureRegistry::~TextureRegistry () {
	if (Loaded) {
		Unload();
	}
}

void TextureRegistry::Unload () {
}

void TextureRegistry::Load (const std::string & configFile) throw (std::runtime_error){
	XML xml;
	xml.Load(configFile);
	xml.SelectSection("textures");
	for (bool res = xml.SelectNode("texture"); res; res = xml.SelectNextNode("texture")) {
		Textures.push_back(Texture());
		Textures.back().Load(xml);
	}
	Loaded = true;
}

int TextureRegistry::GetTextureId (const std::string & name) throw (std::runtime_error) {
	for (int texid = 0; texid < (int)Textures.size(); texid++) {
		if (Textures[texid].GetName() == name) {
			return texid;
		}
	}
	throw std::runtime_error(("No such texture: " + name).c_str());
	return -1;
}

const Texture & TextureRegistry::operator[] (int index) throw (std::runtime_error) {
	if (index < 0 || index >= (int)Textures.size()) {
		throw std::runtime_error ("No such texture index");
	}
	return Textures[index];
}

