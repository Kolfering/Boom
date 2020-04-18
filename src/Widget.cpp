#include "Widget.h"
#include "Menu.h"


Widget::Widget(Menu* menu)
{
    this->menu = menu;
    position.x = 0;
    position.y = 0;
    position.w = 0;
    position.h = 0;
    focus = false;
    displayable = true;
}

Widget::~Widget()
{

}

bool Widget::isDisplayable()
{
    return displayable;
}

bool Widget::isFocus()
{
    return focus;
}

SDL_Rect* Widget::getAdressePosition()
{
    return &position;
}

void Widget::onClic()
{

}

void Widget::onFocus()
{
  focus = true;
}

void Widget::onDeFocus()
{
   focus = false;
}
