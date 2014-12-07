#include "../Globals.h"
#include "../InputMap.h"
#include "MenuActions.h"
#include "MenuButton.h"
#include "ClassSelectionIcon.h"

void State_Menu::Init()
{	
	LoadResources();

	mainMenu = new Menu("Main Menu");
	characterSelectionMenu = new Menu("Character Selection");
	optionsMenu = new Menu("Options");
	creditsMenu = new Menu("Credits");
	pauseMenu = new Menu("Surrender?");
	waveMenu = new Menu("Wave Menu");

	mainMenu->AddButton("Start Game", MenuActions::CharacterSelection);
	mainMenu->AddButton("Options", MenuActions::Options);
	mainMenu->AddButton("Credits", MenuActions::Credits);
	mainMenu->AddButton("Exit", MenuActions::Exit);

	optionsMenu->AddButton("Resolution", MenuActions::Resolution);
	optionsMenu->AddButton("Sound", MenuActions::Sound);
	optionsMenu->AddButton("Back", MenuActions::Back);
	
	creditsMenu->AddImage(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 530, 190, 0);
	creditsMenu->AddButton("Back", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, MenuActions::Back);

	pauseMenu->AddButton("Yesh", MenuActions::Yesh);
	pauseMenu->AddButton("Nah", MenuActions::Nah);

	waveMenu->AddButton("Continue", MenuActions::Continue);
	waveMenu->AddButton("Surrender", MenuActions::Surrender);

	characterSelectionMenu->AddClassIconBig(ClassIconGraphic::WarriorClicked, nullptr);
	characterSelectionMenu->AddClassIconBig(ClassIconGraphic::Disabled, nullptr);
	characterSelectionMenu->AddClassIconBig(ClassIconGraphic::Disabled, nullptr);
	characterSelectionMenu->AddButton("Begin", 570, 520,/* MenuActions::StartGame*/ nullptr);
	characterSelectionMenu->AddButton("Back", 240, 520, MenuActions::Back);

	menuList = { mainMenu, characterSelectionMenu, optionsMenu, creditsMenu, waveMenu, pauseMenu };
	SwitchToMenu("Main Menu");
}

void State_Menu::SwitchToMenu(string newMenu)
{
	//this should be handled in a different way imo, like using some UI_element loop or smth
	//also - ClassIconBig handling and slider handling needs to be included as well.

	if (CurrentMenu != nullptr)
	{
		CurrentMenu->Hide();

		for (auto button : CurrentMenu->buttons)
		{
			button->Hide();
			button->GetText()->Hide();
		}

		for (auto image : CurrentMenu->images)
			image->Hide();

		for (auto icon : CurrentMenu->classIcons)
			icon->Hide();
	}

	for(auto menuit : menuList)
	{
		if (menuit->menuTitle == newMenu)
		{
			CurrentMenu = menuit;
			CurrentMenu->Show();

			for (auto button : CurrentMenu->buttons)
			{
				button->Show();
				button->GetText()->Show();
			}

			for (auto image : CurrentMenu->images)
				image->Show();

			for (auto icon : CurrentMenu->classIcons)
				icon->Show();
		}
	}

}

void State_Menu::Cleanup()
{
	for (auto &menu : menuList)
		menu->Cleanup();

	//imo this should be handled differently, cba deleting every new element we add here tbh.

	graphicEngine::Instance().DestroyUI_ElementGraphic("class ClassSelectionIcon");
	graphicEngine::Instance().DestroyUI_ElementGraphic("class MenuButton");
	graphicEngine::Instance().DestroyUI_ElementGraphic("class Image");
	graphicEngine::Instance().DestroyUI_ElementGraphic("class Menu");
	graphicEngine::Instance().DestroyUI_ElementGraphic("class Text");

	graphicEngine::Instance().CleanUpUIMaps();
	
	for (auto menu : menuList)
		if(menu != nullptr) delete menu;
}

