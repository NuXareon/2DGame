
#include "cCritter.h"
#include "cTrajectory.h"
#include "cScene.h"

cCritter::cCritter()
{
	//ix = 64; iy = 64;
	//SetPosition(64,64);
	//SetCell(2,2);
	
	Init(3, 61);
	//PutInStart(2);
	SetSelected(true);
	sprite_height = 64;

	seq=0;
	delay=0;

	attack=false;
	shoot=false;
	shoot_seq=0;
	shoot_delay=0;

	skill1 = false;
	skill1_seq=0;
	skill1_delay=0;
	skill1_cd=0;
	skill1_range=10;
	skill1_cost=1;
	skill1_damage=20;

	hp=100;
	energy=6;
	damage=20;
	step_length=2;
}
cCritter::~cCritter()
{

}

void cCritter::Init(int nx, int ny)
{
	SetPosition(nx*32,ny*32);
	SetCell(nx,ny);
	ix=48*nx-16*ny;
	iy=48*ny-16*nx;
}

void cCritter::GetRect(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	int offX = ix-sprite_height;
	int offY = iy-sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX-Scene->cx*TILE_SIZE_X)-(offY-Scene->cy*TILE_SIZE_X))/2;
	*posy = ((float)(offX-Scene->cx*TILE_SIZE_Y)+(offY-Scene->cy*TILE_SIZE_Y))/2;

	switch (Trajectory.Faced()) // 0, 0, 64, 100
	{
	case STOPN:	SetRect(rc, 256, 0, 64 + (64 * 4), 100);	break;
	case STOPS: SetRect(rc, 0, 0, 64, 100);					break;
	case STOPSE:SetRect(rc, 448, 0, 64 + (64 * 7), 100);	break;
	case STOPNE:SetRect(rc, 320, 0, 64 + (64 * 5), 100);	break;
	case STOPE: SetRect(rc, 384, 0, 64 + (64 * 6), 100);	break;
	case STOPSO:SetRect(rc, 64, 0, 64 + 64, 100);			break;
	case STOPNO:SetRect(rc, 192, 0, 64 + (64 * 3), 100);	break;
	case STOPO: SetRect(rc, 128, 0, 64 + (64 * 2), 100);	break;

	case N:		SetRect(rc, 0 + (64 * seq), 800, 64 + (64 * seq), 900);		break;
	case S:		SetRect(rc, 0 + (64 * seq), 100, 64 + (64 * seq), 200);		break;
	case SE:	SetRect(rc, 0 + (64 * seq), 500, 64 + (64 * seq), 600);		break;
	case NE:	SetRect(rc, 0 + (64 * seq), 700, 64 + (64 * seq), 800);		break;
	case E:		SetRect(rc, 0 + (64 * seq), 600, 64 + (64 * seq), 700);		break;
	case SO:	SetRect(rc, 0 + (64 * seq), 200, 64 + (64 * seq), 300);		break;
	case NO:	SetRect(rc, 0 + (64 * seq), 400, 64 + (64 * seq), 500);		break;
	case O:		SetRect(rc, 0 + (64 * seq), 300, 64 + (64 * seq), 400);		break;
	}

	if(!Trajectory.IsDone())
	{
		delay++;
		if(delay>=5)
		{
			seq++;
			if(seq>7) seq=0;
			delay=0;
		}
	}
	else
	{
		seq = 0;
	}
}

void cCritter::GetRectLife(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	//Life dependency not implemented
	SetRect(rc,0,32,32,64);
}

void cCritter::GetRectShoot(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx*TILE_SIZE_X+(Scene->cy%2)*TILE_SIZE_X/2);
	*posy = SCENE_Yo + y - (Scene->cy*TILE_SIZE_Y/2);

	SetRect(rc,shoot_seq<<5,64,(shoot_seq+1)<<5,96);
	switch(shoot_seq)
	{
		case 0:	*posx+=5;	break;
		case 1:	*posx-=2;	break;
		case 2: *posx-=8;	break;
		case 3: *posx-=16;	break;
		case 4: *posx-=24;	break;
		case 5:	*posx-=32;	break;
		case 6: *posx-=32;	break;
		case 7: *posx-=32;	break;
	}
	/*
	shoot_delay++;
	if(shoot_delay==4)
	{
		shoot_seq++;
		if(shoot_seq==16) shoot_seq=0;
		shoot_delay=0;
	}
	*/
}

