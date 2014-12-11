
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>


cScene::cScene()
{
	cx=0;
	cy=0;
}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	int i,j,k,n;
	
	map=(int *)malloc(sizeof(int)*(SCENE_AREA*SCENE_AREA));

	FILE *f;
	f=fopen("map.txt","r");
	k=0;
	for(i=0;i<SCENE_AREA;i++)
	{
		for(j=0;j<SCENE_AREA;j++)
		{
			fscanf(f,"%d",&n);
			map[k]=n%2;				/// TREURE %2, parche perque funcioni el mapa antic amb les tiles de iso
			k++;
		}
	}

	fclose(f);
}

void cScene::Move(int pointer)
{
	//map=32x32, visible=20x17 => move=0..32-20,0..32-17=0..12,0..15

	//Up
	if((pointer==MN)||(pointer==MNO)||(pointer==MNE))
	{
		if(cy>0) cy--;
	}
	//South
	else if((pointer==MS)||(pointer==MSO)||(pointer==MSE))
	{
		if(cy<SCENE_AREA-SCENE_HEIGHT) cy++;
	}
	//West
	if((pointer==MO)||(pointer==MNO)||(pointer==MSO))
	{
		if(cx>0) cx--;
	}
	//East
	else if((pointer==ME)||(pointer==MNE)||(pointer==MSE))
	{
		if(cx<SCENE_AREA-SCENE_WIDTH) cx++;
	}
}

void cScene::MoveByRadar(int x,int y)
{
	//move=0..12,0..15
	//rect=(80x68)/4=20x17

	cx=(x-RADAR_Xo)>>2;
	cy=(y-RADAR_Yo)>>2;

	if(cx<=9)		cx=0;
	else if(cx>=21)	cx=12;
	else			cx-=9;
	
	if(cy<=8)		cy=0;
	else if(cy>=23)	cy=15;
	else			cy-=8;
}

bool cScene::Visible(int cellx,int celly)
{
	return ((cellx>=cx)&&(cellx<cx+SCENE_WIDTH)&&(celly>=cy)&&(celly<cy+SCENE_HEIGHT)) ? 1 : 0;
}

void cScene::TileSelected(int mx, int my, int *tx, int *ty, int *atx, int *aty, int *Dx, int *Dy)
{
	int rx,ry,dx,dy;

	*tx=mx-(SCENE_Xo);
	*ty=my-(SCENE_Yo);

	rx=*tx%TILE_SIZE_X;
	ry=*ty%TILE_SIZE_Y;

	*tx=*tx/TILE_SIZE_X;
	*ty=(*ty/TILE_SIZE_Y)*2;

	*atx=*tx;
	*aty=*ty;

	dx=0;
	dy=0;

	//         /\
	// -1,-1  /  \   0,-1
	//       /0,0 \
	//       \    /
	// -1, 1  \  /   0, 1
	//         \/

	if(rx<TILE_SIZE_X/2)
	{
		//Left
		if(ry<TILE_SIZE_Y/2)
		{
			//Up
			if( ((TILE_SIZE_Y/2)-(rx*2)) > ry ) { dx=-1; dy=-1; }
		}
		else
		{	
			//Down
			if( ((TILE_SIZE_Y/2)+(rx*2)) < ry ) { dx=-1; dy=1; }
		}
	}
	else
	{
		rx-=TILE_SIZE_X/2;

		//Right
		if(ry<16)
		{
			//Up
			if( (rx*2) > ry ) dy=-1;
		}
		else
		{	
			//Down
			if( ((TILE_SIZE_X/2)-(rx*2)) < ry ) dy=1;
		}
	}

	*tx+=dx;
	*ty+=dy;

	*Dx=dx;
	*Dy=dy;
}

void cScene::IsoToCartesian(float ix, float iy, int& cx, int& cy)
{
	// 45deg roration of the 2d reticle
	cx = (int)(2 * iy + ix) / 2;
	cy = (int)(2 * iy - ix) / 2;
}

void cScene::CartesianToIso(int cx, int cy, float& ix, float& iy)
{
	// 45deg roration of the 2d reticle
	ix = cx - cy;
	iy = 0.5*(cx + cy);

}