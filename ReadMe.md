# Introduction
This is Team 1's game engine repository for CSC 581 - Game Engine Foundaations. Below are 
compilation instructions, as well as our references for each of the tutorials we used while 
making this project.

# Starting Instructions

**Compiler**: C++ 17 standard

This project uses a Visual Studio solution file to organize and compile. The libraries are included 
within the project for convenience. The following are the relative filepaths used for the  
SDL2 and SDL2_image libraries in Project properties:

**Include Directories**:`$(ProjectDir)\Libs\[LibraryName]\include`

**Library Directories**: `$(ProjectDir)\Libs\[LibraryName]\lib\x64`

**Additional Dependencies**: `SDL2.lib;SDL2main.lib;SDL2_image.lib;%(AdditionalDependencies)`

## Controls

To exit the engine, press the `ESCAPE` key.

To control the player, use the `ARROW KEYS` for movement.

# References
There were tutorials and other references used in the making of this game engine. Here, we 
will list each tutorial and each file that it was used to construct/influence the construction of. 
Additionally, we will provide a short description detailing its use and impact within our codebase. 
Each tutorial can be accessed by clicking on its title:

## Homework 1

### [Parallel Realities Shooter Tutorial - Part 1](https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php)

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

### [Parallel Realities Shooter Tutorial - Part 2](https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php)

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

### [Parallel Realities Shooter Tutorial - Part 5](https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php)

This was referenced mostly for player input and initialization as well as initialization of stages. Our 
implementation does not completely match regarding the overall structure suggested by the guide, as we follow 
a more model-based approach.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`playerController.cpp`](./playerController.cpp) | [`playerController.h`](./playerController.h) |
| [`input.cpp`](./input.cpp) | [`input.h`](./input.h) |
| [`main.cpp`](./main.cpp) | [`structs.h`](./structs.h) |

### [Parallel Realities Platformer Tutorial - Part 1](https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php)

This tutorial was referenced to create the map and stage intialization within the project. We used it as a reference 
for rendering maps as well, though our implementation does not currently implement a tile system, so our code does not 
completely follow this tutorial, applying it to our coordinate system implementation.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`stageManager.cpp`](./stageManager.cpp) | [`stageManager.h`](./stageManager.h) |
| [`stage.cpp`](./stage.cpp) | [`stage.h`](./stage.h) |
| [`stageRenderer.cpp`](./stageRenderer.cpp) |  |
|  | [`definitions.h`](./definitions.h) |

### [Lazy Foo' Productions Collider Tutorial](https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php)

The colliders tutorial was used to build the `checkCollision` method (in the future), for the calculations of checking for 
collisions between `SDL_Rect` objects. It was also used to understand how to handle the result of a collision, 
moving the entity back to where it was, in the `Entity::move` method.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`collisions.cpp`](./collisions.cpp) | |
| [`entity.cpp`](./entity.cpp) | [`entity.h`](./entity.h) |
