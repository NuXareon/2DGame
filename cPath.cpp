
#include "cLog.h"
#include "cPath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

cPath::cPath()
{
	AStar = NULL;
	Done();
	
	nxf=-1;	//New direction initialization (false)
	nyf=-1;
}

cPath::~cPath(){}

void cPath::Make(int *map,int cx,int cy,int cxdest,int cydest, int t)
{
	int status;		//find path?
	int ncx,ncy;	//next cell
	type = t;

	//Exists movement?
	if((cx!=cxdest)||(cy!=cydest))
	{
		world=map;
		AStar=new cAStar();
		AStar->InitializePathfinder();
		AStar->LoadMap(world);
		status=AStar->FindPath(1,cx,cy,cxdest,cydest);

		//Exists path?
		if(status)
		{
			x=cx;
			y=cy;
			xf=cxdest;
			yf=cydest;

			nxf=-1;	
			nyf=-1;

			//1st Direction
			AStar->NextCell(&ncx,&ncy);
			if (type==1&&ncx==xf&&ncy==yf){
				Done();
			}
			else CalcDir(x,y,ncx,ncy);
		}
		else
		{
			//Delete A*
			if(AStar)
			{
				AStar->EndPathfinder();
				delete AStar;
				AStar = NULL;
			}
			//Reset trajectory settings
			Done();
			nxf=-1;
			nyf=-1;
		}
	}
}

void cPath::ReMake(int *map,int cxdest,int cydest, int t)
{
	if(xf!=cxdest && yf!=cydest)
	{
		type = t;
		world=map;
		nxf=cxdest;
		nyf=cydest;
	}
}

int cPath::NextStep(int *px,int *py,int *cx,int *cy,float *ix,float *iy, int step_length)
{
	int move=CONTINUE;

	switch(dir)
	{
		case N:	(*py)-=step_length;						break;
		case S:	(*py)+=step_length;						break;
		case E:	(*px)+=step_length;						break;
		case O:	(*px)-=step_length;						break;
		case NE:(*py)-=step_length; (*px)+=step_length; break;
		case NO:(*py)-=step_length; (*px)-=step_length; break;
		case SE:(*py)+=step_length; (*px)+=step_length; break;
		case SO:(*py)+=step_length; (*px)-=step_length; break;
	}
	/*
	switch(dir)
	{
		case N:	(*iy)-=STEP_LENGTH;						break;
		case S:	(*iy)+=STEP_LENGTH;						break;
		case E:	(*ix)+=STEP_LENGTH*2;						break;
		case O:	(*ix)-=STEP_LENGTH*2;						break;
		case NE:(*iy)-=STEP_LENGTH; (*ix)+=STEP_LENGTH*2; break;
		case NO:(*iy)-=STEP_LENGTH; (*ix)-=STEP_LENGTH*2; break;
		case SE:(*iy)+=STEP_LENGTH; (*ix)+=STEP_LENGTH*2; break;
		case SO:(*iy)+=STEP_LENGTH; (*ix)-=STEP_LENGTH*2; break;
	}
	*/
	
	switch(dir)
	{
		case O:	(*ix)-=step_length*1.5; (*iy)+=step_length*0.5; break;
		case E:	(*ix)+=step_length*1.5; (*iy)-=step_length*0.5;	break;
		case S:	(*ix)-=step_length*0.5;	(*iy)+=step_length*1.5;	break;
		case N:	(*ix)+=step_length*0.5;	(*iy)-=step_length*1.5;	break;
		case NE:(*ix)+=step_length*2;	(*iy)-=step_length*2;	break;
		case NO:(*ix)-=step_length;		(*iy)-=step_length;		break;
		case SE:(*ix)+=step_length;		(*iy)+=step_length;		break;
		case SO:(*ix)-=step_length*2;	(*iy)+=step_length*2;	break;
	}
	
	//Calculate next cell
	if( (((*px)%32)==0) && (((*py)%32)==0))
	{
		x = (*px)>>5; *cx = x;
		y = (*py)>>5; *cy = y;

		if((nxf==-1) && (nyf==-1))
		{
			move=NextCell();
		}
		else//if((nxf>=0) || (nyf>=0))
		{
			AStar->EndPathfinder();
			delete AStar;
			AStar = NULL;
			Make(world,*cx,*cy,nxf,nyf,type);
			//move=CONTINUE;
		}
	}
	return move;
}

int cPath::NextCell()
{	
	int ncx,ncy;

	if((x==xf)&&(y==yf))
	{
		AStar->EndPathfinder();
		delete AStar;
		AStar = NULL;
		return ARRIVE;
	}
	else
	{
		AStar->NextCell(&ncx,&ncy);
		if (type==1&&ncx==xf&&ncy==yf){
			AStar->EndPathfinder();
			delete AStar;
			AStar = NULL;
			return ARRIVE;
		}
		CalcDir(x,y,ncx,ncy);
		return CONTINUE;
	}
}

int cPath::Faced()
{
	return dir;
}
bool cPath::IsDone()
{
	return (dir == STOPN || dir == STOPE || dir == STOPS || dir == STOPO || dir == STOPNE || dir == STOPNO || dir == STOPSE || dir == STOPSO );
}
void cPath::Done()
{
	if (dir == N) dir = STOPN;
	if (dir == NE) dir = STOPNE;
	if (dir == E) dir = STOPE;
	if (dir == SE) dir = STOPSE;
	if (dir == S) dir = STOPS;
	if (dir == SO) dir = STOPSO;
	if (dir == O) dir = STOPO;
	if (dir == NO) dir = STOPNO;
}

void cPath::CalcDir(int x1,int y1,int x2,int y2)
{
	int sdx,sdy;	//sign movement

	sdx=x2-x1;
	sdy=y2-y1;

	// - Horitzontal
	if(sdy==0)
	{
		if(sdx>0) dir=E;
		else	  dir=O;
	}
	// - Vertical
	else if(sdx==0)
	{
		if(sdy>0) dir=S;
		else	  dir=N;
	}
	// - m=dx/dy=1
	else if(abs(sdx)==abs(sdy))
	{
		if(sdx>0)
		{
			if(sdy>0)	dir=SE;
			else		dir=NE;
		}
		else
		{
			if(sdy>0)	dir=SO;
			else		dir=NO;
		}
	}
}

void cPath::Restart()
{
	//Delete A*
	if (AStar)
	{
		AStar->EndPathfinder();
		delete AStar;
		AStar = NULL;
	}
	//Reset trajectory settings
	Done();
	nxf = -1;
	nyf = -1;
}
