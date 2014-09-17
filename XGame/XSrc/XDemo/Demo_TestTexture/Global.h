#ifndef _GLOBAL_H
#define _GLOBAL_H
#include "CGame.h"

extern CGame *g_pGame;

class Global{
public:
	static	IDirect3DDevice9 *GlobalGetDevice()
	{
		return g_pGame->GetD3DDevice();
	}
};
#endif
