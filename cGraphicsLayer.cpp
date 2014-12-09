
#include "cGraphicsLayer.h"
#include "cLog.h"
#include <stdio.h>


cGraphicsLayer::cGraphicsLayer()
{
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

	D3DXCreateTextureFromFileEx(g_pD3DDevice,"1.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTiles[0]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"2.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTiles[1]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"3.bmp",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0xFF408080,NULL,NULL,&texTiles[2]);
	
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"cube_white.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texGrid[0]);
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"cube_black.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texGrid[1]);

	D3DXCreateTextureFromFileEx(g_pD3DDevice,"mouse.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00FF00FF,NULL,NULL,&texMouse);
}

void cGraphicsLayer::UnLoadData()
{
	if(texMouse)
	{
		texMouse->Release();
		texMouse = NULL;
	}
	if(texTiles[0])
	{
		texTiles[0]->Release();
		texTiles[0] = NULL;
	}
	if(texTiles[1])
	{
		texTiles[1]->Release();
		texTiles[1] = NULL;
	}
	if(texTiles[2])
	{
		texTiles[2]->Release();
		texTiles[2] = NULL;
	}
	if(texGrid[0])
	{
		texGrid[0]->Release();
		texGrid[0] = NULL;
	}
	if(texGrid[1])
	{
		texGrid[1]->Release();
		texGrid[1] = NULL;
	}
	if(g_pSprite)
	{
		g_pSprite->Release();
		g_pSprite = NULL;
	}
}

void cGraphicsLayer::Render(int mouseX,int mouseY,cScene *Scene)
{
	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xFF000000, 0, 0 );
	g_pD3DDevice->BeginScene();

		//--- SPRITES ---

		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

			int i,j;
			//Tiles
			for(j=0;j<SCENE_HEIGHT;j++)
			{
				for(i=0;i<SCENE_WIDTH;i++)
				{
					g_pSprite->Draw(texTiles[Scene->map[i][j]],NULL,NULL, 
									&D3DXVECTOR3( float(SCENE_Xo+((j%2)<<5)+(i<<6)), float(SCENE_Yo+(j<<4)), 0.0f), 
									0xFFFFFFFF);
				}
			}
			//Rectangular Grid
			for(j=0;j<600;j+=32)
			{
				for(i=0;i<800;i+=32)
				{
					if(j%64 || i%64)
					g_pSprite->Draw(texGrid[1],NULL,NULL, 
									&D3DXVECTOR3((float)i, (float)j, 0.0f), 
									0x33FFFFFF);
					else
					g_pSprite->Draw(texGrid[0],NULL,NULL, 
									&D3DXVECTOR3((float)i, (float)j, 0.0f), 
									0x33FFFFFF);
				}
			}

			//--- SELECTED ---
			int tx,ty,atx,aty,dx,dy;
			Scene->TileSelected(mouseX,mouseY,&tx,&ty,&atx,&aty,&dx,&dy);

			g_pSprite->Draw(texTiles[2],NULL,NULL, 
							&D3DXVECTOR3( float(SCENE_Xo+((ty%2)<<5)+(tx<<6)), float(SCENE_Yo+(ty<<4)), 0.0f), 
							0x77FFFFFF);
			//--- MOUSE ---
			g_pSprite->Draw(texMouse,NULL,NULL, 
							&D3DXVECTOR3( float(mouseX), float(mouseY), 0.0f), 
							0xFFFFFFFF);

		g_pSprite->End();

		//--- INFO ---
		RECT rc;
		SetRect( &rc, 64, 350, 128, 400 );
		char *info = (char *)malloc(sizeof(char)*200);
		sprintf(info,"Map = %d x %d\nMouse = (%d,%d)\nTile = (%d,%d)\nDirect = (%d,%d)\nIncr = (%d,%d)",
				SCENE_WIDTH,SCENE_HEIGHT,mouseX,mouseY,tx,ty,atx,aty,dx,dy);

		font->DrawText(	NULL, info, -1, &rc, DT_NOCLIP, 
						D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}
