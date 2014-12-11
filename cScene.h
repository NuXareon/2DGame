
#ifndef __SCENE_H__
#define __SCENE_H__

//Resolution
#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

//Visible part
#define SCENE_WIDTH		11 
#define SCENE_HEIGHT	32 // modificar (ara mateix el mapa no es prou gran)

//Dimension = Area^2
#define SCENE_AREA		32

// Tile Size
#define TILE_SIZE_X		64
#define TILE_SIZE_Y		32

//Map coordinate beginning
#define SCENE_Xo		-32 //0-32
#define SCENE_Yo		40  //56-16
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
	void Move(int pointer); // ajustar condicions? --v
	void MoveByRadar(int x,int y);
	bool Visible(int cellx,int celly);
	void TileSelected(int mouse_x,int mouse_y,int *tile_x,int *tile_y,int *atx,int *aty,int *Dx,int *Dy);

	void IsoToCartesian(float ix, float iy, int& cx, int& cy); // Method to tansform isometric to cartesian coordinates
	void CartesianToIso(int cx, int cy, float& ix, float& iy); // Method to tansform cartesian to isometric coordinates

	int *map;
	int cx,cy; // cartesian coordinates
	float isox, isoy; // isometric coordinates
};


#endif
