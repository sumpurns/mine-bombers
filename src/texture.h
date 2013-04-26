#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include <stdexcept>
#include <SDL/SDL.h>
#include "xml.h"

class Texture {
	public:
		Texture () throw(std::runtime_error);
		Texture (const Texture & rhs) throw (std::runtime_error);
		virtual ~Texture ();
		void Load (XML & xml) throw(std::runtime_error);
		void Unload ();
		void LoadImage (const std::string & file) throw(std::runtime_error);
		void UnloadImage ();

		Texture & operator= (const Texture & rhs) throw(std::runtime_error);

		SDL_Surface * GetImage () const throw(std::runtime_error);
		const std::string & GetName () const throw(std::runtime_error);
		void GridDraw (SDL_Surface * screen, int x, int y, int ix, int iy) const throw(std::runtime_error);
		void Draw (SDL_Surface * screen, int x, int y, int ix, int iy) const throw(std::runtime_error);
	protected:
	private:
		SDL_Surface * Image;
		bool Loaded;
		std::string Name;
};

#endif
