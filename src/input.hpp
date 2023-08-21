#ifndef _GOLINPUT_
#define _GOLINPUT_

#include <array>

#include <SDL2/SDL_scancode.h>

class Input
{
public:
    Input();

    void setKeyboardState(const uint8_t *newKeyboardState);

    void updateInputs();

    bool pausePressed();
    bool randomizePressed();

private:
    static const int inputArraySize = 2;
    std::array<bool, inputArraySize> inputArray;
    const uint8_t *keyboardState;
};

#endif
