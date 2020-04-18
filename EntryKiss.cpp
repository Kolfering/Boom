#include "EntryKiss.h"

EntryKiss::EntryKiss(kiss_entry* kiss_e)
{
    entry = kiss_e;
}

EntryKiss::~EntryKiss()
{

}

kiss_entry* EntryKiss::getEntry()
{
    return entry;
}

void EntryKiss::event(Input* input, InputMouse* inputMouse)
{
	if (inputMouse->isMouseClicked && !entry->active &&
		kiss_pointinrect(inputMouse->positionMouse.x, inputMouse->positionMouse.y,
		&entry->rect)) {
		entry->active = 1;
		input->lastCharInput = false;
		SDL_StartTextInput();
		if (entry->wdw) entry->wdw->focus = 0;
		entry->focus = 1;
	} else if (input->isKeyDown && entry->active &&
		input->key[SDL_SCANCODE_RETURN] == 1) {
		entry->active = 0;

		SDL_StopTextInput();
		if (entry->wdw) entry->wdw->focus = 1;
		entry->focus = 0;
		return;
    }else if (input->newLastKeyDown && entry->active &&
		input->key[SDL_SCANCODE_BACKSPACE] == 1) {
		kiss_backspace(entry->text);
		input->newLastKeyDown = false;
	} else if (input->lastCharInput && entry->active) {
		if (kiss_textwidth(entry->font, entry->text,
			input->text) < entry->textwidth &&
			strlen(entry->text) + strlen(input->text) <
			KISS_MAX_LENGTH)
			strcat(entry->text, input->text);
			input->lastCharInput = false;
	}

}
