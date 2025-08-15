#include <system/draw.hpp>

extern Application app;


void prepareScene(Application & app){
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(Application & app){
    SDL_RenderPresent(app.renderer);
}



void blit(Application & app, SDL_Texture* texture, int x, int y, double angle, SDL_RendererFlip flip) {

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    if (angle != 0) {
        // Calculate center point for rotation
        SDL_Point center = {dest.w / 2, dest.h / 2};
        // Use RenderCopyEx for rotation
        SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, &center, flip);
    } else {
        // Use standard RenderCopy if no angle
        SDL_RenderCopy(app.renderer, texture, NULL, &dest);
    }

}


void drawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_SetRenderDrawBlendMode(app.renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(app.renderer, r, g, b, a);
	SDL_RenderFillRect(app.renderer, &rect);
}

void drawRect(SDL_Rect bounds, SDL_Color color){
    SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(app.renderer, &bounds); 
}

void drawOutlineRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_SetRenderDrawBlendMode(app.renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(app.renderer, r, g, b, a);
	SDL_RenderDrawRect(app.renderer, &rect);
}

void drawLine(SDL_Point start, SDL_Point end, SDL_Color color){
    SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(app.renderer, start.x, start.y, end.x, end.y);
}

void drawFLine(SDL_FPoint start, SDL_FPoint end, SDL_Color color){
    SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLineF(app.renderer, start.x, start.y, end.x, end.y);
}
