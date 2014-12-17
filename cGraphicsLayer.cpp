
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

	//Mouse pointers
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"mouse.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texMouse);
	//Iso tiles
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"1.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[0]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"2.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[1]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"3.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTilesIso[2]);

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

}

void cGraphicsLayer::UnLoadData()
{
	if(texMain)
	{
		texMain->Release();
		texMain = NULL;
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
}

bool cGraphicsLayer::Render(int state,cMouse *Mouse,cScene *Scene,cCritter *Critter,cSkeleton *Skeleton, cSkeleton Enemies[],int nEnemies)
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
								DrawScene(Scene);
								DrawUnits(Scene,Critter,Skeleton,Enemies,nEnemies);
								DrawDebug(Scene, Mouse);
								DrawUI();
								//g_pSprite->Draw(texGame,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF); //Graphic User Interface
								break;
			}

		g_pSprite->End();

		DrawMouse(Mouse);

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return true;
}

bool cGraphicsLayer::DrawScene(cScene *Scene)
{
	RECT rc;
	int x,y,n,
		fx,fy,
		pantx,panty;

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
			float screenY = TILE_SIZE_Y*((float)screenTileX+screenTileY)/2;

			n = Scene->map[(y*SCENE_AREA)+x];
			g_pSprite->Draw(texTilesIso[n],NULL,NULL, 
									&D3DXVECTOR3( screenX, screenY, 0.0f), 

									0xFFFFFFFF);
		}
	}
	return true;
}

bool cGraphicsLayer::DrawUnits(cScene *Scene,cCritter *Critter,cSkeleton *Skeleton,cSkeleton Enemies[],int nEnemies)
{
	int cx,cy,posx,posy;
	RECT rc;

	//Draw Critter
	Critter->GetCell(&cx,&cy);
	Critter->GetRect(&rc,&posx,&posy,Scene);

	g_pSprite->Draw(texWarlock,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);
	Critter->getRectHead(&rc);
	g_pSprite->Draw(texHead, &rc, NULL,
		&D3DXVECTOR3(float(posx+8), float(posy-18), 0.0f),
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

	Skeleton->GetRect(&rc,&posx,&posy,Scene);
	g_pSprite->Draw(texSkeleton,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);

	//Draw enemies
	for (int i = 0; i < nEnemies; ++i) {
		cSkeleton enemy = Enemies[i];
		if (enemy.isActive()){
			enemy.GetRect(&rc,&posx,&posy,Scene);
			if (enemy.GetType() == SKELETON_TYPE){
				g_pSprite->Draw(texSkeleton,&rc,NULL, 
								&D3DXVECTOR3(float(posx),float(posy),0.0f), 
								0xFFFFFFFF);
			} 
			else if (enemy.GetType() == GOLEM_TYPE){
				g_pSprite->Draw(texGolem,&rc,NULL, 
								&D3DXVECTOR3(float(posx),float(posy),0.0f), 
								0xFFFFFFFF);
			}
			else if (enemy.GetType() == FIRELOCK_TYPE){
				g_pSprite->Draw(texFirelock,&rc,NULL, 
								&D3DXVECTOR3(float(posx),float(posy),0.0f), 
								0xFFFFFFFF);
			}
			else if (enemy.GetType() == EXPLOSION_TYPE){
				g_pSprite->Draw(texExplosion,&rc,NULL, 
								&D3DXVECTOR3(float(posx),float(posy),0.0f), 
								0xFFFFFFFF);
			}
		}
	}
	/*
	//Draw Fire
	if(Critter->GetShooting())
	{
		if(Critter->IsFiring())
		{
			//Advance animation & draw
			Critter->GetRectShoot(&rc,&posx,&posy,Scene);
			g_pSprite->Draw(texCharacters,&rc,NULL, 
							&D3DXVECTOR3(float(posx),float(posy),0.0f), 
							0xFFFFFFFF);
		}
		else
		{
			//Advance animation
			Critter->GetRectShoot(&rc,&posx,&posy,Scene);
		}
	}
	*/
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
	float screenY = TILE_SIZE_Y*((float)screenTileX+screenTileY)/2;

	g_pSprite->Draw(texTilesIso[0],NULL,NULL, 
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
} //572 202

bool cGraphicsLayer::DrawUI()
{
	RECT rc;
	SetRect( &rc, 0, 0, 572, 202);
	g_pSprite->Draw(texUI,&rc,NULL, 
					&D3DXVECTOR3(float((800-572)/2),float(600-202),0.0f), 
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