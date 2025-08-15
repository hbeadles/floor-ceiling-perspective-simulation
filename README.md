## Floor Ceiling Demonstration - SDL2

This is a simple simulation of floor to ceiling perspective using SDL2. It draws colors on the screen in a grid pattern
directly to a texture. The pixels are normalized on the screen, so from -1 to 1 in the x and y directions. This means that,
as we get closer to 0, our horizon line, our distance gets really large (perspective). 

Color is used to illustrate this effect. We move from red to blue from the top and bottom to the center of the screen. Near the floor / ceiling, the colors have greater intensity; as we move to the center they shift quickly to a uniform blue color, proportional to the increase in "distance"

<!-- <p align="center">
  <img src="https://github.com/waldyr/Sublime-Installer/blob/master/sublime_text.png?raw=true" alt="Sublime's custom image"/>
</p> -->

### Credit - EvalDraw - Ken Silverman

The inspiration for this came directly from Ken Silverman's own script on this procedure which is in his excellent EvalDraw tool. I highly
recommend this for learning and inspiration. 

1. [EVALDRAW](https://advsys.net/ken/download.htm)
    1. Open the zip file for EvalDraw
    2. Go to demos/ceilflor.kc

## Dependencies

This is a CMake project. It has the following dependencies:

1. SDL2



