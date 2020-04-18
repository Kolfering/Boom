#ifndef MENUTEMPLATE_H
#define MENUTEMPLATE_H
#define UPDATETIMEKISS 100

#include "Menu.h"
#include "Widget.h"
#include "kiss_sdl.h"
#include "BoutonKiss.h"
#include "EntryKiss.h"
#include <map>
#include <vector>


class MenuTemplate : public Menu {
protected:
    SDL_Texture* backGround;
    SDL_Rect positionBackGround;
    std::map<std::string,Widget*> listeWidget;
    std::map<SDL_Texture*,SDL_Rect> listeTexture;
    std::vector<kiss_window*> listeWindow;
    std::vector<kiss_textbox*> listeTextBox;
    std::vector<kiss_vscrollbar*> listeVScrollbar;
    std::vector<EntryKiss*> listeEntryBox;
    std::vector<BoutonKiss*> listeKissButton;
    void afficheBackGround();
    void gestionWidget();
    void initialiseMenuTemplate();

public:
    MenuTemplate(Input*,InputMouse*,SDL_Window*);
    MenuTemplate(Input*,InputMouse*,SDL_Window*,Menu*, bool transit = true);
    virtual ~MenuTemplate();
    void executeMenu();
    void afficheMenu();
    std::map<std::string,Widget*> &getListeWidget();
    std::map<SDL_Texture*,SDL_Rect> &getListeTexture();
};

#endif // MENUTEMPLATE_H
