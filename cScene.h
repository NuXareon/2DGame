
#ifndef __SCENE_H__
#define __SCENE_H__

#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

#define SCENE_WIDTH		15
#define SCENE_HEIGHT	40
#define SCENE_Xo		-32
#define SCENE_Yo		-16


class cScene
{

public:
	
	cScene();
	virtual ~cScene();

	void LoadMap(char *file);
	void TileSelected(int mx,int my,int *tx,int *ty,int *atx,int *aty,int *Dx,int *Dy);

	int map[SCENE_WIDTH][SCENE_HEIGHT];
};


#endif
