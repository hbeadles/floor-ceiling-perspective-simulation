#pragma once

#include <base/common.hpp>


void prepareScene(Application & app);
void presentScene(Application & app);
SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface);
SDL_Texture* loadTexture(Application & app, const std::string & filename);
SDL_Texture* loadTextureWithColorKey(Application & app, std::string filename, Uint8 r, Uint8 g, Uint8 b);
void blit(Application & app, SDL_Texture* texture, int x, int y, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
void drawRect(int x, int y, int w, int h, int r, int g, int b, int a);
void drawRect(SDL_Rect bounds, SDL_Color color);
void drawOutlineRect(int x, int y, int w, int h, int r, int g, int b, int a);
void drawLine(SDL_Point start, SDL_Point end, SDL_Color color);
void drawFLine(SDL_FPoint start, SDL_FPoint end, SDL_Color color);
