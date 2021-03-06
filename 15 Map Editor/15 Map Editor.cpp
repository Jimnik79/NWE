//--------------------------------------------------------------------------------------
// File: 15 Map Editor.cpp
// Description: Sample of work with WYSIWYG map editing.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with WYSIWYG (What You See Is What You Get) map editing:
// - width and height;
// - walls and pits. (pits not implemented)

// NOTE:
// - When you update the buffers work with grids, etc. need to use flags to avoid errors sharing data, without resorting to the use of multi-threading technology to access the Direct3D device

// Used content:
// - Content\Texture\Ground.dds
// - Content\Texture\Wall.dds

// In the next version of SDK, maybe, will be added axes (X - red, Y - green, Z - blue) in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine

#include "..\Shared\Map.h"	// Class of Map

#include <math.h>	// sin, cos
#include <stdio.h>	// swprintf_s


CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
C3D		g_p3D;		// 3D class
CCam	g_pCam;		// Camera class
CUI		g_pUI;		// UI class
CPhys	g_pPhys;	// Physics class
CMap	g_pMap;		// Map class


#define	ID_BUTTON_OPEN		0
#define	ID_BUTTON_SAVE		1
#define	ID_BUTTON_WALL		2
#define	ID_BUTTON_PIT		3
#define	ID_BUTTON_STANDART	4
#define	ID_EDITBOX_LENGHT	5	// Map lenght
#define	ID_EDITBOX_WIDTH	6	// Map width
#define	ID_RADIO_WALL		7
#define	ID_RADIO_PIT		8
#define	ID_EDITBOX_I		9	// Index of wall or pit
#define	ID_EDITBOX_X		10
#define	ID_EDITBOX_Z		11
#define	ID_EDITBOX_L		12
#define	ID_EDITBOX_W		13
#define	ID_EDITBOX_H		14
#define	ID_RADIO_POINT		15
#define	ID_RADIO_LINE		16
#define	ID_RADIO_TRIANGLE	17

#define	GROUP_OBJECT		0
#define	GROUP_RENDER		1

#define	ID_FONT_16	0

#define	ID_TEXTURE_GROUND	0	// Grass
#define	ID_TEXTURE_WALL		1
#define	ID_TEXTURE_PIT		2


enum state	// State machine
{
	DOTS,
	LINES,
	TRIANGLES
};
enum state g_eState = TRIANGLES;	// State machine


float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

float	g_fHW = 512.0f;
float	g_fHH = 300.0f;

int		g_iSelectVertex = -1;
int		g_iSelectClick = -1;
int		g_xSelect = 0;
int		g_ySelect = 0;
float	g_vx, g_vy, g_vz;

bool	g_bWall = false;	// Flag to wall building
bool	g_bPit = false;		// Flag to pit building
int		g_index = 0;

bool	g_bUpdateGround = false;// Flag to update ground vertex buffer
bool	g_bAddWall = false;		// Flag to add walls (update vertex and indexes buffers)
bool	g_bUpdateWall = false;	// Flag to update wall vertex buffer
bool	g_bUpdatePit = false;	// Flag to update pit vertex buffer

int		g_iAdd2DPoint = 0;

bool	g_bLMouseClickLast = false;

int		g_Xtek = 0, g_Ytek = 0, g_Xlast = 0, g_Ylast = 0;

ID3D11RasterizerState*	g_pRasWireFrame = NULL;	// Wireframe rasterizer mode


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackMouse();				// Set callback for Mouse Event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame

	g_pTex.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_p3D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pCam.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pPhys.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"15 Map Editor", 1024, 600);
}


