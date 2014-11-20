#include "GameUI.h"


void GameUI::AddBar(float x, float y)
{
		GameUI_bar *bar = new GameUI_bar(x, y);
		bars.push_back(bar);
}

void GameUI::Cleanup()
{
	for (auto bar : bars)
		graphicEngine->DestroyUI_ElementGraphicInstance(bar);
	if (bars.size())
		this->bars.clear();
	bars.shrink_to_fit();
}