#include "golGrid.hpp"

#include <vector>
#include <iostream>

#include <SDL2/SDL_render.h>

#include "input.hpp"

GolGrid::GolGrid()
{
    paused = false;
}

void GolGrid::createGrid(unsigned int w, unsigned int h)
{
    width = w;
    height = h;

    gridOne.resize(width * height);
    gridTwo.resize(width * height);

    swapState = false;

    newGridBegin = gridOne.begin();
    newGridEnd = gridOne.end();
    oldGridBegin = gridTwo.begin();
    oldGridEnd = gridTwo.end();
}

void GolGrid::initializeTexture(SDL_Renderer *renderer, unsigned int displayWidth, unsigned int displayHeight)
{
    gridTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    calculateScaling(displayWidth, displayHeight);
}
void GolGrid::destroyTexture()
{
    SDL_DestroyTexture(gridTexture);
}

void GolGrid::calculateScaling(unsigned int displayWidth, unsigned int displayHeight)
{
    double displayAspectRatio = static_cast<double>(displayWidth) / static_cast<double>(displayHeight);
    double gridAspectRatio = static_cast<double>(width) / static_cast<double>(height);

    if (displayAspectRatio <= gridAspectRatio)
    {
        targetRect.w = displayWidth;
        targetRect.h = displayWidth / gridAspectRatio;
        targetRect.x = 0;
        targetRect.y = (displayHeight - targetRect.h) / 2;
    }
    else
    {
        targetRect.w = displayHeight * gridAspectRatio;
        targetRect.h = displayHeight;
        targetRect.x = (displayWidth - targetRect.w) / 2;
        targetRect.y = 0;
    }
}

void GolGrid::randomizeGrid()
{
    std::srand(time(NULL));

    for (auto newGridIterator = newGridBegin; newGridIterator != newGridEnd; newGridIterator++)
    {
        *newGridIterator = static_cast<bool>(std::rand() % 2);
    }
}

void GolGrid::clearGrid(bool state)
{
    for (auto newGridIterator = newGridBegin; newGridIterator != newGridEnd; newGridIterator++)
    {
        *newGridIterator = state;
    }
}

void GolGrid::clearFrame(bool state)
{
    auto newGridIterator = newGridBegin;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (y < 1 or y > height - 2 or x < 1 or x > width - 2)
                *newGridIterator = state;
            
            newGridIterator++;
        }
    }
}

void GolGrid::setInput(Input *inputPtr)
{
    input = inputPtr;
}

void GolGrid::leftClick(SDL_Point *mousePosition)
{
    if (SDL_PointInRect(mousePosition, &targetRect))
    {
        double squareSize = static_cast<double>(targetRect.w) / static_cast<double>(width);
        change(floor((mousePosition->x - targetRect.x) / squareSize), floor((mousePosition->y - targetRect.y) / squareSize));
    }
}

void GolGrid::change(unsigned int x, unsigned int y)
{
    if (swapState)
    {
        gridTwo[x + y * width] = !gridTwo[x + y * width];
    }
    else
    {
        gridOne[x + y * width] = !gridOne[x + y * width];
    }
}

void GolGrid::togglePaused()
{
    paused = !paused;
}

void GolGrid::tick()
{
    if (not paused)
    {
        swapState = !swapState;
        updateIterators(swapState);

        auto newGridIterator = newGridBegin;
        auto oldGridIterator = oldGridBegin;

        for (unsigned int y = 0; y < height; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {
                *newGridIterator = rule(x, y, *oldGridIterator);

                newGridIterator++;
                oldGridIterator++;
            }
        }
    }
}

void GolGrid::draw(SDL_Renderer *renderer, SDL_Point *center)
{
    SDL_SetRenderTarget(renderer, gridTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    auto newGridIterator = newGridBegin;

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            if (*newGridIterator == true)
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }
            newGridIterator++;
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyEx(renderer, gridTexture, NULL, &targetRect, 0, center, SDL_FLIP_NONE);
}

void GolGrid::updateIterators(bool newSwapState)
{
    if (newSwapState)
    {
        newGridBegin = gridTwo.begin();
        newGridEnd = gridTwo.end();
        oldGridBegin = gridOne.begin();
        oldGridEnd = gridOne.end();
    }
    else
    {
        newGridBegin = gridOne.begin();
        newGridEnd = gridOne.end();
        oldGridBegin = gridTwo.begin();
        oldGridEnd = gridTwo.end();
    }
}

bool GolGrid::rule(unsigned int x, unsigned int y, bool oldValue)
{
    if (not(x < 1 or x > width - 2))
        x_neighbours = {x - 1, x, x + 1};
    else
    {
        if (x < 1)
            x_neighbours = {width - 1, x, x + 1};
        else
            x_neighbours = {x - 1, x, 0};
    }
    if (not(y < 1 or y > height - 2))
        y_neighbours = {y - 1, y, y + 1};
    else
    {
        if (y < 1)
            y_neighbours = {height - 1, y, y + 1};
        else
            y_neighbours = {y - 1, y, 0};
    }

    alive = 0;
    for (auto i : x_neighbours)
    {
        for (auto j : y_neighbours)
        {
            if (x != i or y != j)
            {
                if (swapState)
                {
                    if (gridOne[i + j * width] == true)
                    {
                        alive += 1;
                    }
                }
                else
                {
                    if (gridTwo[i + j * width] == true)
                    {
                        alive += 1;
                    }
                }
            }
        }
    }

    return (alive == 3 or ((oldValue == true) and alive == 2));
}
