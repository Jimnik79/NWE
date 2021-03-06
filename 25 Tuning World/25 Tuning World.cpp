//--------------------------------------------------------------------------------------
// File: 25 Tuning World.cpp
// Description: Tuning World.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with tuning world:

// IN DEVELOPING


#include "..\Engine\NetWars.h"	// Engine

CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
C3D		g_p3D;		// 3D class

#define	ID_FONT_16	0

#define	ID_TEXTURE_BOARD	0
#define	ID_TEXTURE_KNIGHT	1

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pTex.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"25 Tuning World", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT_16 = 0, add font for text
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	g_pTex.add(L"..\\Content\\Mesh\\Board.dds");	// Return ID_TEXTURE_BOARD = 0
	g_pTex.add(L"..\\Content\\Mesh\\KnightW.dds");	// Return ID_TEXTURE_KNIGHT = 1

	g_p3D.init(4.5f, 0.0f, 0.5f, 1.0f, 50.0f, 0.1f, 1.5f);	// 3D initialization

	// Square: position X, position Y, position Z, normal X, normal Y, normal Z, texture X, texture Y
	float	fVertex[6][8] = {
		{ -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 },
		{ -5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0 },
		{  5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
		{  5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
		{  5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0 },
		{ -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 }, };
	g_p3D.addVertex(fVertex, sizeof(fVertex));	// Return 0

	// Cube: position X, position Y, position Z, normal X, normal Y, normal Z, texture X, texture Y
	float	fVertex2[24][8] = {
		{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 },	// Up
		{ 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0 },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
		{ 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0 },

		{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0 },	// Back
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0 },
		{ 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0 },
		{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0 },

		{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 1.0 },	// Right
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0 },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.0 },
		{ 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0 },
	
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 1.0 },	// Forward
		{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 0.0 },
		{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 0.0 },
		{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 1.0 },

		{ 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 1.0 },	// Left
		{ 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 0.0 },
		{ 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 0.0 },
		{ 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 1.0 },

		{ 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0, 1.0 },	// Down
		{ 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.0 },
		{ 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.0 },
		{ 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0, 1.0 }, };
	g_p3D.addVertex(fVertex2, sizeof(fVertex2));	// Return 1

	// Create index buffer
	WORD indices[36] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20, };
	g_p3D.addIndex(indices, sizeof(indices));	// Return 2
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pAppl.clearScreen(g_cClear);

	// Render 3d buffers
	g_p3D.setRender3D();	// Set 3D render
	g_pTex.render(ID_TEXTURE_BOARD);
	g_p3D.renderVertex(0, 0.0f, 0.0f, 0.0f);
	g_pTex.render(ID_TEXTURE_KNIGHT);
	g_p3D.renderIndex(1, 2, 0.0f, 1.0f, 0.0f);

	// Render text
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);
}

