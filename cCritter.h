
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
	void GetIsoCell(float *cellx,float *celly);
	void SetSelected(bool sel);
	bool GetSelected();
	bool GetShooting();
	void stopAttack();
	int getTarget();
	void setTarget(int t);
	bool IsFiring();
	bool isHit();
	bool isDead();
	int getDamage();
	bool reduceHP(int hp);
	void increaseHP(int hp);
	void enemyFaced(int enemy_cx, int enemy_cy);

	void updateAttackSeq();
	void PutInStart(int level); //Puts the player at the level start
	void Restart();
	void nextHead();
	void prevHead();
	int getHead();
	void getRectHead(RECT *rc,float *off);

	void UseSkill1(int cx, int cy, int enemyId);
	void updateSkill1Seq();
	void getSkiill1Rect(RECT *rc);
	bool GetSkill1();
	bool Skill1Hit();
	int GetSkill1Target();
	int getSkill1Damage();
	int GetSkill1Range();

	void UseSkill2();
	void updateSkill2Seq();
	void getSkill2Rect(RECT *rc);
	bool GetSkill2();
	bool Skill2Hit();
	int getSkill2Damage();

	int GetHP();
	int GetMana();
	int GetCD1();
	int GetCD2();

private:
	int x,y;			//Position in total map
	float ix,iy;		//Position in total isometric map
	int cx,cy;			//Cell position in total map in cartesian 
	int icx, icy;		//Cell position in total map in isometric
	bool selected;		//Selected for move or attack
	int hp, energy, damage, step_length;
	int target;
	int head;
	float head_offset;
	bool head_dir;

	//cTrajectory Trajectory;
	cPath Trajectory;

	int seq;			//Sequence animation control
	int delay;			//Animation delay

	bool attack;		//Order to attack established (moving for attack)
	bool shoot;			//Begin attack (to shoot)
	int shoot_seq;		//Shooter sequence animation control
	int shoot_delay;	//Shooter animation delay
	int shoot_dir;
	
	bool skill1;
	int skill1x,skill1y;
	int skill1_seq;
	int skill1_delay;
	int skill1_cd;
	int skill1_range;
	int skill1_cost;
	int skill1_damage;
	int skill1_target;

	bool skill2 = false;
	int skill2_seq;
	int skill2_delay;
	int skill2_cd;
	int skill2_cost;
	int skill2_damage;

	int sprite_height;	//Offset to aply on the vertical axis in case the sprite is larger than 1 tile
};

#endif