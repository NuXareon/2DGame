
#include "cGraphicsLayer.h"
#include "cGame.h"
#include "cLog.h"
#include <stdio.h>


cGraphicsLayer::cGraphicsLayer()
{
	g_pD3D = NULL;
	g_pD3DDevice = NULL;
	g_pSprite = NULL;
	font = NULL;

}

cGraphicsLayer::~cGraphicsLayer(){}

bool cGraphicsLayer::Init(HWND hWnd)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	D3DVIEWPORT9 viewPort = { 0, 0, SCREEN_RES_X, SCREEN_RES_Y, 0.0f, 1.0f };

	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(g_pD3D==NULL)
	{
		Log->Msg("Error creating Direct3D object");
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	d3dpp.Windowed               = FALSE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;	//Efficient page flipping
	d3dpp.BackBufferWidth        = SCREEN_RES_X;
    d3dpp.BackBufferHeight       = SCREEN_RES_Y;
    d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;

	hr = g_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, 
								D3DDEVTYPE_HAL, 
								hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								&d3dpp, 
								&g_pD3DDevice );
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating Direct3D device");
		return false;
	}

	// Configure for 2d operations
    hr = g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    hr = g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	hr = g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting render state");
		return false;
	}

	hr = g_pD3DDevice->SetViewport(&viewPort);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting viewport");
		return false;
	}

	hr = D3DXCreateFont(g_pD3DDevice, 12, 0, FW_BOLD, 0, FALSE, 
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
						DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
						&font );
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating Direct3D font");
		return false;
	}

	return true;
}

void cGraphicsLayer::Finalize()
{
	if(g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if(g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void cGraphicsLayer::LoadData()
{
	D3DXCreateSprite( g_pD3DDevice, &g_pSprite ); 

	//Main menu
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"main.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texMain);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "end.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		NULL, NULL, NULL, &texEnd);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "buttons.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								NULL, NULL, NULL, &texButtons);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "create_character.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								NULL, NULL, NULL, &texCreation);
	//GUI game
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"game.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texGame);
	//Tiles
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"tiles.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texTiles);
	//Characters
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"characters.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texCharacters);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "warlock.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texWarlock);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "heads_white.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texHead);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "sword.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texSlash);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "skeleton.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texSkeleton);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "golem.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texGolem);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "firelock.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texFirelock);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "explosion.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texExplosion);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "boss_ad.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texBossAD);



	D3DXCreateTextureFromFileEx(g_pD3DDevice, "skills.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texSkills);

	//Mouse pointers
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"mouse.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texMouse);
	//Iso tiles
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"t1.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[0]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"t2.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[1]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"t3.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[2]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "t4.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								0xFF408080, NULL, NULL, &texTilesIso[3]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "t5.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								0xFF408080, NULL, NULL, &texTilesIso[4]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "t6.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								0xFF408080, NULL, NULL, &texTilesIso[5]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "t7.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								0xFF408080, NULL, NULL, &texTilesIso[6]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "t8.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0xFF408080, NULL, NULL, &texTilesIso[7]);

	D3DXCreateTextureFromFileEx(g_pD3DDevice,"UI.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texUI);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "skillicons.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texIcons);

	D3DXCreateTextureFromFileEx(g_pD3DDevice, "HP.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texHP);
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "mana.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texMana); 
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "boss.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0x00ff00ff, NULL, NULL, &texBoss);

//	D3DXCreateTextureFromFileEx(g_pD3DDevice, "boss.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
//		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
//		NULL, NULL, NULL, &texBoss);
	
	//Dungeon Floor textures
	D3DXCreateTextureFromFileEx(g_pD3DDevice, "dungeon_floor.png", 0, 0, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		0xFF408080, NULL, NULL, &texFloor);

}

