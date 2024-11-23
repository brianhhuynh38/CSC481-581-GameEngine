# Introduction
This is Team 1's game engine repository for CSC 581 - Game Engine Foundations. Below are 
compilation instructions, as well as our references for each of the tutorials we used while 
making this project. Our network server implementation is also necessary to run in conjunction 
with the server in order to run the program. The code for the network server can be found [here](https://github.com/masonsluder/CSC481_581_NetworkServer).

## Table of Contents

- [Starting Instructions](#starting-instructions)
	- [Controls](#controls)
- [References](#references)
	- [Homework 1 References](#homework-1)
	- [Homework 2 References](#homework-2)
	- [Homework 3 References](#homework-3)
	- [Homework 4 References](#homework-4)

# Starting Instructions

**Compiler**: C++ 17 standard

This project uses a Visual Studio solution file to organize and compile. The libraries are included 
within the project for convenience. The following are the relative filepaths used for the  
`SDL2`, `SDL2_image`, and `libzmq-mt-4_3_5` libraries in Project properties:

**Include Directories**: `$(ProjectDir)\Libs\[LibraryName]\include`

**Library Directories**: `$(ProjectDir)\Libs\[LibraryName]\lib\x64`

**Additional Dependencies**: `SDL2.lib;SDL2main.lib;SDL2_image.lib;libzmq-mt-4_3_5.lib;%(AdditionalDependencies)`

There is a `config.ini` file where certain variables can be modified and customized:
 - Resolution
 - Gravity
 - Network Type: (1 = Client-To-Server, 2 = Peer-to-Peer)

As of Homework 2, this implementation uses a multiproject setup within the solution. Right click 
the solution in the solution explorer, then add the `NetworkServer` project to add to the solution.
The `GameEngine` project acts as the Client to the server, so it can be executed multiple times to create 
multiple clients.

Right click on the `GameEngine -> Debug -> Start New Instance` in order to create another 
client.


## Controls

To exit the engine, press the `ESCAPE` key.

To control the player, use the `ARROW KEYS` for movement, use `UP ARROW` for jump.

To activate proportional resolution, use the `MINUS KEY` (Base resolution is 1280x720)

The timeline's timescale can be modified by using the `8 KEY`, `9 KEY`, `0 KEY`:

|    Key    | Timescales |
| --------- | ---------- |
|  `8 KEY`  |   `0.5x`   |
|  `9 KEY`  |   `1.0x`   |
|  `0 KEY`  |   `2.0x`   |

The player can also be paused and unpaused with the `P KEY`

Press `E` for Action Button (if applicable to current game)

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

## Homework 2

### [CSC 481/581 - Time Lecture Slides](https://docs.google.com/presentation/d/1EZ9PFmYsufonouzSsVSzEfTW6dvkegJ7/edit#slide=id.p67)

In the first homework, `timeline.cpp` was limited to only being used to calculate physics changes.
As a result, it was only used in fixed timesteps. This reference is used to make the `Timeline` 
implementation more fleshed out, allowing for more control over the game engine's timeflow.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`timeline.cpp`](./timeline.cpp) | [`timeline.h`](./timeline.h) |
| [`main.cpp`](./main.cpp) | [`definitions.h`](./definitions.h) |

### [ZeroMQ Guide Tutorials](https://zguide.zeromq.org/docs)

The most recent `cppzmq` library was used in order to create the network code for this implementation.
We are currently using the following pages of the documentation for reference:

- [ZeroMQ Guide - Chapter 1](https://zguide.zeromq.org/docs/chapter1/)
- [ZeroMQ Guide - Chapter 4](https://zguide.zeromq.org/docs/chapter4/)
- [ZeroMQ Guide - Chapter 5](https://zguide.zeromq.org/docs/chapter5/)

These were used to gain a basic understanding on what ZMQ is supposed to accomplish.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`networkServer.cpp`]() | [`networkServer.h`]() |
| [`NetworkClient.cpp`]() | [`NetworkClient.h`]() |

### [ZeroMQ Documentation](https://zeromq.org/socket-api/?language=cpp&library=cppzmq#publish-subscribe-pattern)

This documentation served as our primary reference for developing a PUB-SUB messaging method 
for our Section 2 client and server implementation. The basic example provided was used for 
familiarizing ourself with the code, then was adapted to fit the implementation for Section 2.
This changed the implementation drastically, so not much of the example implementation remains.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`networkServer.cpp`]() | [`networkServer.h`]() |
| [`NetworkClient.cpp`]() | [`NetworkClient.h`]() |

### [CSC 481/581 `ThreadExample.cpp`](https://moodle-courses2425.wolfware.ncsu.edu/pluginfile.php/499841/mod_resource/content/4/ThreadExample.cpp)

The threading example provided on Moodle was used as a reference for our implementations of our thread. Mostly 
for observing the interactions of the mutex lock and conditional variable and how they interact with the threads 
to properly block and determine the order of what has to be accomplished.

We ended up doing a very simplistic threading example in main that splits Entities and Players into separate calculations.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`main.cpp`](./main.cpp) |  |


## Homework 3


### [*Game Engine Architecture* by Jason Gregory](https://www.gameenginebook.com/)

The textbook was used, not necessarily for code snippets, but for information regarding potential designs 
for a component-based system. The book provides several different possible implementations, but we decided 
on a generic component-based system. Some of our component definitions also took inspiration from the textbook as 
well, both with their naming schemes and their contents ([`Transform`](transform.cpp), [`RigidBody`](rigidBody.h)).

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`GameObject.cpp`](GameObject.cpp) | [`GameObject.h`](GameObject.h) |
| [`transform.cpp`](transform.cpp) | [`transform.h`](transform.h) |
| [`rigidBody.cpp`](rigidBody.cpp) | [`rigidBody.h`](rigidBody.h) |
| [`textureMesh.cpp`](textureMesh.cpp) | [`textureMesh.h`](textureMesh.h) |


### [Nlohmann's JSON Library Documentation](https://github.com/nlohmann/json)

This `JSON` library was provided on Moodle and is usable for Homework 3. This library is used primarily 
for serialization and deserialization of the GameObjects when being sent through the network, from both 
the server and the peer clients. The documentation provided on the GitHub repository was used to figure 
out the necessary functions and techniques to properly read in and write out json statements.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
| [`GameObject.cpp`](GameObject.cpp) | [`GameObject.h`](GameObject.h) |
| [`peerToPeer.cpp`](peerToPeer.cpp) | [`peerToPeer.h`](peerToPeer.h) |


## Homework 4

### [*Game Engine Architecture* by Jason Gregory](https://www.gameenginebook.com/)

The textbook was used as a reference to discern a general design for our `Event` system implementation. However, 
our implementation actually works quite differently from the textbook's implementation, since we ended up
implementing `Event`s as a class with defined functionalities rather than a struct that has its functionality
defined through `EventHandler` or `EventManager`. Despite that though, it was an extremely useful resource that 
helped us to understand the basics of how an event system should operate.

|        `.cpp` Files        |            Header Files            |
| -------------------------- | ---------------------------------- |
|                            | [`event.h`](event.h) |
| [`eventManager.cpp`](eventManager.cpp) | [`eventManager.h`](eventManager.h) |
