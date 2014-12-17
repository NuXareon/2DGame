
#include "cSkeleton.h"
#include "cScene.h"

cSkeleton::cSkeleton() :
playerDetected(false),
sightRadius(4) 
{
	SetPosition(0,0);
	SetCell(0,0);
	sprite_height = 68;
	ix=iy=0;
	active=true;
	hp=100;
	damage=5;
	step_length=2;
	type=SKELETON_TYPE;

	shoot_delay=0;
	shoot_seq=0;
}
cSkeleton::~cSkeleton()
{

}

void cSkeleton::Init(int nx, int ny,int _hp, int damage, int _step_length, MonsterType _type)
{
	SetPosition(nx*32,ny*32);
	SetCell(nx,ny);
	ix=48*nx-16*ny;
	iy=48*ny-16*nx;
	hp = _hp;
	step_length = _step_length;
	type = _type;
}


void cSkeleton::GoToCell(int *map, int destcx, int destcy, int type)
{
	// "leave all we're doing"
	attack = false;
	shoot = false;

	// Go
	if (Trajectory.IsDone())	Trajectory.Make(map, cx, cy, destcx, destcy,type);
	else					Trajectory.ReMake(map, destcx, destcy,type);
}

void cSkeleton::GoToPlayer(int *map, int destcx, int destcy)
{
	//(Only implemented attack right to left)
	GoToCell(map, destcx, destcy,1);

	attack = true;
	shoot = false;
}
void cSkeleton::Move()
{
	int mov;

	if (!Trajectory.IsDone())
	{
		mov = Trajectory.NextStep(&x, &y, &cx, &cy,&ix,&iy,step_length);

		if (mov == ARRIVE)
		{
			Trajectory.Done();
			seq = 0;
		}
		else if (mov == CONTINUE)
		{
		}
	}
	else
	{
		//Moved for attack?
		if (attack)
		{
			shoot = true;
			shoot_seq = 0;
			shoot_delay = 0;
			attack = false;
		}
	}
}


void cSkeleton::GetRect(RECT *rc,float *posx,float *posy,cScene *Scene)
{
	float offX = ix-sprite_height;
	float offY = iy-sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX-Scene->cx*TILE_SIZE_X)-(offY-Scene->cy*TILE_SIZE_X))/2;
	*posy = ((float)(offX-Scene->cx*TILE_SIZE_Y)+(offY-Scene->cy*TILE_SIZE_Y))/2;


	if (type == SKELETON_TYPE) SetRect(rc,0,0,64,100); //TODO: variar dependiendo de animacion
	else if (type == GOLEM_TYPE) SetRect(rc,0,0,80,110);
	else if (type == FIRELOCK_TYPE) SetRect(rc,0,0,64,100);
	else if (type == EXPLOSION_TYPE) SetRect(rc,0,0,64,100);
	else SetRect(rc,128,32,160,64); //useless


}

void cSkeleton::GetRectRadar(RECT *rc,int *posx,int *posy)
{
	*posx = RADAR_Xo + ( cx << 2 );
	*posy = RADAR_Yo + ( cy << 2 );

	SetRect(rc,84,32,88,36);
}

void cSkeleton::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cSkeleton::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cSkeleton::SetCell(int cellx,int celly)
{
	cx = cellx;
	cy = celly;
}
void cSkeleton::GetCell(int *cellx,int *celly)
{
	*cellx = cx;
	*celly = cy;
}
MonsterType cSkeleton::GetType()
{
	return type;
}
// Method for player detection IA

void cSkeleton::LookForPlayer(cCritter& thePlayer)
{
	if (!playerDetected)
	{
		// get player cell 
		int playerCellX, playerCellY;
		thePlayer.GetCell(&playerCellX, &playerCellY);

		// compute distance to player
		int mobToPlayerDistance = (int)sqrt(pow((float)playerCellX - this->cx, 2) + pow((float)playerCellY - this->cy, 2));
		
		if (mobToPlayerDistance < sightRadius)
			playerDetected = true;
	}
	
}

void cSkeleton::AttackPlayer(int* map, cCritter& thePlayer)
{
	if (playerDetected)
	{
		int playerCellX, playerCellY;
		thePlayer.GetCell(&playerCellX, &playerCellY);
		GoToPlayer(map, playerCellX, playerCellY);
	}
}

bool cSkeleton::PlayerIsDetected()
{
	return playerDetected;
}

bool cSkeleton::isActive()
{
	return active;
}

void cSkeleton::reduceHP(int x)
{
	hp -= x;
	if (hp<=0) active=false;
}