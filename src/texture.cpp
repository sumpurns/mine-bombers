#include "texture.h"

#include "config.h"

Texture::Texture () throw (std::runtime_error) {
	Loaded = false;
	Image = NULL;
}

Texture::Texture (const Texture & rhs) throw (std::runtime_error) {
	Name = rhs.Name;
	if (rhs.Loaded) {
		Image = SDL_ConvertSurface(rhs.Image, rhs.Image->format, rhs.Image->flags);
	} else {
		Image = NULL;
	}
	Loaded = rhs.Loaded;
}

Texture::~Texture () {
	UnloadImage();
}

void Texture::Load (XML & xml) throw(std::runtime_error) {
	Name = xml.GetAttribute("name");
	LoadImage("tmp/" + xml.GetAttribute("path"));
	Loaded = true;
}

void Texture::LoadImage (const std::string & file) throw(std::runtime_error) {
	SDL_Surface * tmp;
	tmp = SDL_LoadBMP(file.c_str());
	if (!tmp) {
		throw std::runtime_error(("Can't load image " + file).c_str());
	}
	Image = tmp;
}

void Texture::UnloadImage () {
	if (Loaded) {
		if (Image) {
			SDL_FreeSurface(Image);
		}
		Loaded = false;
	}
}

SDL_Surface * Texture::GetImage () const throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Image is not loaded");
	}
	return Image;
}

void Texture::GridDraw (SDL_Surface * screen, int x, int y, int ix, int iy) const throw(std::runtime_error) {
	Draw(screen, x * SQSZ, y * SQSZ, ix, iy);
}

void Texture::Draw (SDL_Surface * screen, int x, int y, int ix, int iy) const throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Image is not loaded");
	}
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_BlitSurface(Image, NULL, screen, &dst);
}

const std::string & Texture::GetName () const throw (std::runtime_error) {
	if (!Loaded) {
		throw std::runtime_error("Not loaded");
	}
	return Name;
}


Texture & Texture::operator= (const Texture & rhs) throw (std::runtime_error) {
	if (this == &rhs) {
		return *this;
	}
	Name = rhs.Name;
	if (rhs.Loaded) {
		Image = SDL_ConvertSurface(rhs.Image, rhs.Image->format, rhs.Image->flags);
	} else {
		Image = NULL;
	}
	Loaded = rhs.Loaded;
	return *this;
}
