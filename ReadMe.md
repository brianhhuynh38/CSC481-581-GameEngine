# Starting Instructions

Compiler: c++ 17 standard

# References
There were tutorials and other references used in the making of this game engine. Here, we 
will list each tutorial and each file that it was used to construct/influence the construction of. 
Additionally, we will provide a short description detailing its use and impact within our codebase. 
Each tutorial can be accessed by clicking on its title:

## [Parallel Realities Shooter Tutorial - Part 1](https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php)

The code was largely taken from the tutorial with some more minor changes with variable naming
and a call to Pump_Events() in order to make the Keyboard inputs work properly. We also did not
use the SDL_Quit event that the tutorial asked for, opting to directly take keyboard input to
quit instead.

Files influenced by this tutorial:

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
|  [`main.cpp`](./main.cpp)  | [`definitions.h`](./definitions.h) |
|  [`draw.cpp`](./draw.cpp)  |        [`draw.h`](./draw.h)        |
| [`input.cpp`](./input.cpp) |       [`input.h`](./input.h)       |
| | [`structs.h`](./structs.h) |
