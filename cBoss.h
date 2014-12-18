#include <windows.h>
#include "cScene.h"
#include "cCritter.h"

class cBoss
{
public:
	cBoss(void);
	~cBoss(void);

	void GetRect(RECT *rc,float *posx,float *posy,cScene *Scene);
	void GetCell(int *cellx,int *celly);
	void GetRectPilar(RECT *rc, float *posx, float *posy,cScene *Scene);
	void GetPilarCell(int *cellx,int *celly);
	int getDamage();
	int getSpawnCd();
	bool isActive();
	void reduceHP(int damage);
	bool pilarIsHit();
	int getHP();

	void SetActive(bool turn);
	void SetSpawnCD(int cd);

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
	int spawn_cd;

	int seq;				//Sequence animation control
	int delay;	
	int delay2;
	int seqpilar;//Animation delay
};

