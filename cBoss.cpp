#include "cBoss.h"


cBoss::cBoss(void)
{
	
	x=320;y=1600;
	cx=10;cy=50;
	sprite_height = 143;
	ix=-320;
	iy=2240;
	active=true;
	hp=1000;
	damage=15;

	seq=0;
	delay=0;
	delay2 = 0;
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

	SetRect(rc,110*seq,0,110*(seq+1),175);
}
void cBoss::GetRectPilar(RECT *rc, float *posx, float *posy, cScene *Scene)
{
	float offX = ix - sprite_height;
	float offY = iy - sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX - cx*TILE_SIZE_X) - (offY - cy*TILE_SIZE_X)) / 2;
	*posy = ((float)(offX - cx*TILE_SIZE_Y) + (offY - cy*TILE_SIZE_Y)) / 2;
	if (seqpilar<=6)SetRect(rc, 110 * seqpilar, 2450, 110 * (seqpilar + 1), 2625);
	else SetRect(rc, 110 * (seqpilar - 7), 2625, 110 * (seqpilar - 6), 2800);
}
void cBoss::GetCell(int *cellx,int *celly)	
{
	*cellx = cx;
	*celly = cy;
}
int cBoss::getDamage()
{
	return damage;
}
void cBoss::reduceHP(int damage)
{
	hp-=damage;
	if(hp <= 0) active=false;
}
bool cBoss::isActive()
{
	return active;
}

void cBoss::SetActive(bool turn)
{
}

void cBoss::updateAttackSeq(cCritter Player)
{
	if (seqpilar == 0) Player.GetCell(&cxpilar, &cypilar);
	delay2++;
	if (delay2 == 100)
	{
		seqpilar++;
		if (seqpilar == 16)
		{
			seqpilar = 0;
		}
		delay2 = 0;
	}

}

void cBoss::updateBoss()
{
	delay++;
	if (delay == 100)
	{
		seq++;
		if (seq == 6)
		{
			seq = 0;
		}
		delay = 0;
	}
}