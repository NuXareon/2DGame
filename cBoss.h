#include <windows.h>
#include "cScene.h"

class cBoss
{
public:
	cBoss(void);
	~cBoss(void);

	void GetRect(RECT *rc,float *posx,float *posy,cScene *Scene);
	void GetCell(int *cellx,int *celly);
	int getDamage();
	bool isActive();
	void reduceHP(int damage);

	void SetActive(bool turn);

	void updateAttackSeq();

private:
	int x,y;				//Position in total map
	int cx,cy;				//Cell position in total map in cartesian
	float ix, iy;			//Position in total map in isometric
	int sprite_height;		//Offset to aply on the vertical axis in case the sprite is larger than 1 tile
	int hp,damage;
	bool active;

	int seq;				//Sequence animation control
	int delay;				//Animation delay
};