void cGraphicsLayer::UnLoadData()
{
	if(texMain)
	{
		texMain->Release();
		texMain = NULL;
	}
	if (texBoss)
	{
		texBoss->Release();
		texBoss = NULL;
	}
	if (texEnd)
	{
		texEnd->Release();
		texEnd = NULL;
	}
	if (texSlash)
	{

		texSlash->Release();
		texSlash = NULL;
	}
	if (texBossAD)
	{
		texBossAD->Release();
		texBossAD = NULL;
	}
	if (texBoss)
	{
		texBoss->Release();
		texBoss = NULL;
	}
	if (texCreation)
	{
		texCreation->Release();
		texCreation = NULL;
	}
	if (texMana)
	{
		texMana->Release();
		texMana = NULL;
	}
	if (texHP)
	{
		texHP->Release();
		texHP = NULL;
	}
	if (texIcons)
	{
		texIcons->Release();
		texIcons = NULL;
	}

	if (texButtons)
	{
		texButtons->Release();
		texButtons = NULL; 
	}
	if(texGame)
	{
		texGame->Release();
		texGame = NULL;
	}
	if(texTiles)
	{
		texTiles->Release();
		texTiles = NULL;
	}
	if(texCharacters)
	{
		texCharacters->Release();
		texCharacters = NULL;
	}
	if (texWarlock)
	{
		texWarlock->Release();
		texWarlock = NULL;
	}
	if (texHead)
	{
		texHead->Release();
		texHead = NULL;
	}
	if (texSkeleton)
	{
		texSkeleton->Release();
		texSkeleton = NULL;
	}
	if (texGolem)
	{
		texGolem->Release();
		texGolem = NULL;
	}
	if (texFirelock)
	{
		texFirelock->Release();
		texFirelock = NULL;
	}
	if (texExplosion)
	{
		texExplosion->Release();
		texExplosion = NULL;
	}
	if (texSkills)
	{
		texSkills->Release();
		texSkills = NULL;
	}
	if(texMouse)
	{
		texMouse->Release();
		texMouse = NULL;
	}
	if(texUI)
	{
		texUI->Release();
		texUI = NULL;
	}
	if(g_pSprite)
	{
		g_pSprite->Release();
		g_pSprite = NULL;
	}
	if (texFloor)
	{
		texFloor->Release();
		texFloor = NULL;
	}
}

