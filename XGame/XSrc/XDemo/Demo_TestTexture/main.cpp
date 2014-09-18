#include "main.h"

int WINAPI WinMain(
				   HINSTANCE hInstance,      // handle to current instance
				   HINSTANCE hPrevInstance,  // handle to previous instance
				   LPSTR lpCmdLine,          // command line
				   int nCmdShow              // show state
				   )
{
	CGame *pGame=new CGame;
	if(pGame->Init(hInstance,true))
	{
		if(pGame->SetUp()){
			pGame->MesLoop();
		}
	}

	pGame->Clean();
/*	pGame->Destory();*/
	delete pGame;
	
	return 1;
}