void State_Menu::Pause()
{
	//???
}
void State_Menu::Resume()
{
	//???
}

void State_Menu::HandleEvents()
{

}

void State_Menu::Update()
{
	//This should be moved to InputHandler
	using ::ButtonState; //zis is kruszal!

	for (unsigned int i = 0; i < CurrentMenu->buttons.size(); i++)
	{
		if ((mouseX >= (CurrentMenu->buttons[i]->x - CurrentMenu->buttons[i]->width / 2)) &&
			(mouseX <= (CurrentMenu->buttons[i]->x + CurrentMenu->buttons[i]->width / 2)) &&
			(mouseY >= (CurrentMenu->buttons[i]->y - CurrentMenu->buttons[i]->height / 2)) &&
			(mouseY <= (CurrentMenu->buttons[i]->y + CurrentMenu->buttons[i]->height / 2)))
		{
			if (CurrentMenu->MarkedButton != CurrentMenu->buttons[i])
			{
				CurrentMenu->MarkedButton = CurrentMenu->buttons[i];
				CurrentMenu->buttons[i]->highlighted = true;
				graphicEngine::Instance().RequestUI_Element_Graphic(CurrentMenu->buttons[i], Hover);
			}
		}
		else
		{
			CurrentMenu->buttons[i]->highlighted = false;
			if (CurrentMenu->buttons[i]->clicked) CurrentMenu->buttons[i]->clicked = false;
			graphicEngine::Instance().RequestUI_Element_Graphic(CurrentMenu->buttons[i], Default);
		}
	}
	if (mouse[LMB] && CurrentMenu->MarkedButton != NULL && CurrentMenu->MarkedButton->highlighted)
	{

			CurrentMenu->MarkedButton->clicked = true;
			graphicEngine::Instance().RequestUI_Element_Graphic(CurrentMenu->MarkedButton, Clicked);
	}
	else if (!mouse[LMB] && CurrentMenu->MarkedButton != NULL && CurrentMenu->MarkedButton->clicked)
	{
		//cout << "Action!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		graphicEngine::Instance().RequestUI_Element_Graphic(CurrentMenu->MarkedButton, Default);
		CurrentMenu->MarkedButton->clicked = false;
		CurrentMenu->MarkedButton->UseFunction();
	}
	CurrentMenu->MarkedButton = NULL;
	/////////////////////////////////////////////////////////////////////
	/*if (CurrentMenu->MarkedButton != NULL)		// Small but fancy debug!
	{
		if (CurrentMenu->MarkedButton->clicked) cout << "Clicked!" << endl;
		else cout << "Highlighted!" << endl;
	}
	else cout << "---" << endl;*/
}

void State_Menu::LoadResources()
{
	//Graphics
	//menu
	graphicEngine::Instance().DefineUI_Element_Graphic("class Menu", "assets/img/UI/Menu.jpg");

	//buttons
	graphicEngine::Instance().DefineUI_Element_Graphic("class MenuButton", "assets/img/UI/button.png");
	graphicEngine::Instance().DefineUI_Element_Graphic("class MenuButton", "assets/img/UI/button_highlighted.png");
	graphicEngine::Instance().DefineUI_Element_Graphic("class MenuButton", "assets/img/UI/button_clicked.png");

	//class icons
	graphicEngine::Instance().DefineUI_Element_Graphic("class ClassSelectionIcon", "assets/img/UI/LockedCrest.png");
	graphicEngine::Instance().DefineUI_Element_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestNormal.png");
	graphicEngine::Instance().DefineUI_Element_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestHovered.png");
	graphicEngine::Instance().DefineUI_Element_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestClicked.png");

	//credits graphic
	graphicEngine::Instance().DefineUI_Element_Graphic("class Image", "assets/img/media/arrowgance.jpg");

	//fonts
	graphicEngine::Instance().textManager->DefineFont("Morpheius", "assets/img/UI/Morpheius_vDark.png", 7, 72, 108, -30);

	//Sounds
}