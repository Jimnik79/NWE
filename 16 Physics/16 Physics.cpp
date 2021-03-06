//--------------------------------------------------------------------------------------
// File: 16 Physics.cpp
// Description: Sample of work with physics engine.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with physics subsystem:
// - the camera and space;
// - the surface and objects;
// - persons and bots;
// - shots and explosions.

// To work with bullets should be (see the camera from the first and from the third person):
// - Set callback for contact of bullets: g_pPhys.setCallbackBullet();
// - Write processing function for contact of bullets: void CALLBACK onBullet(WORD idBullet, byte typeObject, WORD idObject, float x, float y, float z);
// - Get XYZ-normal of person sight: g_pPhys.getPersViewNormal(0, &nxFire, &nyFire, &nzFire);
// - Get the bullet direction: g_pPhys.getBulletDirectionForThirdPersonCamera(x, y, z, &nxFire, &nyFire, &nzFire), where x, y, z - weapon position;
// - Add bullet to physics subsystem: g_pPhys.addBullet(x + nxFire, y + nyFire, z + nzFire, nxFire, nyFire, nzFire, 15), where x, y, z - weapon position; 15 - damage;

// NOTE:
// - Carefully learn the function of load objects for physical engine from "map.cpp" file.
// - Shots occur without sound and flash, that do not add redundant code.

// Used content:
// - Content\Map\Prototype.txtmap
// - Content\Texture\Ground.dds
// - Content\Texture\Wall.dds
// - Content\Texture\Pers.dds
// - Content\Mesh\Weapon\TT\
// - Content\Mesh\Soldier\

// In the next version of SDK, maybe, will be added a detailed description of working with all types of physics objects: camera, surface, persons/bots, shots, explosions and other objects.


#include "..\Engine\NetWars.h"	// Engine

#include "..\Shared\Map.h"	// Class of Map

#include <stdio.h>	// swprintf_s
//#include <time.h>	// struct tm, _time32, _localtime32_s

CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
C3D		g_p3D;		// 3D class
CPhys	g_pPhys;	// Physics class
CCam	g_pCam;		// Camera class
CMap	g_pMap;		// Map class

#define	ID_FONT_15	0

#define	ID_TEXTURE_GROUND	0	// Grass texture
#define	ID_TEXTURE_WALL		1	// Wall texture
#define	ID_TEXTURE_PERS		2	// Dynamic texture of Perses on the Map
#define	ID_TEXTURE_SIGHT	3	// Sight texture
#define	ID_TEXTURE_MAP		4	// Dynamic texture of Map

#define	ID_TT			0	// TT
#define	ID_MESH_SOLDIER	1	// Animated mesh of Soldier


#define	ID_VERTEXBUFFER_GROUND	0	// Ground vertex buffer
#define	ID_INDEXBUFFER_GROUND	1	// Ground index buffer
#define	ID_VERTEXBUFFER_WALLS	2	// Walls vertex buffer
#define	ID_INDEXBUFFER_WALLS	3	// Walls index buffer

float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

float	g_time = 0.0f;		// The current time
float	g_tShot = 0.0f;		// Time of permission for next shot 
float	g_tNewShot = 0.3f;	// The time between shots
float	g_tDelta = 0.01f;	// The added uncertainty to the time between shots, that not to miss the repeated shot in the queue at a time near the time between shots (due to incorrect frame synchronization)

