#include "Libelle.h"
#include "Menu.h"
#include "Jeu.h"

Libelle::Libelle(std::string texte,Menu* menu,SDL_Color color,int taille, bool bold) : Widget(menu)
{
    image = nullptr;
    this->taille = taille;
    this->color = color;
    this->bold = bold;
    setTexte(texte);
}

void Libelle::setTexte(std::string texte)
{
    if(image == nullptr) SDL_DestroyTexture(image);
    this->texte = texte;
    TTF_Font* font = (taille == -1 ? menu->getPolice() : TTF_OpenFont("../Fonts/lucon.ttf",taille));
    if(bold) TTF_SetFontStyle(font,TTF_STYLE_BOLD);
    SDL_Surface* imageTmp = TTF_RenderText_Blended(font,texte.c_str(),color);
    image = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    TTF_CloseFont(font);
    SDL_QueryTexture(image,nullptr,nullptr,&position.w,&position.h);

}


void Libelle::setPosition(int x,int y)
{
  position.x = x;
  position.y = y;
}

SDL_Rect Libelle::getPosition()
{
    return position;
}

Libelle::~Libelle()
{
    SDL_DestroyTexture(image);
}

SDL_Texture* Libelle::getImage()
{
    return image;
}

SDL_Rect Libelle::getTaille()
{
    SDL_Rect taille = {position.w,position.h,0,0};
    return taille;
}
