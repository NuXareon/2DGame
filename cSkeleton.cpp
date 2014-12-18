
#include "cSkeleton.h"
#include "cScene.h"

cSkeleton::cSkeleton() :
playerDetected(false),
sightRadius(4),
isDead(false)
{
	SetPosition(0,0);
	SetCell(0,0);
	sprite_height = 68;
	ix=iy=0;
	active=false;
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

void cSkeleton::Init(int nx, int ny,int _hp, int _damage, int _step_length, int sight, int range, MonsterType _type)
{
	SetPosition(nx*32,ny*32);
	SetCell(nx,ny);
	ix=48*nx-16*ny;
	iy=48*ny-16*nx;
	hp = _hp;
	damage = _damage;
	step_length = _step_length;
	type = _type;
	sightRadius=sight;
	attackRange=range;
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
			if (attack && (attackRange > 0)) {
				int mobToPlayerDistance = (int)sqrt(pow((float)Trajectory.xf - cx, 2) + pow((float)Trajectory.yf - cy, 2));
				if (mobToPlayerDistance <= attackRange) {
					Trajectory.Done();
					seq=0;
					shoot=true;
				}
				if (mobToPlayerDistance > sightRadius+1) {
					Trajectory.Done();
					seq=0;
					shoot=true;
				}
			}
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

void cSkeleton::GetRect(RECT *rc,float *posx,float *posy,cScene *Scene, bool update)
{
	float offX = ix - sprite_height;
	float offY = iy - sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX - Scene->cx*TILE_SIZE_X) - (offY - Scene->cy*TILE_SIZE_X)) / 2;
	*posy = ((float)(offX - Scene->cx*TILE_SIZE_Y) + (offY - Scene->cy*TILE_SIZE_Y)) / 2;


	if (type == SKELETON_TYPE)
	{
		if (!shoot)
		{
			switch (Trajectory.Faced()) // 0, 0, 64, 100
			{
			case S:			SetRect(rc, 64 * seq, 100, 64 * (seq + 1), 200); break; //S,SO
			case SO:		SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case STOPS:		SetRect(rc, 64 * seq, 100, 64 * (seq + 1), 200); break; //S,SO
			case STOPSO:	SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case E:			SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break;
			case SE:		SetRect(rc, 64 * seq, 100, 64 * (seq + 1), 200); break; //E,SE
			case STOPE:		SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break;
			case STOPSE:	SetRect(rc, 64 * seq, 100, 64 * (seq + 1), 200); break; //E,SE
			case O:			SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case NO:		SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break; //O,NO
			case STOPO:		SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case STOPNO:	SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break; //O,NOV
			case N:			SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break;
			case NE:		SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break; //N,NE
			case STOPN:		SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break;
			case STOPNE:	SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break; //N,NE
			}
			if (update) {
				shoot_seq = 0;
				if (!Trajectory.IsDone())
				{
					delay++;
					if (delay >= 8)
					{
						seq++;
						if (seq > 7) seq = 0;
						delay = 0;
					}
				}
				else
				{
					delay++;
					if (delay >= 8)
					{
						seq++;
						if (seq > 3) seq = 0;
						delay = 0;
					}
				}
			}
		}
		else
		{
			switch (Trajectory.Faced()) // 0, 0, 64, 100
			{
			case S:			SetRect(rc, 64 * shoot_seq, 500, 64 * (shoot_seq + 1), 600); break;
			case SO:		SetRect(rc, 64 * shoot_seq, 600, 64 * (shoot_seq + 1), 700); break;
			case STOPS:		SetRect(rc, 64 * shoot_seq, 500, 64 * (shoot_seq + 1), 600); break; 
			case STOPSO:	SetRect(rc, 64 * shoot_seq, 600, 64 * (shoot_seq + 1), 700); break;
			case E:			SetRect(rc, 64 * shoot_seq, 400, 64 * (shoot_seq + 1), 500); break;
			case SE:		SetRect(rc, 64 * shoot_seq, 500, 64 * (shoot_seq + 1), 600); break;
			case STOPE:		SetRect(rc, 64 * shoot_seq, 400, 64 * (shoot_seq + 1), 500); break;
			case STOPSE:	SetRect(rc, 64 * shoot_seq, 500, 64 * (shoot_seq + 1), 600); break; //E,SE
			case O:			SetRect(rc, 64 * shoot_seq, 600, 64 * (shoot_seq + 1), 700); break;
			case NO:		SetRect(rc, 64 * shoot_seq, 700, 64 * (shoot_seq + 1), 800); break; //O,NO
			case STOPO:		SetRect(rc, 64 * shoot_seq, 600, 64 * (shoot_seq + 1), 700); break;
			case STOPNO:	SetRect(rc, 64 * shoot_seq, 700, 64 * (shoot_seq + 1), 800); break; //O,NOV
			case N:			SetRect(rc, 64 * shoot_seq, 700, 64 * (shoot_seq + 1), 800); break;
			case NE:		SetRect(rc, 64 * shoot_seq, 400, 64 * (shoot_seq + 1), 500); break; //N,NE
			case STOPN:		SetRect(rc, 64 * shoot_seq, 700, 64 * (shoot_seq + 1), 800); break;
			case STOPNE:	SetRect(rc, 64 * shoot_seq, 400, 64 * (shoot_seq + 1), 500); break; //N,NE
			}
			/*
			if (update) {
			seq = 0;
				shoot_delay++;
				if (shoot_delay >= 8)
				{
					shoot_seq++;
					if (shoot_seq > 7) shoot_seq = 0;
					shoot_delay = 0;
				}
			}
			*/
		}
		//		SetRect(rc, 0, 0, 64, 100); TODO: variar dependiendo de animacion
		//		SetRect(rc, 0, 0, 64, 100); TODO: variar dependiendo de animacion
	}
	else if (type == GOLEM_TYPE) SetRect(rc, 0, 0, 80, 110);
	else if (type == FIRELOCK_TYPE)
	{
		if (!shoot)
		{
			shoot_seq = 0;
			switch (Trajectory.Faced()) // 0, 0, 64, 100
			{
			case S:			SetRect(rc, 64 * seq, 100 + 400, 64 * (seq + 1), 200 + 400); break; //S,SO
			case SO:		SetRect(rc, 64 * seq, 200 + 400, 64 * (seq + 1), 300 + 400); break;
			case STOPS:		SetRect(rc, 64 * seq, 100 + 400 + 400, 64 * (seq + 1), 200); break; //S,SO
			case STOPSO:	SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case E:			SetRect(rc, 64 * seq, 0 + 400, 64 * (seq + 1), 100 + 400); break;
			case SE:		SetRect(rc, 64 * seq, 100 + 400, 64 * (seq + 1), 200 + 400); break; //E,SE
			case STOPE:		SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break;
			case STOPSE:	SetRect(rc, 64 * seq, 100, 64 * (seq + 1), 200); break; //E,SE
			case O:			SetRect(rc, 64 * seq, 200 + 400, 64 * (seq + 1), 300 + 400); break;
			case NO:		SetRect(rc, 64 * seq, 300 + 400, 64 * (seq + 1), 400 + 400); break; //O,NO
			case STOPO:		SetRect(rc, 64 * seq, 200, 64 * (seq + 1), 300); break;
			case STOPNO:	SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break; //O,NOV
			case N:			SetRect(rc, 64 * seq, 300 + 400, 64 * (seq + 1), 400 + 400); break;
			case NE:		SetRect(rc, 64 * seq, 0 + 400, 64 * (seq + 1), 100 + 400); break; //N,NE
			case STOPN:		SetRect(rc, 64 * seq, 300, 64 * (seq + 1), 400); break;
			case STOPNE:	SetRect(rc, 64 * seq, 0, 64 * (seq + 1), 100); break; //N,NE
			}
			if (!Trajectory.IsDone())
			{
				delay++;
				if (delay >= 8)
				{
					seq++;
					if (seq > 4) seq = 0;
					delay = 0;
				}
			}
			else
			{
				delay++;
				if (delay >= 8)
				{
					seq++;
					if (seq > 5) seq = 0;
					delay = 0;
				}
			}
		}
		else
		{
			seq = 0;
			switch (Trajectory.Faced()) // 0, 0, 64, 100
			{
			case S:			SetRect(rc, 64 * shoot_seq, 500 + 400, 64 * (shoot_seq + 1), 600 + 400); break;
			case SO:		SetRect(rc, 64 * shoot_seq, 600 + 400, 64 * (shoot_seq + 1), 700 + 400); break;
			case STOPS:		SetRect(rc, 64 * shoot_seq, 500 + 400, 64 * (shoot_seq + 1), 600 + 400); break;
			case STOPSO:	SetRect(rc, 64 * shoot_seq, 600 + 400, 64 * (shoot_seq + 1), 700 + 400); break;
			case E:			SetRect(rc, 64 * shoot_seq, 400 + 400, 64 * (shoot_seq + 1), 500 + 400); break;
			case SE:		SetRect(rc, 64 * shoot_seq, 500 + 400, 64 * (shoot_seq + 1), 600 + 400); break;
			case STOPE:		SetRect(rc, 64 * shoot_seq, 400 + 400, 64 * (shoot_seq + 1), 500 + 400); break;
			case STOPSE:	SetRect(rc, 64 * shoot_seq, 500 + 400, 64 * (shoot_seq + 1), 600 + 400); break; //E,SE
			case O:			SetRect(rc, 64 * shoot_seq, 600 + 400, 64 * (shoot_seq + 1), 700 + 400); break;
			case NO:		SetRect(rc, 64 * shoot_seq, 700 + 400, 64 * (shoot_seq + 1), 800 + 400); break; //O,NO
			case STOPO:		SetRect(rc, 64 * shoot_seq, 600 + 400, 64 * (shoot_seq + 1), 700 + 400); break;
			case STOPNO:	SetRect(rc, 64 * shoot_seq, 700 + 400, 64 * (shoot_seq + 1), 800 + 400); break; //O,NOV
			case N:			SetRect(rc, 64 * shoot_seq, 700 + 400, 64 * (shoot_seq + 1), 800 + 400); break;
			case NE:		SetRect(rc, 64 * shoot_seq, 400 + 400, 64 * (shoot_seq + 1), 500 + 400); break; //N,NE
			case STOPN:		SetRect(rc, 64 * shoot_seq, 700 + 400, 64 * (shoot_seq + 1), 800 + 400); break;
			case STOPNE:	SetRect(rc, 64 * shoot_seq, 400 + 400, 64 * (shoot_seq + 1), 500 + 400); break; //N,NE
			}
			/*
			shoot_delay++;
			if (shoot_delay >= 8)
			{

				if (shoot_seq > 4) shoot_seq = 0;
				shoot_delay = 0;
			}
			*/
		}
	}
	else if (type == EXPLOSION_TYPE) SetRect(rc,0,0,64,100);
	else SetRect(rc,128,32,160,64); //useless
}

