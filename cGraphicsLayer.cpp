
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
}

void cGraphicsLayer::UnLoadData()
{
	if(texMain)
	{
		texMain->Release();
		texMain = NULL;
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
	if(texMouse)
	{
		texMouse->Release();
		texMouse = NULL;
	}
	if(g_pSprite)
	{
		g_pSprite->Release();
		g_pSprite = NULL;
	}
}

bool cGraphicsLayer::Render(int state,cMouse *Mouse,cScene *Scene,cCritter *Critter,cSkeleton *Skeleton)
{
	//HRESULT Draw( LPDIRECT3DTEXTURE9 pTexture, CONST RECT *pSrcRect,
	//				CONST D3DXVECTOR3 *pCenter,  CONST D3DXVECTOR3 *pPosition,
	//				D3DCOLOR Color);

	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xFF000000, 0, 0 );
	g_pD3DDevice->BeginScene();

		//--- SPRITES ---
		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

			switch(state)
			{
				case STATE_MAIN:
								g_pSprite->Draw(texMain,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF);
								break;

				case STATE_GAME:
								DrawScene(Scene);
								DrawUnits(Scene,Critter,Skeleton);
								DrawDebug(Scene, Mouse);
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

	//for(y=Scene->cy;y<fy;y++)
	for(y=0;y<SCENE_AREA;++y)
	{
		//int _panty = SCENE_Yo + ((y-Scene->cy)*TILE_SIZE_Y/2);
		
		//for(x=Scene->cx;x<fx;x++)
		for(x=0;x<SCENE_AREA;++x)
		{
			//int _pantx = SCENE_Xo + ((x-Scene->cx)*TILE_SIZE_X)+(((y-Scene->cy)%2)*(TILE_SIZE_X/2));
			int screenTileX = x-Scene->cx;
			int screenTileY = y-Scene->cy;
			float screenX = ISO_OFFSET_X + TILE_SIZE_X*((float)screenTileX-screenTileY)/2;
			float screenY = TILE_SIZE_Y*((float)screenTileX+screenTileY)/2;

			n = Scene->map[(y*SCENE_AREA)+x];
			g_pSprite->Draw(texTilesIso[n],NULL,NULL, 
									&D3DXVECTOR3( screenX, screenY, 0.0f), 

									0xFFFFFFFF);
			//SetRect(&rc,n<<5,0,(n+1)<<5,32);
			//g_pSprite->Draw(texTiles,&rc,NULL, 
			//				&D3DXVECTOR3(float(pantx),float(panty),0.0f), 
			//				0xFFFFFFFF);
		}
	}

	//Draw radar
	x=RADAR_Xo+(Scene->cx<<2);
	y=RADAR_Yo+(Scene->cy<<2);
	SetRect(&rc,0,32,80,100);
	g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(x),float(y),0.0f), 
					0xFFFFFFFF);
	return true;
}

bool cGraphicsLayer::DrawUnits(cScene *Scene,cCritter *Critter,cSkeleton *Skeleton)
{
	int cx,cy,posx,posy;
	RECT rc;

	//Draw Critter
	Critter->GetCell(&cx,&cy);
	if(Scene->Visible(cx,cy))
	{
		Critter->GetRect(&rc,&posx,&posy,Scene);

		g_pSprite->Draw(texWarlock,&rc,NULL, 
						&D3DXVECTOR3(float(posx),float(posy),0.0f), 
						0xFFFFFFFF);
		if(Critter->GetSelected())
		{
			Critter->GetRectLife(&rc,&posx,&posy,Scene);
			g_pSprite->Draw(texMouse,&rc,NULL, 
							&D3DXVECTOR3(float(posx),float(posy),0.0f), 
							0xFFFFFFFF);
		}
	}
	Critter->GetRectRadar(&rc,&posx,&posy);
	g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);
	//Draw Skeleton
	Skeleton->GetCell(&cx,&cy);
	if(Scene->Visible(cx,cy))
	{
		Skeleton->GetRect(&rc,&posx,&posy,Scene);
		g_pSprite->Draw(texCharacters,&rc,NULL, 
						&D3DXVECTOR3(float(posx),float(posy),0.0f), 
						0xFFFFFFFF);
	}
	Skeleton->GetRectRadar(&rc,&posx,&posy);
	g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);
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
	return true;
}

bool cGraphicsLayer::DrawDebug(cScene *Scene, cMouse *Mouse)
{
	//--- SELECTED TILE ---
	float tx,ty,atx,aty,dx,dy;
	int mouseX,mouseY;
	Mouse->GetPosition(&mouseX,&mouseY);
	Scene->TileSelected(mouseX,mouseY,&tx,&ty,&atx,&aty,&dx,&dy);

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
	sprintf(info,"Map = %d x %d\nScene = %d , %d\nMouse = (%d,%d)\nTile = (%f,%f)\nDirect = (%f,%f)\nIncr = (%f,%f) (incremets are for pussies)",
			SCENE_WIDTH,SCENE_HEIGHT,cx,cy,mouseX,mouseY,tx,ty,atx,aty,dx,dy);

	font->DrawText(	NULL, info, -1, &rc, DT_NOCLIP, 
					D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	SetRect( &rc, 5, 5, 69, 149 );
	font->DrawText(	NULL, info, -1, &rc, DT_NOCLIP, 
				D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

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