//--------------------------------------------------------------------------------------
// File: 13 CShader.cpp
// Description: Sample of work with Compute Shader.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with Compute Shader:
// - initialization;
// - calling and getting the result;
// - deinitialization.

// Setting the mesh occurs in 2 stages:
// 1. Adding the mesh when creating device. Returns the ID of the mesh, starting with 0.
// 2. Rendering the mesh when rendering frame.

// Used content:
// - Content\Mesh\Board.dds
// - Content\Mesh\KnightW.dds
// - Content\Mesh\Board.sdkmesh
// - Content\Mesh\Knight.sdkmesh
// - Content\Shader\CS.hlsl

// In the next version of SDK, maybe, will be added complex calculation in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine

#include "CS.h"

#include <math.h>	// sin, cos


CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
C3D		g_p3D;		// 3D class
CCS		g_pCS;		// Compute Shader class

#define	ID_FONT16	0
#define	ID_FONT30	1

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
	g_pAppl.setCallbackDeviceDestroyed();	// Set callback for Destroy Device

	g_pTex.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"13 Compute Shader", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT16 = 0, add font for text
	g_pAppl.addFont(L"Arial", 30, 400);			// Return ID_FONT30 = 1, add font for text
}

//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	g_pTex.add(L"..\\Content\\Mesh\\Board.dds");	// Return ID_TEXTURE_BOARD = 0
	g_pTex.add(L"..\\Content\\Mesh\\KnightW.dds");	// Return ID_TEXTURE_KNIGHT = 1

	g_p3D.init(12.0f, 0.0f, 0.3f, 1.0f, 50.0f, 0.1f, 1.5f);	// 3D initialization

	g_p3D.addMesh(L"..\\Content\\Mesh\\Board.sdkmesh");		// Add mesh of board
	g_p3D.addMesh(L"..\\Content\\Mesh\\Knight.sdkmesh");	// Add mesh of knight

	g_pCS.init(g_pAppl.getD3D());	// Compute shader, initialization
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
void CALLBACK onFrameRender(double fTime, float fT)
{
	g_pText.fps(ID_FONT16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pAppl.clearScreen(g_cClear);

	g_p3D.setRender3D();	// Set 3D render

	// Render static mesh
	g_pTex.render(ID_TEXTURE_BOARD);
	g_p3D.renderMesh(0, 0.0f, 0.0f, 0.0f);

	// Render motion mesh
	g_pTex.render(ID_TEXTURE_KNIGHT);
	float fAlpha = (float)fTime / 15.0f;
	float x = 5.0f *cosf(fAlpha);
	float z = 5.0f *sinf(fAlpha);
	g_p3D.renderMesh(1, x, 0.0f, z);

	// Render text
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);

	g_pCS.render(g_pAppl.getD3D(), g_pAppl.getContext(), fT);
}


//--------------------------------------------------------------------------------------
// DESTROY RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceDestroyed()
{
	g_pCS.free();
}