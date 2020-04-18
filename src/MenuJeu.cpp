#include "MenuJeu.h"
#include "MenuPrincipal.h"
#include "MenuFin.h"
#include "Ennemi.h"
#include "Utile.h"
#include "Jeu.h"
#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <sys/stat.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <fstream>


Carte* MenuJeu::carte = nullptr;
vector<Bomber*> MenuJeu::listeBomber;
int MenuJeu::tempsPause = 0;
int MenuJeu::tempsReprise = 0;
int MenuJeu::tempsSommePause = 0;
MenuJeu::MenuJeu(Input* input,InputMouse* inputMouse,SDL_Window* screen) : Menu(input,inputMouse,screen)
{
    idMenu = IDMenu::MENU_JEU;

    police = TTF_OpenFont("Fonts/lucon.ttf",20);
    policeBonus = TTF_OpenFont("Fonts/lucon.ttf",11);
    coeur = chargerSurface("Images/Heart.png");
    extraGame = chargerSurface("Images/EXTRAGame.png");
    hurryUp = chargerSurface("Images/HurryUp.png");
    gameOver = chargerSurface("Images/GameOver.png");
    hudLetter = chargerSurface("Images/EXTRAIcons.png");
    bonusHUD = chargerSurface("Images/Bonus_Icons.png");
    player1Head = chargerSurface("Images/player1Head.png");
    player2Head = chargerSurface("Images/player2Head.png");
    SDL_Surface* scoreLabelTmp = TTF_RenderText_Shaded(police,"score",COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
    SDL_Surface* gameOverLabel1Tmp = TTF_RenderText_Shaded(police,"GAME",COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
    SDL_Surface* gameOverLabel2Tmp = TTF_RenderText_Shaded(police,"OVER",COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
    SDL_Surface* tempsBonusLabelTmp = TTF_RenderText_Shaded(police,"TIME BONUS !",COLOR_POLICE_HUD,{0,0,0});
    SDL_Surface* getReadyLabelTmp = TTF_RenderText_Shaded(police,"GET READY !",COLOR_POLICE_HUD,{0,0,0});

    scoreLabel = SDL_CreateTextureFromSurface(Jeu::getRenderer(),scoreLabelTmp);
    gameOverLabel1 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),gameOverLabel1Tmp);
    gameOverLabel2 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),gameOverLabel2Tmp);
    tempsBonusLabel = SDL_CreateTextureFromSurface(Jeu::getRenderer(),tempsBonusLabelTmp);
    getReadyLabel = SDL_CreateTextureFromSurface(Jeu::getRenderer(),getReadyLabelTmp);

    SDL_FreeSurface(scoreLabelTmp);
    SDL_FreeSurface(gameOverLabel1Tmp);
    SDL_FreeSurface(gameOverLabel2Tmp);
    SDL_FreeSurface(tempsBonusLabelTmp);
    SDL_FreeSurface(getReadyLabelTmp);

    SDL_Surface* fondTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,CASE_DIMENSION*3,480,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    SDL_Surface* fondJoueurTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,CASE_DIMENSION*2.5,CASE_DIMENSION*CASE_HAUTEUR/2.5,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    SDL_Surface* bordureHautBasTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,fondJoueurTmp->w+1,1,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    SDL_Surface* bordureDroiteGaucheTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,1,fondJoueurTmp->h + 1,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    SDL_Surface* timerBgTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,fondJoueurTmp->w,CASE_DIMENSION,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);

    SDL_Surface* bordureHautBasTimerTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,timerBgTmp->w+1,1,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    SDL_Surface* bordureDroiteGaucheTimerTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,1,timerBgTmp->h + 1,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);

    SDL_Color color = COLOR_BG_JOUEUR;

    SDL_FillRect(fondTmp,nullptr,SDL_MapRGB(fondTmp->format,8,131,187));
    SDL_FillRect(fondJoueurTmp,nullptr,SDL_MapRGB(fondJoueurTmp->format,color.r,color.g,color.b));
    SDL_FillRect(bordureHautBasTmp,nullptr,SDL_MapRGB(bordureHautBasTmp->format,61,63,95));
    SDL_FillRect(bordureDroiteGaucheTmp,nullptr,SDL_MapRGB(bordureDroiteGaucheTmp->format,61,63,95));
    SDL_FillRect(bordureHautBasTimerTmp,nullptr,SDL_MapRGB(bordureHautBasTimerTmp->format,255,255,255));
    SDL_FillRect(bordureDroiteGaucheTimerTmp,nullptr,SDL_MapRGB(bordureDroiteGaucheTimerTmp->format,255,255,255));

    color = COLOR_BG_TIMER;
    SDL_FillRect(timerBgTmp,nullptr,SDL_MapRGB(timerBgTmp->format,color.r,color.g,color.b));

    fond = SDL_CreateTextureFromSurface(Jeu::getRenderer(),fondTmp);
    fondJoueur = SDL_CreateTextureFromSurface(Jeu::getRenderer(),fondJoueurTmp);
    bordureHautBas = SDL_CreateTextureFromSurface(Jeu::getRenderer(),bordureHautBasTmp);
    bordureDroiteGauche = SDL_CreateTextureFromSurface(Jeu::getRenderer(),bordureDroiteGaucheTmp);
    bordureHautBasTimer = SDL_CreateTextureFromSurface(Jeu::getRenderer(),bordureHautBasTimerTmp);
    bordureDroiteGaucheTimer = SDL_CreateTextureFromSurface(Jeu::getRenderer(),bordureDroiteGaucheTimerTmp);
    timerBg = SDL_CreateTextureFromSurface(Jeu::getRenderer(),timerBgTmp);

    SDL_FreeSurface(fondTmp);
    SDL_FreeSurface(fondJoueurTmp);
    SDL_FreeSurface(bordureHautBasTmp);
    SDL_FreeSurface(bordureDroiteGaucheTmp);
    SDL_FreeSurface(bordureHautBasTimerTmp);
    SDL_FreeSurface(bordureDroiteGaucheTimerTmp);
    SDL_FreeSurface(timerBgTmp);

    numberLevel = nullptr;
    numberLevelBackGround = nullptr;

    bossEnergyBar = nullptr;
    bossEnergyBarBlack = nullptr;

    cheatActivated = false;
    niveauCheat = 0;

    nombreJoueur = 1;
    niveau = 1;
    vitesseHury = 6;
    vitesseExtra = 5;
    vitesseGameOver = 6;
    finNiveau = true;
    preparationNiveau = true;
    goBackToMenu = false;
    transitionOn = false;
    pause = false;
    tempsPreparationNiveau = SDL_GetTicks();
    sonHurry = Mix_LoadWAV("Sons/HurryUp.wav");
    sonExtra = Mix_LoadWAV("Sons/EXTRAGame.wav");
    sonGameOver = Mix_LoadWAV("Sons/GameOver.wav");
    sonComplete = Mix_LoadWAV("Sons/CompleteLevel.wav");
    sonTimeBonus = Mix_LoadWAV("Sons/TimeBonus.wav");

    if(!chargerPreferences())
    {
        for(unsigned int i =0;i<nombreJoueur;i++)
            listeBomber.push_back(new Bomber());
    }

    Carte::resetNextLevel();

}

bool MenuJeu::chargerPreferences()
{
    struct stat buf;
    if (stat("preferences.boom", &buf) != -1)
    {
        std::ifstream ifile("preferences.boom");
        boost::archive::text_iarchive iBinaryArchive(ifile);
        bool onePlayer;
        float value1, value2;
        PlayerKeyboard pk;
        iBinaryArchive >> onePlayer >> value1 >> value2;
        Jeu::setVolumeMusic(value1*1.28);
        Jeu::setVolumeSound(value2*1.28);


        nombreJoueur = onePlayer ? 1 : 2;

        for(unsigned int i = 0;i<nombreJoueur;i++)
        {
            listeBomber.push_back(new Bomber());
            iBinaryArchive >> pk;
            listeBomber.at(i)->setPlayerKeyboard(pk);

        }


        ifile.close();
        return true;

    }

    return false;


}
/*
void MenuJeu::chargerContinue()
{
    struct stat buf;
    if (stat("save.boom", &buf) != -1)
    {
        std::ifstream ifile("save.boom");

        try
        {
            boost::archive::binary_iarchive iBinaryArchive(ifile);
            iBinaryArchive >> niveau;
            unsigned int nbPlayer = 1;
            iBinaryArchive >> nbPlayer;

            Bomber::resetCounter();
            for(unsigned int i = 0;i<nbPlayer && i<listeBomber.size();i++)
            {
                 delete listeBomber.at(i);
                 iBinaryArchive >> listeBomber.at(i);
            }
        } catch(boost::archive::archive_exception e)
        {
            std::cerr << e.what();
        }

        ifile.close();

    }

}*/

