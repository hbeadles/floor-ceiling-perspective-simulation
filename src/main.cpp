#include <base/common.hpp>
#include <system/init.hpp>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
Application app;
std::unordered_map<std::string, SDL_Texture*> textureCache;

SDL_Texture* g_texture = nullptr;
bool g_running = true;

/**
 * Computes the pixel value based on the given coordinates and time.
 *
 * @param x The x-coordinate, normalized to a specific range.
 * @param y The y-coordinate, normalized to a specific range.
 * @param t The time value, used for dynamic transformations.
 * @return The computed pixel value as a double.
 */
double compute_current_pixel(double x, double y, double t){
    // Convert time and apply scaling like the original
    t = t * 0.25 + .01;
    double c = cos(t);
    double s = sin(t);
    
    // Initial coordinates
    double x0 = x; 
    double y0 = y; 
    double z0 = 4;
    
    // First transformation: rotate in XY plane
    double x1 = x0*c - y0*s; 
    double y1 = y0*c + x0*s; 
    double z1 = z0;
    
    // // // Second transformation: rotate in ZX plane
    x0 = z1*c - x1*s; 
    y0 = x1*c + z1*s; 
    z0 = y1;
    
    double r = 32.0 / fabs(z0);
    t *= 100;  // Modify t for the texture calculation
    
    double v = floor(x0*r + t) * floor(y0*r - t);
    r = v - floor((v - 7.5) / 16.0) * 16.0;
    //r = (v%16);
    
    double result = r *z0 * z0 * 0.0125;
    
    return result;
}


/**
 * Maps a normalized value (0 to 1) to a color in a blue-to-red gradient.
 *
 * @param value The normalized value to map (clamped between 0 and 1).
 * @return A 32-bit ARGB color value.
 */
Uint32 mapToBlueRedGradient(double value) {
    /**
     * Hacky thresholding to map the value to a color.
     * Basically we're taking the value and mapping it to a color.
     * If its below .10,
     ***/
    if (value <= 0) value = 0;
    if (value >= 1) value = 1;
    
    Uint8 red, green, blue;

    if(value < 0.10){
        double t = value / 0.10;
        red = 0;
        green = (Uint8)(255*t);
        blue = 255;
    }else if (value <0.20){
        double t = (value - 0.20) / 0.10;
        red = 0;
        green= 255;
        blue = (Uint8)(255 * (1 - t));
    }else if (value < 0.30){
        double t = (value - 0.30) / 0.10;
        red = (Uint8)(255 * t);
        green = 255;
        blue = 0;
    }else{
        double t = (value - 0.40) / 0.10;
        red = 255;
        green = (Uint8)(255 * (1-t));
        blue = 0;
    }
    

    return (0xFF << 24) | (red << 16) | (green << 8) | blue;
}
void main_loop() {
    if (!g_running) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    static double t = 0.0;
    t = SDL_GetTicks() * 0.001;

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            g_running = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
            return;
        }
    }

    auto coord = [](float value, float scale, float shift) {
        return (value / (double) SCREEN_WIDTH) * scale + shift;
    };

    SDL_RenderClear(app.renderer);

    void* pixels;
    int pitch;
    if(SDL_LockTexture(g_texture, nullptr, &pixels, &pitch) == 0){
        auto* pixel_buffer = (Uint32*) pixels;
        double min_value = INFINITY;
        double max_value = -INFINITY;

        for(int x = 0; x < SCREEN_WIDTH; x++){
            for(int y = 0; y < SCREEN_HEIGHT; y++){
                double nx = coord(x, 2.3, -1.0);
                double ny = coord(y, 2.3, -1.0);
                double color_value = compute_current_pixel(nx, ny, t);

                // Update min/max
                if (color_value < min_value) min_value = color_value;
                if (color_value > max_value) max_value = color_value;

                pixel_buffer[y * SCREEN_WIDTH + x] = mapToBlueRedGradient(color_value);
            }
        }

        // Print min/max every 60 frames (about once per second at 60fps)
        static int frame_count = 0;
        if (app.debug) {
            if (++frame_count % 60 == 0) {
                printf("Min value: %f, Max value: %f, Range: %f\n", min_value, max_value, max_value - min_value);
            }
        }
    }
    SDL_UnlockTexture(g_texture);

    SDL_RenderCopy(app.renderer, g_texture, nullptr, nullptr);
    SDL_RenderPresent(app.renderer);
}

/**
 * The main function - sets up everything and starts the appropriate main loop
 */
int main(int argc, char* argv[]){
    if(!initSDL("Floor Ceil Perspective")){
        printf("Failed to initialize SDL\n");
        return EXIT_FAILURE;
    }

    app.debug = false;
    g_texture = SDL_CreateTexture(
        app.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    if(!g_texture){
        printf("Failed to create texture\n");
        return EXIT_FAILURE;
    }

    auto coord = [](float value, float scale, float shift) {
        return (value / (double) SCREEN_WIDTH) * scale + shift;
    };

    printf("Value of min x: %f, max x: %f\n", coord(0, 2.0, -1.0), coord(SCREEN_WIDTH, 2.0, -1.0));
    printf("Value of min y: %f, max y: %f\n", coord(0, 2.0, -1.0), coord(SCREEN_HEIGHT, 2.0, -1.0));

#ifdef __EMSCRIPTEN__
    // Use Emscripten's main loop for web
    emscripten_set_main_loop(main_loop, 0, 1);  // 0 = use browser's requestAnimationFrame
#else
    // Traditional main loop for desktop
    while(g_running){
        main_loop();
    }
#endif

    // Cleanup
    if (g_texture) {
        SDL_DestroyTexture(g_texture);
    }

    return EXIT_SUCCESS;
}

