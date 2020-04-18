#include "Bouton.h"
#include "Menu.h"
#include "Widget.h"

Bouton::Bouton(Menu* menu) : Widget(menu)
{
    indiceImage = 0;
}

Bouton::~Bouton()
{
    SDL_DestroyTexture(image[0]);
    SDL_DestroyTexture(image[1]);
}

void Bouton::onFocus()
{
    focus = true;
    indiceImage = 1;
}

void Bouton::onDeFocus()
{
    focus = false;
    indiceImage = 0;
}

SDL_Texture* Bouton::getImage()
{
    return image[indiceImage];
}