void MenuJeu::chargerContinue()
{
    struct stat buf;
    if (stat("save.boom", &buf) != -1)
    {
        std::ifstream ifile("save.boom");

        try
        {
            boost::archive::text_iarchive iBinaryArchive(ifile);
            iBinaryArchive >> niveau;
            unsigned int nbPlayer = 1;
            iBinaryArchive >> nbPlayer;
            vector<PlayerKeyboard> pk;


            for(unsigned int i = 0;i<listeBomber.size();i++)
            {
                pk.push_back(listeBomber.at(i)->getPlayerKeyboard());
                delete listeBomber.at(i);
                listeBomber.erase(listeBomber.begin() + i);
                i--;
            }
            for(unsigned int i = 0;i<nbPlayer && i <nombreJoueur;i++)
            {
                Bomber* b;
                iBinaryArchive >> b;
                listeBomber.push_back(b);
                listeBomber.at(i)->setPlayerKeyboard(pk.at(i));
            }

            if(nombreJoueur > nbPlayer)
            {
                listeBomber.push_back(new Bomber());
                listeBomber.at(listeBomber.size()-1)->setPlayerKeyboard(pk.at(1));
            }

        } catch(boost::archive::archive_exception e)
        {
            std::cerr << e.what();

            for(unsigned int i = 0;i<listeBomber.size();i++)
            {
                delete listeBomber.at(i);
                listeBomber.erase(listeBomber.begin() + i);
                i--;
            }

            niveau = 1;
            for(unsigned int i = 0;i<nombreJoueur;i++)
                listeBomber.push_back(new Bomber());
        }

        ifile.close();

    }

}
/*
void MenuJeu::sauvegarderAutomatiquement()
{
    std::ofstream ofile("save.boom");
    boost::archive::binary_oarchive oBinaryArchive(ofile);
    oBinaryArchive << niveau;
    oBinaryArchive << listeBomber.size();

    for(unsigned int i = 0;i<listeBomber.size();i++)
        oBinaryArchive << listeBomber.at(i);

    ofile.close();
}*/

void MenuJeu::sauvegarderAutomatiquement()
{
    std::ofstream ofile("save.boom");
    boost::archive::text_oarchive oBinaryArchive(ofile);
    oBinaryArchive << niveau << listeBomber.size();

    for(unsigned int i = 0;i<listeBomber.size();i++)
        oBinaryArchive << listeBomber.at(i);

    ofile.close();
}

bool MenuJeu::chargerNiveau()
{
    for(unsigned int i =0;i<listeBomber.size();i++)
    {
        for(unsigned int j = 0;listeBomber.at(i)->getListeBombes().size();j++)
        {
            delete listeBomber.at(i)->getListeBombes().at(j);
            listeBomber.at(i)->getListeBombes().erase(listeBomber.at(i)->getListeBombes().begin() + j);
            j--;
        }
    }

    if(carte)
        delete carte;
    carte = nullptr;
    tempsAffichageTemps = 0;
    scoreAAfficher = 0;
    tempsCalculScore = 0;
    tempsPreparationNiveau = 0;
    tempsPreparationNiveauAfter = 0;
    tempsAlienModeActif = 0;
    tempsFinNiveau = 0;
    afficheScore = false;
    enCalculScore = false;
    hurryMessage = false;
    hurryStop = false;
    sonHurryDone = false;
    extraMessage = false;
    extraStop = false;
    sonExtraDone = false;
    gameOverMessage = false;
    gameOverStop = false;
    sonGameOverDone = false;
    int w,h;
    SDL_QueryTexture(hurryUp,nullptr,nullptr,&w,&h);
    posHurry.x = CASE_DIMENSION*4 + CASE_LONGUEUR*(CASE_DIMENSION)/2 - w/2;
    posHurry.y = -h;
    posHurry.w = w;
    posHurry.h = h;
    SDL_QueryTexture(extraGame,nullptr,nullptr,&w,&h);
    posExtra.x = CASE_DIMENSION*4 + CASE_LONGUEUR*(CASE_DIMENSION)/2 - w/2;
    posExtra.y = -h;
    posExtra.w = w;
    posExtra.h = h;
    SDL_QueryTexture(gameOver,nullptr,nullptr,&w,&h);
    posGameOver.x = CASE_DIMENSION*4 + CASE_LONGUEUR*(CASE_DIMENSION)/2 - w/2;
    posGameOver.y = -h;
    posGameOver.w = w;
    posGameOver.h = h;
    tempsHuryStandByAfter = 0;
    tempsExtraStandByAfter = 0;
    tempsGameOverStandByAfter = 0;

     if(cheatActivated)
    {
        niveau = niveauCheat;
        cheatActivated = false;
        cheatMSG = "";
    }


    sauvegarderAutomatiquement();



    try {
        carte = new Carte(niveau);
        for(unsigned int i = 0;i<listeBomber.size();i++)
        {
            listeBomber.at(i)->preparerNiveau();
            listeBomber.at(i)->setCaseSpawn(carte->getListeSpawn()[i]);
            listeBomber.at(i)->spawn();
        }

        char buffer[] = "00";
        sprintf(buffer,"%02d",niveau);
        if(numberLevel)
            SDL_DestroyTexture(numberLevel);
        if(numberLevelBackGround)
            SDL_DestroyTexture(numberLevelBackGround);
        SDL_Surface* numberLevelTmp = TTF_RenderText_Blended(police,buffer,COLOR_POLICE_HUD);
        SDL_Surface* numberLevelBackGroundTmp = SDL_CreateRGBSurface(0,numberLevelTmp->w,numberLevelTmp->h,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
        SDL_FillRect(numberLevelBackGroundTmp,nullptr,SDL_MapRGB(numberLevelBackGroundTmp->format,0,0,0));
        numberLevel = SDL_CreateTextureFromSurface(Jeu::getRenderer(),numberLevelTmp);
        numberLevelBackGround = SDL_CreateTextureFromSurface(Jeu::getRenderer(),numberLevelBackGroundTmp);
        SDL_SetTextureAlphaMod(numberLevelBackGround,125);
        SDL_FreeSurface(numberLevelTmp);
        SDL_FreeSurface(numberLevelBackGroundTmp);

        if(bossEnergyBar)
        {
            SDL_DestroyTexture(bossEnergyBar);
            SDL_DestroyTexture(bossEnergyBarBlack);
        }

        if(carte->hasBos())
        {
            bossEnergyBar = chargerSurface("Images/EnergyBar.png");
            SDL_QueryTexture(bossEnergyBar,nullptr,nullptr,&w,&h);
            SDL_Surface* srf = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
            SDL_FillRect(srf,nullptr,SDL_MapRGB(srf->format,0,0,0));
            bossEnergyBarBlack = SDL_CreateTextureFromSurface(Jeu::getRenderer(),srf);
            SDL_FreeSurface(srf);

            posEnergyBar.w = w;
            posEnergyBar.h = h;
            posEnergyBar.x = CASE_DIMENSION*4 + CASE_LONGUEUR*(CASE_DIMENSION)/2 - w/2;
            posEnergyBar.y = -h;

            antiPosEnergyBarBlack.h = h;
            antiPosEnergyBarBlack.y = 0;
            antiPosEnergyBarBlack.x = 0;

            posEnergyBarBlack.h = h - 4;
        }

            niveau++;

        Mix_PlayMusic(carte->getMusique(),-1);

    }   catch(std::string& e)
        {
            if(carte)
                delete carte;
            carte = nullptr;
            std::cerr << e << std::endl;
            return false;
        }

    return true;

}

MenuJeu::~MenuJeu()
{
    TTF_CloseFont(police);
    TTF_CloseFont(policeBonus);
    SDL_DestroyTexture(fond);
    SDL_DestroyTexture(fondJoueur);
    SDL_DestroyTexture(bordureDroiteGauche);
    SDL_DestroyTexture(bordureHautBas);
    SDL_DestroyTexture(bordureHautBasTimer);
    SDL_DestroyTexture(bordureDroiteGaucheTimer);
    SDL_DestroyTexture(player1Head);
    SDL_DestroyTexture(player2Head);
    SDL_DestroyTexture(timerBg);
    SDL_DestroyTexture(coeur);
    SDL_DestroyTexture(scoreLabel);
    SDL_DestroyTexture(gameOverLabel1);
    SDL_DestroyTexture(gameOverLabel2);
    SDL_DestroyTexture(hurryUp);
    SDL_DestroyTexture(extraGame);
    SDL_DestroyTexture(gameOver);
    SDL_DestroyTexture(tempsBonusLabel);
    SDL_DestroyTexture(getReadyLabel);
    SDL_DestroyTexture(hudLetter);
    SDL_DestroyTexture(bonusHUD);

    if(numberLevel)
        SDL_DestroyTexture(numberLevel);
    if(numberLevelBackGround)
        SDL_DestroyTexture(numberLevelBackGround);

    if(bossEnergyBar)
    {
        SDL_DestroyTexture(bossEnergyBar);
        SDL_DestroyTexture(bossEnergyBarBlack);
    }


    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        delete listeBomber.at(i);
        listeBomber.erase(listeBomber.begin() + i);
        i--;
    }

    if(carte)
        delete carte;
    carte = nullptr;

    Mix_FreeChunk(sonHurry);
    Mix_FreeChunk(sonExtra);
    Mix_FreeChunk(sonGameOver);
    Mix_FreeChunk(sonComplete);
    Mix_FreeChunk(sonTimeBonus);
}

void MenuJeu::handleEvents()
{
  if(input->key[SDL_SCANCODE_BACKSPACE])
      goBackToMenu = true;

  if(input->key[SDL_SCANCODE_P])
  {
      setPause();
      input->key[SDL_SCANCODE_P] = 0;
  }

  if(input->newLastCharInput)
  {
      input->newLastCharInput = false;
      cheatMSG += input->lastCharInput;
      std::string cheatCode = "benoitledieugoniveau";
      if(cheatMSG.length() > cheatCode.length()+2)
          cheatMSG.erase(0,1);

      if(cheatMSG.substr(0,cheatMSG.length()-2) == "benoitledieugoniveau")
      {
          int niveauToGo = atoi(cheatMSG.substr(cheatMSG.length()-2,std::string::npos).c_str());
          if(niveauToGo > 0 )
          {
              cheatActivated = true;
              niveauCheat = niveauToGo;
              finNiveau = true;
          }

      }
  }
}



