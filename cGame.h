
#ifndef __GAME_H__
#define __GAME_H__


#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cScene.h"


class cGame
{
	//random
public:
	cGame();
	virtual ~cGame();

	bool Init(HWND hWnd,HINSTANCE hInst,bool exclusive);
	bool Loop(); 
	void Finalize();

	cGraphicsLayer Graphics;
	cInputLayer Input;
	cScene Scene;

private:
	void Render(int mouseX,int mouseY);
};


#endif
