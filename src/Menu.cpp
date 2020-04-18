#include "Menu.h"
#include "Jeu.h"

Menu::Menu(Input* input,InputMouse* inputMouse,SDL_Window* screen)
{
    this->input = input;
    this->screen = screen;
    this->inputMouse = inputMouse;
    this->transition = nullptr;
    transitionFinie = false;
}

Menu::Menu(Input* input,InputMouse* inputMouse,SDL_Window* screen,Menu* menuParent) : Menu(input,inputMouse,screen)
{
   this->menuParent = menuParent;
   menuParent->addSousMenu(this);
}

TTF_Font* Menu::getPolice()
{
    return police;
}

void Menu::handleEvents()
{

}

Menu* Menu::getMenuParent()
{
    return menuParent;
}

std::vector<Menu*> &Menu::getListeSousMenu()
{
    return listeSousMenu;
}

void Menu::afficherTransition()
{
    if(transition)
    {
        if(!transition->isFinie())
            transition->execute();

        int w,h;
        SDL_QueryTexture(transition->getImage(),nullptr,nullptr,&w,&h);
        SDL_Rect pos = {0,0,w,h};
        SDL_RenderCopy(Jeu::getRenderer(),transition->getImage(),nullptr,&pos);

        if(transition->isFinie())
        {
            transitionFinie = true;
            if(!transition->isIn())
            {
                delete transition;
                transition = nullptr;

                if(!SDL_ShowCursor(SDL_QUERY))
                    SDL_ShowCursor(SDL_ENABLE);

            }
            else
            {
                if(transition->getMenu())
                {
                    Menu* menu = transition->getMenu();
                    delete transition;
                    transition = nullptr;
                    Jeu::setMenu(menu);
                }
            }

        }


    }
}


Menu::~Menu()
{
    if(transition)
    {
        delete transition;
        transition = nullptr;
    }

    for(unsigned int i = 0;i<listeSousMenu.size();i++)
    {
        delete listeSousMenu.at(i);
        listeSousMenu.erase(listeSousMenu.begin() + i);
        i--;
    }

}

void Menu::addSousMenu(Menu* sousMenu)
{
    listeSousMenu.push_back(sousMenu);
}

Transition* Menu::getTransition()
{
    return transition;
}

void Menu::setTransition(Transition* trans)
{
    if(!transition)
        transition = trans;
    else
        delete trans;
}

IDMenu Menu::getIDMenu()
{
    return idMenu;
}

Input* Menu::getInput()
{
    return input;
}

InputMouse* Menu::getInputMouse()
{
    return inputMouse;
}

SDL_Window* Menu::getScreen()
{
    return screen;
}

