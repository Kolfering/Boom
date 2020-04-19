#include "Carte.h"
#include "EnnemiSoldier.h"
#include "EnnemiSarge.h"
#include "EnnemiLizzy.h"
#include "EnnemiTaur.h"
#include "EnnemiGunner.h"
#include "EnnemiThing.h"
#include "EnnemiGhost.h"
#include "EnnemiSmoulder.h"
#include "EnnemiSkully.h"
#include "EnnemiGiggler.h"
#include "EnnemiHeadBoss.h"
#include "EnnemiAlienBoss.h"
#include "Bomb.h"
#include "ProjectileFireBall.h"
#include "ProjectileShot.h"
#include "ProjectileMGShot.h"
#include "ProjectileBolt.h"
#include "ProjectileFlame.h"
#include "ProjectilePlasma.h"
#include "ProjectileMagma.h"
#include "ProjectileHeadMissile.h"
#include "ProjectileEgg.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "tinyxml2.h"

using std::vector;
using std::ifstream;
using std::cerr;
using std::endl;
using namespace tinyxml2;

vector< vector<Case*> > Carte::tableau;
vector<Bonus*> Carte::listeBonus;
vector<Teleporter*> Carte::listeTeleporter;
vector<Effet*> Carte::listeEffet;
vector<Ennemi*> Carte::listeEnnemi;
vector<Score*> Carte::listeScore;
vector<Projectile*> Carte::listeProjectile;
vector<Coin*> Carte::listeCoin;
vector<Lettre*> Carte::listeLettre;
Mix_Music* Carte::musique = nullptr;
bool Carte::noCoin = false;
bool Carte::noTeleporter = false;
bool Carte::boss = false;
unsigned int Carte::tempsEcoule = 0;
unsigned int Carte::temps = 0;
bool Carte::hasNextLevel = true;


Carte::Carte(unsigned int niveau)
{
    for(unsigned int i =0;i<12;i++)
        listeEnnemiLoad[i] = false;

    if(!chargerCarte(niveau))
        throw std::string("Impossible de charger le niveau");

    tempsEcoule = 0;

}

void Carte::resetNextLevel()
{
    hasNextLevel = true;
}


// HAUTEUR = LIGNE ET PUIS LONGUEUR = COLONNE
Case* Carte::getCase(unsigned int y,unsigned int x)
{
    y-=1;
    x-=1;

    if(tableau.size() <= y)
        return nullptr;
    if(tableau.at(y).size() <= x)
        return nullptr;

    return tableau.at(y).at(x);
}

bool Carte::hasBos()
{
    return boss;
}

Ennemi* Carte::getBoss()
{
    if(boss)
    {
        for(unsigned int i = 0;i<listeEnnemi.size();i++)
        {
            if(listeEnnemi.at(i)->getIDEnnemi() == IDEnnemi::ALIENBOSS)
            {
                return listeEnnemi.at(i);
            }
        }

    }

    return nullptr;
}

Case* Carte::getCaseByPixel(int y, int x)
{
    x -= CASE_DIMENSION*4;
    y -= CASE_DIMENSION;
    return Carte::getCase((y/CASE_DIMENSION)+1,(x/CASE_DIMENSION+1));
}
bool Carte::isNoCoin()
{
    return noCoin;
}

