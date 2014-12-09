
#include "cScene.h"
#include <stdio.h>


cScene::cScene() {}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	int i=0,j=0,x;
	bool fin=false;
	FILE *f;
	f=fopen("map.txt","r");
	while(!fin)
	{
		fscanf(f,"%d",&x);
		map[i][j]=x;
		i++;
		if(i==SCENE_WIDTH)
		{
			i=0;
			j++;
			if(j==SCENE_HEIGHT) fin=true;
		}
	}
	fclose(f);
}

void cScene::TileSelected(int mx, int my, int *tx, int *ty, int *atx, int *aty, int *Dx, int *Dy)
{
	int rx,ry,dx,dy;

	*tx=mx-(SCENE_Xo);
	*ty=my-(SCENE_Yo);

	rx=*tx%64;
	ry=*ty%32;

	*tx=*tx>>6;
	*ty=(*ty>>5)<<1;

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

	if(rx<32)
	{
		//Left
		if(ry<16)
		{
			//Up
			if( (16-(rx>>1)) > ry ) { dx=-1; dy=-1; }
		}
		else
		{	
			//Down
			if( (16+(rx>>1)) < ry ) { dx=-1; dy=1; }
		}
	}
	else
	{
		rx-=32;

		//Right
		if(ry<16)
		{
			//Up
			if( (rx>>1) > ry ) dy=-1;
		}
		else
		{	
			//Down
			if( (32-(rx>>1)) < ry ) dy=1;
		}
	}

	*tx+=dx;
	*ty+=dy;

	*Dx=dx;
	*Dy=dy;
}
