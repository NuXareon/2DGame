
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>
#include "cLog.h"



cScene::cScene():
level(1)
{
	cx=0;
	cy=0;

	map = (int *)malloc(sizeof(int)*(SCENE_AREA*SCENE_AREA));
	mapLogic = (int *)malloc(sizeof(int)*(SCENE_AREA*SCENE_AREA));
	mapTiles = (int *)malloc(sizeof(int)*(SCENE_AREA*SCENE_AREA));

}

cScene::~cScene()
{
	free(map);
	free(mapLogic);
	free(mapTiles);
}

void cScene::LoadMap(char *file)
{
	int i,j,k,n;

	cLog *Log = cLog::Instance();

	if (level == 1)
	{
		FILE *f;
		f = fopen("level1.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				map[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 2)
	{
		FILE *f;
		f = fopen("level2.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				map[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 3)
	{
		FILE *f;
		f = fopen("level3.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				map[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 4)
	{
		FILE *f;
		f = fopen("level4.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				map[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else
	{
		FILE *f;
		f = fopen("map.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				map[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	
}


void cScene::LoadMapLogic()
{
	int i, j, k, n;

	cLog *Log = cLog::Instance();

	if (level == 1)
	{
		FILE *f;
		f = fopen("level1_Logic.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapLogic[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 2)
	{
		FILE *f;
		f = fopen("level2_Logic.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapLogic[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 3)
	{
		FILE *f;
		f = fopen("level3_Logic.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapLogic[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 4)
	{
		FILE *f;
		f = fopen("level4_Logic.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapLogic[k] = n;
				k++;
			}
		}

		fclose(f);
	}

}



void cScene::LoadMapTiles()
{
	int i, j, k, n;

	cLog *Log = cLog::Instance();

	if (level == 1)
	{
		FILE *f;
		f = fopen("level1_Tiles.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapTiles[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 2)
	{
		FILE *f;
		f = fopen("level2_Tiles.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapTiles[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 3)
	{
		FILE *f;
		f = fopen("level3_Tiles.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapTiles[k] = n;
				k++;
			}
		}

		fclose(f);
	}
	else if (level == 4)
	{
		FILE *f;
		f = fopen("level4_Tiles.txt", "r");
		k = 0;
		for (i = 0; i<SCENE_AREA; i++)
		{
			for (j = 0; j<SCENE_AREA; j++)
			{
				fscanf(f, "%d", &n);
				mapTiles[k] = n;
				k++;
			}
		}

		fclose(f);
	}

}
void cScene::Move(int pointer)
{
	//map=32x32, visible=20x17 => move=0..32-20,0..32-17=0..12,0..15

	//Up
	if((pointer==MN)||(pointer==MNO)||(pointer==MNE))
	{
		if(cy>0) cy--;
		if(cx>0) cx--;
	}
	//South
	else if((pointer==MS)||(pointer==MSO)||(pointer==MSE))
	{
		if(cy<SCENE_ISO_Y-SCENE_HEIGHT) cy++;
		if(cx<SCENE_ISO_X-SCENE_WIDTH) cx++;
	}
	//West
	if((pointer==MO)||(pointer==MNO)||(pointer==MSO))
	{
		if(cy<SCENE_ISO_Y-SCENE_HEIGHT) cy++;
		if(cx>0) cx--;
	}
	//East
	else if((pointer==ME)||(pointer==MNE)||(pointer==MSE))
	{
		if(cy>0) cy--;
		if(cx<SCENE_ISO_X-SCENE_WIDTH) cx++;
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

void cScene::TileSelectedDebug(int mx, int my, float *tx, float *ty, float *atx, float *aty, float *Dx, float *Dy)
{
	int rx, ry;
	float dx, dy;

	*tx=(my+(mx-ISO_OFFSET_X-TILE_SIZE_X/2)/2)-(SCENE_Xo);
	*ty=(my-(mx-ISO_OFFSET_X-TILE_SIZE_Y)/2)-(SCENE_Yo);

	rx=(int)*tx%TILE_SIZE_X;
	ry=(int)*ty%TILE_SIZE_Y;

	*tx=*tx/TILE_SIZE_Y+cx;
	*ty=*ty/TILE_SIZE_Y+cy;

	*atx=*tx;
	*aty=*ty;

	dx=0;
	dy=0;
	/*
	int _x = x-Scene->cx;
	int _y = y-Scene->cy;
	float __pantx = 550 + TILE_SIZE_X*((float)_x-_y)/2;
	float __panty = TILE_SIZE_Y*((float)_x+_y)/2;
	*/
	// old Jesus
	//         /\
	// -1,-1  /  \   0,-1
	//       /0,0 \
	//       \    /
	// -1, 1  \  /   0, 1
	//         \/


	//          /\
	// -1,1/2  /  \   1,-1/2
	//        /0,0 \
	//        \    /
	// -1,1/2  \  /   1, 1/2
	//          \/
	/*
	if(rx<TILE_SIZE_X/2)
	{
		//Left
		if(ry<TILE_SIZE_Y/2)
		{
			//Up
			if( ((TILE_SIZE_Y/2)-(rx*2)) > ry ) { dx=-1; dy=-1; }
			//if( ((TILE_SIZE_Y/2)-(rx*2)) > ry ) { dx = -1; dy = -0.5; }

		}
		else
		{	
			//Down
			if (((TILE_SIZE_Y / 2) + (rx * 2)) < ry) { dx = -1; dy = 1; }
			//if (((TILE_SIZE_Y / 2) + (rx * 2)) < ry) { dx = -1; dy = 0.5; }

		}
	}
	else
	{
		rx-=TILE_SIZE_X/2;

		//Right
		if(ry<16)
		{
			//Up
			if ((rx * 2) > ry){ dx = 1;  dy = -0.5; }
		}
		else
		{	
			//Down
			if (((TILE_SIZE_X / 2) - (rx * 2)) < ry) { dy = 0.5; }
		}		
	}
	
	*tx+=dx;
	*ty+=dy;
	*/
	*Dx=0.0f;
	*Dy=0.0f;
}

void cScene::TileSelected(int mouse_x,int mouse_y,float *tx, float *ty)
{
	 float atx, aty, Dx, Dy;
	 TileSelectedDebug(mouse_x, mouse_y, tx, ty, &atx, &aty, &Dx, &Dy);
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


int cScene::GetLevel()
{
	return level;
}