#include "TextBox.h"
#include "Utile.h"
#include <map>
#include "MenuTemplate.h"

TextBox::TextBox(Menu* menu,SDL_Scancode val) : Widget(menu)
{
    image[0] = chargerSurface("../Images/textbox.png");
    image[1] = chargerSurface("../Images/textbox2.png");
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    clicked = false;
    value = val;
    std::string lib = SDL_GetKeyName(SDL_GetKeyFromScancode(value));
    lib = lib.length() > 9 ? lib.substr(0,9) : lib;
    libelle = new Libelle(lib,menu,{0,0,0},12);
}

TextBox::~TextBox()
{
    SDL_DestroyTexture(image[0]);
    SDL_DestroyTexture(image[1]);
}

void TextBox::setClicked(bool clic)
{
    clicked = clic;
}

void TextBox::onClic()
{
    clicked = true;
    menu->getInput()->keysym = SDL_SCANCODE_UNKNOWN;
    MenuTemplate* m = static_cast<MenuTemplate*>(menu);

    for(std::map<std::string,Widget*>::iterator it = m->getListeWidget().begin();it != m->getListeWidget().end(); ++it)
    {
        if(TextBox* txtBox = dynamic_cast<TextBox*>(it->second))
        {
            if(txtBox != this && txtBox->isClicked())
                txtBox->setClicked(false);
        }
    }

}

SDL_Scancode TextBox::getValue()
{
    return value;
}


void TextBox::setValue(SDL_Scancode code)
{
    this->value = code;
    std::string lib = SDL_GetKeyName(SDL_GetKeyFromScancode(value));
    lib = lib.length() > 9 ? lib.substr(0,9) : lib;
    libelle->setTexte(lib);
    libelle->setPosition(position.x + (position.w/2) - (libelle->getTaille().x/2),position.y + (position.h/2) - (libelle->getTaille().y/2));
}

SDL_Texture* TextBox::getImage()
{
    if(!clicked)
        return image[0];
    return image[1];
}

void TextBox::setPosition(int x,int y)
{
    position.x = x;
    position.y = y;

    libelle->setPosition(position.x + (position.w/2) - (libelle->getTaille().x/2),position.y + (position.h/2) - (libelle->getTaille().y/2));
}

bool TextBox::isClicked()
{
    return clicked;
}

Libelle* TextBox::getLibelle()
{
    return libelle;
}