void MenuJeu::executeMenu()
{
   preparationNiveau = preparerNiveau();

   if(!goBackToMenu)
   {
       if(!pause)
       {
           if(!preparationNiveau)
           {

            deplacerBomber();
            executeBombe();
            executeBonus();
            executeCase();
            executeCoin();
            executeEffet();
            executeEnnemi();
            executeLetter();
            executeTeleporter();
            executeScore();
            executeProjectile();
            executeScore();
            calculEnergyBar();
            calculHury();
            calculExtra();
            calculGameOver();
            calculTemps();
            calculFinNiveau();
        }

      }
   }
    else
        Jeu::setMenu(new MenuPrincipal(input,inputMouse,screen));


}

void MenuJeu::afficheMenu()
{
    afficherHud();
    if(!preparationNiveau)
   {
        afficherCaseBordure();
        afficherEnergyBar();
        afficherNumberLevel();
        afficherCaseFixe();
        afficherTeleporter();
        afficherBonus();
        afficherBombe();
        afficherCoin();
        afficherExplosion();
        afficherBomber();
        afficherCaseCassable();
        afficherEnnemi();
        afficherProjectile();
        afficherEffet();
        afficherLettre();
        afficherScore();
        afficherMessage();
        afficherTransition();
   }
}
void MenuJeu::calculTemps()
{
    if(carte->getTemps()!=carte->getTempsEcoule())
    {
        if(getTime() - tempsAffichageTemps > 1000)
        {
            tempsAffichageTemps = getTime();
            carte->setTempsEcoule(carte->getTempsEcoule() + 1000);

            if(carte->getTempsEcoule() == carte->getTemps())
            {
                for(unsigned int i = 0;i<carte->getListeEnnemi().size();i++)
                    carte->getListeEnnemi().at(i)->accelerer();
            }
        }
    }
}

void MenuJeu::finirNiveau()
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1,sonComplete,0);
    tempsFinNiveau = getTime();

    for(unsigned int i = 0;i<listeBomber.size();i++)
        listeBomber.at(i)->activerCelebration();
}

bool MenuJeu::preparerNiveau()
{
    if(finNiveau)
    {
        if(carte)
        {
            if(afficheScore)
            {
                if(carte->getTemps() - carte->getTempsEcoule() !=0)
                    enCalculScore = true;
                else
                    enCalculScore = false;

                if(enCalculScore)
                {
                    if(carte->getTempsEcoule()!=carte->getTemps())
                    {
                        if(carte->getTemps() - carte->getTempsEcoule() >= 60000)
                        {
                            if(SDL_GetTicks() - tempsEntrePointScore > TEMPS_ENTRE_BIGPOINT_SCORE)
                            {
                                carte->setTempsEcoule(carte->getTempsEcoule() + 60000);
                                for(unsigned int i = 0;i<listeBomber.size();i++)
                                    listeBomber.at(i)->augmenterScore(600);

                            scoreAAfficher +=600;

                            tempsEntrePointScore = SDL_GetTicks();
                            Mix_PlayChannel(-1,sonTimeBonus,0);
                            }
                        }
                        else if(SDL_GetTicks() - tempsEntrePointScore > TEMPS_ENTRE_POINT_SCORE)
                        {
                            carte->setTempsEcoule(carte->getTempsEcoule() + 1000);
                            for(unsigned int i = 0;i<listeBomber.size();i++)
                                listeBomber.at(i)->augmenterScore(10);

                            scoreAAfficher +=10;

                            tempsEntrePointScore = SDL_GetTicks();
                            Mix_PlayChannel(-1,sonTimeBonus,0);
                        }
                        tempsCalculScore = SDL_GetTicks();
                        gererCalculScore();
                        return true;
                    }

                    enCalculScore = false;
                }

                if(SDL_GetTicks() - tempsCalculScore < TEMPS_CALCUL_SCORE)
                {
                    gererCalculScore();
                    return true;
                }
            }
        }

        if(SDL_GetTicks() - tempsPreparationNiveau < TEMPS_PREPARATION_NIVEAU)
        {
            if(Carte::getHasNextLevel())
            {
                SDL_Rect posLabel;
                SDL_Rect posValeur;
                char buffer[] = "Level 00";
                sprintf(buffer,"Level %02d",niveau);
                SDL_Surface* levelTmp = TTF_RenderText_Shaded(police,buffer,COLOR_POLICE_HUD,{0,0,0});
                SDL_Texture* level = SDL_CreateTextureFromSurface(Jeu::getRenderer(),levelTmp);
                int wgetReady,hgetReady,hlevel,wlevel;
                SDL_QueryTexture(getReadyLabel,nullptr,nullptr,&wgetReady,&hgetReady);
                SDL_QueryTexture(level,nullptr,nullptr,&wlevel,&hlevel);
                posLabel.x = CASE_DIMENSION*4 + ((CASE_LONGUEUR+1)*CASE_DIMENSION)/2 - wgetReady/2;
                posValeur.x = CASE_DIMENSION*4 + ((CASE_LONGUEUR+1)*CASE_DIMENSION)/2 - wlevel/2;
                posValeur.y = (2+CASE_HAUTEUR)*CASE_DIMENSION/2 - (hgetReady + hlevel)/2;
                posValeur.h = hlevel;
                posValeur.w = wlevel;
                posLabel.y = posValeur.y + CASE_DIMENSION;
                posLabel.h = hgetReady;
                posLabel.w = wgetReady;
                SDL_RenderCopy(Jeu::getRenderer(),getReadyLabel,nullptr,&posLabel);
                SDL_RenderCopy(Jeu::getRenderer(),level,nullptr,&posValeur);
                SDL_DestroyTexture(level);
                SDL_FreeSurface(levelTmp);
            }
            else
                Jeu::setMenu(new MenuFin(input,inputMouse,screen));

            return true;

        }
    finNiveau = false;
    if(!chargerNiveau())
    {
        Jeu::setMenu(new MenuPrincipal(input,inputMouse,screen));
        return true;
    }



    }

    return false;
}

void MenuJeu::gererCalculScore()
{
    SDL_Rect posLabel;
    SDL_Rect posValeur;
    std::string score;
    std::stringstream ss;
    ss << scoreAAfficher;
    score = ss.str();
    SDL_Surface* tempsBonusTmp = TTF_RenderText_Shaded(police,score.c_str(),COLOR_POLICE_HUD,{0,0,0});
    SDL_Texture* tempsBonus = SDL_CreateTextureFromSurface(Jeu::getRenderer(),tempsBonusTmp);
    int wtempsBonusLabel,htempsBonusLabel,wtempsBonus,htempsBonus;
    SDL_QueryTexture(tempsBonusLabel,nullptr,nullptr,&wtempsBonusLabel,&htempsBonusLabel);
    posLabel.x = CASE_DIMENSION*4 + ((CASE_LONGUEUR+1)*CASE_DIMENSION)/2 - wtempsBonusLabel/2;
    SDL_QueryTexture(tempsBonus,nullptr,nullptr,&wtempsBonus,&htempsBonus);
    posValeur.x = CASE_DIMENSION*4 + ((CASE_LONGUEUR+1)*CASE_DIMENSION)/2 - wtempsBonus/2;
    posLabel.y = (2+CASE_HAUTEUR)*CASE_DIMENSION/2 - (htempsBonusLabel + htempsBonus)/2;
    posValeur.y = posLabel.y + CASE_DIMENSION;
    posValeur.h = htempsBonus;
    posValeur.w = wtempsBonus;
    posLabel.h = htempsBonusLabel;
    posLabel.w = wtempsBonusLabel;
    SDL_RenderCopy(Jeu::getRenderer(),tempsBonusLabel,nullptr,&posLabel);
    SDL_RenderCopy(Jeu::getRenderer(),tempsBonus,nullptr,&posValeur);
    SDL_DestroyTexture(tempsBonus);
    SDL_FreeSurface(tempsBonusTmp);
    tempsPreparationNiveau = SDL_GetTicks();
}

void MenuJeu::calculFinNiveau()
{
    if(Carte::getListeEnnemi().empty())
    {
        if(getTime() - tempsFinNiveau > TEMPS_FIN_NIVEAU)
        {
              tempsPreparationNiveau = getTime();
              finNiveau = true;

              if(carte->getTempsEcoule() != carte->getTemps() && carte->getTempsEcoule() != 0)
                afficheScore = true;
        }

    }
}