bool Carte::chargerCarte(int niveau)
{
    XMLDocument xmlDocument;
    XMLError xmlResult = xmlDocument.LoadFile("../Niveaux/Niveaux.xml");

    if(xmlResult != XMLError::XML_SUCCESS)
    {
        cerr << "Impossible de charger le fichier des niveaux" << endl;
        xmlDocument.PrintError();
        return false;
    }

    XMLNode * pRoot = xmlDocument.FirstChild();

    if (pRoot == nullptr)
    {
        cerr << "Impossible de trouver la racine du fichier des niveaux" << endl;
        return false;
    }

    int lvl;
    XMLElement* xmlLevel = pRoot->FirstChildElement("Level");
    while(xmlLevel)
    {
        xmlLevel->QueryIntAttribute("Index",&lvl);
        if(niveau!=lvl) xmlLevel = xmlLevel->NextSiblingElement("Level"); else break;
    }

    if(xmlLevel)
    {
        int msq,caseFond,caseDur,caseCassable,caseBordure,tmps;
        xmlLevel->QueryIntAttribute("CaseFond",&caseFond);
        xmlLevel->QueryIntAttribute("CaseDur",&caseDur);
        xmlLevel->QueryIntAttribute("CaseCassable",&caseCassable);
        xmlLevel->QueryIntAttribute("CaseBordure",&caseBordure);
        xmlLevel->QueryIntAttribute("Musique",&msq);
        xmlLevel->QueryIntAttribute("Temps",&tmps);

        Case::case_bordure = caseBordure;
        Case::case_fond = caseFond;
        Case::case_cassable = caseCassable;
        Case::case_dur = caseDur;
        temps = tmps;

        std::stringstream ms;
        ms << "../Musiques/Boom_Music_" << (msq+1) << ".ogg";
        musique = Mix_LoadMUS(ms.str().c_str());

        noCoin = false;
        noTeleporter = false;
        boss = false;

        for(int i = 0;i<CASE_HAUTEUR;i++)
        {
            vector <Case *> colonne(CASE_LONGUEUR);
            tableau.push_back(colonne);
        }

        XMLElement* xmlCases = xmlLevel->FirstChildElement("Cases");

        XMLElement* xmlCase = xmlCases->FirstChildElement("Case");
        while(xmlCase)
        {
            bool coin = false;
            int x,y,type,indiceTeleporter = 0,indiceTeleporterFrere = 0;
            TypeCase typeCase;
            xmlCase->QueryIntAttribute("X",&x);
            xmlCase->QueryIntAttribute("Y",&y);
            xmlCase->QueryIntAttribute("Type",&type);
            typeCase = static_cast<TypeCase>(type);

            if(type== static_cast<Uint32>(TypeCase::COIN))
            {
                coin = true;
                typeCase = TypeCase::FOND;
            }
            else if(type> static_cast<Uint32>(TypeCase::COIN))
            {
                indiceTeleporter = type;
                xmlCase->QueryIntAttribute("TeleportTo",&indiceTeleporterFrere);
                typeCase = TypeCase::FOND;
            }

            SDL_Rect position;
            position.x = CASE_DEPART_JEU_X + 32*x;
            position.y = CASE_DEPART_JEU_Y + 32*y;
            position.w = CASE_DIMENSION;
            position.h = CASE_DIMENSION;
            Case* c = new Case(typeCase,position);
            c->getAdressePositionDamier()->x = x+1;
            c->getAdressePositionDamier()->y = y+1;
            tableau.at(y).at(x) = c;

            if(coin)
            {
                listeCoin.push_back(new Coin(c));
            }
            if(indiceTeleporter>0)
            {
                c->setTeleporter(new Teleporter(c,indiceTeleporter,indiceTeleporterFrere));
                listeTeleporter.push_back(c->getTeleporter());
            }
            xmlCase = xmlCase->NextSiblingElement("Case");
        }

        if(listeCoin.empty())
            noCoin = true;
        else
            Coin::initialiserCoin();

        if(listeTeleporter.empty())
            noTeleporter = true;
        else
            Teleporter::initialiserTeleporter();

        XMLElement* xmlSpawns = xmlLevel->FirstChildElement("Spawns");
        XMLElement* xmlSpawn = xmlSpawns->FirstChildElement("Spawn");
        int y,x,index;
        xmlSpawn->QueryIntAttribute("Player",&index);
        xmlSpawn->QueryIntAttribute("X",&x);
        xmlSpawn->QueryIntAttribute("Y",&y);
        listeSpawn[index-1] = getCase(y,x);
        xmlSpawn = xmlSpawn->NextSiblingElement("Spawn");
        xmlSpawn->QueryIntAttribute("Player",&index);
        xmlSpawn->QueryIntAttribute("X",&x);
        xmlSpawn->QueryIntAttribute("Y",&y);
        listeSpawn[index-1] = getCase(y,x);

        XMLElement* xmlEnnemis = xmlLevel->FirstChildElement("Ennemis");
        XMLElement* xmlEnnemi = xmlEnnemis->FirstChildElement("Ennemi");
        IDEnnemi t;
        while(xmlEnnemi)
        {
            xmlEnnemi->QueryIntAttribute("Type",&index);
            xmlEnnemi->QueryIntAttribute("X",&x);
            xmlEnnemi->QueryIntAttribute("Y",&y);
            t = static_cast<IDEnnemi>(index);

            switch(t)
            {
            case IDEnnemi::SOLDIER:
                ajouterEnnemi(new EnnemiSoldier(getCase(y,x)));
                break;
            case IDEnnemi::SARGE:
                ajouterEnnemi(new EnnemiSarge(getCase(y,x)));
                break;
            case IDEnnemi::LIZZY:
                ajouterEnnemi(new EnnemiLizzy(getCase(y,x)));
                break;
            case IDEnnemi::TAUR:
                ajouterEnnemi(new EnnemiTaur(getCase(y,x)));
                break;
            case IDEnnemi::GUNNER:
                ajouterEnnemi(new EnnemiGunner(getCase(y,x)));
                break;
            case IDEnnemi::THING:
                ajouterEnnemi(new EnnemiThing(getCase(y,x)));
                break;
            case IDEnnemi::GHOST:
                ajouterEnnemi(new EnnemiGhost(getCase(y,x)));
                break;
            case IDEnnemi::SMOULDER:
                ajouterEnnemi(new EnnemiSmoulder(getCase(y,x)));
                break;
            case IDEnnemi::SKULLY:
                ajouterEnnemi(new EnnemiSkully(getCase(y,x)));
                break;
            case IDEnnemi::GIGGLER:
                ajouterEnnemi(new EnnemiGiggler(getCase(y,x)));
                break;
            case IDEnnemi::HEADBOSS:
                ajouterEnnemi(new EnnemiHeadBoss(getCase(y,x)));
                break;
            case IDEnnemi::ALIENBOSS:
                ajouterEnnemi(new EnnemiAlienBoss(getCase(y,x)));
                break;
            }

            listeEnnemiLoad[static_cast<Uint32>(t)] = true;
            xmlEnnemi = xmlEnnemi->NextSiblingElement("Ennemi");
        }

        Bomber::initialiserBomber();
        Case::initialiserCase();
        Bomb::initialiserBombe();
        Explosion::initialiserExplosion();
        Effet::initialiserEffet();
        Score::initialiserScore();
        Ennemi::initialiserEnnemi();
        Bonus::initialiserBonus();
        Lettre::initialiserLettre();

        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SOLDIER)])
            EnnemiSoldier::initialiserEnnemiSoldier();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SARGE)])
            EnnemiSarge::initialiserEnnemiSarge();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SOLDIER)] || listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SARGE)])
            ProjectileShot::initialiserProjectileShot();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::LIZZY)])
        {
            EnnemiLizzy::initialiserEnnemiLizzy();
            ProjectileFireBall::initialiserProjectileFireBall();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::TAUR)])
            EnnemiTaur::initialiserEnnemiTaur();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GUNNER)])
        {
            ProjectileMGShot::initialiserProjectileMGShot();
            EnnemiGunner::initialiserEnnemiGunner();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::THING)])
        {
            ProjectileBolt::initialiserProjectileBolt();
            EnnemiThing::initialiserEnnemiThing();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GHOST)])
            EnnemiGhost::initialiserEnnemiGhost();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SMOULDER)])
        {
            EnnemiSmoulder::initialiserEnnemiSmoulder();
            ProjectileFlame::initialiserProjectileFlame();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SKULLY)])
        {
            EnnemiSkully::initialiserEnnemiSkully();
            ProjectilePlasma::initialiserProjectilePlasma();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GIGGLER)])
        {
            EnnemiGiggler::initialiserEnnemiGiggler();
            ProjectileMagma::initialiserProjectileMagma();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::HEADBOSS)])
        {
            EnnemiHeadBoss::initialiserEnnemiHeadBoss();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::ALIENBOSS)])
        {
            boss = true;
            EnnemiAlienBoss::initialiserEnnemiAlienBoss();
            ProjectileEgg::initialiserProjectileEgg();
            EnnemiGiggler::initialiserEnnemiGiggler();
            ProjectileMagma::initialiserProjectileMagma();
            EnnemiSkully::initialiserEnnemiSkully();
            ProjectilePlasma::initialiserProjectilePlasma();
            EnnemiSmoulder::initialiserEnnemiSmoulder();
            ProjectileFlame::initialiserProjectileFlame();
            EnnemiGhost::initialiserEnnemiGhost();
            ProjectileBolt::initialiserProjectileBolt();
            EnnemiThing::initialiserEnnemiThing();
            ProjectileMGShot::initialiserProjectileMGShot();
            EnnemiGunner::initialiserEnnemiGunner();
            EnnemiTaur::initialiserEnnemiTaur();
            EnnemiLizzy::initialiserEnnemiLizzy();
            ProjectileFireBall::initialiserProjectileFireBall();
            ProjectileShot::initialiserProjectileShot();
            EnnemiSarge::initialiserEnnemiSarge();
            EnnemiSoldier::initialiserEnnemiSoldier();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::ALIENBOSS)] || listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::HEADBOSS)])
            ProjectileHeadMissile::initialiserProjectileHeadMissile();

        hasNextLevel = xmlLevel->NextSiblingElement("Level");

        return true;
    }

    else
    {
        cerr << "Impossible de charger le niveau " << niveau << endl;
        return false;
    }



}

