#include "Button.h"

Button::Button(string text, float x, float y, void(*function)()) : x(x), y(y), text(text), action(*function) 
{
	width = ButtonSize::ButtonWidth;
	height = ButtonSize::ButtonHeight;
}

/*void Button::UseFunction()
{
	(*action)();
}*/