void MenuJeu::afficherHud()
{
    int wFond,hFond,wFondJoueur,hFondJoueur,w,h;
    SDL_QueryTexture(fond,nullptr,nullptr,&wFond,&hFond);
    SDL_QueryTexture(fondJoueur,nullptr,nullptr,&wFondJoueur,&hFondJoueur);

    SDL_Rect posFond;
    posFond.x = 0;
    posFond.y = 0;
    posFond.h = hFond;
    posFond.w = wFond;
    SDL_RenderCopy(Jeu::getRenderer(),fond,nullptr,&posFond);


    SDL_Rect posFondTimer;
    int wTimerBg,hTimerBg;
    SDL_QueryTexture(timerBg,nullptr,nullptr,&wTimerBg,&hTimerBg);
    posFondTimer.x = wFond/2 - wFondJoueur/2;
    posFondTimer.y = WINDOW_HEIGHT/2 - hTimerBg/2;
    posFondTimer.h = hTimerBg;
    posFondTimer.w = wTimerBg;
    SDL_RenderCopy(Jeu::getRenderer(),timerBg,nullptr,&posFondTimer);

    unsigned int temps = 0;
    if(carte)
        temps = carte->getTemps() - carte->getTempsEcoule();

    unsigned int minute = temps/(1000*60);
    unsigned int seconde = temps/1000 - minute*60;
    char tempsAffichage[6];

    sprintf(tempsAffichage,"%02d:%02d",minute,seconde);

    SDL_Color color = COLOR_POLICE_HUD;

    if(temps<=30000)
        color = COLOR_HURRY_TIMER;

    SDL_Surface* timerTmp = TTF_RenderText_Shaded(police,tempsAffichage,color,COLOR_BG_TIMER);
    SDL_Texture* timer = SDL_CreateTextureFromSurface(Jeu::getRenderer(),timerTmp);
    SDL_FreeSurface(timerTmp);

    int wTimer,hTimer;
    SDL_QueryTexture(timer,nullptr,nullptr,&wTimer,&hTimer);
    SDL_Rect posTimer;
    posTimer.x = posFondTimer.x + wTimerBg/2 - wTimer/2;
    posTimer.y = posFondTimer.y + hTimerBg/2 - hTimer/2;
    posTimer.h = hTimer;
    posTimer.w = wTimer;
    SDL_RenderCopy(Jeu::getRenderer(),timer,nullptr,&posTimer);
    SDL_DestroyTexture(timer);

    SDL_Rect posBordureHaut;
    SDL_QueryTexture(bordureHautBasTimer,nullptr,nullptr,&w,&h);
    posBordureHaut.w = w;
    posBordureHaut.h = h;
    posBordureHaut.x = posFondTimer.x;
    posBordureHaut.y = posFondTimer.y;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBasTimer,nullptr,&posBordureHaut);

    SDL_Rect posBordureBas;
    posBordureBas.w = w;
    posBordureBas.h = h;
    posBordureBas.x = posFondTimer.x - 1;
    posBordureBas.y = posFondTimer.y + hTimerBg;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBasTimer,nullptr,&posBordureBas);

    SDL_Rect posBordureGauche;
    SDL_QueryTexture(bordureDroiteGaucheTimer,nullptr,nullptr,&w,&h);
    posBordureGauche.h = h;
    posBordureGauche.w = w;
    posBordureGauche.x = posFondTimer.x - 1;
    posBordureGauche.y = posFondTimer.y;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGaucheTimer,nullptr,&posBordureGauche);

    SDL_Rect posBordureDroite;
    posBordureDroite.h = h;
    posBordureDroite.w = w;
    posBordureDroite.x = posFondTimer.x + wTimerBg;
    posBordureDroite.y = posFondTimer.y;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGaucheTimer,nullptr,&posBordureDroite);

    SDL_Rect posFondJ1;
    posFondJ1.x = posFondTimer.x;
    posFondJ1.y = posFondTimer.y/2 - hFondJoueur/2;
    posFondJ1.h = hFondJoueur;
    posFondJ1.w = wFondJoueur;
    SDL_RenderCopy(Jeu::getRenderer(),fondJoueur,nullptr,&posFondJ1);

    SDL_QueryTexture(bordureHautBas,nullptr,nullptr,&w,&h);

    posBordureHaut.x = posFondJ1.x;
    posBordureHaut.y = posFondJ1.y;
    posBordureHaut.h = h;
    posBordureHaut.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBas,nullptr,&posBordureHaut);

    posBordureBas.x = posFondJ1.x - 1;
    posBordureBas.y = posFondJ1.y + hFondJoueur;
    posBordureBas.h = h;
    posBordureBas.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBas,nullptr,&posBordureBas);

    SDL_QueryTexture(bordureDroiteGauche,nullptr,nullptr,&w,&h);
    posBordureGauche.x = posFondJ1.x - 1;
    posBordureGauche.y = posFondJ1.y;
    posBordureGauche.h = h;
    posBordureGauche.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGauche,nullptr,&posBordureGauche);

    posBordureDroite.x = posFondJ1.x + wFondJoueur;
    posBordureDroite.y = posFondJ1.y;
    posBordureDroite.h = h;
    posBordureDroite.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGauche,nullptr,&posBordureDroite);


    std::stringstream ss;
    std::string s;
    if(listeBomber.size()>0)
    {
        ss << listeBomber.at(0)->getMegaVie() << "X";
    }
    else
        ss << "0X";

    s = ss.str();
    SDL_Surface* joueur1Tmp = TTF_RenderText_Shaded(police,s.c_str(),COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
    SDL_Texture* joueur1 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),joueur1Tmp);
    SDL_FreeSurface(joueur1Tmp);
    int wJoueur1,hJoueur1,wp1Head,hp1Head;
    SDL_QueryTexture(joueur1,nullptr,nullptr,&wJoueur1,&hJoueur1);
    SDL_QueryTexture(player1Head,nullptr,nullptr,&wp1Head,&hp1Head);
    SDL_Rect posLabelJ1,posHeadP1;
    posHeadP1.x = posFondJ1.x + wFondJoueur/2 - 16*2;
    posHeadP1.y = posFondJ1.y + CASE_DIMENSION/2 - hp1Head/2;
    posHeadP1.w = wp1Head;
    posHeadP1.h = hp1Head;
    posLabelJ1.x = posHeadP1.x + posHeadP1.w;
    posLabelJ1.y = posHeadP1.y;
    posLabelJ1.h = hJoueur1;
    posLabelJ1.w = wJoueur1;
    SDL_RenderCopy(Jeu::getRenderer(),player1Head,nullptr,&posHeadP1);
    SDL_RenderCopy(Jeu::getRenderer(),joueur1,nullptr,&posLabelJ1);

    SDL_Rect posCoeurBase;
    posCoeurBase.x = posFondJ1.x + wFondJoueur/2 - 16*2;
    posCoeurBase.y = posLabelJ1.y + hJoueur1*1.5;


    SDL_Rect posHudLetter,antiPosHudLetter;
    int wLetter,hLetter;
    SDL_QueryTexture(hudLetter,nullptr,nullptr,&wLetter,&hLetter);

     posHudLetter.x = posFondJ1.x + wFondJoueur/2 - (hLetter*5)/2;
     posHudLetter.y = posLabelJ1.y + hJoueur1*1.5 + CASE_DIMENSION + CASE_DIMENSION/5;
     posHudLetter.h = hLetter;
     posHudLetter.w = hLetter;
     antiPosHudLetter.h = hLetter;
     antiPosHudLetter.w = hLetter;
     antiPosHudLetter.y = 0;



    bool player1 = false;
    if(listeBomber.size()>0)
    {
        if(!listeBomber.at(0)->isGameEnded())
        {
            player1 = true;
            unsigned int coeurEntier = listeBomber.at(0)->getVie()/2;
            unsigned int demiCoeur = listeBomber.at(0)->getVie() - coeurEntier*2;


            SDL_Rect posCoeur;
            posCoeur.h = CASE_DIMENSION/2;
            posCoeur.w = CASE_DIMENSION/2;

            SDL_Rect antiPosCoeur;
            antiPosCoeur.w = CASE_DIMENSION/2;
            antiPosCoeur.h = CASE_DIMENSION/2;
            antiPosCoeur.y = 0;

            for(unsigned int i = 1;i<=8;i++)
            {
                posCoeur.x = posCoeurBase.x + ((i-1)%4)*CASE_DIMENSION/2;
                posCoeur.y = posCoeurBase.y + ((i-1)/4)*CASE_DIMENSION/2;

                if(i<=coeurEntier)
                {
                    antiPosCoeur.x = 2*18;
                    SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                }
                else
                {
                    if(demiCoeur)
                    {
                        antiPosCoeur.x = 18;
                        SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                        demiCoeur = 0;
                    }
                    else
                    {
                         antiPosCoeur.x = 0;
                         SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                    }

                }

            }

            for(unsigned int i = 0;i<5;i++)
            {
                antiPosHudLetter.x = (listeBomber.at(0)->getListeExtraLettre().at(i) ? i+1 : 0)*hLetter;
                SDL_RenderCopy(Jeu::getRenderer(),hudLetter,&antiPosHudLetter,&posHudLetter);
                posHudLetter.x += hLetter;
            }

        }



    }
    if(!player1)
    {
        int wGameOverLabel,hGameOverLabel;
        SDL_QueryTexture(gameOverLabel1,nullptr,nullptr,&wGameOverLabel,&hGameOverLabel);
        posCoeurBase.x = posFondJ1.x + wFondJoueur/2 - wGameOverLabel/2;
        posCoeurBase.h = hGameOverLabel;
        posCoeurBase.w = wGameOverLabel;
        SDL_RenderCopy(Jeu::getRenderer(),gameOverLabel1,nullptr,&posCoeurBase);
        posCoeurBase.y += CASE_DIMENSION/2;
        SDL_QueryTexture(gameOverLabel2,nullptr,nullptr,&wGameOverLabel,&hGameOverLabel);
        posCoeurBase.h = hGameOverLabel;
        posCoeurBase.w = wGameOverLabel;
        SDL_RenderCopy(Jeu::getRenderer(),gameOverLabel2,nullptr,&posCoeurBase);

        antiPosHudLetter.x = 0;
        for(unsigned int i = 0;i<5;i++)
        {
            SDL_RenderCopy(Jeu::getRenderer(),hudLetter,&antiPosHudLetter,&posHudLetter);
            posHudLetter.x += hLetter;
        }

    }


     posHudLetter.x = posFondJ1.x + wFondJoueur/2 - (hLetter*5)/2;
     posHudLetter.y = posLabelJ1.y + hJoueur1 + CASE_DIMENSION + CASE_DIMENSION/5;
     posHudLetter.h = hLetter;
     posHudLetter.w = hLetter;
     antiPosHudLetter.h = hLetter;
     antiPosHudLetter.w = hLetter;
     antiPosHudLetter.y = 0;


    SDL_Rect posBonusHud,antiPosBonusHud,posLabelBonus;
    int wbonusHUD,hbonusHUD;
    SDL_QueryTexture(bonusHUD,nullptr,nullptr,&wbonusHUD,&hbonusHUD);
    antiPosBonusHud.h = hbonusHUD/2;
    antiPosBonusHud.w = hbonusHUD/2;
    antiPosBonusHud.y = hbonusHUD/2;
    posBonusHud.h = hbonusHUD/2;
    posBonusHud.w = hbonusHUD/2;
    posBonusHud.y = posLabelJ1.y + hJoueur1*1.5 + CASE_DIMENSION*1.75;
    posBonusHud.x = posFondJ1.x + wFondJoueur/2 - (hbonusHUD/2*5)/2;
    posLabelBonus.y = posBonusHud.y + posBonusHud.h;

    for(unsigned int i = 0;i<5;i++)
    {
        if(listeBomber.size()>0)
        {
            switch(i)
            {
            case 0:
                antiPosBonusHud.y = hbonusHUD;
                if(listeBomber.at(0)->getNombreBombes() > 0)
                {
                    antiPosBonusHud.y = 0;
                    char lab[] = "x0";
                    sprintf(lab,"x%1d",listeBomber.at(0)->getNombreBombes());
                    SDL_Surface* labelNbBombesTmp = TTF_RenderText_Shaded(policeBonus,lab,COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
                    SDL_Texture* labelNbBombes = SDL_CreateTextureFromSurface(Jeu::getRenderer(),labelNbBombesTmp);
                    SDL_FreeSurface(labelNbBombesTmp);
                    SDL_QueryTexture(labelNbBombes,nullptr,nullptr,&posLabelBonus.w,&posLabelBonus.h);
                    posLabelBonus.x = posBonusHud.x;
                    SDL_RenderCopy(Jeu::getRenderer(),labelNbBombes,nullptr,&posLabelBonus);
                    SDL_DestroyTexture(labelNbBombes);
                }
                break;
            case 1:
                antiPosBonusHud.y = !listeBomber.at(0)->isBombeRapide() ? hbonusHUD/2 : 0;
                break;
            case 2:
                antiPosBonusHud.y = hbonusHUD;
                if(listeBomber.at(0)->getDistanceExplosion() > 0)
                {
                    antiPosBonusHud.y = 0;
                    char lab[] = "x0";
                    sprintf(lab,"x%1d",listeBomber.at(0)->getDistanceExplosion());
                    SDL_Surface* labelDistanceExplosionTmp = TTF_RenderText_Shaded(policeBonus,lab,COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
                    SDL_Texture* labelDistanceExplosion = SDL_CreateTextureFromSurface(Jeu::getRenderer(),labelDistanceExplosionTmp);
                    SDL_FreeSurface(labelDistanceExplosionTmp);
                    SDL_QueryTexture(labelDistanceExplosion,nullptr,nullptr,&posLabelBonus.w,&posLabelBonus.h);
                    posLabelBonus.x = posBonusHud.x;
                    SDL_RenderCopy(Jeu::getRenderer(),labelDistanceExplosion,nullptr,&posLabelBonus);
                    SDL_DestroyTexture(labelDistanceExplosion);
                }
                break;
            case 3:
                antiPosBonusHud.y = !listeBomber.at(0)->isProtection() ? hbonusHUD/2 : 0;
                break;
            case 4:
                antiPosBonusHud.y = !listeBomber.at(0)->isVitesseUp() ? hbonusHUD/2 : 0;
                break;
            }
        }


        antiPosBonusHud.x = i * antiPosBonusHud.w;
        SDL_RenderCopy(Jeu::getRenderer(),bonusHUD,&antiPosBonusHud,&posBonusHud);
        posBonusHud.x += hbonusHUD/2;
    }


     SDL_Rect posScoreLabel;
     int wScoreLabel,hScoreLabel;
     SDL_QueryTexture(scoreLabel,nullptr,nullptr,&wScoreLabel,&hScoreLabel);
     posScoreLabel.x = posFondJ1.x + wFondJoueur/2 - wScoreLabel/2;
     posScoreLabel.y = posLabelJ1.y + hJoueur1*1.5 + CASE_DIMENSION*5/2;
     posScoreLabel.h = hScoreLabel;
     posScoreLabel.w = wScoreLabel;
     SDL_RenderCopy(Jeu::getRenderer(),scoreLabel,nullptr,&posScoreLabel);
     SDL_DestroyTexture(joueur1);

     char score1[] = "000000";
     if(listeBomber.size()>0)
         sprintf(score1,"%06d",listeBomber.at(0)->getScore());

     SDL_Surface* scoreJoueur1Tmp = TTF_RenderText_Blended(police,score1,COLOR_POLICE_HUD);
     SDL_Texture* scoreJoueur1 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),scoreJoueur1Tmp);
     SDL_FreeSurface(scoreJoueur1Tmp);

     SDL_Rect posScoreJoueur;
     int wScoreJoueur1,hScoreJoueur1;
     SDL_QueryTexture(scoreJoueur1,nullptr,nullptr,&wScoreJoueur1,&hScoreJoueur1);
     posScoreJoueur.x = posFondJ1.x + wFondJoueur/2 - wScoreJoueur1/2;
     posScoreJoueur.y = posScoreLabel.y + CASE_DIMENSION/2;
     posScoreJoueur.h = hScoreJoueur1;
     posScoreJoueur.w = wScoreJoueur1;
     SDL_RenderCopy(Jeu::getRenderer(),scoreJoueur1,nullptr,&posScoreJoueur);
     SDL_DestroyTexture(scoreJoueur1);


    SDL_Rect posFondJ2;
    posFondJ2.x = posFondTimer.x;
    posFondJ2.y = (posTimer.y + hTimer) + posTimer.y/2 - hFondJoueur/2;
    posFondJ2.h = hFondJoueur;
    posFondJ2.w = wFondJoueur;
    SDL_RenderCopy(Jeu::getRenderer(),fondJoueur,nullptr,&posFondJ2);

    SDL_QueryTexture(bordureHautBas,nullptr,nullptr,&w,&h);

    posBordureHaut.x = posFondJ2.x;
    posBordureHaut.y = posFondJ2.y;
    posBordureHaut.h = h;
    posBordureHaut.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBas,nullptr,&posBordureHaut);

    posBordureBas.x = posFondJ2.x - 1;
    posBordureBas.y = posFondJ2.y + hFondJoueur;
    posBordureBas.h = h;
    posBordureBas.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureHautBas,nullptr,&posBordureBas);

    SDL_QueryTexture(bordureDroiteGauche,nullptr,nullptr,&w,&h);
    posBordureGauche.x = posFondJ2.x - 1;
    posBordureGauche.y = posFondJ2.y;
    posBordureGauche.h = h;
    posBordureGauche.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGauche,nullptr,&posBordureGauche);

    posBordureDroite.x = posFondJ2.x + wFondJoueur;
    posBordureDroite.y = posFondJ2.y;
    posBordureDroite.h = h;
    posBordureDroite.w = w;
    SDL_RenderCopy(Jeu::getRenderer(),bordureDroiteGauche,nullptr,&posBordureDroite);


    ss.str(std::string());
    ss.clear();
    if(listeBomber.size()>1)
    {
        ss << listeBomber.at(1)->getMegaVie() << "X";
    }
    else
        ss << "0X";

    s = ss.str();
    SDL_Surface* joueur2Tmp = TTF_RenderText_Shaded(police,s.c_str(),COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
    SDL_Texture* joueur2 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),joueur2Tmp);
    SDL_FreeSurface(joueur2Tmp);

    int wJoueur2,hJoueur2,wp2Head,hp2Head;
    SDL_QueryTexture(joueur2,nullptr,nullptr,&wJoueur2,&hJoueur2);
    SDL_QueryTexture(player2Head,nullptr,nullptr,&wp2Head,&hp2Head);
    SDL_Rect posLabelJ2,posHeadP2;
    posHeadP2.x = posFondJ2.x + wFondJoueur/2 - 16*2;
    posHeadP2.y = posFondJ2.y + CASE_DIMENSION/2 - hp2Head/2;
    posHeadP2.w = wp2Head;
    posHeadP2.h = hp2Head;
    posLabelJ2.x = posHeadP2.x + posHeadP2.w;
    posLabelJ2.y = posFondJ2.y + CASE_DIMENSION/2 - hJoueur2/2;
    posLabelJ2.h = hJoueur2;
    posLabelJ2.w = wJoueur2;

    SDL_RenderCopy(Jeu::getRenderer(),player2Head,nullptr,&posHeadP2);
    SDL_RenderCopy(Jeu::getRenderer(),joueur2,nullptr,&posLabelJ2);


    posCoeurBase.x = posFondJ2.x + wFondJoueur/2 - 16*2;
    posCoeurBase.y = posLabelJ2.y + hJoueur2*1.5;

    posHudLetter.x = posFondJ2.x + wFondJoueur/2 - (hLetter*5)/2;
    posHudLetter.y = posLabelJ2.y + hJoueur2*1.5 + CASE_DIMENSION + CASE_DIMENSION/5;

    bool player2 = false;
    if(listeBomber.size()>1)
    {
        if(!listeBomber.at(1)->isGameEnded())
        {
            player2 = true;
            unsigned int coeurEntier = listeBomber.at(1)->getVie()/2;
            unsigned int demiCoeur = listeBomber.at(1)->getVie() - coeurEntier*2;


            SDL_Rect posCoeur;
            posCoeur.h = CASE_DIMENSION/2;
            posCoeur.w = CASE_DIMENSION/2;

            SDL_Rect antiPosCoeur;
            antiPosCoeur.w = CASE_DIMENSION/2;
            antiPosCoeur.h = CASE_DIMENSION/2;
            antiPosCoeur.y = 0;

            for(unsigned int i = 1;i<=8;i++)
            {
                posCoeur.x = posCoeurBase.x + ((i-1)%4)*CASE_DIMENSION/2;
                posCoeur.y = posCoeurBase.y + ((i-1)/4)*CASE_DIMENSION/2;

                if(i<=coeurEntier)
                {
                    antiPosCoeur.x = 2*18;
                    SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                }
                else
                {
                    if(demiCoeur)
                    {
                        antiPosCoeur.x = 18;
                        SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                        demiCoeur = 0;
                    }
                    else
                    {
                         antiPosCoeur.x = 0;
                         SDL_RenderCopy(Jeu::getRenderer(),coeur,&antiPosCoeur,&posCoeur);
                    }

                }

            }
            for(unsigned int i = 0;i<5;i++)
            {
                antiPosHudLetter.x = (listeBomber.at(1)->getListeExtraLettre().at(i) ? i+1 : 0)*hLetter;
                SDL_RenderCopy(Jeu::getRenderer(),hudLetter,&antiPosHudLetter,&posHudLetter);
                posHudLetter.x += hLetter;
            }
        }




    }
    if(!player2)
    {
        int wGameOverLabel,hGameOverLabel;
        SDL_QueryTexture(gameOverLabel1,nullptr,nullptr,&wGameOverLabel,&hGameOverLabel);
        posCoeurBase.x = posFondJ2.x + wFondJoueur/2 - wGameOverLabel/2;
        posCoeurBase.h = hGameOverLabel;
        posCoeurBase.w = wGameOverLabel;
        SDL_RenderCopy(Jeu::getRenderer(),gameOverLabel1,nullptr,&posCoeurBase);
        posCoeurBase.y += CASE_DIMENSION/2;
        SDL_QueryTexture(gameOverLabel2,nullptr,nullptr,&wGameOverLabel,&hGameOverLabel);
        posCoeurBase.h = hGameOverLabel;
        posCoeurBase.w = wGameOverLabel;
        SDL_RenderCopy(Jeu::getRenderer(),gameOverLabel2,nullptr,&posCoeurBase);

        antiPosHudLetter.x = 0;
        for(unsigned int i = 0;i<5;i++)
        {
            SDL_RenderCopy(Jeu::getRenderer(),hudLetter,&antiPosHudLetter,&posHudLetter);
            posHudLetter.x += hLetter;
        }

    }


        antiPosBonusHud.h = hbonusHUD/2;
        antiPosBonusHud.w = hbonusHUD/2;
        antiPosBonusHud.y = hbonusHUD/2;
        posBonusHud.h = hbonusHUD/2;
        posBonusHud.w = hbonusHUD/2;
        posBonusHud.y = posLabelJ2.y + hJoueur2*1.5 + CASE_DIMENSION*1.75;
        posBonusHud.x = posFondJ2.x + wFondJoueur/2 - (hbonusHUD/2*5)/2;
        posLabelBonus.y = posBonusHud.y + posBonusHud.h;

        for(unsigned int i = 0;i<5;i++)
        {
            if(listeBomber.size() > 1)
            {
                switch(i)
                {
                case 0:
                     antiPosBonusHud.y = hbonusHUD;
                    if(listeBomber.at(1)->getNombreBombes() > 0)
                    {
                        antiPosBonusHud.y = 0;
                        char lab[] = "x0";
                        sprintf(lab,"x%1d",listeBomber.at(1)->getNombreBombes());
                        SDL_Surface* labelNbBombesTmp = TTF_RenderText_Shaded(policeBonus,lab,COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
                        SDL_Texture* labelNbBombes = SDL_CreateTextureFromSurface(Jeu::getRenderer(),labelNbBombesTmp);
                        SDL_FreeSurface(labelNbBombesTmp);
                        SDL_QueryTexture(labelNbBombes,nullptr,nullptr,&posLabelBonus.w,&posLabelBonus.h);
                        posLabelBonus.x = posBonusHud.x;
                        SDL_RenderCopy(Jeu::getRenderer(),labelNbBombes,nullptr,&posLabelBonus);
                        SDL_DestroyTexture(labelNbBombes);
                    }
                    break;
                case 1:
                    antiPosBonusHud.y = !listeBomber.at(1)->isBombeRapide() ? hbonusHUD/2 : 0;
                    break;
                case 2:
                    antiPosBonusHud.y = hbonusHUD;
                    if(listeBomber.at(1)->getDistanceExplosion() > 0)
                    {
                        antiPosBonusHud.y = 0;
                        char lab[] = "x0";
                        sprintf(lab,"x%1d",listeBomber.at(1)->getDistanceExplosion());
                        SDL_Surface* labelDistanceExplosionTmp = TTF_RenderText_Shaded(policeBonus,lab,COLOR_POLICE_HUD,COLOR_BG_JOUEUR);
                        SDL_Texture* labelDistanceExplosion = SDL_CreateTextureFromSurface(Jeu::getRenderer(),labelDistanceExplosionTmp);
                        SDL_FreeSurface(labelDistanceExplosionTmp);
                        SDL_QueryTexture(labelDistanceExplosion,nullptr,nullptr,&posLabelBonus.w,&posLabelBonus.h);
                        posLabelBonus.x = posBonusHud.x;
                        SDL_RenderCopy(Jeu::getRenderer(),labelDistanceExplosion,nullptr,&posLabelBonus);
                        SDL_DestroyTexture(labelDistanceExplosion);
                    }
                    break;
                case 3:
                    antiPosBonusHud.y = !listeBomber.at(1)->isProtection() ? hbonusHUD/2 : 0;
                    break;
                case 4:
                    antiPosBonusHud.y = !listeBomber.at(1)->isVitesseUp() ? hbonusHUD/2 : 0;
                    break;
                }
            }

            antiPosBonusHud.x = i * antiPosBonusHud.w;
            SDL_RenderCopy(Jeu::getRenderer(),bonusHUD,&antiPosBonusHud,&posBonusHud);
            posBonusHud.x += hbonusHUD/2;
        }


     SDL_QueryTexture(scoreLabel,nullptr,nullptr,&wScoreLabel,&hScoreLabel);
     posScoreLabel.x = posFondJ2.x + wFondJoueur/2 - wScoreLabel/2;
     posScoreLabel.y = posLabelJ2.y + hJoueur2*1.5 + CASE_DIMENSION*5/2;
     posScoreLabel.h = hScoreLabel;
     posScoreLabel.w = wScoreLabel;
     SDL_RenderCopy(Jeu::getRenderer(),scoreLabel,nullptr,&posScoreLabel);
     SDL_DestroyTexture(joueur2);

     char score2[] = "000000";
     if(listeBomber.size()>1)
         sprintf(score2,"%06d",listeBomber.at(1)->getScore());

     SDL_Surface* scoreJoueur2Tmp = TTF_RenderText_Blended(police,score2,COLOR_POLICE_HUD);
     SDL_Texture* scoreJoueur2 = SDL_CreateTextureFromSurface(Jeu::getRenderer(),scoreJoueur2Tmp);
     SDL_FreeSurface(scoreJoueur2Tmp);

     int wScoreJoueur2,hScoreJoueur2;
     SDL_QueryTexture(scoreJoueur2,nullptr,nullptr,&wScoreJoueur2,&hScoreJoueur2);
     posScoreJoueur.x = posFondJ2.x + wFondJoueur/2 - wScoreJoueur2/2;
     posScoreJoueur.y = posScoreLabel.y + CASE_DIMENSION/2;
     posScoreJoueur.h = hScoreJoueur2;
     posScoreJoueur.w = wScoreJoueur2;
     SDL_RenderCopy(Jeu::getRenderer(),scoreJoueur2,nullptr,&posScoreJoueur);
     SDL_DestroyTexture(scoreJoueur2);

}

