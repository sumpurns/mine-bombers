#ifndef __TEXTURES_BANK_H__
#define __TEXTURES_BANK_H__

#include "texture.h"


class TextureRegistry {
	public:
		TextureRegistry () throw(std::runtime_error);
		virtual ~TextureRegistry ();

		const Texture & operator[] (int index) throw (std::runtime_error);

		void Load (const std::string & configFile) throw (std::runtime_error);
		void Unload ();

		int GetTextureId (const std::string & name) throw(std::runtime_error);

		const Texture & GetTexture(int id) throw(std::runtime_error);
	protected:
	private:
		bool Loaded;
		std::vector<Texture> Textures;
};


#endif
