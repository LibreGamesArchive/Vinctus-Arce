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
	resolutionMenu = new Menu("Resolution Settings");
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

	resolutionMenu->AddImage(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SliderDimension::sliderBarWidth, SliderDimension::sliderBarHeight, 1);
	resolutionMenu->AddImage(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SliderDimension::sliderWidth, SliderDimension::sliderHeight, 2);
	resolutionMenu->AddButton("Back", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, MenuActions::Options);

	menuList = { mainMenu, characterSelectionMenu, optionsMenu, resolutionMenu, creditsMenu, waveMenu, pauseMenu };
	SwitchToMenu("Main Menu");
}

void State_Menu::SwitchToMenu(string newMenu)
{
	if (CurrentMenu != nullptr)
	{
		CurrentMenu->Hide();
	}

	for(auto menuit : menuList)
	{
		if (menuit->menuTitle == newMenu)
		{
			CurrentMenu = menuit;
			CurrentMenu->Show();
		}
	}

}

void State_Menu::Cleanup()
{
	for (auto &menu : menuList)
		menu->Cleanup();

	vector <string> UIGraphicsToBeDeleted = { "class ClassSelectionIcon", "class MenuButton", "class Image",
		"class Menu", "class Text" };

	for (auto &graphic : UIGraphicsToBeDeleted)
		graphicEngine::Instance().DestroyUIElementGraphic(graphic);

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
	//This still should be moved to InputHandler
	using ::ButtonState; //zis is kruszal!

	for (unsigned int i = 0; i < CurrentMenu->buttons.size(); i++)
	{
		//mouse pointer is in range
		if ((mouseX >= (CurrentMenu->buttons[i]->x - CurrentMenu->buttons[i]->width / 2)) &&
			(mouseX <= (CurrentMenu->buttons[i]->x + CurrentMenu->buttons[i]->width / 2)) &&
			(mouseY >= (CurrentMenu->buttons[i]->y - CurrentMenu->buttons[i]->height / 2)) &&
			(mouseY <= (CurrentMenu->buttons[i]->y + CurrentMenu->buttons[i]->height / 2)))
		{
			//denoting currently mouseovered button as MakredButton, setting state to highlight state
			if (CurrentMenu->MarkedButton != CurrentMenu->buttons[i])
			{
				CurrentMenu->MarkedButton = CurrentMenu->buttons[i];
				CurrentMenu->buttons[i]->SetHighlightedState();
			}
			//set clicked state if the button has been highlighted and left mouse button has been used on it
			if (mouse[LMB] && CurrentMenu->MarkedButton != NULL && CurrentMenu->MarkedButton->IsHighlighted())
			{
				CurrentMenu->MarkedButton->SetClickedState();
			}
			//set to normal state and use button's function if the mouse button has been realised during the clicked state
			else if (!mouse[LMB] && CurrentMenu->MarkedButton != NULL && CurrentMenu->MarkedButton->IsClicked())
			{
				CurrentMenu->MarkedButton->SetNormalState();
				CurrentMenu->MarkedButton->UseFunction();
			}
		}
		//set to normal state if mouse is no longer inside the button's boundary
		else
		{
			CurrentMenu->buttons[i]->SetNormalState();
		}
	}
	CurrentMenu->MarkedButton = NULL;
}

void State_Menu::LoadResources()
{
	//Graphics
	//menu
	graphicEngine::Instance().DefineUIElement_Graphic("class Menu", "assets/img/UI/Menu.jpg");

	//buttons
	graphicEngine::Instance().DefineUIElement_Graphic("class MenuButton", "assets/img/UI/button.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class MenuButton", "assets/img/UI/button_highlighted.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class MenuButton", "assets/img/UI/button_clicked.png");

	//class icons
	graphicEngine::Instance().DefineUIElement_Graphic("class ClassSelectionIcon", "assets/img/UI/LockedCrest.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestNormal.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestHovered.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class ClassSelectionIcon", "assets/img/UI/WarriorCrestClicked.png");

	//credits graphic
	graphicEngine::Instance().DefineUIElement_Graphic("class Image", "assets/img/media/arrowgance.jpg");

	//slider
	graphicEngine::Instance().DefineUIElement_Graphic("class Image", "assets/img/UI/SliderBar.png");
	graphicEngine::Instance().DefineUIElement_Graphic("class Image", "assets/img/UI/Slider.png");

	//fonts
	graphicEngine::Instance().textManager->DefineFont("Morpheius", "assets/img/UI/Morpheius_vDark.png", 7, 72, 108, -30);

	//Sounds
}