void MenuJeu::setPause()
{
    if(!preparationNiveau)
    {
         pause = !pause;
        if(!pause)
        {
            tempsReprise = SDL_GetTicks();
            tempsSommePause += tempsReprise - tempsPause;
        }
        else
            tempsPause = SDL_GetTicks();
    }

}

int MenuJeu::getTempsPause()
{
    return tempsSommePause;
}

void MenuJeu::afficherMessage()
{
    if(Ennemi::getAlienMode() && !extraMessage)
    {
        SDL_Rect pos = {posExtra.x,posExtra.y,posExtra.w,posExtra.h};
        SDL_RenderCopy(Jeu::getRenderer(),extraGame,nullptr,&pos);
    }

    if(!hurryMessage && sonHurryDone)
    {
        SDL_Rect pos2 = {posHurry.x,posHurry.y,posHurry.w,posHurry.h};
        SDL_RenderCopy(Jeu::getRenderer(),hurryUp,nullptr,&pos2);
    }

    if(!gameOverMessage && sonGameOver)
    {
        SDL_Rect pos3 = {posGameOver.x,posGameOver.y,posGameOver.w,posGameOver.h};
        SDL_RenderCopy(Jeu::getRenderer(),gameOver,nullptr,&pos3);
    }
}

void MenuJeu::calculEnergyBar()
{
    if(carte->hasBos())
    {
        if(carte->getBoss())
        {
            if(carte->getBoss()->getVie() !=0)
            {
                 if(posEnergyBar.y != 2)
                    posEnergyBar.y+=1;
            }
            else
            {
                if(posEnergyBar.y != -posEnergyBar.h)
                    posEnergyBar.y-=1;
            }
        }

    }


}

