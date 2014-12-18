
#include "cGame.h"
#include "cLog.h"


//Intitialize EventManager instance
cGame::cGame() :
Event(&Scene, &Critter, &Mobs)
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
	Scene.LoadMapLogic();
	Scene.LoadMapTiles();

	// Provisional initialization for testing purposes (TODO: range!)
/*
	Enemies[0].Init(5,7,100,5,2,4,0,SKELETON_TYPE);
	Enemies[1].Init(30,7,200,10,1,4,0,GOLEM_TYPE);
	Enemies[2].Init(50,7,100,4,2,7,4,FIRELOCK_TYPE);
	Enemies[3].Init(55,55,50,20,4,4,0,EXPLOSION_TYPE);
	Enemies[4].Init(50,50,50,20,4,4,0,EXPLOSION_TYPE);
	Enemies[5].Init(53,55,50,20,4,4,0,EXPLOSION_TYPE);
	Enemies[6].Init(55,53,50,20,4,4,0,EXPLOSION_TYPE);
	Enemies[7].Init(53,53,50,20,4,4,0,EXPLOSION_TYPE);
	nEnemies=8;
	//Enemies[0].Init(5,7,100,5,2,SKELETON_TYPE);
	//Enemies[1].Init(30,7,200,7,1,GOLEM_TYPE);
	//Enemies[2].Init(50,7,100,4,2,FIRELOCK_TYPE);
	//Enemies[3].Init(55,55,50,12,4,EXPLOSION_TYPE);
	//nEnemies = 4;
	*/

	nEnemies = LoadEnemies();

	Skeleton.SetActive(false);

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
	bool release_and_press = false;
	switch (state)
	{
	case STATE_MAIN:
		if (Mouse->ButtonDown(LEFT))
		{
			//Play button
			if (Mouse->In(270, 440, 510, 500))
			{
				state = STATE_CREATION;
			}
			//Exit button
			else if (Mouse->In(270, 510, 510, 570))
			{
				return false;
			}
		}
		break;
	case STATE_CREATION:
		static int release_and_press;
		if (Mouse->ButtonDown(LEFT))
		{
			if (release_and_press)
			{
				if (Mouse->In(357, 460, 430, 590))
				{
					state = STATE_GAME;
				}
				else if (Mouse->In(310, 470, 345, 513))
				{
					Critter.prevHead();
				}
				else if (Mouse->In(440, 470, 475, 513))
				{
					Critter.nextHead();
				}
			}
			release_and_press = false;
		}
		else if (Mouse->ButtonUp(LEFT))
		{
			release_and_press = true;
		}
		break;

		case STATE_GAME:
						ProcessOrder();
						Critter.Move();
						Skeleton.Move();
						for(int i = 0; i < nEnemies; ++i) if(Enemies[i].isActive()) Enemies[i].Move();
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
						/*
						Skeleton.GetRect(&rc, &ix, &iy, &Scene);
						if(Mouse->In(ix,iy,ix+rc.right-rc.left,iy+rc.bottom-rc.top))
						{
							if(!Critter.GetShooting()){
								Skeleton.GetCell(&cx,&cy);
								Critter.GoToEnemy(Scene.map,Scene.cx+cx,Scene.cx+cy,0);
								attack=true;
							}
						}
						*/
						for (int i = 0; i < nEnemies; ++i) {
							if (Enemies[i].isActive()){
								Enemies[i].GetRect(&rc,&ix,&iy,&Scene,false);
								//Enemies[i].GetIsoPos(&ix,&iy,&Scene);
								if(Mouse->In(ix,iy,ix+rc.right-rc.left,iy+rc.bottom-rc.top))
								{
									Enemies[i].GetCell(&cx,&cy);
									Critter.GoToEnemy(Scene.map,cx,cy,i);
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
		/*
		Skeleton.GetCell(&cx,&cy);
		if(Mouse->InCell(&Scene,cx,cy))
		{
			Mouse->SetPointer(ATTACK);
			attack=true;
		}
		*/
		for (int i = 0; i < nEnemies; ++i) {
			if (Enemies[i].isActive()) {
				Enemies[i].GetRect(&rc,&ix,&iy,&Scene,false);
				//Enemies[i].GetIsoPos(&ix,&iy,&Scene);
				if(Mouse->In(ix,iy,ix+rc.right-rc.left,iy+rc.bottom-rc.top))
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

	if(Input.KeyDown(DIK_1)) // <-- windowz plz, f*ck u.
	{
		// TODO: Comprovar posicion mouse per cada enemy + range
		int target=-1;
		Mouse->GetPosition(&x,&y);
		Scene.TileSelected(x,y,&fcx,&fcy);
		Critter.GetCell(&cx,&cy);
		int mouseToPlayerDistance = (int)sqrt(pow((float)cx - fcx, 2) + pow((float)cy - fcy, 2));
		if (mouseToPlayerDistance <= Critter.GetSkill1Range()) {
			for (int i = 0; i < nEnemies; ++i) {
				if (Enemies[i].isActive()) {
					Enemies[i].GetRect(&rc,&ix,&iy,&Scene,false);
					//Enemies[i].GetIsoPos(&ix,&iy,&Scene);
					if(Mouse->In(ix,iy,ix+rc.right-rc.left,iy+rc.bottom-rc.top)) target=i;
				}
			}
		}
		if (target!=-1) Critter.UseSkill1(fcx,fcy,target);
	}
}

void cGame::ProcessAttacks()
{
	int cx,cy;
	Critter.updateAttackSeq();
	Critter.updateSkill1Seq();
	if(Critter.GetSkill1()&&Critter.Skill1Hit()) {
		int enemyId = Critter.GetSkill1Target();
		if (Enemies[enemyId].isActive()) Enemies[enemyId].reduceHP(Critter.getSkill1Damage());
	}
	else if (Critter.GetShooting()) {
		int enemyId = Critter.getTarget();
		Enemies[enemyId].GetCell(&cx,&cy);
		Critter.enemyFaced(cx, cy);
		if (Critter.isHit()) { // TODO: Check distance from target
			Enemies[enemyId].reduceHP(Critter.getDamage());
			if (!Enemies[enemyId].isActive())
			{
				Critter.stopAttack(); 
			}
		}
		if (Enemies[enemyId].isDead())
			Critter.increaseHP(20);
	}
	for (int i = 0; i < nEnemies; i++) {
		if (Enemies[i].isActive()) {
			Enemies[i].updateAttackSeq();
			if (Enemies[i].GetShooting()&&Enemies[i].isHit()) { // TODO: Check distance from target for ranged enemies
				bool dead = Critter.reduceHP(Enemies[i].getDamage());
				if(dead) {
					Critter.Restart();
					Critter.PutInStart(Scene.level);
					// TODO: Reset enemies (read logical map?)
				}
			}
		}
	}
}

// Method to implement enemy actions
void cGame::DoEnemyTurn()
{
	for (int i = 0; i < nEnemies; ++i) {
		Enemies[i].LookForPlayer(Critter); // search for player within sigh radius

		if (Enemies[i].PlayerIsDetected()&&!Enemies[i].GetShooting())
		{
			// get player cell 
			int playerCellX, playerCellY;
			Critter.GetCell(&playerCellX, &playerCellY);

			Enemies[i].GoToPlayer(Scene.map, playerCellX, playerCellY);
		}
	}

/*	Skeleton.LookForPlayer(Critter); // search for player within sigh radius

	if (Skeleton.PlayerIsDetected())
	{
		// get player cell 
		int playerCellX, playerCellY;
		Critter.GetCell(&playerCellX, &playerCellY);

		Skeleton.GoToPlayer(Scene.map, playerCellX, playerCellY);
	}*/
}

void cGame::ProcessEvents()
{

	if (Event.CheckForEvent())
	{
		if (Event.GetEventType() == 2) //Next Level
		{
			Event.GoToNextLevel();

			for (int i = 0; i < nEnemies; i++)
				Enemies[i].Restart();

			nEnemies = LoadEnemies();
		}

		// Ambush event on level 2
		if (Scene.level == 2 && Event.GetEventType() == 3)
		{
			for (int i = 0; i < nEnemies; i++)
				Enemies[i].SetActive(true);
		}
	}

}

int cGame::LoadEnemies()
{
	
	int numEnem = 0;

	for (int i = 0; i < (SCENE_AREA*SCENE_AREA); i++)
	{
		if (Scene.mapLogic[i]>3)
		{
			int x = i%SCENE_AREA;
			int y = (int)i / SCENE_AREA;

			if (Scene.mapLogic[i] == 4)
			{
				Enemies[numEnem].Init(x,y,60,5,2,5,0,SKELETON_TYPE);
				Enemies[numEnem].SetActive(true);
				numEnem++;
			}
			else if (Scene.mapLogic[i] == 5)
			{
				Enemies[numEnem].Init(x, y, 100, 5, 2,5,0, GOLEM_TYPE);
				Enemies[numEnem].SetActive(true);
				numEnem++;
			}
			else if (Scene.mapLogic[i] == 6)
			{
				Enemies[numEnem].Init(x, y, 40, 2, 2,8,5, FIRELOCK_TYPE);
				Enemies[numEnem].SetActive(true);
				numEnem++;
			}
			else if (Scene.mapLogic[i] == 7)
			{
				Enemies[numEnem].Init(x, y, 10, 15, 2,7,0, EXPLOSION_TYPE);
				Enemies[numEnem].SetActive(true);
				numEnem++;
			}
		}	
	}

	//Deactivating mobs in level 2 for the ambush
	if (Scene.level == 2)
	{
		for (int i = 0; i < nEnemies; i++)
		{
			if (Enemies[i].GetType() == GOLEM_TYPE)
				continue;

			Enemies[i].SetActive(false);
		}
	}

	return numEnem;

}