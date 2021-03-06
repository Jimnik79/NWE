//--------------------------------------------------------------------------------------
// File: Game.h
// Description: Class of Proyotype game.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


class CGame
{
protected:
	
	CRITICAL_SECTION m_cs;	// Critical section
#define	LOCK()		EnterCriticalSection(&m_cs);
#define	UNLOCK()	LeaveCriticalSection(&m_cs);

public:
	CGame();	// Constructor
	~CGame();	// Destructor

};


bool initPerses()	// Initialization of the persons in the physics subsystem, so that they could then be connected without the addition, ie through activation
{
	if (g_bInitPerses)
		return false;

	for (WORD id = 0; id < MAX_TEST; id++)	// Add MAX_PLR persons to the physics subsystem
		g_pPhys.addPers(500.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100);

	for (WORD id = 0; id < MAX_TEST; id++)	// Deactivate them all
		g_pPhys.setPersActive(id, false);

	g_bInitPerses = true;

	return true;
}

void activatePersInGame(WORD idPers, BYTE weapon)	// Activate the person in the game (in the physics subsystem)
{
	g_pPhys.setPersActive(idPers, true);
	g_Plr[idPers].weapon = weapon;
}
