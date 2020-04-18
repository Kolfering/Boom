#include "EntryKissNetworkChat.h"
#include "Utile.h"

EntryKissNetworkChat::EntryKissNetworkChat(kiss_entry* kiss_e,kiss_textbox* kiss_textbox, Network_Client* network_client) : EntryKiss(kiss_e)
{
    this->textbox = kiss_textbox;
    this->network_client = network_client;
    network_server = nullptr;
}

EntryKissNetworkChat::EntryKissNetworkChat(kiss_entry* kiss_e,kiss_textbox* kiss_textbox, Network_Server* network_server) : EntryKiss(kiss_e)
{
    this->textbox = kiss_textbox;
    this->network_server = network_server;
    network_client = nullptr;
}

EntryKissNetworkChat::~EntryKissNetworkChat()
{

}

void EntryKissNetworkChat::event(Input* input, InputMouse* inputMouse)
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
        trim(entry->text);
        if(strcmp(entry->text,"")!=0)
        {
            kiss_array_appendstring(textbox->array,0,"You:",entry->text);
            if(network_client)
                network_client->sendMessage(entry->text);
            else
                network_server->sendMessage(entry->text);
            strcpy(entry->text,"");
        }
        input->key[SDL_SCANCODE_RETURN] = 0;
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
