
#ifndef __SCENE_H__
#define __SCENE_H__

//Resolution
#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

//Visible part
#define SCENE_WIDTH		25
#define SCENE_HEIGHT	19 

//Dimension = Area^2
#define SCENE_AREA		64  // Level size 64x64
//#define SCENE_ISO_X		50  // (550+32*32)/32
//#define SCENE_ISO_X		50  // (550+32*32)/32
//#define SCENE_ISO_Y		32
#define SCENE_ISO_X		80  // (550+64*64)/64
#define SCENE_ISO_Y		64


// Tile Size
#define TILE_SIZE_X		64
#define TILE_SIZE_Y		32

// Map Offset
#define ISO_OFFSET_X	550
#define ISO_OFFSET_Y	-58

//Map coordinate beginning
#define SCENE_Xo		0 //0-32
#define SCENE_Yo		0  //56-16
#define SCENE_Xf		( SCENE_Xo + (SCENE_WIDTH*TILE_SIZE_X) )
#define SCENE_Yf		( SCENE_Yo + (SCENE_HEIGHT*TILE_SIZE_Y) )

//Radar position (left,top)
#define RADAR_Xo		672
#define RADAR_Yo		60
#define RADAR_Xf		800
#define RADAR_Yf		188

class cScene
{

public:
	
	cScene();
	virtual ~cScene();

	void LoadMap(char *file);
	void LoadMapLogic();
	void LoadMapTiles();
	void LoadMapFloor();


	void Move(int pointer); // ajustar condicions? --v
	void MoveByRadar(int x,int y);
	bool Visible(int cellx,int celly);
	void TileSelectedDebug(int mouse_x,int mouse_y,float *tx, float *ty,float *atx,float *aty,float *Dx,float *Dy);  // Note: tx and ty are floats, in order to get de correct tile use [floor(tx),floor(ty)].
	void TileSelected(int mouse_x,int mouse_y,float *tx, float *ty);

	void IsoToCartesian(float ix, float iy, int& cx, int& cy); // Method to tansform isometric to cartesian coordinates
	void CartesianToIso(int cx, int cy, float& ix, float& iy); // Method to tansform cartesian to isometric coordinates

	int GetLevel();

	int level; //level number
	int *map; // level map
	int *mapLogic; //Map level with event triggers
	int *mapTiles;
	int *mapFloor;

	int cx,cy; // cartesian coordinates
	float isox, isoy; // isometric coordinates
};


#endif
