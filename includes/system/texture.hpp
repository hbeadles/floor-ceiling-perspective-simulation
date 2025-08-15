#pragma once
#include <base/common.hpp>


SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface);
SDL_Texture* loadTexture(Application & app, const std::string & filename);
SDL_Texture* loadTextureWithColorKey(Application & app, std::string filename, Uint8 r, Uint8 g, Uint8 b);