/*bool Carte::chargerCarte(unsigned int niveau)
{
    std::stringstream lvl;
    lvl << "Niveaux/Niveau"<<niveau<<".txt";
    ifstream fichier(lvl.str().c_str());

    if(!fichier)
    {
        cerr << "Impossible d'ouvrir le fichier du niveau " << niveau << endl;
        return false;
    }
    else
    {
        unsigned int msq = 0;
        fichier >> Case::case_fond >> Case::case_dur >> Case::case_cassable >> Case::case_bordure >> msq;

        std::stringstream ms;
        ms << "Musiques/Boom_Music_" << (msq+1) << ".ogg";
        musique = Mix_LoadMUS(ms.str().c_str());
        noCoin = false;
        noTeleporter = false;
        boss = false;

        TypeCase type;
        int typ;
        for(int i = 0;i<CASE_HAUTEUR;i++)
        {
            vector <Case *> colonne;

            for(int j = 0;j<CASE_LONGUEUR;j++)
            {
                unsigned int indiceTeleporterFrere = 0;
                unsigned int indiceTeleporter = 0;
                bool coin = false;
                fichier >> typ;
                type = static_cast<TypeCase>(typ);
                if(type==COIN)
                {
                    coin = true;
                    type = TypeCase::FOND;
                }
                else if(type>COIN)
                {
                    indiceTeleporter = type;
                    fichier >> indiceTeleporterFrere;
                    type = TypeCase::FOND;
                }
                SDL_Rect position;
                position.x = CASE_DEPART_JEU_X + 32*j;
                position.y = CASE_DEPART_JEU_Y + 32*i;
                position.w = CASE_DIMENSION;
                position.h = CASE_DIMENSION;
                colonne.push_back(new Case(type,position));
                colonne.at(j)->getAdressePositionDamier()->x = j+1;
                colonne.at(j)->getAdressePositionDamier()->y = i+1;

                if(coin)
                {
                    listeCoin.push_back(new Coin(colonne.at(j)));
                }

                if(indiceTeleporter>0)
                {
                    colonne.at(j)->setTeleporter(new Teleporter(colonne.at(j),indiceTeleporter,indiceTeleporterFrere));
                    listeTeleporter.push_back(colonne.at(j)->getTeleporter());
                }
            }

            tableau.push_back(colonne);
        }

        if(listeCoin.empty())
            noCoin = true;
        else
            Coin::initialiserCoin();

        if(listeTeleporter.empty())
            noTeleporter = true;
        else
            Teleporter::initialiserTeleporter();

        fichier >> temps;
        int y,x;
        fichier >> y >> x;
        listeSpawn[0] = getCase(y,x);
        fichier >> y >> x;
        listeSpawn[1] = getCase(y,x);

        int ind;
        IDEnnemi t;
        while(!fichier.eof())
        {
            fichier >> y >> x >> ind;
            t = static_cast<IDEnnemi>(ind);
            switch(t)
            {
            case IDEnnemi::SOLDIER:
                ajouterEnnemi(new EnnemiSoldier(getCase(y,x)));
                break;
            case IDEnnemi::SARGE:
                ajouterEnnemi(new EnnemiSarge(getCase(y,x)));
                break;
            case IDEnnemi::LIZZY:
                ajouterEnnemi(new EnnemiLizzy(getCase(y,x)));
                break;
            case IDEnnemi::TAUR:
                ajouterEnnemi(new EnnemiTaur(getCase(y,x)));
                break;
            case IDEnnemi::GUNNER:
                ajouterEnnemi(new EnnemiGunner(getCase(y,x)));
                break;
            case IDEnnemi::THING:
                ajouterEnnemi(new EnnemiThing(getCase(y,x)));
                break;
            case IDEnnemi::GHOST:
                ajouterEnnemi(new EnnemiGhost(getCase(y,x)));
                break;
            case IDEnnemi::SMOULDER:
                ajouterEnnemi(new EnnemiSmoulder(getCase(y,x)));
                break;
            case IDEnnemi::SKULLY:
                ajouterEnnemi(new EnnemiSkully(getCase(y,x)));
                break;
            case IDEnnemi::GIGGLER:
                ajouterEnnemi(new EnnemiGiggler(getCase(y,x)));
                break;
            case IDEnnemi::HEADBOSS:
                ajouterEnnemi(new EnnemiHeadBoss(getCase(y,x)));
                break;
            case IDEnnemi::ALIENBOSS:
                ajouterEnnemi(new EnnemiAlienBoss(getCase(y,x)));
                break;
            }
            listeEnnemiLoad[t] = true;
        }

        Bomber::initialiserBomber();
        Case::initialiserCase();
        Bomb::initialiserBombe();
        Explosion::initialiserExplosion();
        Effet::initialiserEffet();
        Score::initialiserScore();
        Ennemi::initialiserEnnemi();
        Bonus::initialiserBonus();
        Lettre::initialiserLettre();

        if(listeEnnemiLoad[SOLDIER])
            EnnemiSoldier::initialiserEnnemiSoldier();
        if(listeEnnemiLoad[SARGE])
            EnnemiSarge::initialiserEnnemiSarge();
        if(listeEnnemiLoad[SOLDIER] || listeEnnemiLoad[SARGE])
            ProjectileShot::initialiserProjectileShot();
        if(listeEnnemiLoad[LIZZY])
        {
            EnnemiLizzy::initialiserEnnemiLizzy();
            ProjectileFireBall::initialiserProjectileFireBall();
        }
        if(listeEnnemiLoad[TAUR])
            EnnemiTaur::initialiserEnnemiTaur();
        if(listeEnnemiLoad[GUNNER])
        {
            ProjectileMGShot::initialiserProjectileMGShot();
            EnnemiGunner::initialiserEnnemiGunner();
        }
        if(listeEnnemiLoad[THING])
        {
            ProjectileBolt::initialiserProjectileBolt();
            EnnemiThing::initialiserEnnemiThing();
        }
        if(listeEnnemiLoad[GHOST])
            EnnemiGhost::initialiserEnnemiGhost();
        if(listeEnnemiLoad[SMOULDER])
        {
            EnnemiSmoulder::initialiserEnnemiSmoulder();
            ProjectileFlame::initialiserProjectileFlame();
        }
        if(listeEnnemiLoad[SKULLY])
        {
            EnnemiSkully::initialiserEnnemiSkully();
            ProjectilePlasma::initialiserProjectilePlasma();
        }
        if(listeEnnemiLoad[GIGGLER])
        {
            EnnemiGiggler::initialiserEnnemiGiggler();
            ProjectileMagma::initialiserProjectileMagma();
        }
        if(listeEnnemiLoad[HEADBOSS])
        {
            EnnemiHeadBoss::initialiserEnnemiHeadBoss();
        }
        if(listeEnnemiLoad[ALIENBOSS])
        {
            boss = true;
            EnnemiAlienBoss::initialiserEnnemiAlienBoss();
            ProjectileEgg::initialiserProjectileEgg();
            EnnemiGiggler::initialiserEnnemiGiggler();
            ProjectileMagma::initialiserProjectileMagma();
            EnnemiSkully::initialiserEnnemiSkully();
            ProjectilePlasma::initialiserProjectilePlasma();
            EnnemiSmoulder::initialiserEnnemiSmoulder();
            ProjectileFlame::initialiserProjectileFlame();
            EnnemiGhost::initialiserEnnemiGhost();
            ProjectileBolt::initialiserProjectileBolt();
            EnnemiThing::initialiserEnnemiThing();
            ProjectileMGShot::initialiserProjectileMGShot();
            EnnemiGunner::initialiserEnnemiGunner();
            EnnemiTaur::initialiserEnnemiTaur();
            EnnemiLizzy::initialiserEnnemiLizzy();
            ProjectileFireBall::initialiserProjectileFireBall();
            ProjectileShot::initialiserProjectileShot();
            EnnemiSarge::initialiserEnnemiSarge();
            EnnemiSoldier::initialiserEnnemiSoldier();
        }
        if(listeEnnemiLoad[ALIENBOSS] || listeEnnemiLoad[HEADBOSS])
            ProjectileHeadMissile::initialiserProjectileHeadMissile();

        fichier.close();
    }

    return true;

}*/