void MenuJeu::calculExtra()
{
    if(!Carte::isNoCoin())
    {
        for(unsigned int i = 0;i<carte->getListeCoin().size();i++)
        {
            if(!carte->getListeCoin().at(i)->isPickUp())
                return;
        }

        if(Ennemi::getAlienMode())
        {
            if(getTime() - tempsAlienModeActif > TEMPS_ALIEN_MODE)
            {
                Ennemi::setAlienMode(false);
                for(unsigned int i = 0;i<carte->getListeLettre().size();i++)
                {
                    carte->retirerLettre(i);
                    i--;
                }
            }
        }
        if(!extraMessage)
        {
            if(!sonExtraDone)
            {
                Mix_PlayChannel(-1,sonExtra,0);
                sonExtraDone = true;
                Ennemi::setAlienMode(true);
                tempsAlienModeActif = getTime();
            }

            int wExtra,hExtra;
            SDL_QueryTexture(extraGame,nullptr,nullptr,&wExtra,&hExtra);
            if(posExtra.y  == WINDOW_HEIGHT/2 - hExtra/2)
            {
                if(!extraStop)
                {
                    extraStop = true;
                    tempsExtraStandByAfter = getTime();
                }

                if(getTime() - tempsExtraStandByAfter < TEMPS_HURRY_STANDBY)
                    return;
            }

             if((posExtra.y + vitesseExtra  > WINDOW_HEIGHT/2 - hExtra/2) && !extraStop)
                 posExtra.y += vitesseExtra - ((posExtra.y + vitesseExtra) - (WINDOW_HEIGHT/2 - (hExtra/2)));

            else
                posExtra.y += vitesseExtra;

            if(posExtra.y >= WINDOW_HEIGHT)
            {
                extraMessage = true;
            }
        }
    }
}

