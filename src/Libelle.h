#ifndef LIBELLE_H
#define LIBELLE_H
#include <SDL.h>
#include "Menu.h"
#include "Widget.h"
#include <string>


class Libelle : public Widget{
private:
    SDL_Texture* image;
    std::string texte;
    SDL_Color color;
    int taille;
    bool bold;
public:
    Libelle(std::string,Menu*,SDL_Color,int = -1, bool = false);
    ~Libelle();
    void setPosition(int,int);
    SDL_Rect getPosition();
    SDL_Texture* getImage();
    SDL_Rect getTaille();
    void setTexte(std::string texte);

};

#endif // LIBELLE_H
