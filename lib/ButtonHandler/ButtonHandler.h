#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

void ButtonHandler_init();
void ButtonHandler_checkButtons();
bool ButtonHandler_isPressed(int buttonIndex);
void ButtonHandler_reset();

#endif