#include "MenuTemplate.h"
#include "Jeu.h"
#include "Utile.h"
#include <math.h>


MenuTemplate::MenuTemplate(Input* input,InputMouse* inputMouse,SDL_Window* screen) : Menu(input,inputMouse,screen)
{
    initialiseMenuTemplate();
    transition = new Transition(false,500);
}

MenuTemplate::MenuTemplate(Input* input,InputMouse* inputMouse,SDL_Window* screen,Menu* menuPar,bool transit) : Menu(input,inputMouse,screen,menuPar)
{
    initialiseMenuTemplate();
    if(transit) transition = new Transition(false,500);
}

void MenuTemplate::initialiseMenuTemplate()
{
    police = TTF_OpenFont("Fonts/lucon.ttf",30);
    int w,h;
    backGround = chargerSurface("Images/backGround2.png");
    SDL_QueryTexture(backGround,nullptr,nullptr,&w,&h);
    positionBackGround.x = 0;
    positionBackGround.y = 0;
    positionBackGround.w = w;
    positionBackGround.h = h;
}

void MenuTemplate::afficheBackGround()
{
    SDL_RenderCopy(Jeu::getRenderer(),backGround,nullptr,&positionBackGround);
    for(std::map<SDL_Texture*,SDL_Rect>::iterator it = listeTexture.begin();it != listeTexture.end(); ++it)
            SDL_RenderCopy(Jeu::getRenderer(),it->first,nullptr,&it->second);

    for(std::map<std::string,Widget*>::iterator it = listeWidget.begin();it != listeWidget.end(); ++it)
    {
        if(it->second->isDisplayable())
            SDL_RenderCopy(Jeu::getRenderer(),it->second->getImage(),nullptr,it->second->getAdressePosition());
    }

 /*   for(unsigned int i = 0;i<listeWindow.size();i++)
    {
        kiss_window_draw(&listeWindow.at(i),Jeu::getRenderer());
    }*/

    for(unsigned int i = 0;i<listeTextBox.size();i++)
    {
        kiss_textbox_draw(listeTextBox.at(i),Jeu::getRenderer());
    }

    for(unsigned int i = 0;i<listeVScrollbar.size();i++)
    {
        kiss_vscrollbar_draw(listeVScrollbar.at(i),Jeu::getRenderer());
    }

    for(unsigned int i = 0;i<listeKissButton.size();i++)
    {
        kiss_button_draw(listeKissButton.at(i)->getButton(),Jeu::getRenderer());
    }

    for(unsigned int i = 0;i<listeEntryBox.size();i++)
    {
        kiss_entry_draw(listeEntryBox.at(i)->getEntry(),Jeu::getRenderer());
    }

}

std::map<std::string,Widget*> &MenuTemplate::getListeWidget()
{
    return listeWidget;
}

std::map<SDL_Texture*,SDL_Rect> &MenuTemplate::getListeTexture()
{
    return listeTexture;
}

void MenuTemplate::gestionWidget()
{
    int w,h;
    std::map<std::string,Widget*> mapTmp = listeWidget;
    for(std::map<std::string,Widget*>::iterator it = mapTmp.begin();it != mapTmp.end(); ++it)
    {
        if(it->second->isDisplayable())
        {
            SDL_QueryTexture(it->second->getImage(),nullptr,nullptr,&w,&h);
            if(Jeu::getPositionSouris().x > it->second->getAdressePosition()->x && Jeu::getPositionSouris().x < it->second->getAdressePosition()->x + w
               && Jeu::getPositionSouris().y > it->second->getAdressePosition()->y && Jeu::getPositionSouris().y < it->second->getAdressePosition()->y + h
               && SDL_ShowCursor(SDL_QUERY))
                it->second->onFocus();
            else
                it->second->onDeFocus();

            if(it->second->isFocus() && inputMouse->mouse[SDL_BUTTON_LEFT])
            {
                inputMouse->mouse[SDL_BUTTON_LEFT] = 0;
                it->second->onClic();
                break;
            }
        }

    }


}
void MenuTemplate::executeMenu()
{
    gestionWidget();


    for(unsigned int i = 0;i<listeTextBox.size();i++)
    {
        kiss_textbox_event(listeTextBox.at(i),input,inputMouse);
    }

    for(unsigned int i = 0;i<listeVScrollbar.size();i++)
    {
        if(kiss_vscrollbar_event(listeVScrollbar.at(i),input,inputMouse) || listeVScrollbar.at(i)->fraction == 1)
        {
            if(listeVScrollbar.at(i)->textbox && listeVScrollbar.at(i)->textbox->array && listeVScrollbar.at(i)->textbox->array->length)
            {
                int firstline = (int) ((listeVScrollbar.at(i)->textbox->array->length -
                listeVScrollbar.at(i)->textbox->maxlines) * listeVScrollbar.at(i)->fraction + 0.5);
                if (firstline >= 0) listeVScrollbar.at(i)->textbox->firstline = firstline;
            }
        }
    }

    for(unsigned int i = 0;i<listeKissButton.size();i++)
    {
        if(kiss_button_event(listeKissButton.at(i)->getButton(),input,inputMouse))
            listeKissButton.at(i)->onClick();
    }

    for(unsigned int i = 0;i<listeEntryBox.size();i++)
    {
        if(inputMouse->isMouseClicked && !kiss_pointinrect(Jeu::getPositionSouris().x, Jeu::getPositionSouris().y,&listeEntryBox.at(i)->getEntry()->rect))
           listeEntryBox.at(i)->getEntry()->active = 0;
        listeEntryBox.at(i)->event(input,inputMouse);
    }

}

void MenuTemplate::afficheMenu()
{
    afficheBackGround();
    afficherTransition();
}

MenuTemplate::~MenuTemplate()
{
    for(std::map<std::string,Widget*>::iterator it = listeWidget.begin();it != listeWidget.end(); it++)
        delete it->second;

    for(std::map<SDL_Texture*,SDL_Rect>::iterator it = listeTexture.begin();it != listeTexture.end(); it++)
        SDL_DestroyTexture(it->first);

    for(unsigned int i = 0;i<listeKissButton.size();i++)
    {
        delete listeKissButton.at(i);
        listeKissButton.erase(listeKissButton.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeEntryBox.size();i++)
    {
        delete listeEntryBox.at(i);
        listeEntryBox.erase(listeEntryBox.begin() + i);
        i--;
    }

    listeWidget.erase(listeWidget.begin(),listeWidget.end());
    listeTexture.erase(listeTexture.begin(),listeTexture.end());
    SDL_DestroyTexture(backGround);
    TTF_CloseFont(police);

}
