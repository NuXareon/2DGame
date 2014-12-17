
#include "cGame.h"
#include "cLog.h"


//Intitialize EventManager instance
cGame::cGame() :
Event(&Scene, &Critter, &Mobs),
isEventUp(false)
{
}
cGame::~cGame(){}

bool cGame::Init(HWND hWnd,HINSTANCE hInst,bool exclusive)
{
	bool res;
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

	// Provisional initialization for testing purposes
	Enemies[0].Init(5,7);
	Enemies[1].Init(10,10);
	nEnemies=2;

	//
	return true;
}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}

bool cGame::Loop()
{
	bool res;
	//Input
	res = LoopInput();
	if(!res) return false;
	
	//Process
	res = LoopProcess();
	if(!res) return false;

	//Output
	res = LoopOutput();
	if(!res) return false;
	
	return true;
}

bool cGame::LoopInput()
{
	bool res;
	cLog *Log = cLog::Instance();

	res = Input.Read();
	if(!res)
	{
		Log->Msg("Error reading Input!");
		return false;
	}
	return true;
}

bool cGame::LoopProcess()
{
	cMouse *Mouse;
	Mouse = Input.GetMouse();

	switch(state)
	{
		case STATE_MAIN:
						if(Mouse->ButtonDown(LEFT))
						{
							//Play button
							if(Mouse->In(334,236,420,278))
							{
								state = STATE_GAME;
							}
							//Exit button
							else if(Mouse->In(426,236,512,278))
							{
								return false;
							}
						}
						break;

		case STATE_GAME:
						ProcessOrder();
						Critter.Move();
						Skeleton.Move();
						for(int i = 0; i < nEnemies; ++i) Enemies[i].Move();
						ProcessAttacks();
						ProcessEvents();
						break;
	}

	return true;
}

bool cGame::LoopOutput()
{
	bool res;
	res = Render();
	return res;
}

bool cGame::Render()
{
	bool res;
	res = Graphics.Render(state,Input.GetMouse(),&Scene,&Critter,&Skeleton,Enemies,nEnemies);
	return res;
}

