
#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <windows.h>
#include "cCritter.h"
#include <cmath>

class cScene;

class cSkeleton
{
public:
	cSkeleton(void);
	virtual ~cSkeleton(void);
	void Init(int x, int y);

	void GoToCell(int *map, int destcx, int destcy,int type); //type= 0 - normal, 1 - attack
	void GoToPlayer(int *map, int destcx, int destcy);
	void Move();
	void GetRect(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);
	
	// Enemy IA
	void LookForPlayer(cCritter& thePlayer);
	void AttackPlayer(int* map,cCritter& thePlayer);
	bool PlayerIsDetected();

private:
	bool playerDetected;	// Flag for player detection
	int sightRadius;		//Radius of sight
	int x,y;				//Position in total map
	int cx,cy;				//Cell position in total map in cartesian
	int ix, iy;				//Cell position in total map in isometric
	int sprite_height;		//Offset to aply on the vertical axis in case the sprite is larger than 1 tile

	cPath Trajectory;		// Enemy path

	int seq;				//Sequence animation control
	int delay;				//Animation delay

	bool attack;			//Order to attack established (moving for attack)
	bool shoot;				//Begin attack (to shoot)
	int shoot_seq;			//Shooter sequence animation control
	int shoot_delay;		//Shooter animation delay
};

#endif