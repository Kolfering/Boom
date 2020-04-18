#include "Jeu.h"
#include "MenuPrincipal.h"
#include "Utile.h"
#include "InputJoystick.h"
#include <algorithm>

Menu* Jeu::menu;
std::map<SDL_JoystickID, JoystickKeys*> Jeu::joystickMap;
SDL_Rect Jeu::positionSouris = {0,0,0,0};
SDL_Renderer* Jeu::renderer;
unsigned int Jeu::volumeMusic = 64;
unsigned int Jeu::volumeSound = 64;
Jeu::Jeu(SDL_Window* screen,SDL_Renderer* renderer,Input* input,InputMouse* inputMouse)
{
    this->screen = screen;
    this->renderer = renderer;
    this->input = input;
    this->inputMouse = inputMouse;
    menu = new MenuPrincipal(input,inputMouse,screen);
}

Jeu::~Jeu()
{
    delete menu;
}

void Jeu::initJoystick(SDL_JoystickID id)
{
    if(joystickMap[id] == nullptr) joystickMap[id] = new JoystickKeys();
}

void Jeu::cleanJoystick()
{
    for(std::map<SDL_JoystickID,JoystickKeys*>::iterator it = joystickMap.begin();it != joystickMap.end(); it++)
        delete it->second;
    joystickMap.clear();
}

void Jeu::setVolumeMusic(float vlm)
{
    volumeMusic = vlm;
    Mix_VolumeMusic(volumeMusic);
}

void Jeu::setVolumeSound(float vlm)
{
    volumeSound = vlm;
    Mix_Volume(-1,volumeSound);
}

SDL_Rect Jeu::getPositionSouris()
{
    return positionSouris;
}

void Jeu::setMenu(Menu* m)
{
    if(!(std::find(m->getListeSousMenu().begin(), m->getListeSousMenu().end(), menu) != m->getListeSousMenu().end())
    && !(std::find(menu->getListeSousMenu().begin(), menu->getListeSousMenu().end(), m) != menu->getListeSousMenu().end()))
        delete menu;
    menu = m;
}

Menu* Jeu::getMenu()
{
    return menu;
}

SDL_Renderer* Jeu::getRenderer()
{
    return renderer;
}

std::map<SDL_JoystickID, JoystickKeys*>& Jeu::getJoystick()
{
    return joystickMap;
}


void Jeu::start()
{
  //  SDL_StartTextInput();
    while (!input->quit)
    {
        UpdateEvents();
        calculCPS();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        menu->handleEvents();
        menu->executeMenu();
        menu->afficheMenu();
        SDL_RenderPresent(renderer);
    }
   // SDL_StopTextInput();

}

void Jeu::calculCPS()
{
    timeAfterCPS = SDL_GetTicks();

    if(timeAfterCPS-timeBeforeCPS<CPS_RATE)
        SDL_Delay(CPS_RATE - (timeAfterCPS-timeBeforeCPS));

    timeBeforeCPS = timeAfterCPS;

}


void Jeu::UpdateEvents()
{
    SDL_Event eventActu;
	while(SDL_PollEvent(&eventActu))
	{
		switch (eventActu.type)
		{
case SDL_JOYAXISMOTION:
    {
        JoystickKeys* joystick = joystickMap[eventActu.jaxis.which];

        if(joystick != nullptr)
        {

            if ((eventActu.jaxis.value < -3200 ) || (eventActu.jaxis.value > 3200 ))
            {
                if(eventActu.jaxis.axis == 1)
                {
                    if(eventActu.jaxis.value > 0)
                    {
                        joystick->down = true;
                        joystick->up = false;
                        joystick->left = false;
                        joystick->right = false;
                    }
                    else
                    {
                        joystick->up = true;
                        joystick->left = false;
                        joystick->right = false;
                        joystick->down = false;
                    }
                }
                else
                {
                    if(eventActu.jaxis.value > 0)
                    {
                        joystick->down = false;
                        joystick->up = false;
                        joystick->left = false;
                        joystick->right = true;
                    }
                    else
                    {
                        joystick->up = false;
                        joystick->left = true;
                        joystick->right = false;
                        joystick->down = false;
                    }
                }
            }
            else
            {
                if(eventActu.jaxis.axis == 1)
                {
                    joystick->down = false;
                    joystick->up = false;
                }
                else
                {
                    joystick->left = false;
                    joystick->right = false;
                }
            }
        }
    }
    break;
    case SDL_JOYBUTTONDOWN:
        {
        JoystickKeys* joystick = joystickMap[eventActu.jbutton.which];
        joystick->bomb = true;
        break;
        }
        case SDL_JOYBUTTONUP:
        {
        JoystickKeys* joystick = joystickMap[eventActu.jbutton.which];
        joystick->bomb = false;
        break;
        }

        case SDL_TEXTINPUT:
            input->lastCharInput = eventActu.text.text[0];
            input->newLastCharInput = true;
            strcpy(input->text,eventActu.text.text);
            break;
		case SDL_KEYDOWN:
			input->key[eventActu.key.keysym.scancode]=1;
			input->keysym = eventActu.key.keysym.scancode;
			input->isKeyDown = true;
			input->newLastKeyDown = true;
			switch(eventActu.key.keysym.scancode)
			{
            case SDL_SCANCODE_UP:
                input->key[SDL_SCANCODE_DOWN] = 0;
                input->key[SDL_SCANCODE_LEFT] = 0;
                input->key[SDL_SCANCODE_RIGHT] = 0;
                break;
            case SDL_SCANCODE_DOWN:
                input->key[SDL_SCANCODE_UP] = 0;
                input->key[SDL_SCANCODE_LEFT] = 0;
                input->key[SDL_SCANCODE_RIGHT] = 0;
                break;
            case SDL_SCANCODE_LEFT:
                input->key[SDL_SCANCODE_DOWN] = 0;
                input->key[SDL_SCANCODE_UP] = 0;
                input->key[SDL_SCANCODE_RIGHT] = 0;
                break;
            case SDL_SCANCODE_RIGHT:
                input->key[SDL_SCANCODE_DOWN] = 0;
                input->key[SDL_SCANCODE_LEFT] = 0;
                input->key[SDL_SCANCODE_UP] = 0;
                break;
            case SDL_SCANCODE_RETURN:

                if(input->key[SDL_SCANCODE_LALT])
                {
                    toggleScreenMode(screen);
                    input->key[SDL_SCANCODE_LALT] = 0;

                }
                break;

			}
			break;
        case SDL_KEYUP:
			input->key[eventActu.key.keysym.scancode]=0;
			input->isKeyDown = false;
            break;
		case SDL_QUIT:
			input->quit = 1;
			break;
        case SDL_MOUSEMOTION:
            positionSouris.x = eventActu.motion.x;
            positionSouris.y = eventActu.motion.y;
            inputMouse->isMouseMotion = true;
            inputMouse->positionMouse.x = positionSouris.x;
            inputMouse->positionMouse.y = positionSouris.y;
            inputMouse->positionMouse.xrel = eventActu.motion.xrel;
            inputMouse->positionMouse.yrel = eventActu.motion.yrel;
            break;
        case SDL_MOUSEBUTTONDOWN:
            inputMouse->isMouseClicked = true;
            inputMouse->isMouseUp = false;
            inputMouse->mouse[eventActu.button.button] = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            inputMouse->mouse[eventActu.button.button] = 0;
            inputMouse->isMouseClicked = false;
            inputMouse->isMouseUp = true;
            break;
		default:
			break;
		}
	}
}
