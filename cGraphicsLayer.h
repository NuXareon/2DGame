
#ifndef __GRAPHICSLAYER_H__
#define __GRAPHICSLAYER_H__


#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxerr8.lib")
#pragma comment(lib,"winmm.lib")


#include <D3D9.h>
#include <D3DX9.h>
#include "cScene.h"


class cGraphicsLayer  
{

public:
	
	cGraphicsLayer();
	virtual ~cGraphicsLayer();

	bool Init(HWND hWnd); 
	void Finalize();
	
	void LoadData();
	void UnLoadData();
	void Render(int mouseX,int mouseY,cScene *Scene);

	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDevice;
	LPD3DXSPRITE g_pSprite;

	LPDIRECT3DTEXTURE9 texTiles[3];
	LPDIRECT3DTEXTURE9 texGrid[2];
	LPDIRECT3DTEXTURE9 texMouse;

	LPD3DXFONT font;
};


#endif