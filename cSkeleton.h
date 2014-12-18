
#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <windows.h>
#include "cCritter.h"
#include <cmath>

enum MonsterType {SKELETON_TYPE, GOLEM_TYPE, FIRELOCK_TYPE, EXPLOSION_TYPE};

class cScene;

class cSkeleton
{
public:
	cSkeleton(void);
	virtual ~cSkeleton(void);
	void Init(int x, int y, int hp, int damage, int step_length, int sight, int range, MonsterType type);

	void GoToCell(int *map, int destcx, int destcy,int type); //type= 0 - normal, 1 - attack
	void GoToPlayer(int *map, int destcx, int destcy);
	void Move();
	void GetRect(RECT *rc,float *posx,float *posy,cScene *Scene, bool update);
	void GetIsoPos(float *posx,float *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);
	bool GetShooting();
	int getDamage();
	bool isHit();
	MonsterType GetType();
	bool isActive();

	void SetActive(bool turn);

	void updateAttackSeq();
	void reduceHP(int x);

	// Enemy IA
	void LookForPlayer(cCritter& thePlayer);
	void AttackPlayer(int* map,cCritter& thePlayer);
	bool PlayerIsDetected();

private:
	bool playerDetected;	// Flag for player detection
	int sightRadius;		//Radius of sight
	int x,y;				//Position in total map
	int cx,cy;				//Cell position in total map in cartesian
	float ix, iy;				//Position in total map in isometric
	int sprite_height;		//Offset to aply on the vertical axis in case the sprite is larger than 1 tile

	bool active;
	int hp,damage,step_length,attackRange;
	MonsterType type;

	cPath Trajectory;		// Enemy path

	int seq;				//Sequence animation control
	int delay;				//Animation delay

	bool attack;			//Order to attack established (moving for attack)
	bool shoot;				//Begin attack (to shoot)
	int shoot_seq;			//Shooter shoot_seq animation control
	int shoot_delay;		//Shooter animation delay
};

#endif