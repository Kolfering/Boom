#include "BoutonKiss.h"

BoutonKiss::BoutonKiss(kiss_button* kiss_b)
{
    button = kiss_b;
}

BoutonKiss::~BoutonKiss()
{

}

kiss_button* BoutonKiss::getButton()
{
    return button;
}
