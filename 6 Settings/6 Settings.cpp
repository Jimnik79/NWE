//--------------------------------------------------------------------------------------
// File: 6 Settings.cpp
// Description: Sample of Settings.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with settings.


// In the next version of SDK, maybe, will be added audio and control settings in the next version of engine.
// In the next version of SDK, maybe, will be moved to this example LOG and register settings in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine


CEngine	g_pAppl;	// Base Engine class
CText	g_pText;	// Text class

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background
bool	g_bSettings = false;

#define	ID_FONT_20	0


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackKeyboard();			// Set callback for Keyboard event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"6 Settings", 1024, 600);
}


//--------------------------------------------------------------------------------------
// KEYBOARD EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onKeyboard(UINT nChar, bool bKeyDown)
{
	if (bKeyDown)
	{
		switch (nChar)
		{
		case VK_F2:	// 0x71 F2 key
			g_pAppl.activeSettings(g_bSettings = !g_bSettings);
			break;
		}
	}
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Please enter font_20 for settings
	g_pAppl.addFont(L"Arial", 20, FW_MEDIUM);	// Return ID_FONT20 = 0, add font for text

	// Set the folder of video settings in the register
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_20, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for fps
	g_pText.addSpace(ID_FONT_20, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Add space for timer
	g_pText.addShadow(ID_FONT_20, L"Click 'F2' to show video settings.", DT_LEFT, COLOR_ORANGE, 0.5f);
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);
	g_pText.pos(1, 105, 2);
	g_pText.pos(2, 5, 30);
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_20, 0, DT_LEFT, COLOR_RED);	// fps
	g_pText.timer(ID_FONT_20, 1, DT_LEFT, COLOR_RED);	// timer

	g_pAppl.clearScreen(g_cClear);
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 3);	// Render fps, timer and Click 'F2'
}


