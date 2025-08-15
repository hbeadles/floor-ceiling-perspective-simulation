#include <system/texture.hpp>

static SDL_Texture* getTexture(const std::string & filename);
extern Application app;
extern std::unordered_map<std::string, SDL_Texture*> textureCache;

/**
 * Converts an SDL_Surface to an SDL_Texture.
 *
 * @param surface The SDL_Surface to convert.
 * @param destroySurface A flag indicating whether to free the surface after conversion (non-zero to free).
 * @return A pointer to the created SDL_Texture, or nullptr if the conversion fails.
 */
SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface)
{
	SDL_Texture *texture;

	texture = SDL_CreateTextureFromSurface(app.renderer, surface);

	if (destroySurface)
	{
		SDL_FreeSurface(surface);
	}

	return texture;
}
/**
 * Retrieves a cached SDL_Texture by its filename.
 *
 * @param filename The filename of the texture to retrieve.
 * @return A pointer to the cached SDL_Texture, or nullptr if the texture is not found.
 */
static SDL_Texture* getTexture(const std::string & filename){
    auto it = textureCache.find(filename);
    if (it == textureCache.end()) {
        // Key not found
        return nullptr;
    }
    // Key found, return the associated texture
    return it->second;
}

/**
 * Loads an SDL_Texture from a file and caches it.
 *
 * @param app The application instance containing the SDL renderer.
 * @param filename The filename of the texture to load.
 * @return A pointer to the loaded SDL_Texture, or nullptr if loading fails.
 */
SDL_Texture* loadTexture(Application & app, const std::string & filename){
    SDL_Texture* texture = getTexture(filename);
    if(!texture){
        texture = IMG_LoadTexture(app.renderer, filename.c_str());
        textureCache[filename] = texture;
    }
    return texture;
}


/**
 * Loads an SDL_Texture from a file with a specified color key and caches it.
 *
 * @param app The application instance containing the SDL renderer.
 * @param filename The filename of the texture to load.
 * @param r The red component of the color key.
 * @param g The green component of the color key.
 * @param b The blue component of the color key.
 * @return A pointer to the loaded SDL_Texture, or nullptr if loading fails.
 */
SDL_Texture* loadTextureWithColorKey(Application & app, std::string filename, Uint8 r, Uint8 g, Uint8 b){
    SDL_Surface* surf = IMG_Load(filename.c_str());
    if(!surf){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load failed: %s", IMG_GetError());
        return nullptr;
    }
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, r, g, b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surf);
    if(!texture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return nullptr;
    }
    SDL_FreeSurface(surf);
    return texture;
}
