#include "input.hpp"

#include <array>

#include <SDL2/SDL_scancode.h>


Input::Input()
{
    for (unsigned int i = 0; i < inputArraySize; i++)
        inputArray[i] = false;
}

void Input::setKeyboardState(const uint8_t *newKeyboardState)
{
    keyboardState = newKeyboardState;
}

void Input::updateInputs()
{
    for (unsigned int i = 0; i < inputArraySize; i++)
        inputArray[i] = false;
    if (keyboardState[SDL_SCANCODE_SPACE])
        inputArray[0] = true;
    if (keyboardState[SDL_SCANCODE_R])
        inputArray[1] = true;
}

bool Input::pausePressed(){ return inputArray[0]; }
bool Input::randomizePressed() { return inputArray[1]; }