void MenuJeu::afficherNumberLevel()
{
    SDL_Rect posNumberLevel;
    int w,h;
    SDL_QueryTexture(numberLevel,nullptr,nullptr,&w,&h);
    posNumberLevel.x = WINDOW_WIDTH - w;
    posNumberLevel.y = 0;
    posNumberLevel.w = w;
    posNumberLevel.h = h;
    SDL_RenderCopy(Jeu::getRenderer(),numberLevelBackGround,nullptr,&posNumberLevel);
    posNumberLevel.y = 2;
    SDL_RenderCopy(Jeu::getRenderer(),numberLevel,nullptr,&posNumberLevel);
}

void MenuJeu::calculHury()
{
    if(carte->getTemps() - carte->getTempsEcoule() <= 30000)
    {
        if(!hurryMessage)
        {
            if(!sonHurryDone)
            {
                Mix_PlayChannel(-1,sonHurry,0);
                sonHurryDone = true;
            }

            int wHurry,hHurry;
            SDL_QueryTexture(hurryUp,nullptr,nullptr,&wHurry,&hHurry);
            if(posHurry.y  == WINDOW_HEIGHT/2 - hHurry/2)
            {
                if(!hurryStop)
                {
                    hurryStop = true;
                    tempsHuryStandByAfter = getTime();
                }

                if(getTime() - tempsHuryStandByAfter < TEMPS_HURRY_STANDBY)
                    return;
            }

             if((posHurry.y + vitesseHury  > WINDOW_HEIGHT/2 - hHurry/2) && !hurryStop)
                 posHurry.y += vitesseHury - ((posHurry.y + vitesseHury) - (WINDOW_HEIGHT/2 - (hHurry/2)));

            else
                posHurry.y += vitesseHury;

            if(posHurry.y >= WINDOW_HEIGHT)
            {
                hurryMessage = true;
            }
        }
    }
}

void MenuJeu::calculGameOver()
{
    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        if(!listeBomber.at(i)->isGameEnded())
            return;
    }

    if(!gameOverMessage)
    {
        if(!sonGameOverDone)
        {
            Mix_PauseMusic();
            Mix_PlayChannel(-1,sonGameOver,0);
            sonGameOverDone = true;
        }

          int wGameOver,hGameOver;
          SDL_QueryTexture(gameOver,nullptr,nullptr,&wGameOver,&hGameOver);
          if(posGameOver.y  == WINDOW_HEIGHT/2 - hGameOver/2)
            {
                if(!gameOverStop)
                {
                    gameOverStop = true;
                    tempsGameOverStandByAfter = getTime();
                }

                if(getTime() - tempsGameOverStandByAfter < TEMPS_HURRY_STANDBY)
                    return;
            }

            if((posGameOver.y + vitesseGameOver  > WINDOW_HEIGHT/2 - hGameOver/2) && !gameOverStop)
                 posGameOver.y += vitesseGameOver - ((posGameOver.y + vitesseGameOver) - (WINDOW_HEIGHT/2 - (hGameOver/2)));

            else
                posGameOver.y += vitesseGameOver;

            if(posGameOver.y >= WINDOW_HEIGHT)
            {
                gameOverMessage = true;
                tempsGameOverTransition = getTime();
            }
    }
    else
    {
        if(getTime() - tempsGameOverTransition > TEMPS_BEFORE_TRANSITION)
        {
            if(!transition && !transitionOn)
            {
                 transition = new Transition(true,1000);
                 transitionOn = true;
            }
            else
            {
                 if(transitionFinie)
                    goBackToMenu = true;
            }

        }
    }
}


void MenuJeu::afficherProjectile()
{
    for(unsigned int i = 0;i<carte->getListeProjectile().size();i++)
    {
        if(!carte->getListeProjectile().at(i)->isFini())
            SDL_RenderCopy(Jeu::getRenderer(),carte->getListeProjectile().at(i)->getImage(),carte->getListeProjectile().at(i)->getAdresseAntiPosition(),carte->getListeProjectile().at(i)->getAdressePosition());
    }
}
void MenuJeu::afficherCaseBordure()
{
    SDL_Rect pos = {0,0,CASE_DIMENSION,CASE_DIMENSION};
    SDL_Rect antiPos = {0,0,CASE_DIMENSION,CASE_DIMENSION};

    antiPos.x = BORDURE_COIN_HAUT_GAUCHE * CASE_DIMENSION;
    pos.x = CASE_DIMENSION*3;
    SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);

    antiPos.x = BORDURE_COIN_HAUT_DROITE * CASE_DIMENSION;
    pos.x = WINDOW_WIDTH - CASE_DIMENSION;
    SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);

    antiPos.x = BORDURE_COIN_BAS_GAUCHE * CASE_DIMENSION;
    pos.x = CASE_DIMENSION*3;
    pos.y = WINDOW_HEIGHT - CASE_DIMENSION;
    SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);

    antiPos.x = BORDURE_COIN_BAS_DROITE * CASE_DIMENSION;
    pos.x = WINDOW_WIDTH - CASE_DIMENSION;
    pos.y = WINDOW_HEIGHT - CASE_DIMENSION;
    SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);

    for(unsigned int i = 1;i<=CASE_LONGUEUR;i++)
    {
        pos.x = CASE_DIMENSION*3 + i*CASE_DIMENSION;
        pos.y = 0;
        antiPos.x = BORDURE_HAUT * CASE_DIMENSION;
        SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);
        antiPos.x = BORDURE_BAS * CASE_DIMENSION;
        pos.y = CASE_DIMENSION * (CASE_HAUTEUR+1);
        SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);
    }
    for(unsigned int i = 1;i<=CASE_HAUTEUR;i++)
    {
        pos.x = CASE_DIMENSION*3;
        pos.y = CASE_DIMENSION * i;
        antiPos.x = BORDURE_GAUCHE * CASE_DIMENSION;
        SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);
        pos.x = CASE_DIMENSION*3 + CASE_DIMENSION * (CASE_LONGUEUR+1);
        antiPos.x = BORDURE_DROITE * CASE_DIMENSION;
        SDL_RenderCopy(Jeu::getRenderer(),carte->getCaseBordure(),&antiPos,&pos);
    }
}

void MenuJeu::ajouterScore(unsigned int score,int posX,int posY,Bomber* bomber)
{
    carte->ajouterScore(new Score(posX,posY,score));
    if(score>9)
        bomber->augmenterScore(score);
}

void MenuJeu::afficherScore()
{
    for(unsigned int i = 0;i<carte->getListeScore().size();i++)
    {

        if(!carte->getListeScore().at(i)->isFini())
            SDL_RenderCopy(Jeu::getRenderer(),carte->getListeScore().at(i)->getSurface(),carte->getListeScore().at(i)->getAdresseAntiPosition(),carte->getListeScore().at(i)->getAdressePosition());
        else
        {
            carte->retirerScore(i);
            i--;
        }
    }
}

void MenuJeu::afficherCoin()
{
    for(unsigned int i = 0;i<carte->getListeCoin().size();i++)
    {
        if(!carte->getListeCoin().at(i)->isToRemove())
            SDL_RenderCopy(Jeu::getRenderer(),carte->getListeCoin().at(i)->getImage(),carte->getListeCoin().at(i)->getAdresseAntiPosition(),carte->getListeCoin().at(i)->getAdressePosition());
        else
        {
              carte->retirerCoin(i);
              i--;
        }
    }
}
void MenuJeu::afficherEnnemi()
{
    for(unsigned int i = 0;i<carte->getListeEnnemi().size();i++)
    {
        if(!carte->getListeEnnemi().at(i)->isMort())
        {
            if(carte->getListeEnnemi().at(i)->isApparaitre())
                SDL_RenderCopy(Jeu::getRenderer(),carte->getListeEnnemi().at(i)->getImage(),carte->getListeEnnemi().at(i)->getAdresseAntiPosition(),carte->getListeEnnemi().at(i)->getAdressePosition());

        }
    }

}
void MenuJeu::afficherBonus()
{
    for(unsigned int i = 0;i<carte->getListeBonus().size();i++)
    {
        if(carte->getListeBonus().at(i)->isDisponible())
        {
            if(carte->getListeBonus().at(i)->isApparaitre())
                SDL_RenderCopy(Jeu::getRenderer(),carte->getListeBonus().at(i)->getImage(),carte->getListeBonus().at(i)->getAdresseAntiPosition(),carte->getListeBonus().at(i)->getCase()->getAdressePositionPixel());
        }
        else
        {
            carte->getListeBonus().at(i)->getCase()->setBonus(nullptr);
            delete carte->getListeBonus().at(i);
            carte->getListeBonus().erase(carte->getListeBonus().begin() + i);
            i--;
        }
    }
}

