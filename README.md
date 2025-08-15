## Floor Ceiling Demonstration - SDL2

This is a simple simulation of floor to ceiling perspective using SDL2. It draws colors on the screen in a grid pattern
directly to a texture. The pixels are normalized on the screen, so from -1 to 1 in the x and y directions. This means that,
as we get closer to 0, our horizon line, our distance gets really large (perspective). 

Color is used to illustrate this effect. We move from red to blue from the top and bottom to the center of the screen. Near the floor / ceiling, the colors have greater intensity; as we move to the center they shift quickly to a uniform blue color, proportional to the increase in "distance"

<img src="" alt="Alt text" style="width: 50%; height: auto;">


You can view this in the browser 
1. [View in Browser](https://hbeadles.github.io/floor_ceiling_perspective_simulation/floor_ceil_perspective.html)

### Credit - EvalDraw - Ken Silverman

The inspiration for this came directly from Ken Silverman's own script on this procedure which is in his excellent EvalDraw tool. I highly
recommend this for learning and inspiration. 

1. [EVALDRAW](https://advsys.net/ken/download.htm)
    1. Open the zip file for EvalDraw
    2. Go to demos/ceilflor.kc

## Dependencies

This is a CMake project. It has the following dependencies:

1. [SDL2](https://www.libsdl.org/)
2. [SDL2_image](https://www.libsdl.org/projects/SDL_image/) 
3. [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
4. [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

The other dependencies outside of SDL2 are optional, I have them included because I use them to init SDL2 generally. 

I have a build in emscripten, which will allow you to view this in a browser. 


### Building

To build the project locally, you can use cmake:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

You can build with emscripten as well (requires emsdk installed and sourced)

```bash
mkdir build-web
cd build-web
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake cmake --build . --config Release
```

### How it works?

The key logic is the `compute_current_pixel` function in `src/main.cpp`. The first thing we do is normalize the pixel coordinates to a range of roughly -1 to 1.
(I made it slightly higher to make the horizon line more visible). This means that the center of the screen is at (0, 0). 
From there, we compute a few things:

1.        double x1 = x0*c - y0*s; 
          double y1 = y0*c + x0*s;
          double z1 = z0;
   1. This transforms the pixel coordinates in the xy plane, using the cos / sine matrix transformation.
   2. Rotates us around the z-axis, so basically in a circle
2.        x0 = z1*c - x1*s; 
          y0 = x1*c + z1*s;
          z0 = y1;
   1. This transforms the pixel coordinates in the xz plane, using the cos / sine matrix transformation.
   2. The effect of is a rotation around the y-axis, so moving up and down
3. The combination of these effects give a rotation
4. Next is computing the horizon line, which is done with:
   1.        double r = 32.0 / fabs(z0);
   2. As we get closer to the horizon, note that z0 equals y1, so as y gets closer to 0, distance grows massively
5. Next we create bands of color across the screen, done with the following: 
   1.        double v = floor(x0*r + t) * floor(y0*r - t);
   2.        r = v - floor((v - 10.0) / 16.0) * 16.0
   3.  This code does a form of ordered dithering, [Reference](https://en.wikipedia.org/wiki/Ordered_dithering). Essentially we compute a threshold map that causes pixels to change color based on their position
and their distance from the horizon line. The result is a banding effect that gives the illusion of depth. Honestly, this is a brilliant piece of code. 
   4. You can experiment with this. Modifying $10.0$ will change the color bands. Changing $16.0$ will change the dithering size, either larger or smaller. 
   5. What's happening is we're binning pixels into set discrete bins, which then repeat across the screen. As you get closer to horizon, x0 and y0 get closer to 0, and so our grid coordinates get repeated more frequently

