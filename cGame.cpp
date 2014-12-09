
#include "cGame.h"
#include "cLog.h"


cGame::cGame() {}
cGame::~cGame(){}

bool cGame::Init(HWND hWnd,HINSTANCE hInst,bool exclusive)
{
	bool res=true;
	cLog *Log = cLog::Instance();

	res = Graphics.Init(hWnd);
	if(!res)
	{
		Log->Msg("Error initializing Graphics!");
		return false;
	}

	res = Input.Init(hInst,hWnd,exclusive,USE_MOUSE|USE_KEYBOARD);
	if(!res)
	{
		Log->Msg("Error initializing Input!");
		return false;
	}
	Input.SetMousePosition(SCREEN_RES_X >> 1,SCREEN_RES_Y >> 1);

	Graphics.LoadData();

	Scene.LoadMap("map.txt");

	return res;
}

bool cGame::Loop()
{
	bool res=true;
	cLog *Log = cLog::Instance();
	int mouseX,mouseY;

	//Input
	res = Input.Read();
	if(!res)
	{
		Log->Msg("Error reading Input!");
		return false;
	}
	Input.GetMousePosition(&mouseX,&mouseY);
	
	//Process
	

	//Output
	Render(mouseX,mouseY);
	
	return res;
}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}

void cGame::Render(int mouseX,int mouseY)
{
	Graphics.Render(mouseX,mouseY,&Scene);
}
