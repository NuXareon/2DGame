
#include "cSkeleton.h"
#include "cScene.h"

cSkeleton::cSkeleton() :
playerDetected(false),
sightRadius(4) 
{
	SetPosition(64,64);
	SetCell(2,2);
}
cSkeleton::~cSkeleton()
{

}


void cSkeleton::GoToCell(int *map, int destcx, int destcy)
{
	// "leave all we're doing"
	attack = false;
	shoot = false;

	// Go
	if (Trajectory.IsDone())	Trajectory.Make(map, cx, cy, destcx, destcy);
	else					Trajectory.ReMake(map, destcx, destcy);
}

void cSkeleton::GoToPlayer(int *map, int destcx, int destcy)
{
	//(Only implemented attack right to left)
	GoToCell(map, destcx + 1, destcy);

	attack = true;
	shoot = false;
}
void cSkeleton::Move()
{
	int mov;

	if (!Trajectory.IsDone())
	{
		mov = Trajectory.NextStep(&x, &y, &cx, &cy);

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


void cSkeleton::GetRect(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	SetRect(rc,128,32,160,64);
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
		GoToPlayer(&map, playerCellX, playerCellY);
	}
}

bool cSkeleton::PlayerIsDetected()
{
	return playerDetected;
}