void cSkeleton::GetIsoPos(float *posx,float *posy,cScene *Scene)
{
	float offX = ix-sprite_height;
	float offY = iy-sprite_height;

	*posx = ISO_OFFSET_X + ((float)(offX-Scene->cx*TILE_SIZE_X)-(offY-Scene->cy*TILE_SIZE_X))/2;
	*posy = ((float)(offX-Scene->cx*TILE_SIZE_Y)+(offY-Scene->cy*TILE_SIZE_Y))/2;
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
bool cSkeleton::GetShooting()
{
	return shoot;
}
int cSkeleton::getDamage()
{
	return damage;
}
bool cSkeleton::isHit()
{
	if (type==EXPLOSION_TYPE) return (shoot_seq==2);
	return (shoot_seq==5&&shoot_delay==0);  // TODO: update depending enemy attack animation;
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
		else 
			playerDetected = false;
	}
	
}

void cSkeleton::AttackPlayer(int* map, cCritter& thePlayer)
{
	if (playerDetected&&!shoot)
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

void cSkeleton::SetActive(bool turn)
{
	active = turn;
}

void cSkeleton::reduceHP(int x)
{
	hp -= x;
	if (hp <= 0) 
	{ 
		isDead = true;
		active = false; 
	}
}

void cSkeleton::updateAttackSeq()
{
	if (shoot) {
		shoot_delay++;
		if(shoot_delay==4)
		{
			shoot_seq++;
			if(type==EXPLOSION_TYPE&&shoot_seq==2) active=false;
			if(shoot_seq>7) {
				shoot_seq=0;
				shoot=false;
			}
			shoot_delay=0;
		}
	}
	else {
		shoot_delay=0;
		shoot_seq=0;
	}
}