Case** Carte::getListeSpawn()
{
    return listeSpawn;
}
Teleporter* Carte::getTeleporterFrere(unsigned int indice)
{
    for(unsigned int i = 0;i<listeTeleporter.size();i++)
    {
        if(indice == listeTeleporter.at(i)->getIndice())
            return listeTeleporter.at(i);
    }

    return nullptr;
}

unsigned int Carte::getTemps()
{
    return temps;
}

unsigned int Carte::getTempsEcoule()
{
    return tempsEcoule;
}

SDL_Texture* Carte::getCaseBordure()
{
    return Case::getImageBordure();
}

void Carte::setTempsEcoule(unsigned int tempsEcoule)
{
    Carte::tempsEcoule = tempsEcoule;
    if(Carte::tempsEcoule>temps)
        Carte::tempsEcoule = temps;
}


vector<Bonus*> &Carte::getListeBonus()
{
    return listeBonus;
}

vector<Coin*> &Carte::getListeCoin()
{
    return listeCoin;
}

vector<Effet*> &Carte::getListeEffet()
{
    return listeEffet;
}

vector<Score*> &Carte::getListeScore()
{
    return listeScore;
}

vector<Projectile*> &Carte::getListeProjectile()
{
    return listeProjectile;
}

vector<vector <Case*> >&Carte::getTableau()
{
    return tableau;
}

