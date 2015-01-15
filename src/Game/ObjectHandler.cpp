#include "ObjectHandler.h"
#include "MobileObject.h"
#include "StaticObject.h"
#include "../Globals.h"
#include <iostream>


//ObjectHandler* ObjectHandler::instance = nullptr; //this definition is needed, without it we get unrsolved link external 2001

ObjectHandler::ObjectHandler()
{

}

template<class Type> void ObjectHandler::CreateObject()	
{
	if (std::is_same<Type, Player>::value)
	{
		Player *object = new Player();
		object->Init(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 3);
		objects.push_back(object);
	}
	else if (std::is_same<Type, StaticObject>::value)
	{
		StaticObject *object = new StaticObject();
		//object->Init(2, 2); 
		objects.push_back(object);
	}
}

template<class Type> void ObjectHandler::DestroyObject(Type *objectToDestroy)
{
	auto it = GetIterator(objectToDestroy);
	objects.erase(it);
	objects.shrink_to_fit();
	objectToDestroy->SetAlive(false);
	delete objectToDestroy;
}

void ObjectHandler::SpawnObject()
{
	for (auto object : objects)
		object->SetAlive(true);
}

void ObjectHandler::CleanUp()
{
	if (objects.size())
	{
		for (auto object : objects) 
		{
			graphicEngine::Instance().DestroyAnimationInstance(object);
		}
		for (unsigned int i = 0; i < objects.size(); i++) 
		{
			DestroyObject(objects[i]);
		}
	}
	objects.shrink_to_fit();

}


std::vector <GameObject*>::iterator ObjectHandler::GetIterator(GameObject * object)
{
	return std::find(objects.begin(), objects.end(), object);
}

template void ObjectHandler::CreateObject<Player>(); //it is needed, without it we get unrsolved link external 2019 -_-
template void ObjectHandler::CreateObject<StaticObject>();