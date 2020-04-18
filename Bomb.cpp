#include "Bomb.h"
#include "Utile.h"
#include "Enumeration.h"
#include "Carte.h"

using std::vector;
SDL_Texture* Bomb::image = nullptr;
Mix_Chunk* Bomb::sonExplosion = nullptr;
Bomb::Bomb(Bomber* bomber,Case* caseBomb)
{
    this->bomber = bomber;
    this->caseBomb = caseBomb;
    caseBomb->setBombe(this);
    counter = 0;
    timePose = getTime();
    timeBefore = 0;
    aExplose = false;
    antiPosition.h = TAILLE_SPRITE;
    antiPosition.w = TAILLE_SPRITE;
}

void Bomb::initialiserBombe()
{
    image = chargerSurface("Images/bomb.png");
    sonExplosion = Mix_LoadWAV("Sons/Explosion.wav");
    Mix_VolumeChunk(sonExplosion,MIX_MAX_VOLUME/2);
}

void Bomb::releaseBombe()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(sonExplosion)
        Mix_FreeChunk(sonExplosion);
    sonExplosion = nullptr;
}

SDL_Rect* Bomb::getAntiPositionAdresse()
{
    return &antiPosition;
}

void Bomb::execute()
{
    calculNextFrame();
    antiPosition.x = counter * TAILLE_SPRITE;
    antiPosition.y = 0;

}
void Bomb::calculNextFrame()
{
    if(!aExplose)
    {
        if(getTime() - timePose < bomber->getTempsExplosion()-2000)
        {
            if(getTime() - timeBefore > TEMPS_FRAME_NORMAL)
            {
                counter++;
                if(counter>=BOMB_SPRITE-1)
                    counter = 0;

                timeBefore = getTime();
            }
        }
        else if(getTime() - timePose < bomber->getTempsExplosion())
        {
            if(getTime() - timeBefore > TEMPS_FRAME_ATTENTION)
            {
                counter++;
                if(counter>=BOMB_SPRITE)
                    counter = 1;

                timeBefore = getTime();
            }
        }
        else
            calculExplosion();
    }
}

Bomber* Bomb::getBomber()
{
    return bomber;
}

void Bomb::calculExplosion()
{
    if(!aExplose)
    {
        Mix_PlayChannel(-1,sonExplosion,0);
        aExplose = true;
        listeExplosions.push_back(new Explosion(this,caseBomb,EXPLOSION_MILLIEU,COEF_DEGAT));
        Case* c;
        for(unsigned int i = 1;i<=bomber->getDistanceExplosion();i++)
        {
            c = Carte::getCase(caseBomb->getPositionDamier().y-i,caseBomb->getPositionDamier().x);

            if(c)
            {

                if(c->getType() == FOND)
                    listeExplosions.push_back(new Explosion(this,c,EXPLOSION_HAUT,COEF_DEGAT*i/2));


                else
                {
                    if(c->getType() == CASSABLE && i == 1)
                    {
                        listeExplosions.push_back(new Explosion(this,c,EXPLOSION_HAUT,COEF_DEGAT*i/2));
                        c->setExplose(bomber);
                    }
                    break;
                }

            }
            else
                break;


        }

        for(unsigned int i = 1;i<=bomber->getDistanceExplosion();i++)
        {
            c = Carte::getCase(caseBomb->getPositionDamier().y+i,caseBomb->getPositionDamier().x);

            if(c)
            {
                if(c->getType() == FOND)
                    listeExplosions.push_back(new Explosion(this,c,EXPLOSION_BAS,COEF_DEGAT*i/2));

                else
                {
                    if(c->getType() == CASSABLE && i == 1)
                    {
                        listeExplosions.push_back(new Explosion(this,c,EXPLOSION_BAS,COEF_DEGAT*i/2));
                        c->setExplose(bomber);
                    }
                    break;
                }

            }
            else
                break;


        }

        for(unsigned int i = 1;i<=bomber->getDistanceExplosion();i++)
        {
            c = Carte::getCase(caseBomb->getPositionDamier().y,caseBomb->getPositionDamier().x+i);

            if(c)
            {

                if(c->getType() == FOND)
                        listeExplosions.push_back(new Explosion(this,c,EXPLOSION_DROITE,COEF_DEGAT*i/2));

                else
                {
                    if(c->getType() == CASSABLE && i == 1)
                    {
                        listeExplosions.push_back(new Explosion(this,c,EXPLOSION_DROITE,COEF_DEGAT*i/2));
                        c->setExplose(bomber);
                    }
                     break;
                }

            }
            else
                break;


        }

        for(unsigned int i = 1;i<=bomber->getDistanceExplosion();i++)
        {
            c = Carte::getCase(caseBomb->getPositionDamier().y,caseBomb->getPositionDamier().x-i);

            if(c)
            {

                if(c->getType() == FOND)
                    listeExplosions.push_back(new Explosion(this,c,EXPLOSION_GAUCHE,COEF_DEGAT*i/2));

                else
                {
                    if(c->getType() == CASSABLE && i == 1)
                    {
                        listeExplosions.push_back(new Explosion(this,c,EXPLOSION_GAUCHE,COEF_DEGAT*i/2));
                        c->setExplose(bomber);

                    }
                     break;
                }

            }
            else
                break;


        }
    }

}



bool Bomb::isExplose()
{
    return aExplose;
}
vector<Explosion*> &Bomb::getListeExplosions()
{
    return listeExplosions;
}
SDL_Texture* Bomb::getImage()
{
    return image;
}
Case* Bomb::getCase()
{
    return caseBomb;
}
Bomb::~Bomb()
{
    for(unsigned int i = 0;i<listeExplosions.size();i++)
    {
        delete listeExplosions.at(i);
        listeExplosions.erase(listeExplosions.begin() + i);
        i--;
    }

    caseBomb->setBombe(nullptr);
}
