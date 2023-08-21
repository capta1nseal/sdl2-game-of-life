#ifndef _GOLAPPLICATION_
#define _GOLAPPLICATION_

#include <chrono>

#include <SDL2/SDL.h>

#include "vec2.hpp"
#include "input.hpp"
#include "golGrid.hpp"

std::chrono::_V2::steady_clock::time_point now();

class GolApplication
{
public:
    GolApplication();

    void run();

private:
    bool running;
    unsigned int displayWidth, displayHeight;
    bool fullscreen;
    SDL_Point center;

    SDL_DisplayMode displayMode;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Event event;
    const uint8_t *keyboardState;

    SDL_Point mousePosition;
    Vec2 worldMousePosition;

    Input input;

    GolGrid grid;

    bool toChange;

    void initializeSdl();
    void destroySdl();

    void initializeInput();

    void initializeGrid();

    void handleEvents();

    void tick();

    void makeChanges();

    void draw();
};

#endif
