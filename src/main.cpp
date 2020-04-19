#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include <cstdio>
#include "Jeu.h"
#include "Utile.h"
#include "Enumeration.h"


int main(int argc, char* argv[])
{

    #if DEBUG
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING,"1");
    #endif // DEBUG

    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }


    atexit(SDL_Quit);
    SDL_ShowCursor(SDL_DISABLE);


    SDL_Window* screen = SDL_CreateWindow("Boom",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Surface* image = SDL_LoadBMP("../Images/player1_icon2.bmp");
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 255));
    SDL_SetWindowIcon(screen,image);
    SDL_Renderer* renderer = SDL_CreateRenderer(screen,-1,SDL_RENDERER_TARGETTEXTURE);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    if ( !screen )
    {
        printf("Unable to set %dx%d video: %s\n", WINDOW_WIDTH,WINDOW_HEIGHT,SDL_GetError());
        return EXIT_FAILURE;
    }

    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);
    Mix_AllocateChannels(15);
    Input input;
    InputMouse inputMouse;
    memset(&input,0,sizeof(input));
    memset(&inputMouse,0,sizeof(inputMouse));
    Jeu* jeu = new Jeu(screen,renderer,&input,&inputMouse);
    jeu->start();
    delete jeu;
    TTF_Quit();
    Mix_CloseAudio();
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
    return EXIT_SUCCESS;
}
