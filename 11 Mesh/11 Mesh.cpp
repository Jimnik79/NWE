//--------------------------------------------------------------------------------------
// File: 11 Mesh.cpp
// Description: Sample of work with mesh.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with wesh:
// - static;
// - motion;
// - keyframe animation.

// Setting the mesh occurs in 2 stages:
// 1. Adding the mesh when creating device. Returns the ID of the mesh, starting with 0.
// 2. Rendering the mesh when rendering frame.

// NOTE:
// - Meshes are divided into meshes with texture and meshes without texture. If without texture then texture should be set manually.

// To receive a *.sdkmesh file should be:
// 1. Be exported 3D-model (mesh) to the *.x file, for example, from Max.
// 2. Use a utility MeshConvert.exe from DirectX SDK june 2010 to convert mesh from the *.x file to the *.sdkmesh file.
// For the engine is being developed own format of files (*.mesh) for 3D-models with animations support and exporter to him for Max and further, possibly, for other 3D-editors.

// Used content:
// - Content\Mesh\Board.dds
// - Content\Mesh\KnightW.dds
// - Content\Mesh\Board.sdkmesh
// - Content\Mesh\Knight.sdkmesh
// - Content\Mesh\Soldier\

// In the next version of SDK, maybe, will be added working with many subset in the next version of engine.
// Much later, maybe, will be added example of streaming resources/meshes.


#include "..\Engine\NetWars.h"	// Engine

#include <math.h>	// sin, cos


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
	return g_pAppl.run(L"11 Mesh", 1024, 600);
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

	g_p3D.init(12.0f, 0.0f, 0.3f, 1.0f, 50.0f, 0.1f, 1.5f);	// 3D initialization

	g_p3D.addMesh(L"..\\Content\\Mesh\\Board.sdkmesh");		// Add mesh of board
	g_p3D.addMesh(L"..\\Content\\Mesh\\Knight.sdkmesh");	// Add mesh of knight
	g_p3D.addMeshAnim(L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh", L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh_anim");	// Add Mesh with Animation
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
void CALLBACK onFrameRender(double fTime, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pAppl.clearScreen(g_cClear);

	g_p3D.setRender3D();	// Set 3D render

	// Render static mesh
	g_pTex.render(ID_TEXTURE_BOARD);
	g_p3D.renderMesh(0, 0.0f, 0.0f, 0.0f);

	////// NEED Add a twist in different angles

	// Render motion mesh
	g_pTex.render(ID_TEXTURE_KNIGHT);
	float fAlpha = (float)fTime / 15.0f;
	float x = 5.0f *cosf(fAlpha);
	float z = 5.0f *sinf(fAlpha);
	g_p3D.renderMesh(1, x, 0.0f, z);

	// Render mesh with keyframe animation
	x = -6.6f *cosf(fAlpha);
	z = 6.6f *sinf(fAlpha);
	g_p3D.renderMeshAnim(2, x, 0.0f, z, fAlpha);

	// Render text
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);
}

