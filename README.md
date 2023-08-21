# sdl2-game-of-life
An implementation of conway's game of life made in c++ with the sdl2 graphical framework.

## Installation
Currently there are no prebuilt binaries available, but building is very simlple. To compile with the provided makefile on linux:
- You will need make, g++ and sdl2 development libraries in your include path.
- Run "make build" from the root of the repository.
- "make test" and "make clean" have been provided for convenience.
- "make clean" should be unnecessary as the dependency resolution and recompilation of binaries should be fully implemented in the makefile.
-
- I have not tried to compile this on windows but it should be very simple, as sdl2 is cross-platform.

### Project status
Game of life is fully implemented, and I'm currently working on making it more fun to interact with.

#### Controls
- Space toggles whether the simulation is paused.
- Backspace clears the grid.
- Return (enter) fills the grid with living cells, and erases the squares at the very edges. this produces surprisingly beautiful patterns.
- If you're wondering how to resize the grid, sorry for not implementing a way to do that through the program yet! It can be changed in src/golApplication.cpp->GolApplication::initializeGrid()