int	g_iMessage = 0;
wchar_t	g_sMessages[10][96] = { L"" };	// Current messages


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameMove();			// Set callback for Frame Move
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pTex.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pPhys.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pCam.setHModule(g_pAppl.getHModule());	// Set NetWars.dll

	g_pPhys.setCallbackBullet();			// Set callback for Contact of Bullets

	// Run Engine
	return g_pAppl.run(L"16 Physics", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	g_pAppl.addFont(L"Arial", 15, FW_MEDIUM);	// Return ID_FONT_15 = 0, add font for text

	// Create mesh of Ground
	g_pMap.load(L"..\\Content\\Map\\Prototype.txtmap");	// Load the map for the game
	g_pMap.loadPhysics();

	g_pPhys.addPers(500.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100);	// Add own person to the physics subsystem
	g_pPhys.addPers(495.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100);	// 
	g_pPhys.addPers(505.0f, 0.0f, 500.0f, 0.5f, 2.0f, 100);	// 
	g_pPhys.addPers(500.0f, 0.0f, 495.0f, 0.5f, 2.0f, 100);	// 
	g_pPhys.addPers(500.0f, 0.0f, 505.0f, 0.5f, 2.0f, 100);	// 
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_15, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_15, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	for (int i = 0; i < 15; i++)
		g_pText.addSpace(ID_FONT_15, TEXT_SHADOW, 500, DT_LEFT, 0.2f);	// 

	// Textures
	g_pTex.add(L"..\\Content\\Texture\\Ground.dds");	// Return ID_TEXTURE_GROUND = 0
	g_pTex.add(L"..\\Content\\Texture\\Wall.dds");	// Return ID_TEXTURE_WALL = 1
	g_pTex.add(L"..\\Content\\Texture\\Pers.dds");	// Return ID_TEXTURE_PERS = 2
	g_pTex.add(L"..\\Content\\Texture\\Sight.dds");	// Return ID_TEXTURE_SIGHT = 3

	// Create Dynamic Texture of Map
	g_pTex.addDynamic(2048, 2048);	// Return ID_TEXTURE_MAP = 4
	UINT* map = new UINT[4194304];	// 2048 * 2048
	for (int i = 0; i < 4194304; i++)
		map[i] = 0x8060ff60;	// Green field
	//*map++ = 0xff00ff00;	// Green field
	for (int id = 0; id < g_pMap.getNWalls(); id++)
	{
		for (int i = int(2.0f*g_pMap.getWZ(id)); i < int(2.0f*(g_pMap.getWZ(id) + g_pMap.getWW(id))); i++)
			for (int j = int(2.0f*g_pMap.getWX(id)); j < int(2.0f*(g_pMap.getWX(id) + g_pMap.getWL(id))); j++)
				map[2048 * i + j] = 0x806060ff;	// Red field
	}
	g_pTex.setMap(ID_TEXTURE_MAP, map);	// Set texture map
	delete[] map;


	g_p3D.init(1.0f, 0.0f, 0.3f, 0.5f, 1.5f, 0.1f, 1.5f);	// 3D initialization
	g_pCam.setType(CAMERA_THIRDPERSON);			// Set the third person camera
	g_pCam.setPosition(500.0f, 1.5f, 500.0f);	// Set the camera position

	// Load mesh, including animated 
	g_p3D.addMesh(L"..\\Content\\Mesh\\Weapon\\TT\\TT.sdkmesh");	// Add TT, return ID_TT = 0
	g_p3D.addMeshAnim(L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh", L"..\\Content\\Mesh\\Soldier\\soldier.sdkmesh_anim");	// Add Mesh with Animation, return ID_MESH_SOLDIER = 1

	// Create PLAYING MAP
	g_p3D.addVertex(g_pMap.getVertexGround(), g_pMap.getNVertexGround() << 5);	// Create vertex buffer of ground, return ID_VERTEXBUFFER_GROUND = 0
	g_p3D.addIndex(g_pMap.getIndexGround(), g_pMap.getNIndexGround() << 1);		// Create index buffer of ground, return ID_INDEXBUFFER_GROUND = 1
	g_p3D.addVertex(g_pMap.getVertexWalls(), g_pMap.getNVertexWalls() << 5);	// Create vertex buffer of walls, return ID_VERTEXBUFFER_WALLS = 2
	g_p3D.addIndex(g_pMap.getIndexWalls(), g_pMap.getNIndexWalls() << 1);		// Create index buffer of walls, return ID_INDEXBUFFER_WALLS = 3
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer

	for (int i = 0; i < 5; i++)
		g_pText.pos(2 + i, 5, 50 + 20 * i);

	for (int i = 0; i < 10; i++)
		g_pText.pos(7 + i, 5, 200 + 20 * i);
}


void addMessage(LPCWSTR sMsg)
{
	memcpy(g_sMessages[g_iMessage++], sMsg, (wcslen(sMsg) + 1) << 1);
	if (g_iMessage > 9)
		g_iMessage = 0;
}


//--------------------------------------------------------------------------------------
// UPDATES TO THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameMove(double, float)	// time, fTime
{
	if (g_pAppl.getLMouseClick() && g_tShot < g_time - g_tNewShot)	// If the first shot in queue (during the game) // If the last shot was a "long time" and there is bullet in the clip, then make the shot
	{
		g_tShot = g_time + g_tNewShot - g_tDelta;	// Time of permission for next shot
		float nxFire, nyFire, nzFire;	// The normal direction of the shot
		g_pPhys.getPersViewNormal(0, &nxFire, &nyFire, &nzFire);	// Get XYZ-normal of person sight
		float x = g_pPhys.getPersX(0), y = 1.2f, z = g_pPhys.getPersZ(0);	// Weapon position
		g_pPhys.getBulletDirectionForThirdPersonCamera(x, y, z, &nxFire, &nyFire, &nzFire);	// Get the bullet direction
		g_pPhys.addBullet(x + nxFire, y + nyFire, z + nzFire, nxFire, nyFire, nzFire, 15);	// Add bullet to physics subsystem, damage = 15
		wchar_t	sMsg[96];
		swprintf_s(sMsg, 96, L"Departure of bullet: %.02f  %.02f  %.02f  %.02f  %.02f  %.02f", x, y, z, nxFire, nyFire, nzFire);	// Departure of bullet
		addMessage(sMsg);
	}
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double time, float)
{
	g_time = (float)time;

	g_pText.fps(ID_FONT_15, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_15, 1, DT_LEFT, COLOR_ORANGE);	// timer

	g_pPhys.objectProcessing();	// Processing interaction of all objects in physics subsystem
	g_pPhys.setCameraAfterObjectProcessing();	// Adjust the camera position after processing of physics subsystem 

	// Printing players parameters
	for (WORD id = 0; id < 5; id++)
		if (g_pPhys.getPersActive(id))
		{
			wchar_t	sPers[96];	// g_sPers[id]
			swprintf_s(sPers, 96, L"pos=%.02f  %.02f  %.02f,  sa=%.02f,  nor=%.02f  %.02f  %.02f,  sa=%.02f, speed=%d", g_pPhys.getPersX(id), g_pPhys.getPersY(id), g_pPhys.getPersZ(id),
				g_pPhys.getPersSA(id), g_pPhys.getPersNX(id), g_pPhys.getPersNY(id), g_pPhys.getPersNZ(id), g_pPhys.getPersNA(id), g_pPhys.getPersSpeedType(id));	// Contact the bullet or the end of flight
			g_pText.dynamic(ID_FONT_15, 2 + id, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, sPers);	// g_sPers[id]
		}

	// Messages
	for (int i = 0; i < 10; i++)
		g_pText.dynamic(ID_FONT_15, 7 + i, TEXT_SHADOW, DT_LEFT, COLOR_ORANGE, g_sMessages[i]);	// 

	g_pAppl.clearScreen(g_cClear);

	// Render 3d buffers
	g_p3D.setRender3D();	// Set 3D render

	// Render map (ground and walls)
	g_pTex.render(ID_TEXTURE_GROUND);
	g_p3D.renderIndex(ID_VERTEXBUFFER_GROUND, ID_INDEXBUFFER_GROUND, 0.0f, 0.0f, 0.0f);
	g_pTex.render(ID_TEXTURE_WALL);
	g_p3D.renderIndex(ID_VERTEXBUFFER_WALLS, ID_INDEXBUFFER_WALLS, 0.0f, 0.0f, 0.0f);

	for (WORD id = 0; id < g_pPhys.getNPers(); id++)
		if (g_pPhys.getPersActive(id))
			g_p3D.renderMeshTexA(0, g_pPhys.getPersX(id) + 0.5f * sinf(g_pPhys.getPersNA(id)), g_pPhys.getPersY(id) + 2.0f / 1.5f, g_pPhys.getPersZ(id) + 0.5f * cosf(g_pPhys.getPersNA(id)), g_pPhys.getPersNA(id));	// TT

	// Render soldiers
	//g_p3D.renderMeshAnim(ID_MESH_SOLDIER, x, 0.0f, z, fAlpha);
	for (WORD id = 0; id < g_pPhys.getNPers(); id++)
		if (g_pPhys.getPersActive(id))
			g_p3D.renderMeshAnimScale(0, g_pPhys.getPersX(id), g_pPhys.getPersY(id), g_pPhys.getPersZ(id), g_pPhys.getPersNA(id) - 0.12f, 2.0f);	// 2.76f - 2 meters, 0.12f - uneven mesh

	// Render text
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 17);
}


//--------------------------------------------------------------------------------------
// CONTACT OF BULLETS EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onBullet(WORD idBullet, byte typeObject, WORD idObject, float x, float y, float z)
{
	wchar_t	sMsg[96];
	swprintf_s(sMsg, 96, L"Contact bullet:  %.02f  %.02f  %.02f  %i  %i  %i", x, y, z, idBullet, typeObject, idObject);
	addMessage(sMsg);

	if (typeObject == OBJ_PERS)
	{
		if (!g_pPhys.getPersHealth(idObject))	// If a person/bot was killed, then remove him
		{
		}
	}
}