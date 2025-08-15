#include <system/init.hpp>

extern Application app;
extern std::unordered_map<std::string, SDL_Texture*> textureCache;

/**
 * Logs an SDL error message and returns a constant error code.
 *
 * @param fmt The format string for the error message.
 * @return A constant error code (3).
 */
int throw_sdl_err(const char* fmt){
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        fmt,
        SDL_GetError()
    );
    return 3; // constant error code.
}
/**
 * Initializes the SDL_mixer audio subsystem.
 *
 * @return True if the audio subsystem is successfully initialized, false otherwise.
 */
bool initAudio() {
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    Mix_AllocateChannels(MAX_SND_CHANNELS);
    return true;
}
/**
 * Initializes the SDL subsystems, creates the application window and renderer,
 * and sets up required SDL components like SDL_image, SDL_mixer, and SDL_ttf.
 *
 * @param title The title of the application window.
 * @return True if initialization is successful, false otherwise.
 */
bool initSDL(std::string title)
{
    bool success = true;

#ifdef __EMSCRIPTEN__
    // For Emscripten, initialize video and audio together
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        throw_sdl_err("Error initializing SDL");
        success = false;
    }
#else
    // Desktop initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        throw_sdl_err("Error initializing video");
        success = false;
    }
#endif

    if (success) {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
            printf("Warning: Linear texture filtering not enabled!");
        }

        app.window = SDL_CreateWindow(title.c_str(),
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);

        if (app.window == NULL){
            throw_sdl_err("Create Window failed %s");
            success = false;
        } else {
            app.renderer = SDL_CreateRenderer(app.window, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (app.renderer == NULL){
                throw_sdl_err("Renderer could not be created %s");
                success = false;
            } else {
                SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Try to initialize SDL_image
                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    printf("Warning: SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    // Don't fail completely - you might not need images for this project
                }

                // Try to initialize audio (non-fatal for this project)
                if (!initAudio()){
                    printf("Warning: SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    printf("Continuing without audio...\n");
                    // Don't set success = false here
                }

                // Try to initialize TTF
                if(TTF_Init() == -1){
                    printf("Warning: SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    printf("Continuing without TTF...\n");
                    // Don't set success = false here unless you actually need fonts
                }
            }
        }
    }

    if (success) {
        SDL_Texture* screenTex = SDL_CreateTexture(
            app.renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH, SCREEN_HEIGHT);

        if (!screenTex){
            throw_sdl_err("SDL could not generate base screen texture");
            success = false;
        } else {
            app.frame_texture = screenTex;
        }
    }

    if (success) {
        SDL_ShowCursor(0);
    }

    return success;
}
/**
 * Cleans up and releases all SDL resources, including the renderer, window,
 * textures, and other subsystems like SDL_mixer, SDL_ttf, and SDL_image.
 */
void cleanup(){
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    app.window = nullptr;
    app.renderer = nullptr;

    for(auto& texture : textureCache){
        SDL_DestroyTexture(texture.second);
    }
    SDL_DestroyTexture(app.frame_texture);
    textureCache.clear();

    SDL_Quit();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
}