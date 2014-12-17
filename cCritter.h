
#ifndef __CRITTER_H__
#define __CRITTER_H__

#include <windows.h>
#include "cTrajectory.h"
#include "cPath.h"

class cScene;

class cCritter
{
public:
	cCritter(void);
	virtual ~cCritter(void);
	void Init(int x, int y);

	void GoToCell(int *map,int destcx,int destcy,int type);  //type= 0 - normal, 1 - attack
	void GoToEnemy(int *map,int destcx,int destcy,int targetId);
	void Move();
	void GetRect(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectLife(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectShoot(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);
	void SetSelected(bool sel);
	bool GetSelected();
	bool GetShooting();
	void stopAttack();
	int getTarget();
	void setTarget(int t);
	bool IsFiring();
	bool isHit();
	int getDamage();

	void PutInStart(int level); //Puts the player at the level start

private:
	int x,y;			//Position in total map
	int ix,iy;			//Position in total isometric map
	int cx,cy;			//Cell position in total map in cartesian 
	int icx, icy;		//Cell position in total map in isometric
	bool selected;		//Selected for move or attack
	int hp, energy, damage, attackSpeed;
	int target;

	//cTrajectory Trajectory;
	cPath Trajectory;

	int seq;			//Sequence animation control
	int delay;			//Animation delay

	bool attack;		//Order to attack established (moving for attack)
	bool shoot;			//Begin attack (to shoot)
	int shoot_seq;		//Shooter sequence animation control
	int shoot_delay;	//Shooter animation delay
	int sprite_height;	//Offset to aply on the vertical axis in case the sprite is larger than 1 tile
};

#endif