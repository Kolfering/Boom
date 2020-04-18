#include "BoutonRetour.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL_ttf.h>
#include "CheckBox.h"
#include "TextBox.h"
#include "VolumeBar.h"
#include "MenuTemplate.h"
#include "MenuJeu.h"
#include "Menu.h"
#include "MenuPrincipal.h"
#include "MenuOption.h"
#include "MenuFin.h"
#include "Jeu.h"
#include "Enumeration.h"

BoutonRetour::BoutonRetour(Menu* menu,IDMenu IDMenuRetour) : Bouton(menu)
{
    this->IDMenuRetour = IDMenuRetour;
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Back",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Back",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonRetour::onClic()
{
   if(!menu->getTransition())
   {
       SDL_ShowCursor(SDL_DISABLE);
       switch(IDMenuRetour)
       {
           case IDMenu::MENU_PRINCIPAL :
               {
                if(dynamic_cast<MenuOption*>(menu)!=nullptr)
                {
                    std::ofstream ofile("preferences.boom");
                    boost::archive::text_oarchive oBinaryArchive(ofile);
                    MenuTemplate* m = static_cast<MenuTemplate*>(menu);
                    CheckBox* box = static_cast<CheckBox*>(m->getListeWidget()["checkBox1"]);
                    VolumeBar* volumeBar = static_cast<VolumeBar*>(m->getListeWidget()["volumeBarMusic"]);
                    VolumeBar* volumeBar2 = static_cast<VolumeBar*>(m->getListeWidget()["volumeBarSound"]);
                    PlayerKeyboard playerKeyboard1 = {
                    static_cast<TextBox*>(m->getListeWidget()["textBox61"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox62"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox63"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox64"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox65"])->getValue(),
                    static_cast<CheckBox*>(m->getListeWidget()["checkBoxPad1"])->getValue()
                    };
                    PlayerKeyboard playerKeyboard2 = {
                    static_cast<TextBox*>(m->getListeWidget()["textBox71"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox72"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox73"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox74"])->getValue(),
                    static_cast<TextBox*>(m->getListeWidget()["textBox75"])->getValue(),
                    static_cast<CheckBox*>(m->getListeWidget()["checkBoxPad2"])->getValue()
                    };
                    oBinaryArchive << box->getValue();
                    oBinaryArchive << (int)volumeBar->getValue();
                    oBinaryArchive << (int)volumeBar2->getValue();
                    oBinaryArchive << playerKeyboard1;
                    oBinaryArchive << playerKeyboard2;
                    ofile.close();
                }
               }
                menu->setTransition(new Transition(true,500,new MenuPrincipal(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
            break;
           case IDMenu::MENU_JEU :
                menu->setTransition(new Transition(true,500,new MenuJeu(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
            break;
            case IDMenu::MENU_OPTION :
                menu->setTransition(new Transition(true,500,new MenuOption(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
            case IDMenu::MENU_FIN :
                menu->setTransition(new Transition(true,500,new MenuFin(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
            break;
            case IDMenu::MENU_MULTIPLAYER :
                SDL_ShowCursor(SDL_ENABLE);
                Jeu::setMenu(menu->getMenuParent());
            break;
            case IDMenu::MENU_GATHERNETWORK : {
                SDL_ShowCursor(SDL_ENABLE);
                Jeu::setMenu(menu->getMenuParent());
                std::vector<Menu*>::iterator position = std::find(menu->getMenuParent()->getListeSousMenu().begin(),menu->getMenuParent()->getListeSousMenu().end(),menu);
                if(position != menu->getMenuParent()->getListeSousMenu().end())
                   menu->getMenuParent()->getListeSousMenu().erase(position);
                delete menu;

            break;
            }
            case IDMenu::MENU_JOINNETWORK :
                SDL_ShowCursor(SDL_ENABLE);
                Jeu::setMenu(menu->getMenuParent());
                std::vector<Menu*>::iterator position = std::find(menu->getMenuParent()->getListeSousMenu().begin(),menu->getMenuParent()->getListeSousMenu().end(),menu);
                if(position != menu->getMenuParent()->getListeSousMenu().end())
                   menu->getMenuParent()->getListeSousMenu().erase(position);
                delete menu;

            break;

       }
   }
}

BoutonRetour::~BoutonRetour()
{

}