void cCritter::GetRectRadar(RECT *rc,int *posx,int *posy)
{
	*posx = RADAR_Xo + ( cx << 2 );
	*posy = RADAR_Yo + ( cy << 2 );

	SetRect(rc,80,32,84,36);
}

void cCritter::GoToCell(int *map,int destcx,int destcy,int type)
{
	// "leave all we're doing"
	attack=false;
	shoot=false;

	// Go
	if(Trajectory.IsDone())	Trajectory.Make(map,cx,cy,destcx,destcy,type);
	else					Trajectory.ReMake(map,destcx,destcy,type);
}

void cCritter::GoToEnemy(int *map,int destcx,int destcy,int enemyId)
{
	GoToCell(map,destcx,destcy,1);

	attack=true;
	shoot=false;
	target=enemyId;
}
void cCritter::Move()
{
	int mov;

	if(!Trajectory.IsDone())
	{
		mov=Trajectory.NextStep(&x,&y,&cx,&cy,&ix,&iy,step_length);

		if(mov==ARRIVE)
		{
			Trajectory.Done();
			seq=0;
		}
		else if(mov==CONTINUE)
		{
		}
	}
	else
	{
		//Moved for attack?
		if(attack)
		{
			shoot=true;
			shoot_seq=0;
			shoot_delay=0;
			attack=false;
		}
	}
}

void cCritter::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cCritter::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cCritter::SetCell(int cellx,int celly)
{
	cx = cellx;
	cy = celly;
}
void cCritter::GetCell(int *cellx,int *celly)
{
	*cellx = cx;
	*celly = cy;
}
void cCritter::SetSelected(bool sel)
{
	selected = sel;
}
bool cCritter::GetSelected()
{
	return selected;
}
bool cCritter::GetShooting()
{
	return shoot;
}
int cCritter::getTarget()
{
	return target;
}
void cCritter::setTarget(int t)
{
	target=t;
}
bool cCritter::IsFiring()
{
	return (shoot_seq<8);
}
bool cCritter::isHit()
{
	return (shoot_seq==8&&shoot_delay==0);
}
int cCritter::getDamage()
{
	return damage;
}

bool cCritter::reduceHP(int _hp)
{
	hp -= _hp;
	return (hp<=0);
}

void cCritter::stopAttack()
{
	attack=false;
	shoot=false;
}

void cCritter::PutInStart(int level)
{
	if (level == 1)
	{
		Init(3,61);
	}
	else if (level == 2)
	{
		Init(3, 30);
	}
	else if (level == 3)
	{
		Init(3, 60);
	}
	else if (level == 4)
	{
		Init(24, 62);
	}
}

void cCritter::Restart()
{
	seq = 0;
	delay = 0;

	attack = false;
	shoot = false;
	skill1=false;
	shoot_seq = 0;
	shoot_delay = 0;

	hp=100;
	energy=6;

	Trajectory.Restart();

}

void cCritter::updateAttackSeq()
{
	if (shoot) {
		shoot_delay++;
		if(shoot_delay==4)
		{
			shoot_seq++;
			if(shoot_seq==16) shoot_seq=0;
			shoot_delay=0;
		}
	}
	else {
		shoot_delay=0;
		shoot_seq=0;
	}
}

void cCritter::updateSkill1Seq()
{
	if (skill1) {
		skill1_delay++;
		if(skill1_delay==2)
		{
			skill1_seq++;
			if(skill1_seq==8) {
				skill1_seq=0;
				skill1=false;
			}
			skill1_delay=0;
		}
	}
	else {
		skill1_delay=0;
		skill1_seq=0;
	}
	if (skill1_cd>0)--skill1_cd;
}


void cCritter::UseSkill1(int cx, int cy, int enemyId)
{
	if(!skill1&&skill1_cd==0&&energy>=skill1_cost) {
		energy-=skill1_cost;
		skill1_seq=0;
		skill1_delay=0;
		skill1_cd=60;
		skill1x=cx;
		skill1y=cy;
		skill1_target=enemyId;
		skill1=true;
		shoot=false;
		//attack=false;
		//Trajectory.Restart();
	}
}

bool cCritter::GetSkill1()
{
	return skill1;
}

bool cCritter::Skill1Hit() 
{
	return (skill1_seq==0&&skill1_delay==1);
}

int cCritter::GetSkill1Target()
{
	return skill1_target;
}

int cCritter::getSkill1Damage()
{
	return skill1_damage;
}

int cCritter::GetSkill1Range()
{
	return skill1_range;
}