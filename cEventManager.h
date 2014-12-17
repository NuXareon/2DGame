#pragma once
#include "cScene.h"
#include "cSkeleton.h"
#include "cCritter.h"
#include <vector>


class cEventManager
{
public:
	cEventManager(cScene* _Scene, cCritter* _Player, std::vector <cSkeleton>* _Mobs);
	~cEventManager();

	cScene* Scene;
	cCritter* Player;
	std::vector <cSkeleton>* Mobs;

	void GoToNextLevel();

	bool CheckForEvent();
	int GetEventType();
	void IsDone();

private:
	int eventType;
};

