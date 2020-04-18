#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <stdbool.h>

typedef struct
{
	char key[SDL_NUM_SCANCODES];
	SDL_Scancode keysym;
	char lastCharInput;
	bool newLastCharInput;
	bool newLastKeyDown;
	char quit;
	bool isKeyDown;
	char text[32];

} Input;

typedef struct
{
    float x;
    float y;
    float xrel;
    float yrel;

}Position;

typedef struct
{
    char mouse[8];
    bool isMouseClicked;
    bool isMouseUp;
    bool isMouseMotion;
    Position positionMouse;

} InputMouse;




#endif // INPUT_H
