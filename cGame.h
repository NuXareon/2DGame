
#ifndef __GAME_H__
#define __GAME_H__

#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cScene.h"
#include "cEventManager.h"
#include <vector>

#define STATE_MAIN	0
#define STATE_GAME	1

class cGame
{
public:
	cGame();
	virtual ~cGame();

	bool Init(HWND hWnd,HINSTANCE hInst,bool exclusive);
	bool Loop(); 
	void Finalize();

private:
	bool LoopInput();
	bool LoopProcess();
	bool LoopOutput();

	int LoadEnemies();

	cGraphicsLayer Graphics;
	cInputLayer Input;
	cScene Scene;
	cCritter Critter;
	cSkeleton Skeleton;
	cSkeleton Enemies[100];
	int nEnemies;

	cEventManager Event;

	//Dynamic vector of enemies
	std::vector <cSkeleton> Mobs;

	int state;

	void ProcessEvents();
	void DoEnemyTurn();
	void ProcessOrder();
	void ProcessAttacks();
	bool Render();
};

#endif
