#ifndef _GOLGRID_
#define _GOLGRID_

#include <vector>

#include <SDL2/SDL_render.h>

#include "input.hpp"

class GolGrid
{
public:
    GolGrid();

    void createGrid(unsigned int w, unsigned int h);

    void initializeTexture(SDL_Renderer *renderer, unsigned int displayWidth, unsigned int displayHeight);
    void destroyTexture();

    void calculateScaling(unsigned int displayWidth, unsigned int displayHeight);

    void randomizeGrid();

    void clearGrid(bool state = false);

    void clearFrame(bool state = false);

    void setInput(Input *inputPtr);

    void leftClick(SDL_Point *mousePosition);

    void change(unsigned int x, unsigned int y);

    void togglePaused();

    void tick();

    void draw(SDL_Renderer *renderer, SDL_Point *center);

private:
    bool swapState; // false if gridOne is newest, true if gridTwo is newest

    bool paused;

    unsigned int width, height;

    std::vector<bool> gridOne, gridTwo;

    std::vector<bool>::iterator newGridBegin, newGridEnd, oldGridBegin, oldGridEnd;

    std::array<unsigned int, 3> x_neighbours, y_neighbours;

    unsigned char alive;

    Input *input;

    SDL_Texture *gridTexture;
    SDL_Rect targetRect;

    void updateIterators(bool newSwapState);

    bool rule(unsigned int x, unsigned int y, bool oldValue);
};

#endif