vector<Lettre*> &Carte::getListeLettre()
{
    return listeLettre;
}

vector<Teleporter*> &Carte::getListeTeleporter()
{
    return listeTeleporter;
}

vector<Ennemi*> &Carte::getListeEnnemi()
{
    return listeEnnemi;
}

void Carte::ajouterEnnemi(Ennemi* ennemi)
{
    listeEnnemi.push_back(ennemi);
}

void Carte::ajouterProjectile(Projectile* projectile)
{
    listeProjectile.push_back(projectile);
}

Mix_Music* Carte::getMusique()
{
    return musique;
}

bool Carte::getHasNextLevel()
{
    return hasNextLevel;
}

void Carte::ajouterEffet(Effet* effet)
{
    Mix_PlayChannel(-1,effet->getSon(),0);
    listeEffet.push_back(effet);
}
void Carte::ajouterBonus(Bonus* bonus)
{
    listeBonus.push_back(bonus);
}

void Carte::ajouterScore(Score* score)
{
    listeScore.push_back(score);
}

void Carte::retirerScore(unsigned int indice)
{
    delete listeScore.at(indice);
    listeScore.erase(listeScore.begin() + indice);
}
// RETIRER SIGNIFIE PRIS PAR LE JOUEUR
void Carte::retirerBonus(Bonus* bonus)
{
    for(unsigned i = 0;i<listeBonus.size();i++)
    {
        if(bonus->equals(listeBonus.at(i)))
        {
            listeBonus.at(i)->getCase()->setBonus(nullptr);
            listeBonus.at(i)->setCase(nullptr);
            listeBonus.at(i)->setDisponible(false);
            listeBonus.erase(listeBonus.begin() + i);
            break;
        }
    }
}