//--------------------------------------------------------------------------------------
// INITIALIZE
//--------------------------------------------------------------------------------------
void CALLBACK onInitialize()
{
	// Adding fonts
	g_pAppl.addFont(L"Arial", 16, FW_MEDIUM);	// Return ID_FONT_16 = 0, adding font for text

	// Adding UI controls
	g_pUI.addButton(-1, ID_FONT_16, L"Open", 150, 1, 0.8f);	// Return ID_BUTTON_OPEN = 0
	g_pUI.addButton(-1, ID_FONT_16, L"Save", 150, 1, 0.8f);	// Return ID_BUTTON_SAVE = 1
	g_pUI.addButton(-1, ID_FONT_16, L"WALL", 150, 1, 0.8f);	// Return ID_BUTTON_WALL = 2
	g_pUI.addButton(-1, ID_FONT_16, L"PIT", 150, 1, 0.8f);	// Return ID_BUTTON_PIT = 3
	g_pUI.addButton(-1, ID_FONT_16, L"Standart Map", 150, 1, 0.8f);	// Return ID_BUTTON_STANDART = 4

	g_pUI.addEditBox(-1, ID_FONT_16, L"1024.00", 85, 0.8f);	// ID_EDITBOX_LENGHT = 5
	g_pUI.setFloat(ID_EDITBOX_LENGHT, 2, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"1024.00", 85, 0.8f);	// ID_EDITBOX_WIDTH = 6
	g_pUI.setFloat(ID_EDITBOX_WIDTH, 2, 0);

	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_OBJECT, L"WALL", COLOR_GOLD, 0.8f);	// Return ID_RADIO_WALL = 7
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_OBJECT, L"PIT", COLOR_GOLD, 0.8f);	// Return ID_RADIO_PIT = 8

	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_I = 9
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_X = 10
	g_pUI.setFloat(ID_EDITBOX_X, 2, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_Z = 11
	g_pUI.setFloat(ID_EDITBOX_Z, 2, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_L = 12
	g_pUI.setFloat(ID_EDITBOX_L, 2, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_W = 13
	g_pUI.setFloat(ID_EDITBOX_W, 2, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_H = 14
	g_pUI.setFloat(ID_EDITBOX_H, 2, 0);

	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Point", COLOR_GOLD, 0.8f);	// Return ID_RADIO_POINT = 15
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Line", COLOR_GOLD, 0.8f);	// Return ID_RADIO_LINE = 16
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Triangle", COLOR_GOLD, 0.8f);	// Return ID_RADIO_TRIANGLE = 17
	g_pUI.setChecked(ID_RADIO_TRIANGLE, true);

	// Create mesh of Ground
	//g_pMap.load(L"..\\Content\\Map\\Prototype.txtmap");	// Load map 
	//g_pMap.loadPhysics();
}


//--------------------------------------------------------------------------------------
// GUI EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onGUI(UINT idEvent, int idControl)
{
	switch (idEvent)
	{
	case EVENT_CLICK:	// Click Event
		switch (idControl)
		{
		case ID_BUTTON_OPEN:
			g_pMap.load(L"test.txtmap");
			g_bUpdateGround = true;	// Flag to update ground vertex buffer
			g_bAddWall = true;		// Flag to add walls (update vertex and indexes buffers)
			return;
		case ID_BUTTON_SAVE:
			g_pMap.save(L"test.txtmap");
			return;
		case ID_BUTTON_WALL:
			g_bWall = true;
			return;
		case ID_BUTTON_PIT:
			g_bPit = true;
			return;
		case ID_BUTTON_STANDART:
			g_pMap.setStandartMap();
			g_bUpdateGround = true;	// Flag to update ground vertex buffer
			g_bAddWall = true;		// Flag to add walls (update vertex and indexes buffers)
			float fLenght = g_pMap.getLenght();	// Set map lenght
			float fWidth = g_pMap.getWidth();	// Set map width
			wchar_t	s[16];
			swprintf_s(s, 16, L"%.02f", fLenght);	g_pUI.text(ID_EDITBOX_LENGHT, s);	// Map lenght
			swprintf_s(s, 16, L"%.02f", fWidth);	g_pUI.text(ID_EDITBOX_WIDTH, s);	// Map width
			return;
		}
		break;

	case EVENT_CHANGE:	// Changed Event
		switch (idControl)
		{
		case ID_RADIO_POINT:
			g_eState = DOTS;
			return;
		case ID_RADIO_LINE:
			g_eState = LINES;
			return;
		case ID_RADIO_TRIANGLE:
			g_eState = TRIANGLES;
			return;
		}
		break;

	case EVENT_ENTER:	// Pressed enter Event: EditBox
		switch (idControl)
		{
		case ID_EDITBOX_LENGHT:	// Map lenght
		case ID_EDITBOX_WIDTH:	// Map width
		{
			float fLenght, fWidth;
			swscanf_s(g_pUI.getText(ID_EDITBOX_LENGHT), L"%f", &fLenght);	// Map lenght
			swscanf_s(g_pUI.getText(ID_EDITBOX_WIDTH), L"%f", &fWidth);	// Map width
			g_pMap.setLenght(fLenght);	// Set map lenght
			g_pMap.setWidth(fWidth);	// Set map width
			g_pMap.setGround();	// Set ground
			g_bUpdateGround = true;
		}	break;

		case ID_EDITBOX_I:	// Index
		{
			wchar_t	s[16];
			swscanf_s(g_pUI.getText(ID_EDITBOX_I), L"%d", &g_index);	// Index
			if (g_index < g_pMap.getNWalls() && g_index >= 0)
			{
				swprintf_s(s, 16, L"%.03f", g_pMap.getWX(g_index));	g_pUI.text(ID_EDITBOX_X, s);
				swprintf_s(s, 16, L"%.03f", g_pMap.getWZ(g_index));	g_pUI.text(ID_EDITBOX_Z, s);
				swprintf_s(s, 16, L"%.03f", g_pMap.getWL(g_index));	g_pUI.text(ID_EDITBOX_L, s);
				swprintf_s(s, 16, L"%.03f", g_pMap.getWW(g_index));	g_pUI.text(ID_EDITBOX_W, s);
				swprintf_s(s, 16, L"%.03f", g_pMap.getWH(g_index));	g_pUI.text(ID_EDITBOX_H, s);
			}
			else
			{
				g_pUI.text(ID_EDITBOX_X, s);
				g_pUI.text(ID_EDITBOX_Z, s);
				g_pUI.text(ID_EDITBOX_L, s);
				g_pUI.text(ID_EDITBOX_W, s);
				g_pUI.text(ID_EDITBOX_H, s);
			}
		}	break;

		case ID_EDITBOX_X:	// X position
		case ID_EDITBOX_Z:	// Z position
		case ID_EDITBOX_L:	// Lenght (X)
		case ID_EDITBOX_W:	// Width (Z)
		case ID_EDITBOX_H:	// Height (Y)
		{	int id;
			float x, z, l, w, h;
			swscanf_s(g_pUI.getText(ID_EDITBOX_I), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_Z), L"%f", &z);	// Z position
			swscanf_s(g_pUI.getText(ID_EDITBOX_X), L"%f", &x);	// X position
			swscanf_s(g_pUI.getText(ID_EDITBOX_L), L"%f", &l);	// Lenght (X)
			swscanf_s(g_pUI.getText(ID_EDITBOX_W), L"%f", &w);	// Width (Z)
			swscanf_s(g_pUI.getText(ID_EDITBOX_H), L"%f", &h);	// Height (Y)
			if (g_pUI.getChecked(ID_RADIO_WALL))
			{
				g_pMap.updateWall(id, x, z, l, w, h);
				g_bUpdateWall = true;
			}
			if (g_pUI.getChecked(ID_RADIO_PIT))
			{
				g_pMap.updatePit(id, x, z, l, w, h);
				g_bUpdatePit = true;
			}
		}	break;
		}
		break;
	}
}


//--------------------------------------------------------------------------------------
// MOUSE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onMouse(bool bLeftButtonDown, bool bRightButtonDown, bool, bool, bool, int, int xPos, int yPos)
{
	g_Xtek = xPos;
	g_Ytek = yPos;

	//float fX = (float)xPos;
	//float fY = (float)yPos;
	if (g_bWall)	// If building wall
	{
		if (bLeftButtonDown && !g_bLMouseClickLast)	// LMouse click
		{
			float x, y, z;	// The direction vector
			D3DXVECTOR3	vNor;
			if (g_p3D.get3DCoordFromScreenPoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, xPos, yPos, &x, &y, &z))
			{
				wchar_t	s[16];
				swprintf_s(s, 16, L"%d", g_pMap.getNWalls());	g_pUI.text(ID_EDITBOX_I, s);

				swprintf_s(s, 16, L"%.02f", x);	g_pUI.text(ID_EDITBOX_X, s);
				swprintf_s(s, 16, L"%.02f", z);	g_pUI.text(ID_EDITBOX_Z, s);
				g_pUI.text(ID_EDITBOX_L, L"10.00");
				g_pUI.text(ID_EDITBOX_W, L"0.50");
				g_pUI.text(ID_EDITBOX_H, L"2.00");
				g_pMap.addWall(x, z, 10.0f, 0.5f, 2.0f);	// Add wall
				g_bAddWall = true;
				g_bWall = false;
			}
		}
	}
	else if (g_bPit)	// If building pit
	{
		if (bLeftButtonDown && !g_bLMouseClickLast)	// LMouse click
		{
			float x, y, z;	// The direction vector
			D3DXVECTOR3	vNor;
			if (g_p3D.get3DCoordFromScreenPoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, xPos, yPos, &x, &y, &z))
			{
				g_pMap.addPit(x, z, 10.0f, 0.5f, 2.0f);	// Add pit
				g_bPit = false;
			}
		}
	}
	else
		if (bRightButtonDown)
		{
			g_iSelectVertex = -1;
			g_pUI.text(ID_EDITBOX_I, L"");	// Index
			g_pUI.text(ID_EDITBOX_X, L"");	// X position
			g_pUI.text(ID_EDITBOX_Z, L"");	// Z position
			g_pUI.text(ID_EDITBOX_L, L"");	// Lenght (X)
			g_pUI.text(ID_EDITBOX_W, L"");	// Width (Z)
			g_pUI.text(ID_EDITBOX_H, L"");	// Height (Y)
		}

	////// NEED ADD. If you unpressed left mouse button about point(s), then these point(s) should be equated.

	if (bRightButtonDown)
		g_iSelectVertex = -1;

	g_bLMouseClickLast = bLeftButtonDown;
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 150, DT_LEFT, 0.5f);	// Return 2, add space for X,Z position

	g_pTex.add(L"..\\Content\\Texture\\Ground.dds");	// Return ID_TEXTURE_GROUND = 0
	g_pTex.add(L"..\\Content\\Texture\\Wall.dds");	// Return ID_TEXTURE_WALL = 1

	// 3D initialization
	g_p3D.init(20.0f, 0.0f, 0.3f, 5.f, 50.0f, 0.1f, 1.5f);	// 3D initialization
	g_pCam.setType(CAMERA_MAPEDITOR);			// Set the map editor camera
	g_pCam.setPosition(500.0f, 0.0f, 500.0f);	// Set the camera position

	g_pMap.setGround();	// Set ground, default: 1024*1024

	g_pUI.setInt(ID_EDITBOX_I, g_pMap.getNIndexGround());	// g_pGround.getNVertex());

	// Create PLAYING MAP
	g_p3D.addVertex(g_pMap.getVertexGround(), g_pMap.getNVertexGround() << 5);	// Create vertex buffer of ground, return ID_VERTEXBUFFER_GROUND = 0
	g_p3D.addIndex(g_pMap.getIndexGround(), g_pMap.getNIndexGround() << 1);		// Create index buffer of ground, return ID_INDEXBUFFER_GROUND = 1
	g_p3D.addIndex(g_pMap.getIndexGround(), g_pMap.getNIndexGround() << 1);		// Create index buffer of ground, return ID_INDEXBUFFER_GROUND = 1
	g_p3D.addVertex(g_pMap.getVertexWalls(), g_pMap.getNVertexWalls() << 5);	// Create vertex buffer of walls, return ID_VERTEXBUFFER_WALLS = 2
	g_p3D.addIndex(g_pMap.getIndexWalls(), g_pMap.getNIndexWalls() << 1);		// Create index buffer of walls, return ID_INDEXBUFFER_WALLS = 3
	g_p3D.addIndex(g_pMap.getIndexWalls(), g_pMap.getNIndexWalls() << 1);		// Create index buffer of walls, return ID_INDEXBUFFER_WALLS = 3
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer
	g_pText.pos(2, 5, 475);	// X,Z position

	g_pUI.pos(ID_BUTTON_OPEN, 5, 20);
	g_pUI.pos(ID_BUTTON_SAVE, 5, 50);
	g_pUI.pos(ID_BUTTON_WALL, 5, 80);
	g_pUI.pos(ID_BUTTON_PIT, 5, 110);
	g_pUI.pos(ID_BUTTON_STANDART, 5, 140);
	g_pUI.pos(ID_EDITBOX_LENGHT, 5, 172);
	g_pUI.pos(ID_EDITBOX_WIDTH, 95, 172);
	g_pUI.pos(ID_RADIO_WALL, 5, 205);
	g_pUI.pos(ID_RADIO_PIT, 95, 205);
	g_pUI.pos(ID_EDITBOX_I, 5, 230);
	g_pUI.pos(ID_EDITBOX_X, 5, 260);
	g_pUI.pos(ID_EDITBOX_Z, 5, 290);
	g_pUI.pos(ID_EDITBOX_L, 5, 320);
	g_pUI.pos(ID_EDITBOX_W, 5, 350);
	g_pUI.pos(ID_EDITBOX_H, 5, 380);
	g_pUI.pos(ID_RADIO_POINT, 5, 410);
	g_pUI.pos(ID_RADIO_LINE, 5, 430);
	g_pUI.pos(ID_RADIO_TRIANGLE, 5, 450);
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	// Update coordinate
	if (g_Xtek != g_Xlast || g_Ytek != g_Ylast)
	{
		float x, y, z;
		if (g_p3D.get3DCoordFromScreenPoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, g_Xtek, g_Ytek, &x, &y, &z))
		{
			wchar_t	s[32];
			swprintf_s(s, 32, L"%.2f %.2f %.2f", x, y, z);
			g_pText.dynamic(ID_FONT_16, 2, 2, DT_LEFT, COLOR_ORANGE, s);	// X,Z position
		}
		g_Xlast = g_Xtek;
		g_Ylast = g_Ytek;
	}

	if (g_eState != DOTS)
	{
		if (g_bUpdateGround)
		{
			g_p3D.updateVertex(0, g_pMap.getVertexGround(), g_pMap.getNVertexGround() << 5);	// 32 bytes per vertex
			g_bUpdateGround = false;
		}

		if (g_bAddWall)	// If need to update vertex and indexes buffers
		{
			g_p3D.updateVertex(3, g_pMap.getVertexWalls(), g_pMap.getNVertexWalls() << 5);	// 32 bytes per vertex
			g_p3D.updateIndex(4, g_pMap.getIndexWalls(), g_pMap.getNIndexWalls() << 1);	// 2 bytes per index
			g_p3D.updateIndex(5, g_pMap.getIndexLineWalls(), g_pMap.getNIndexLineWalls() << 1);	// 
			g_bAddWall = false;
		}

		if (g_bUpdateWall)	// If need to update vertex buffer
		{
			g_p3D.updateVertex(3, g_pMap.getVertexWalls(), g_pMap.getNVertexWalls() << 5);	// 32 bytes per vertex
			g_bUpdateWall = false;	// Vertex update completed
		}
	}

	g_pAppl.clearScreen(g_cClear);

	g_p3D.setRender3D();	// Set 3D render

	g_pTex.render(ID_TEXTURE_GROUND);
	switch (g_eState)
	{
	case DOTS:	// Rendering point list
		g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		g_p3D.renderIndex(0, 1, 0.0f, 0.0f, 0.0f);
		g_pTex.render(ID_TEXTURE_WALL);
		g_p3D.renderIndex(3, 4, 0.0f, 0.0f, 0.0f);
		break;
	case LINES:	// Rendering line list
		g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		g_p3D.renderIndex(0, 2, 0.0f, 0.0f, 0.0f);	// With line indexes
		g_pTex.render(ID_TEXTURE_WALL);
		g_p3D.renderIndex(3, 5, 0.0f, 0.0f, 0.0f);	// With line indexes
		break;
	case TRIANGLES:	// Rendering triangle list
		g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_p3D.renderIndex(0, 1, 0.0f, 0.0f, 0.0f);
		g_pTex.render(ID_TEXTURE_WALL);
		g_p3D.renderIndex(3, 4, 0.0f, 0.0f, 0.0f);
		break;
	}

	g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// Rendering triangle list

	// Render text
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, 0);
	g_pUI.render();
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 3);	// fps, timer, X,Z position
}

