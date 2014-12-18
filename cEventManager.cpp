#include "cEventManager.h"


cEventManager::cEventManager(cScene* _Scene, cCritter* _Player, std::vector <cSkeleton>* _Mobs):
eventType(0)
{
	this->Scene = _Scene;
	this->Player = _Player;
	this->Mobs = _Mobs;
}


cEventManager::~cEventManager()
{
}

bool cEventManager::CheckForEvent()
{
	//Get Player Position in cells
	int PlayerCx, PlayerCy;
	Player->GetCell(&PlayerCx, &PlayerCy);

	//index in scene maps corresponding to player position
	int mapIndex = SCENE_AREA*PlayerCy + PlayerCx;

	eventType = Scene->mapLogic[mapIndex];

	if (eventType != 0 && eventType < 4)
	{
		return true;
	}
	else if (eventType == 9)
		return true;

	else
		return false;
}

void cEventManager::GoToNextLevel()
{
	(Scene->level)++;
	
	Scene->LoadMap("map.txt");
	Scene->LoadMapTiles();
	Scene->LoadMapLogic();

	Player->Restart();
	Player->PutInStart(Scene->level);
	IsDone();
}

int cEventManager::GetEventType()
{
	return eventType;
}

void cEventManager::IsDone()
{
	eventType = 0;
}