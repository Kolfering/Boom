#include "MenuOption.h"
#include "Libelle.h"
#include "CheckBox.h"
#include "VolumeBar.h"
#include "TextBox.h"
#include "BoutonRetour.h"
#include "Jeu.h"
#include <sys/stat.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <fstream>


MenuOption::MenuOption(Input* input,InputMouse* inputMouse,SDL_Window* sdl_window) : MenuTemplate(input,inputMouse,sdl_window)
{
   idMenu = IDMenu::MENU_OPTION;

   int w,h;
   BoutonRetour* boutonRetour = new BoutonRetour(this,MENU_PRINCIPAL);

   SDL_QueryTexture(boutonRetour->getImage(),nullptr,nullptr,&w,&h);

   boutonRetour->getAdressePosition()->x = 0;
   boutonRetour->getAdressePosition()->y = WINDOW_HEIGHT - h;

   Libelle* libelle = new Libelle("Player Mode",this,{240,220,230},25);
   SDL_Rect pos = {WINDOW_WIDTH/10,WINDOW_HEIGHT/10,0,0};
   libelle->setPosition(pos.x,pos.y);

   Libelle* libelle1 = new Libelle("1 Player",this,{240,220,230},20);
   SDL_Rect pos1 = {pos.x + libelle->getTaille().x/3,pos.y + libelle->getTaille().y,0,0};
   libelle1->setPosition(pos1.x,pos1.y);

   Libelle* libelle2 = new Libelle("2 Players",this,{240,220,230},20);
   SDL_Rect pos2 = {pos1.x,pos1.y + libelle2->getTaille().y,0,0};
   libelle2->setPosition(pos2.x,pos2.y);

   Libelle* libelle3 = new Libelle("Volume",this,{240,220,230},25);
   SDL_Rect pos3 = {WINDOW_WIDTH*3/5,WINDOW_HEIGHT/10,0,0};
   libelle3->setPosition(pos3.x,pos3.y);

   Libelle* libelle4 = new Libelle("Music",this,{240,220,230},20);
   SDL_Rect pos4 = {pos3.x + libelle3->getTaille().x/3,pos3.y + libelle3->getTaille().y,0,0};
   libelle4->setPosition(pos4.x,pos4.y);

   Libelle* libelle5 = new Libelle("Sound",this,{240,220,230},20);
   SDL_Rect pos5 = {pos4.x,pos4.y + libelle4->getTaille().y,0,0};
   libelle5->setPosition(pos5.x,pos5.y);

   Libelle* libelle6 = new Libelle("Player 1 Keyboard",this,{240,220,230},20);
   SDL_Rect pos6 = {pos.x,WINDOW_HEIGHT/2,0,0};
   libelle6->setPosition(pos6.x,pos6.y);

   Libelle* libelle61 = new Libelle("Up",this,{240,220,230},18);
   SDL_Rect pos61 = {pos.x,pos6.y + libelle6->getTaille().y*2,0,0};
   libelle61->setPosition(pos61.x,pos61.y);

   Libelle* libelle62 = new Libelle("Down",this,{240,220,230},18);
   SDL_Rect pos62 = {pos.x,pos61.y + libelle61->getTaille().y,0,0};
   libelle62->setPosition(pos62.x,pos62.y);

   Libelle* libelle63 = new Libelle("Right",this,{240,220,230},18);
   SDL_Rect pos63 = {pos.x,pos62.y + libelle62->getTaille().y,0,0};
   libelle63->setPosition(pos63.x,pos63.y);

   Libelle* libelle64 = new Libelle("Left",this,{240,220,230},18);
   SDL_Rect pos64 = {pos.x,pos63.y + libelle63->getTaille().y,0,0};
   libelle64->setPosition(pos64.x,pos64.y);

   Libelle* libelle65 = new Libelle("Bomb",this,{240,220,230},18);
   SDL_Rect pos65 = {pos.x,pos64.y + libelle64->getTaille().y,0,0};
   libelle65->setPosition(pos65.x,pos65.y);

   TextBox* textBox61 = new TextBox(this,SDL_SCANCODE_W);
   SDL_Rect pos611 = {pos.x + 75,pos61.y,0,0};
   textBox61->setPosition(pos611.x,pos611.y);

   TextBox* textBox62 = new TextBox(this,SDL_SCANCODE_S);
   SDL_Rect pos621 = {pos.x + 75,pos62.y,0,0};
   textBox62->setPosition(pos621.x,pos621.y);

   TextBox* textBox63 = new TextBox(this,SDL_SCANCODE_D);
   SDL_Rect pos631 = {pos.x + 75,pos63.y,0,0};
   textBox63->setPosition(pos631.x,pos631.y);

   TextBox* textBox64 = new TextBox(this,SDL_SCANCODE_A);
   SDL_Rect pos641 = {pos.x + 75,pos64.y,0,0};
   textBox64->setPosition(pos641.x,pos641.y);

   TextBox* textBox65 = new TextBox(this,SDL_SCANCODE_SPACE);
   SDL_Rect pos651 = {pos.x + 75,pos65.y,0,0};
   textBox65->setPosition(pos651.x,pos651.y);

   Libelle* libelle7 = new Libelle("Player 2 Keyboard",this,{240,220,230},20);
   SDL_Rect pos8 = {pos3.x,WINDOW_HEIGHT/2,0,0};
   libelle7->setPosition(pos8.x,pos8.y);

   Libelle* libelle71 = new Libelle("Up",this,{240,220,230},18);
   SDL_Rect pos71 = {pos3.x,pos8.y + libelle7->getTaille().y *2 ,0,0};
   libelle71->setPosition(pos71.x,pos71.y);

   Libelle* libelle72 = new Libelle("Down",this,{240,220,230},18);
   SDL_Rect pos72 = {pos3.x,pos71.y + libelle71->getTaille().y,0,0};
   libelle72->setPosition(pos72.x,pos72.y);

   Libelle* libelle73 = new Libelle("Right",this,{240,220,230},18);
   SDL_Rect pos73 = {pos3.x,pos72.y + libelle62->getTaille().y,0,0};
   libelle73->setPosition(pos73.x,pos73.y);

   Libelle* libelle74 = new Libelle("Left",this,{240,220,230},18);
   SDL_Rect pos74 = {pos3.x,pos73.y + libelle73->getTaille().y,0,0};
   libelle74->setPosition(pos74.x,pos74.y);

   Libelle* libelle75 = new Libelle("Bomb",this,{240,220,230},18);
   SDL_Rect pos75 = {pos3.x,pos74.y + libelle74->getTaille().y,0,0};
   libelle75->setPosition(pos75.x,pos75.y);

   TextBox* textBox71 = new TextBox(this,SDL_SCANCODE_UP);
   SDL_Rect pos711 = {pos3.x + 75,pos71.y,0,0};
   textBox71->setPosition(pos711.x,pos711.y);

   TextBox* textBox72 = new TextBox(this,SDL_SCANCODE_DOWN);
   SDL_Rect pos721 = {pos3.x + 75,pos72.y,0,0};
   textBox72->setPosition(pos721.x,pos721.y);

   TextBox* textBox73 = new TextBox(this,SDL_SCANCODE_RIGHT);
   SDL_Rect pos731 = {pos3.x + 75,pos73.y,0,0};
   textBox73->setPosition(pos731.x,pos731.y);

   TextBox* textBox74 = new TextBox(this,SDL_SCANCODE_LEFT);
   SDL_Rect pos741 = {pos3.x + 75,pos74.y,0,0};
   textBox74->setPosition(pos741.x,pos741.y);

   TextBox* textBox75 = new TextBox(this,SDL_SCANCODE_RCTRL);
   SDL_Rect pos751 = {pos3.x + 75,pos75.y,0,0};
   textBox75->setPosition(pos751.x,pos751.y);

   VolumeBar* volumeBar = new VolumeBar(this);
   volumeBar->setPosition(pos4.x + libelle4->getTaille().x + libelle4->getTaille().x/4,pos4.y);

   VolumeBar* volumeBar2 = new VolumeBar(this);
   volumeBar2->setPosition(pos5.x + libelle5->getTaille().x + libelle5->getTaille().x/4,pos5.y);

   Libelle* libellePad1 = new Libelle("Pad",this,{240,220,230},18);
   SDL_Rect posPad1 = {pos.x,pos65.y + libellePad1->getTaille().y *2,0,0};
   libellePad1->setPosition(posPad1.x,posPad1.y);

   Libelle* libellePad2 = new Libelle("Pad",this,{240,220,230},18);
   SDL_Rect posPad2 = {pos3.x,pos75.y + libellePad2->getTaille().y*2,0,0};
   libellePad2->setPosition(posPad2.x,posPad2.y);

   CheckBox* checkBox1 = new CheckBox(this,true);
   CheckBox* checkBox2 = new CheckBox(this);

   SDL_QueryTexture(checkBox1->getImage(),nullptr,nullptr,&w,&h);

   checkBox1->getAdressePosition()->y = pos1.y;
   checkBox1->getAdressePosition()->x = pos1.x - w*2;

   checkBox2->getAdressePosition()->y = pos2.y;
   checkBox2->getAdressePosition()->x = pos2.x - w*2;

   GroupCheckBox* groupCheckBox = new GroupCheckBox(this);
   groupCheckBox->ajouterCheckBox(checkBox1);
   groupCheckBox->ajouterCheckBox(checkBox2);

   CheckBox* checkBoxPad1 = new CheckBox(this);
   CheckBox* checkBoxPad2 = new CheckBox(this);

   checkBoxPad1->getAdressePosition()->y = libellePad1->getAdressePosition()->y;
   checkBoxPad1->getAdressePosition()->x = textBox65->getAdressePosition()->x + textBox65->getAdressePosition()->w/2 - checkBoxPad1->getAdressePosition()->w/2;

   checkBoxPad2->getAdressePosition()->y = libellePad2->getAdressePosition()->y;
   checkBoxPad2->getAdressePosition()->x = textBox75->getAdressePosition()->x + textBox75->getAdressePosition()->w/2 - checkBoxPad2->getAdressePosition()->w/2;

   listeWidget["libelleP1"] = libelle6;
   listeWidget["libelleP11"] = libelle61;
   listeWidget["libelleP12"] = libelle62;
   listeWidget["libelleP13"] = libelle63;
   listeWidget["libelleP14"] = libelle64;
   listeWidget["libelleP15"] = libelle65;

   listeWidget["libelleP2"] = libelle7;
   listeWidget["libelleP21"] = libelle71;
   listeWidget["libelleP22"] = libelle72;
   listeWidget["libelleP23"] = libelle73;
   listeWidget["libelleP24"] = libelle74;
   listeWidget["libelleP25"] = libelle75;

   listeWidget["textBoxLibelle61"] = textBox61->getLibelle();
   listeWidget["textBox61"] = textBox61;
   listeWidget["textBoxLibelle62"] = textBox62->getLibelle();
   listeWidget["textBox62"] = textBox62;
   listeWidget["textBoxLibelle63"] = textBox63->getLibelle();
   listeWidget["textBox63"] = textBox63;
   listeWidget["textBoxLibelle64"] = textBox64->getLibelle();
   listeWidget["textBox64"] = textBox64;
   listeWidget["textBoxLibelle65"] = textBox65->getLibelle();
   listeWidget["textBox65"] = textBox65;

   listeWidget["textBoxLibelle71"] = textBox71->getLibelle();
   listeWidget["textBox71"] = textBox71;
   listeWidget["textBoxLibelle72"] = textBox72->getLibelle();
   listeWidget["textBox72"] = textBox72;
   listeWidget["textBoxLibelle73"] = textBox73->getLibelle();
   listeWidget["textBox73"] = textBox73;
   listeWidget["textBoxLibelle74"] = textBox74->getLibelle();
   listeWidget["textBox74"] = textBox74;
   listeWidget["textBoxLibelle75"] = textBox75->getLibelle();
   listeWidget["textBox75"] = textBox75;

   listeWidget["boutonRetour"] = boutonRetour;
   listeWidget["libellePlayerMode"] = libelle;
   listeWidget["libellePlayer1"] = libelle1;
   listeWidget["libellePlayer2"] = libelle2;
   listeWidget["libelleSound"] = libelle3;
   listeWidget["libelleVolumeMusic"] = libelle4;
   listeWidget["libelleVolumeSound"] = libelle5;
   listeWidget["volumeBarMusic"] = volumeBar;
   listeWidget["volumeBarMusicGrab"] = volumeBar->getVolumeGrab();
   listeWidget["volumeBarSound"] = volumeBar2;
   listeWidget["volumeBarSoundGrab"] = volumeBar2->getVolumeGrab();
   listeWidget["checkBox1"] = checkBox1;
   listeWidget["checkBox2"] = checkBox2;
   listeWidget["groupCheckBox"] = groupCheckBox;

   listeWidget["libellePad1"] = libellePad1;
   listeWidget["libellePad2"] = libellePad2;
   listeWidget["checkBoxPad1"] = checkBoxPad1;
   listeWidget["checkBoxPad2"] = checkBoxPad2;


    struct stat buf;

    try
    {
        if (stat("preferences.boom", &buf) != -1)
        {
            std::ifstream ifile("preferences.boom");
            boost::archive::text_iarchive iBinaryArchive(ifile);
            bool onePlayer;
            float value1,value2;
            iBinaryArchive >> onePlayer >> value1 >> value2;
            PlayerKeyboard pk1,pk2;
            iBinaryArchive >> pk1;
            iBinaryArchive >> pk2;

            textBox61->setValue(pk1.up);
            textBox62->setValue(pk1.down);
            textBox63->setValue(pk1.right);
            textBox64->setValue(pk1.left);
            textBox65->setValue(pk1.bomb);
            checkBoxPad1->setValue(pk1.pad);

            textBox71->setValue(pk2.up);
            textBox72->setValue(pk2.down);
            textBox73->setValue(pk2.right);
            textBox74->setValue(pk2.left);
            textBox75->setValue(pk2.bomb);
            checkBoxPad2->setValue(pk2.pad);

            volumeBar->setValue(value1);
            volumeBar2->setValue(value2);

            if(!onePlayer)
            {
                checkBox1->setValue(false);
                checkBox2->setValue(true);
            }

            ifile.close();

        }
    }
    catch(boost::archive::archive_exception e)
    {

    }


}

void MenuOption::handleEvents()
{

   for(std::map<std::string,Widget*>::iterator it = listeWidget.begin();it != listeWidget.end(); ++it)
   {
        if(TextBox* txtBox = dynamic_cast<TextBox*>(it->second))
        {
            if(txtBox->isClicked())
            {
                if(input->keysym != SDL_SCANCODE_UNKNOWN)
                {
                    txtBox->setValue(input->keysym);

                    for(std::map<std::string,Widget*>::iterator it2 = listeWidget.begin();it2 != listeWidget.end(); ++it2)
                    {
                        if(TextBox* txtBox2 = dynamic_cast<TextBox*>(it2->second))
                        {
                            if(!txtBox2->isClicked() && txtBox2->getValue() == txtBox->getValue())
                            {
                                  txtBox2->setValue(SDL_SCANCODE_UNKNOWN);
                                  break;
                            }
                        }
                    }

                    txtBox->setClicked(false);
                    break;
                }
            }
        }
   }


}

MenuOption::~MenuOption()
{

}
