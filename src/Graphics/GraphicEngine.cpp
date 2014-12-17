#include "../GameEngine.h"
#include <iostream>

GraphicEngine::GraphicEngine()
{
	al_init_primitives_addon();
	al_init_image_addon();

	textManager = new TextManager();
}

void GraphicEngine::DefineAnimation(std::string ownersClassName, std::vector<Sprite *> animations)
{
	AnimationsMap.emplace(ownersClassName, animations);
}

void GraphicEngine::CreateAnimationInstance(AnimatedGraphic *owner)
{
	//getting the class name
	const std::type_info& info = typeid(owner);
	string className = static_cast<string>(info.name());

	//setting default sprite
	for (auto &graphMap : AnimationsMap)
	{
		if (graphMap.first == className)
		{
			AnimationOwnersMap.push_back(std::make_pair(owner, graphMap.second.at(0)));
			break;
		}
	}
}

void GraphicEngine::RequestAnimation(AnimatedGraphic *owner, int animationNumber)
{
	//getting name of the class of object passed
	const std::type_info& info = typeid(*owner);
	string className = static_cast<string>(info.name());

	std::pair<AnimatedGraphic *, Sprite *> *currentPair = nullptr;

	//searching for the object in a GameObjectsMap
	for (auto &elem : AnimationOwnersMap)
	{
		if (elem.first == owner)
		{
			currentPair = &elem;
			break;
		}
	}

	if (currentPair != nullptr)
	{
		for (auto &animation : AnimationsMap) //searching for animation
		{
			if (animation.first == className)
			{
				currentPair->second = animation.second.at(animationNumber); //changing its animation to indicated one
				break;
			}
		}
	}
}

void GraphicEngine::DefineGraphic(std::string ownersClassName, std::string pathName)
{
	ALLEGRO_BITMAP *newBitmap = al_load_bitmap(pathName.c_str()); //c_str() is used to convert to the const char

	if (!newBitmap) std::cerr << "Couldn't load image named " << pathName << std::endl;

	if (GraphicsMap.find(ownersClassName) != GraphicsMap.end()) //if the class already exists
	{
		GraphicsMap[ownersClassName].push_back(newBitmap); //then add bitmap to it
		return;
	}
	//else
	vector <ALLEGRO_BITMAP *> newBitmapVector = { newBitmap }; //make a vector of bitmap(s)
	GraphicsMap.emplace(ownersClassName, newBitmapVector); //and create a new element in map
}
//here
void GraphicEngine::DefineGraphic(std::string ownersClassName, ALLEGRO_BITMAP *bitmap)
{
	if (GraphicsMap.find(ownersClassName) != GraphicsMap.end()) //if the class already exists
	{
		GraphicsMap[ownersClassName].push_back(bitmap); //then add bitmap to it
		return;
	}
	//else
	vector <ALLEGRO_BITMAP *> newBitmapVector = { bitmap }; //make a vector of bitmap(s)
	GraphicsMap.emplace(ownersClassName, newBitmapVector); //and create a new element in map
}

void GraphicEngine::CreateGraphicInstance(StaticGraphic *owner)
{
	//getting the class name
	const std::type_info& info = typeid(*owner);
	string className = static_cast<string>(info.name());


	//setting default graphic
	for (auto &graphMap : GraphicsMap)
	{
		if (graphMap.first == className)
		{
			GraphicOwnersMap.push_back(std::make_pair(owner, graphMap.second.at(0)));
			break;
		}
	}
}

void GraphicEngine::RequestGraphic(StaticGraphic *owner, int graphicID)
{
	const std::type_info& info = typeid(*owner);
	string className = static_cast<string>(info.name());

	std::pair<StaticGraphic *, ALLEGRO_BITMAP *> *currentPair = nullptr;

	for (auto &elem : GraphicOwnersMap)
	{
		if (elem.first == owner)
		{
			currentPair = &elem;
			break;
		}
	}

	if (currentPair != nullptr)
	{
		for (auto &graphMap : GraphicsMap)
		{
			if (graphMap.first == className)
			{
				currentPair->second = graphMap.second.at(graphicID);
				break;
			}
		}
	}
}

void GraphicEngine::Render()
{
	for (auto &Gobject : AnimationsMap)
	{
		//some animation algorithm
	}
	for (int i = 0; i < 3; i++)
	{
		for (auto &UIElement : GraphicOwnersMap)
		{
			if (UIElement.second != nullptr && UIElement.first->IsVisible() && UIElement.first->GetZ() == i)
				al_draw_bitmap(UIElement.second, (UIElement.first->GetX() - (UIElement.first->GetWidth() / 2)),
				(UIElement.first->GetY() - (UIElement.first->GetHeight() / 2)), 0);
		}
	}
}

void GraphicEngine::DestroyGraphicInstance(StaticGraphic *owner)
{
	vector<std::pair<StaticGraphic *, ALLEGRO_BITMAP *>>::iterator pos = GraphicOwnersMap.begin();
	for (auto &UIElement : GraphicOwnersMap)
	{
		
		if (UIElement.first == owner)
		{
			GraphicOwnersMap.erase(pos);
			break;
		}
		pos++;
	}
}

void GraphicEngine::DestroyGraphic(std::string className)
{
	for (auto &graphMap : GraphicsMap)
	{
		if (graphMap.first == className)
		{
			for (auto bitmap : graphMap.second)
			{
				al_destroy_bitmap(bitmap);
			}
			break;
		}
	}
}

void GraphicEngine::CleanUpUIMaps()
{
	GraphicsMap.clear();

	GraphicOwnersMap.clear();
	GraphicOwnersMap.shrink_to_fit();
}

void GraphicEngine::Destroy()
{
	for (auto &graphMap : GraphicsMap)
	{
		for (auto bitmap : graphMap.second)
		{
			if(bitmap != nullptr) al_destroy_bitmap(bitmap);
		}
	}
	CleanUpUIMaps();
}