bool cGraphicsLayer::Render(int state,cMouse *Mouse,cScene *Scene,cCritter *Critter,cSkeleton *Skeleton, cSkeleton Enemies[],int nEnemies, cBoss *Boss)
{
	//HRESULT Draw( LPDIRECT3DTEXTURE9 pTexture, CONST RECT *pSrcRect,
	//				CONST D3DXVECTOR3 *pCenter,  CONST D3DXVECTOR3 *pPosition,
	//				D3DCOLOR Color);

	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xFF000000, 0, 0 );
	g_pD3DDevice->BeginScene();

		//--- SPRITES ---
		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		RECT rc;

			switch(state)
			{
				case STATE_MAIN:

								
								g_pSprite->Draw(texMain,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF);
								rc.left = 0;
								rc.top = 0;
								rc.bottom = 60;
								rc.right = 240;
								if (Mouse->In(270, 440, 270 + 240, 440 + 60))
								{
									rc.left = 240;
									rc.right = 480;

								}
								g_pSprite->Draw(texButtons, &rc, NULL, &D3DXVECTOR3(270.0f, 440.0f, 0.0f), 0xFFFFFFFF);

								rc.left = 0;
								rc.right = 240;
								rc.top = 60;
								rc.bottom = 120;
								if (Mouse->In(270, 510, 270 + 240, 510 + 60))
								{
									rc.left = 240;
									rc.right = 480;
								}
								g_pSprite->Draw(texButtons, &rc, NULL, &D3DXVECTOR3(270.0f, 510.0f, 0.0f), 0xFFFFFFFF);
								break;

				case STATE_CREATION:

								
								rc.left = 0;
								rc.top = 0;
								rc.bottom = 100;
								rc.right = 64;
								g_pSprite->Draw(texCreation, NULL, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
								g_pSprite->Draw(texWarlock, &rc, NULL, &D3DXVECTOR3(358.0f, 487.0f, 0.0f), 0xFFFFFFFF);
								rc.right = 50;
								rc.top = 0 + (Critter->getHead()*50);
								rc.bottom =50 + (Critter->getHead() * 50);
								g_pSprite->Draw(texHead, &rc, NULL, &D3DXVECTOR3(366.0f, 470.0f, 0.0f), 0xFFFFFFFF);
								break;

				case STATE_GAME:
								DrawScene(Scene,Critter);
								DrawUnits(Scene,Critter,Skeleton,Enemies,nEnemies);
								DrawBoss(Scene,Boss);
							//	DrawDebug(Scene, Mouse);
								DrawUI(Critter);
								//g_pSprite->Draw(texGame,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF); //Graphic User Interface
								break;
				case STATE_END:
						
					g_pSprite->Draw(texEnd, NULL, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
						
					break;
			}

		g_pSprite->End();

		DrawMouse(Mouse);

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return true;
}

bool cGraphicsLayer::DrawScene(cScene *Scene, cCritter *Critter)
{
	RECT rc;
	int x,y,n,
		fx,fy,
		pantx,panty;
	float posx,posy;

	//Critter->GetPosition(&posx,&posy);
	Critter->GetIsoCell(&posx,&posy);
	//Tile based map
	fx=Scene->cx+SCENE_WIDTH;
	fy=Scene->cy+SCENE_HEIGHT;

	for(y=0;y<SCENE_AREA;++y)
	{
		for(x=0;x<SCENE_AREA;++x)
		{
			int screenTileX = x-Scene->cx;
			int screenTileY = y-Scene->cy;
			float screenX = ISO_OFFSET_X + TILE_SIZE_X*((float)screenTileX-screenTileY)/2;

			float screenY = ISO_OFFSET_Y + TILE_SIZE_Y*((float)screenTileX+screenTileY)/2;

			n = Scene->mapTiles[(y*SCENE_AREA)+x];
			n--;

			if (n >= 0)
			{
				
				if (n == 4)
				{
					int pictureRow = (int)Scene->mapFloor[(y*SCENE_AREA) + x] % 5;
					int pictureColumn = (int)Scene->mapFloor[(y*SCENE_AREA) + x] / 5;
					int rectX = pictureRow * 64;
					int rectY = pictureColumn * 32;
					//SetRect(&rc, 0, 0, 64, 32);
					SetRect(&rc, rectX, rectY, 64+rectX, 32+rectY);

					
					g_pSprite->Draw(texFloor, &rc, NULL, &D3DXVECTOR3(screenX, screenY, 0.0f), 0xFFFFFFFF);
				}
				else
					g_pSprite->Draw(texTilesIso[n], NULL, NULL, &D3DXVECTOR3(screenX, screenY+ISO_OFFSET_Y, 0.0f), 0xFFFFFFFF);

			}



		}
	}
	return true;
}

bool cGraphicsLayer::DrawUnits(cScene *Scene,cCritter *Critter,cSkeleton *Skeleton,cSkeleton Enemies[],int nEnemies)
{
	int cx,cy,posx,posy,posx2,posy2;
	float ix,iy,off;
	RECT rc;

	//Draw Critter
	Critter->GetCell(&cx,&cy);
	Critter->GetRect(&rc,&posx,&posy,Scene);
	//Critter->GetPosition(&posx2,&posy2);

	g_pSprite->Draw(texWarlock,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);
	Critter->getRectHead(&rc,&off);
	g_pSprite->Draw(texHead, &rc, NULL,
		&D3DXVECTOR3(float(posx+8+off), float(posy-18+3*off), 0.0f),
		0xFFFFFFFF);
	/*
	if(Critter->GetSelected())
	{
		Critter->GetRectLife(&rc,&posx,&posy,Scene);
		g_pSprite->Draw(texMouse,&rc,NULL, 
						&D3DXVECTOR3(float(posx),float(posy),0.0f), 
						0xFFFFFFFF);
		*/
	//}
	/*
	Skeleton->GetRect(&rc,&ix,&iy,Scene);
	g_pSprite->Draw(texSkeleton,&rc,NULL, 
					&D3DXVECTOR3(ix,iy,0.0f), 
					0xFFFFFFFF);
*/
	//Draw enemies
	for (int i = 0; i < nEnemies; ++i) {
		if (Enemies[i].isActive()){
			Enemies[i].GetRect(&rc,&ix,&iy,Scene,true);
			if (Enemies[i].GetType() == SKELETON_TYPE){
				g_pSprite->Draw(texSkeleton,&rc,NULL, 
								&D3DXVECTOR3(ix,iy,0.0f), 
								0xFFFFFFFF);
			} 
			else if (Enemies[i].GetType() == GOLEM_TYPE){
				g_pSprite->Draw(texGolem,&rc,NULL, 
								&D3DXVECTOR3(ix,iy,0.0f), 
								0xFFFFFFFF);
			}
			else if (Enemies[i].GetType() == FIRELOCK_TYPE){
				g_pSprite->Draw(texFirelock,&rc,NULL, 
								&D3DXVECTOR3(ix,iy,0.0f), 
								0xFFFFFFFF);
			}
			else if (Enemies[i].GetType() == EXPLOSION_TYPE){
				g_pSprite->Draw(texExplosion,&rc,NULL, 
								&D3DXVECTOR3(ix,iy,0.0f), 
								0xFFFFFFFF);
			} else if (Enemies[i].GetType() == BOSS_AD) {
				g_pSprite->Draw(texBossAD, &rc, NULL,
					&D3DXVECTOR3(ix, iy, 0.0f),
					0xFFFFFFFF);
			}
		} else if (Enemies[i].isDead()){
			Enemies[i].GetIsoPos(&ix,&iy,Scene);
			if (Enemies[i].GetType() == SKELETON_TYPE) {
				SetRect(&rc,256,800,320,900);
				g_pSprite->Draw(texSkeleton,&rc,NULL, 
					&D3DXVECTOR3(ix,iy,0.0f), 
					0xFFFFFFFF);
			} else if (Enemies[i].GetType() == FIRELOCK_TYPE) {
				SetRect(&rc,256,1200,320,1300);
				g_pSprite->Draw(texFirelock,&rc,NULL, 
					&D3DXVECTOR3(ix,iy,0.0f), 
					0xFFFFFFFF);
			} else if (Enemies[i].GetType() == GOLEM_TYPE) {
				SetRect(&rc,320,1320,400,1430);
				g_pSprite->Draw(texGolem,&rc,NULL, 
					&D3DXVECTOR3(ix,iy,0.0f), 
					0xFFFFFFFF);
			} else if (Enemies[i].GetType() == BOSS_AD) {
				SetRect(&rc, 320,	800, 384, 900);
				g_pSprite->Draw(texBossAD, &rc, NULL,
					&D3DXVECTOR3(ix, iy, 0.0f),
					0xFFFFFFFF);
			}
		}

	}

	// Skills
	if(Critter->GetSkill1()) {
		int enemyId = Critter->GetSkill1Target();
		//Enemies[enemyId].GetRect(&rc,&ix,&iy,Scene);
		Enemies[enemyId].GetIsoPos(&ix,&iy,Scene);
		Critter->getSkiill1Rect(&rc);
		g_pSprite->Draw(texSkills,&rc,NULL, 
						&D3DXVECTOR3(ix-30,iy-275,0.0f), 
						0xFFFFFFFF);
	}
	if (Critter->GetSkill2()) {

		Critter->GetPosition(&posx, &posy);
		Critter->getSkill2Rect(&rc);
		g_pSprite->Draw(texSkills, &rc, NULL,
			&D3DXVECTOR3(300, 200, 0.0f),
			0xFFFFFFFF);
	}
	//Draw Fire
	if(Critter->GetShooting())
	{
			Critter->GetRectShoot(&rc,&posx,&posy,Scene);
			g_pSprite->Draw(texSlash,&rc,NULL, 
							&D3DXVECTOR3(float(posx),float(posy),0.0f), 
							0xFFFFFFFF);
	}
	
	return true;
}

bool cGraphicsLayer::DrawBoss(cScene *Scene, cBoss *Boss)
{
	RECT rc;
	float ix,iy;
	if (Boss->isActive()){
		Boss->GetRect(&rc,&ix,&iy,Scene);
		g_pSprite->Draw(texBoss,&rc,NULL, 
						&D3DXVECTOR3(ix,iy,0.0f), 
						0xFFFFFFFF);
		Boss->GetRectPilar(&rc, &ix, &iy, Scene);
		 g_pSprite->Draw(texBoss, &rc, NULL,
			&D3DXVECTOR3(ix, iy, 0.0f),
			0xFFFFFFFF);
	}

	return true;
}

bool cGraphicsLayer::DrawDebug(cScene *Scene, cMouse *Mouse)
{
	//--- SELECTED TILE ---
	float tx,ty,atx,aty,dx,dy;
	int mouseX,mouseY;
	Mouse->GetPosition(&mouseX,&mouseY);
	Scene->TileSelectedDebug(mouseX,mouseY,&tx,&ty,&atx,&aty,&dx,&dy);

	int screenTileX = floor(tx)-Scene->cx;
	int screenTileY = floor(ty)-Scene->cy;
	float screenX = ISO_OFFSET_X + TILE_SIZE_X*((float)screenTileX-screenTileY)/2;
	float screenY = ISO_OFFSET_Y + TILE_SIZE_Y*((float)screenTileX+screenTileY)/2;

	g_pSprite->Draw(texTilesIso[4],NULL,NULL, 
							&D3DXVECTOR3( screenX, screenY, 0.0f), 
							0xFFFF0000);

	//--- INFO ---
	RECT rc;
	int cx,cy;
	cx = Scene->cx; cy = Scene->cy;
	SetRect( &rc, 400, 350, 320, 544 );
	char *info = (char *)malloc(sizeof(char)*200);

	int index = SCENE_AREA*cy + cx;
	sprintf(info,"Map = %d x %d\nScene = %d , %d\nMouse = (%d,%d)\nTile = (%f,%f)\nDirect = (%f,%f)\nIncr = (%f,%f) (incremets are for pussies)\nLevel = %d\nEvent Type = %d",
			SCENE_WIDTH,SCENE_HEIGHT,cx,cy,mouseX,mouseY,tx,ty,atx,aty,dx,dy,Scene->level,Scene->mapLogic[index]);

	font->DrawText(	NULL, info, -1, &rc, DT_NOCLIP, 
					D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	SetRect( &rc, 5, 5, 69, 149 );
	font->DrawText(	NULL, info, -1, &rc, DT_NOCLIP, 
				D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	return true;
}

bool cGraphicsLayer::DrawUI(cCritter *Critter)
{
	RECT rc;

	g_pSprite->Draw(texUI,NULL,NULL, 
					&D3DXVECTOR3(float((800-611)/2),float(600-172),0.0f), 
					0xFFFFFFFF);
	SetRect(&rc, 0, 0, 65, 105);
	g_pSprite->Draw(texHP, &rc, NULL,
		&D3DXVECTOR3(float((800 - 611) / 2) + 9.0f, float(600 - 105), 0.0f),
		0xFFFFFFFF);
	SetRect(&rc, 65, 100-Critter->GetHP(), 130, 105);
	g_pSprite->Draw(texHP, &rc, NULL,
		&D3DXVECTOR3(float((800 - 611) / 2) + 9.0f, float(600 - 105 + 100 - Critter->GetHP()), 0.0f),
		0xFFFFFFFF);
	SetRect(&rc, 115 * Critter->GetMana(), 0, 115 * (Critter->GetMana() + 1), 105);
	g_pSprite->Draw(texMana, &rc, NULL,
		&D3DXVECTOR3(float(611-28) , float(600 - 105), 0.0f),
		0xFFFFFFFF);



	SetRect(&rc, 50, 50, 100, 100);

	g_pSprite->Draw(texIcons, &rc, NULL,
		&D3DXVECTOR3(float(611 - 320), float(600 - 70), 0.0f),
		0xFFFFFFFF);

	if (Critter->GetMana() > 0)
	{
		SetRect(&rc, 100, 50 + Critter->GetCD1(), 150, 100);
	}
	else
	{
		SetRect(&rc, 0, 50 + Critter->GetCD1(), 50, 100);
	}

		g_pSprite->Draw(texIcons, &rc, NULL,
			&D3DXVECTOR3(float(611 - 320), float(600 - 70 + Critter->GetCD1()), 0.0f),
			0xFFFFFFFF);

		//

		SetRect(&rc, 50, 0, 100, 50);

		g_pSprite->Draw(texIcons, &rc, NULL,
			&D3DXVECTOR3(float(611 - 220), float(600 - 70), 0.0f),
			0xFFFFFFFF);

		if (Critter->GetMana() > 0)
		{
			SetRect(&rc, 100, Critter->GetCD2(), 150, 50);
		}
		else
		{
			SetRect(&rc, 0, Critter->GetCD2(), 50, 50);
		}

		g_pSprite->Draw(texIcons, &rc, NULL,
			&D3DXVECTOR3(float(611 - 220), float(600 - 70 + Critter->GetCD2()), 0.0f),
			0xFFFFFFFF);

	return true;
}

bool cGraphicsLayer::DrawMouse(cMouse *Mouse)
{
	RECT rc;
	int mx,my,posx,posy;

	//Mouse selection box
	Mouse->GetPosition(&mx,&my);

	if(Mouse->GetSelection()==SELECT_SCENE)
	{
		int sx,sy;
		Mouse->GetSelectionPoint(&sx,&sy);
		SetRect(&rc,sx,sy,mx,my);
		DrawRect(rc,0x0000ff00);
	}

	//Mouse
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		
		Mouse->GetRect(&rc,&posx,&posy);
		HRESULT hr = g_pSprite->Draw(texMouse,&rc,NULL,&D3DXVECTOR3(float(mx+posx),float(my+posy),0.0f),0xFFFFFFFF);
		if(FAILED(hr))
		{
			cLog *Log = cLog::Instance();
			Log->Error(hr,"mouse pointer");
			return false;
		}

	g_pSprite->End();

	return true;
}

bool cGraphicsLayer::DrawRect(RECT rc, D3DCOLOR color)
{
	RECT rect;
	int xo,yo,xf,yf;

	if((rc.left==rc.right)&&(rc.top==rc.bottom)) return false;

	if(rc.left < rc.right)
	{
		xo = rc.left;	xf = rc.right;
	}
	else
	{
		xo = rc.right;	xf = rc.left;
	}
	if(rc.top < rc.bottom)
	{
		yo = rc.top;	yf = rc.bottom;
	}
	else
	{
		yo = rc.bottom;	yf = rc.top;
	}

	//Top
	SetRect(&rect,xo,yo,xf+1,yo+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Bottom
	SetRect(&rect,xo,yf,xf,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Left
	SetRect(&rect,xo,yo,xo+1,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Right
	SetRect(&rect,xf,yo,xf+1,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	
	return true;
}

