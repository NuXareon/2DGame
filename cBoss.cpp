#include "cBoss.h"


cBoss::cBoss(void)
{
	
	x=320;y=1600;
	cx=10;cy=50;
	sprite_height = 68;
	ix=iy=1024;
	active=true;
	hp=100;
	damage=15;

	seq=0;
	delay=0;
}


cBoss::~cBoss(void)
{
}

void cBoss::GetRect(RECT *rc,float *posx,float *posy,cScene *Scene) 
{
	float offX = ix - sprite_height;
	float offY = iy - sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX - Scene->cx*TILE_SIZE_X) - (offY - Scene->cy*TILE_SIZE_X)) / 2;
	*posy = ((float)(offX - Scene->cx*TILE_SIZE_Y) + (offY - Scene->cy*TILE_SIZE_Y)) / 2;

	SetRect(rc,0,0,110,175);
}
int cBoss::getDamage()
{
	return damage;
}
bool cBoss::isActive()
{
	return active;
}

void cBoss::SetActive(bool turn)
{
}

void cBoss::updateAttackSeq()
{
}