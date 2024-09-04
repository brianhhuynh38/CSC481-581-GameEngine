# Starting Instructions

Compiler: C++ 17 standard

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

## [Parallel Realities Shooter Tutorial - Part 2](https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php)

The code functions were referenced from the tutorial with changes mainly relating to the overall structure
of our engine. Our [`Entity`](./entity.cpp) implementation was class-based using a [`Vector2D`](./vector2D.cpp) 
class we created in order to store `floats` as coordinate dimensions rather than ints. The rendering functions 
are largely the same, but with tweaks to better fit our implementation and architecture.

Files influenced by this tutorial:

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
|  [`main.cpp`](./main.cpp)  |  |
|  [`draw.cpp`](./draw.cpp)  |       [`draw.h`](./draw.h)         |
| [`entity.cpp`](./entity.cpp) | [`entity.h`](./entity.h) |
