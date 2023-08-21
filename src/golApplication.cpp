#include "golApplication.hpp"

#include <chrono>
#include <iostream>

#include <SDL2/SDL.h>

#include "vec2.hpp"
#include "input.hpp"
#include "golGrid.hpp"

std::chrono::_V2::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

GolApplication::GolApplication()
{
    initializeSdl();
    initializeInput();
    initializeGrid();
}

void GolApplication::run()
{
    std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<double, std::chrono::_V2::steady_clock::period>> start;

        std::chrono::duration<double> delta;

        int counter = 0;

        running = true;
        draw();
        while (running)
        {
            start = now();

            handleEvents();

            tick();

            if (toChange)
            {
                makeChanges();
                toChange = false;
            }

            draw();

            delta = now() - start;
            
            if (counter % 60 == 0)
                std::cout << delta.count() << "\n";

            counter += 1;
        }

        destroySdl();
}

void GolApplication::initializeSdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w / 2;
    displayHeight = displayMode.h / 2;
    center.x = displayWidth / 2;
    center.y = displayHeight / 2;

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow(
        "game of life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlags);

    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    keyboardState = SDL_GetKeyboardState(NULL);
}
void GolApplication::destroySdl()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    grid.destroyTexture();
    SDL_Quit();
}

void GolApplication::initializeInput()
{
    input.setKeyboardState(keyboardState);
}

void GolApplication::initializeGrid()
{
    grid.createGrid(192, 120);
    grid.initializeTexture(renderer, displayWidth, displayHeight);
    grid.randomizeGrid();
}

void GolApplication::handleEvents()
{
    SDL_PumpEvents();

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                displayWidth = event.window.data1;
                displayHeight = event.window.data2;

                grid.calculateScaling(displayWidth, displayHeight);
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;
            case SDL_SCANCODE_SPACE:
                grid.togglePaused();
                break;
            case SDL_SCANCODE_BACKSPACE:
                grid.clearGrid();
                break;
            case SDL_SCANCODE_RETURN:
                grid.clearGrid(true);
                grid.clearFrame();
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                toChange = true;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    input.updateInputs();
}

void GolApplication::tick()
{
    grid.tick();
}

void GolApplication::makeChanges()
{
    grid.leftClick(&mousePosition);
}

void GolApplication::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 15, 47, 255);
    SDL_RenderClear(renderer);

    grid.draw(renderer, &center);

    SDL_RenderPresent(renderer);
}