void cGame::ProcessOrder()
{
	cMouse *Mouse;
	int mx, my, msx, msy, p, cx, cy, x, y;
	float ix, iy, atx, aty, dx, dy, fcx, fcy;
	bool attack;

	int s=5; //marge for directional pointers
	int xo,xf,yo,yf;
	int b4pointer;
	static int release_and_press;
	RECT rc;

	Mouse = Input.GetMouse();
	b4pointer = Mouse->GetPointer();
	Mouse->GetPosition(&mx,&my);

    // Enemy Moves!
	DoEnemyTurn();

	//Player Moves!
	if(Mouse->ButtonDown(LEFT))
	{
		Mouse->SetPointer(NORMAL);
		
		if(Mouse->In(SCENE_Xo,SCENE_Yo,SCENE_Xf,SCENE_Yf))
		{
			if(Mouse->GetSelection()!=SELECT_SCENE)
			{
				//Select movement/attack
				if(Critter.GetSelected())
				{
					if(release_and_press)
					{
						attack=false;
						//Attack
						Skeleton.GetRect(&rc, &x, &y, &Scene);
						if(Mouse->In(x,y,x+rc.right-rc.left,y+rc.bottom-rc.top))
						{
							if(!Critter.GetShooting()){
								Skeleton.GetCell(&cx,&cy);
								Critter.GoToEnemy(Scene.map,Scene.cx+cx,Scene.cx+cy,0);
								attack=true;
							}
						}
						for (int i = 0; i < nEnemies; ++i) {
							if (Enemies[i].isActive()){
								Enemies[i].GetRect(&rc,&x,&y,&Scene);
								if(Mouse->In(x,y,x+rc.right-rc.left,y+rc.bottom-rc.top))
								{
									Enemies[i].GetCell(&cx,&cy);
									Critter.GoToEnemy(Scene.map,Scene.cx+cx,Scene.cx+cy,i);
									attack=true;
								}
							}
						}
						//Movement
						if (!attack) //Si estas atacant a un enemic el seguent goto sera un moviment normal!
						{
							Mouse->GetPosition(&x,&y);
							Scene.TileSelected(x,y,&fcx,&fcy);
							cx=floor(fcx); cy=floor(fcy);
							Critter.GoToCell(Scene.map,cx,cy,0);
						}
					}
				}
				//Begin selection
				else
				{
					Mouse->SetSelection(SELECT_SCENE);
					Mouse->SetSelectionPoint(mx,my);
				}
			}
		}
		release_and_press = false;
	}
	else if(Mouse->ButtonUp(LEFT))
	{
		release_and_press = true;

		if(Mouse->GetSelection()==SELECT_SCENE)
		{
			Mouse->GetSelectionPoint(&msx,&msy);

			xo = min(msx,mx)+(Scene.cx<<5)-SCENE_Xo;
			xf = max(msx,mx)+(Scene.cx<<5)-SCENE_Xo;
			yo = min(msy,my)+(Scene.cy<<5)-SCENE_Yo,
			yf = max(msy,my)+(Scene.cy<<5)-SCENE_Yo;
			
			Critter.GetPosition(&x,&y);
			if((xo<(x+32))&&(xf>=x)&&(yo<(y+32))&&(yf>=y))
				Critter.SetSelected(true);
		}
		Mouse->SetSelection(SELECT_NOTHING);

		//Mouse over Critter
		Critter.GetCell(&cx,&cy);
		Mouse->GetPosition(&x,&y);
		Scene.TileSelected(x,y,&fcx,&fcy);
		if(cx==floor(fcx)&&cy==floor(fcy))
		{
			Mouse->SetPointer(SELECT);
			return;
		}
				//Arrow mouse pointers to move through scene
		if	   (Mouse->In(             s,             s,SCREEN_RES_X-s,SCREEN_RES_Y-s)) Mouse->SetPointer(NORMAL);
		else if(Mouse->In(             s,             0,SCREEN_RES_X-s,             s)) Mouse->SetPointer(MN);
		else if(Mouse->In(             s,SCREEN_RES_Y-s,SCREEN_RES_X-s,  SCREEN_RES_Y)) Mouse->SetPointer(MS);
		else if(Mouse->In(SCREEN_RES_X-s,             s,  SCREEN_RES_X,SCREEN_RES_Y-s)) Mouse->SetPointer(ME);
		else if(Mouse->In(             0,             s,             s,SCREEN_RES_Y-s)) Mouse->SetPointer(MO);
		else if(Mouse->In(             0,             0,             s,             s)) Mouse->SetPointer(MNO);
		else if(Mouse->In(             0,SCREEN_RES_Y-s,             s,  SCREEN_RES_Y)) Mouse->SetPointer(MSO);
		else if(Mouse->In(SCREEN_RES_X-s,             0,  SCREEN_RES_X,             s)) Mouse->SetPointer(MNE);
		else if(Mouse->In(SCREEN_RES_X-s,SCREEN_RES_Y-s,  SCREEN_RES_X,  SCREEN_RES_Y)) Mouse->SetPointer(MSE);
		else																			Mouse->SetPointer(NORMAL);

		p = Mouse->GetPointer();
		if((p>=MN)&&(p<=MSO))	Scene.Move(p);
		//Mouse over Enemy
		attack=false;
		Skeleton.GetCell(&cx,&cy);
		if(Mouse->InCell(&Scene,cx,cy))
		{
			Mouse->SetPointer(ATTACK);
			attack=true;
		}
		for (int i = 0; i < nEnemies; ++i) {
			if (Enemies[i].isActive()) {
				Enemies[i].GetRect(&rc,&x,&y,&Scene);
				if(Mouse->In(x,y,x+rc.right-rc.left,y+rc.bottom-rc.top))
				{
					Mouse->SetPointer(ATTACK);
					attack=true;
				}
			}
		}
		if(!attack&&Mouse->In(s,SCENE_Yo,SCENE_Xf,SCENE_Yf-s))
		{
			//Critter selected pointing, where to move
			if(Critter.GetSelected())	Mouse->SetPointer(MOVE);
			//Critter selected but mouse out map
			else						Mouse->SetPointer(NORMAL);
		}

	}
	if(Mouse->ButtonDown(RIGHT))
	{

	}

	if(b4pointer!=Mouse->GetPointer()) Mouse->InitAnim();
}

void cGame::ProcessAttacks()
{
	if (Critter.GetShooting()&&Critter.isHit()) {
		int enemyId = Critter.getTarget();
		Enemies[enemyId].reduceHP(Critter.getDamage());
		if (!Enemies[enemyId].isActive()) Critter.stopAttack();
	}
}

// Method to implement enemy actions
void cGame::DoEnemyTurn()
{
	for (int i = 0; i < nEnemies; ++i) {
		Enemies[i].LookForPlayer(Critter); // search for player within sigh radius

		if (Enemies[i].PlayerIsDetected())
		{
			// get player cell 
			int playerCellX, playerCellY;
			Critter.GetCell(&playerCellX, &playerCellY);

			Enemies[i].GoToPlayer(Scene.map, playerCellX, playerCellY);
		}
	}

	Skeleton.LookForPlayer(Critter); // search for player within sigh radius

	if (Skeleton.PlayerIsDetected())
	{
		// get player cell 
		int playerCellX, playerCellY;
		Critter.GetCell(&playerCellX, &playerCellY);

		Skeleton.GoToPlayer(Scene.map, playerCellX, playerCellY);
	}
}

void cGame::ProcessEvents()
{

	if (Event.CheckForEvent())
	{
		if (Event.GetEventType() == 2) //Next Level
		{
			Event.GoToNextLevel();
		}

	}

}