#include "Explosion.h"
#include "Utile.h"
#include "Bomber.h"
#include "Bomb.h"
#include "Ennemi.h"

SDL_Texture* Explosion::image = nullptr;
unsigned int Explosion::counterID = 0;
Explosion::Explosion(Bomb* bombe,Case* casePositionExplosion,DirectionExplosion directionExplosion,float degat)
{
    counter = 0;
    counterID++;
    ID = counterID;
    this->degat = degat;
    this->bombe = bombe;
    explosionFinie = false;
    this->directionExplosion = directionExplosion;
    this->casePositionExplosion = casePositionExplosion;
    this->casePositionExplosion->setExplosion(this);
    timeBefore = getTime();
    timeTrigger = timeBefore;
    antiPosition.h = TAILLE_SPRITE_EXPLOSION;
    antiPosition.w = TAILLE_SPRITE_EXPLOSION;
    antiPosition.x = counter * TAILLE_SPRITE_EXPLOSION;
    antiPosition.y = directionExplosion * TAILLE_SPRITE_EXPLOSION;
}

void Explosion::initialiserExplosion()
{
    image = chargerSurface("../Images/explosion.png");
}

void Explosion::releaseExplosion()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
Explosion::~Explosion()
{
    if(casePositionExplosion->getExplosion() == this)
        casePositionExplosion->setExplosion(nullptr);
}

Bomb* Explosion::getBombe()
{
    return bombe;
}

float Explosion::getDegat()
{
    return degat;
}

void Explosion::detectionPersonnage()
{
    if(casePositionExplosion->getBomber())
        casePositionExplosion->getBomber()->prendreDegat(degat);
    if(casePositionExplosion->getEnnemi())
        casePositionExplosion->getEnnemi()->prendreDegat(bombe->getBomber(),ID);
    if(casePositionExplosion->getBombe() && (getTime() - timeTrigger > TIME_EXPLOSION_TRIGGER))
        casePositionExplosion->getBombe()->calculExplosion();
}

SDL_Rect* Explosion::getAntiPositionAdresse()
{
    return &antiPosition;
}


void Explosion::execute()
{
    detectionPersonnage();
    calculNextFrame();
    antiPosition.x = counter * TAILLE_SPRITE_EXPLOSION;
    antiPosition.y = directionExplosion * TAILLE_SPRITE_EXPLOSION;

}

bool Explosion::isExplosionFinie()
{
    return explosionFinie;
}

SDL_Texture* Explosion::getImage()
{
    return image;
}

Case* Explosion::getCase()
{
    return casePositionExplosion;
}

void Explosion::calculNextFrame()
{

    if(getTime()-timeBefore > TEMPS_PAR_SPRITE_EXPLOSION)
    {
        counter++;
        if(counter == EXPLOSION_FRAME)
            explosionFinie = true;


        timeBefore = getTime();
    }
}