void MenuJeu::afficherTeleporter()
{
    for(unsigned int i = 0;i<carte->getListeTeleporter().size();i++)
       SDL_RenderCopy(Jeu::getRenderer(),carte->getListeTeleporter().at(i)->getImage(),carte->getListeTeleporter().at(i)->getAntiPosition(),carte->getListeTeleporter().at(i)->getCase()->getAdressePositionPixel());
}

void MenuJeu::afficherEffet()
{
    for(unsigned int i = 0;i<carte->getListeEffet().size();i++)
    {
        if(!carte->getListeEffet().at(i)->isFini())
            SDL_RenderCopy(Jeu::getRenderer(),carte->getListeEffet().at(i)->getImage(),carte->getListeEffet().at(i)->getAdresseAntiPosition(),carte->getListeEffet().at(i)->getAdressePosition());
        else
        {
            delete carte->getListeEffet().at(i);
            carte->getListeEffet().erase(carte->getListeEffet().begin() + i);
            i--;
        }
    }
}

void MenuJeu::afficherCaseFixe()
{
    for(int i = 1;i<=CASE_HAUTEUR;i++)
    {
        for(int j = 1;j<=CASE_LONGUEUR;j++)
        {
            SDL_RenderCopy(Jeu::getRenderer(),Case::getImageFond(),0,Carte::getCase(i,j)->getAdressePositionPixel());
            if(Carte::getCase(i,j)->getType() == DUR)
                SDL_RenderCopy(Jeu::getRenderer(),Carte::getCase(i,j)->getImage(),Carte::getCase(i,j)->getAdresseAntiPositionPixel(),Carte::getCase(i,j)->getAdressePositionPixel());
        }
    }
}

void MenuJeu::afficherCaseCassable()
{
    for(int i = 1;i<=CASE_HAUTEUR;i++)
    {
        for(int j = 1;j<=CASE_LONGUEUR;j++)
        {
            if(Carte::getCase(i,j)->getType() == CASSABLE)
                SDL_RenderCopy(Jeu::getRenderer(),Carte::getCase(i,j)->getImage(),Carte::getCase(i,j)->getAdresseAntiPositionPixel(),Carte::getCase(i,j)->getAdressePositionPixel());
        }
    }
}

void MenuJeu::afficherLettre()
{
    for(unsigned int i = 0;i<carte->getListeLettre().size();i++)
        SDL_RenderCopy(Jeu::getRenderer(),carte->getListeLettre().at(i)->getImage(),carte->getListeLettre().at(i)->getAdresseAntiPosition(),carte->getListeLettre().at(i)->getAdressePosition());
}

void MenuJeu::afficherBomber()
{
    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        if(!listeBomber.at(i)->isGameEnded())
        {
            SDL_RenderCopy(Jeu::getRenderer(),listeBomber.at(i)->getImage(),listeBomber.at(i)->getAdresseAntiPosition(),listeBomber.at(i)->getAdressePosition());

            if((listeBomber.at(i)->isImmunise() && listeBomber.at(i)->isApparaitreProtection()) || (listeBomber.at(i)->isTmpImmunise() && listeBomber.at(i)->isApparaitreTmpProtection()))
                SDL_RenderCopy(Jeu::getRenderer(),listeBomber.at(i)->getImageProtection(),listeBomber.at(i)->getAdresseAntiPositionProtection(),listeBomber.at(i)->getAdressePosition());
        }

    }

}

void MenuJeu::afficherExplosion()
{
    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        for(unsigned int j = 0;j<listeBomber.at(i)->getListeBombes().size();j++)
        {
            for(unsigned int k = 0;k<listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().size();k++)
                SDL_RenderCopy(Jeu::getRenderer(),listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k)->getImage(),listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k)->getAntiPositionAdresse(),listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k)->getCase()->getAdressePositionPixel());
        }
    }

}

void MenuJeu::afficherEnergyBar()
{
    if(carte->hasBos())
    {
        unsigned int vieMax = ALIENBOSS_VIEMAX;
        unsigned int vieCourante = carte->getBoss() ? carte->getBoss()->getVie() : 0;
        SDL_RenderCopy(Jeu::getRenderer(),bossEnergyBar,nullptr,&posEnergyBar);
        int tranche = vieMax/40;
        antiPosEnergyBarBlack.w =  (40 - (vieCourante/tranche))*7;
        posEnergyBarBlack.y = posEnergyBar.y + 2;
        posEnergyBarBlack.w = antiPosEnergyBarBlack.w;
        posEnergyBarBlack.x = posEnergyBar.x + posEnergyBar.w - antiPosEnergyBarBlack.w - 2;
        SDL_RenderCopy(Jeu::getRenderer(),bossEnergyBarBlack,&antiPosEnergyBarBlack,&posEnergyBarBlack);
    }

}

void MenuJeu::afficherBombe()
{
    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        for(unsigned int j = 0;j<listeBomber.at(i)->getListeBombes().size();j++)
        {
            if(!(listeBomber.at(i)->getListeBombes().at(j)->isExplose()))
                 SDL_RenderCopy(Jeu::getRenderer(),listeBomber.at(i)->getListeBombes().at(j)->getImage(),listeBomber.at(i)->getListeBombes().at(j)->getAntiPositionAdresse(),listeBomber.at(i)->getListeBombes().at(j)->getCase()->getAdressePositionPixel());
        }

    }
}

void MenuJeu::executeBombe()
{
    for(unsigned int i = 0;i<listeBomber.size();i++)
    {
        for(unsigned int j = 0;j<listeBomber.at(i)->getListeBombes().size();j++)
        {
            if(!(listeBomber.at(i)->getListeBombes().at(j)->isExplose()))
                  listeBomber.at(i)->getListeBombes().at(j)->execute();

            for(unsigned int k = 0;k<listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().size();k++)
            {
                if(!listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k)->isExplosionFinie())
                    listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k)->execute();

                else
                {
                    delete listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().at(k);
                    listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().erase(listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().begin() + k);
                    k--;

                }
            }


            if(listeBomber.at(i)->getListeBombes().at(j)->isExplose())
            {
                if(listeBomber.at(i)->getListeBombes().at(j)->getListeExplosions().empty())
                {
                    delete listeBomber.at(i)->getListeBombes().at(j);
                    listeBomber.at(i)->getListeBombes().erase(listeBomber.at(i)->getListeBombes().begin()+j);
                    j--;
                }
            }



        }
    }
}

void MenuJeu::executeEffet()
{
    for(unsigned int i = 0;i<carte->getListeEffet().size();i++)
        carte->getListeEffet().at(i)->execute();
}

void MenuJeu::executeTeleporter()
{
    for(unsigned int i = 0;i<carte->getListeTeleporter().size();i++)
        carte->getListeTeleporter().at(i)->execute();
}

void MenuJeu::executeCoin()
{
    for(unsigned int i = 0;i<carte->getListeCoin().size();i++)
        carte->getListeCoin().at(i)->execute();
}

void MenuJeu::executeCase()
{
    for(unsigned int i = 1;i<=CASE_HAUTEUR;i++)
    {
        for(unsigned int j = 1;j<=CASE_LONGUEUR;j++)
        {
            if(Carte::getCase(i,j)->getType() == CASSABLE)
                Carte::getCase(i,j)->execute();
        }
    }
}

void MenuJeu::executeProjectile()
{
    for(unsigned int i = 0;i<carte->getListeProjectile().size();i++)
    {
        if(!carte->getListeProjectile().at(i)->isFini())
            carte->getListeProjectile().at(i)->deplacer();
        else
        {
            delete carte->getListeProjectile().at(i);
            carte->getListeProjectile().erase(carte->getListeProjectile().begin() + i);
            i--;
        }
    }
}

void MenuJeu::executeLetter()
{
    for(unsigned int i = 0;i<carte->getListeLettre().size();i++)
        carte->getListeLettre().at(i)->execute();
}
void MenuJeu::executeEnnemi()
{
    for(unsigned int i = 0;i<carte->getListeEnnemi().size();i++)
    {
        if(!carte->getListeEnnemi().at(i)->isMort())
            carte->getListeEnnemi().at(i)->deplacerEnnemi();
        else
        {
            carte->retirerEnnemi(i);
            i--;

            if(carte->getListeEnnemi().empty())
                finirNiveau();

        }
    }

}
void MenuJeu::executeBonus()
{
    for(unsigned int i = 0;i<carte->getListeBonus().size();i++)
      carte->getListeBonus().at(i)->calculAffichageBonus();
}

void MenuJeu::executeScore()
{
    for(unsigned int i = 0;i<carte->getListeScore().size();i++)
    {
        if(getTime() - carte->getListeScore().at(i)->getTimer() > TEMPS_SCORE)
            carte->getListeScore().at(i)->setFini(true);
        else
            carte->getListeScore().at(i)->setPosition(carte->getListeScore().at(i)->getPositionReal().x,carte->getListeScore().at(i)->getPositionReal().y-0.3);
    }
}

vector <Bomber*>& MenuJeu::getListeBomber()
{
    return listeBomber;
}

void MenuJeu::deplacerBomber()
{
    for(unsigned int i=0;i<listeBomber.size();i++)
    {
        if(!listeBomber.at(i)->isGameEnded())
            listeBomber.at(i)->deplacer(input);
    }
}