void Carte::retirerEnnemi(unsigned int indice)
{
    if(Ennemi::getAlienMode())
        listeLettre.push_back(new Lettre(listeEnnemi.at(indice)->getPosition()));

    delete listeEnnemi.at(indice);
    listeEnnemi.erase(listeEnnemi.begin() + indice);
}

void Carte::retirerLettre(unsigned int indice)
{
    delete listeLettre.at(indice);
    listeLettre.erase(listeLettre.begin() + indice);
}
void Carte::retirerCoin(unsigned int indice)
{
    listeCoin.at(indice)->getCase()->setCoin(nullptr);
    listeCoin.at(indice)->setCase(nullptr);
    delete listeCoin.at(indice);
    listeCoin.erase(listeCoin.begin() + indice);
}
Carte::~Carte()
{


    for(unsigned int i = 0;i<listeBonus.size();i++)
    {
        delete listeBonus.at(i);
        listeBonus.erase(listeBonus.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeEffet.size();i++)
    {
        delete listeEffet.at(i);
        listeEffet.erase(listeEffet.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeTeleporter.size();i++)
    {
        delete listeTeleporter.at(i);
        listeTeleporter.erase(listeTeleporter.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeProjectile.size();i++)
    {
        delete listeProjectile.at(i);
        listeProjectile.erase(listeProjectile.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeScore.size();i++)
    {
        delete listeScore.at(i);
        listeScore.erase(listeScore.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeCoin.size();i++)
    {
        delete listeCoin.at(i);
        listeCoin.erase(listeCoin.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeLettre.size();i++)
    {
        delete listeLettre.at(i);
        listeLettre.erase(listeLettre.begin() + i);
        i--;
    }

    for(unsigned int i = 0;i<listeEnnemi.size();i++)
    {
        delete listeEnnemi.at(i);
        listeEnnemi.erase(listeEnnemi.begin() + i);
        i--;
    }

    for(unsigned int i =0;i<tableau.size();i++)
    {
        for(unsigned int j =0;j<tableau[i].size();j++)
        {
            delete tableau[i][j];
            tableau[i].erase(tableau[i].begin() + j);
            j--;
        }
        tableau.erase(tableau.begin() + i);
        i--;
    }

    Bomber::releaseBomber();
    Case::releaseCase();
    Bomb::releaseBombe();
    Explosion::releaseExplosion();
    Effet::releaseEffet();
    Score::releaseScore();
    Ennemi::releaseEnnemi();
    Bonus::releaseBonus();
    Lettre::releaseLettre();

    if(!noCoin)
    {
        Coin::releaseCoin();
    }
    if(!noTeleporter)
    {
        Teleporter::releaseTeleporter();
    }

        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SOLDIER)])
            EnnemiSoldier::releaseEnnemiSoldier();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SARGE)])
            EnnemiSarge::releaseEnnemiSarge();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SOLDIER)] || listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SARGE)])
            ProjectileShot::releaseProjectileShot();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::LIZZY)])
        {
             ProjectileFireBall::releaseProjectileFireBall();
             EnnemiLizzy::releaseEnnemiLizzy();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::TAUR)])
            EnnemiTaur::releaseEnnemiTaur();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GUNNER)])
        {
            EnnemiGunner::releaseEnnemiGunner();
            ProjectileMGShot::releaseProjectileMGShot();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::THING)])
        {
            ProjectileBolt::releaseProjectileBolt();
            EnnemiThing::releaseEnnemiThing();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GHOST)])
            EnnemiGhost::releaseEnnemiGhost();
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SMOULDER)])
        {
            EnnemiSmoulder::releaseEnnemiSmoulder();
            ProjectileFlame::releaseProjectileFlame();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::SKULLY)])
        {
            EnnemiSkully::releaseEnnemiSkully();
            ProjectilePlasma::releaseProjectilePlasma();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::GIGGLER)])
        {
            EnnemiGiggler::releaseEnnemiGiggler();
            ProjectileMagma::releaseProjectileMagma();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::HEADBOSS)])
        {
            EnnemiHeadBoss::releaseEnnemiHeadBoss();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::ALIENBOSS)])
        {
            EnnemiAlienBoss::releaseEnnemiAlienBoss();
            ProjectileEgg::releaseProjectileEgg();
            EnnemiGiggler::releaseEnnemiGiggler();
            ProjectileMagma::releaseProjectileMagma();
            EnnemiSkully::releaseEnnemiSkully();
            ProjectilePlasma::releaseProjectilePlasma();
            EnnemiSmoulder::releaseEnnemiSmoulder();
            ProjectileFlame::releaseProjectileFlame();
            EnnemiGhost::releaseEnnemiGhost();
            ProjectileBolt::releaseProjectileBolt();
            EnnemiThing::releaseEnnemiThing();
            EnnemiGunner::releaseEnnemiGunner();
            ProjectileMGShot::releaseProjectileMGShot();
            EnnemiTaur::releaseEnnemiTaur();
            ProjectileFireBall::releaseProjectileFireBall();
            EnnemiLizzy::releaseEnnemiLizzy();
            ProjectileShot::releaseProjectileShot();
            EnnemiSarge::releaseEnnemiSarge();
            EnnemiSoldier::releaseEnnemiSoldier();
        }
        if(listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::HEADBOSS)] || listeEnnemiLoad[static_cast<Uint32>(IDEnnemi::ALIENBOSS)])
        {
            ProjectileHeadMissile::releaseProjectileHeadMissile();
        }

        if(musique)
            Mix_FreeMusic(musique);
        musique = nullptr;

}
