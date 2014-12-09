
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "cInputLayer.h"

class cMouse  
{

public:
	cMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
	~cMouse();

	bool	ButtonDown(int button);
	bool	ButtonUp(int button);
	int		GetWheelMovement();
	void	GetMovement(int *dx, int *dy);

	bool Acquire();
	bool Unacquire();

	void SetPosition(int xo, int yo);
	void GetPosition(int *xpos, int *ypos);
	bool Read();
	 
private:
	LPDIRECTINPUTDEVICE8 m_pDIDev;
	DIMOUSESTATE	m_state;
	int x,y;
};

#endif 
