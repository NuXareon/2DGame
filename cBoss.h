#include <windows.h>
#include "cScene.h"
#include "cCritter.h"

class cBoss
{
public:
	cBoss(void);
	~cBoss(void);

	void GetRect(RECT *rc,float *posx,float *posy,cScene *Scene);
	void GetRectPilar(RECT *rc, float *posx, float *posy,cScene *Scene);
	int getDamage();
	bool isActive();

	void SetActive(bool turn);

	void updateAttackSeq(cCritter Player);
	void updateBoss();

private:
	int x,y;				//Position in total map
	int cx,cy;				//Cell position in total map in cartesian
	int cxpilar, cypilar;
	float ix, iy;			//Position in total map in isometric
	int sprite_height;		//Offset to aply on the vertical axis in case the sprite is larger than 1 tile
	int hp,damage;
	bool active;

	int seq;				//Sequence animation control
	int delay;	
	int delay2;
	int seqpilar;//Animation delay
};

