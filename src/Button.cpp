#include "Button.h"

Button::Button(string text, float x, float y, void(*function)()) : UI_element(x, y), text(text), action(*function) 
{
	width = ButtonSize::ButtonWidth;
	height = ButtonSize::ButtonHeight;
	
	highlighted = false;
	clicked = false;
}

//Debug methods
void Button::UseFunction()
{
	(*action)();
}