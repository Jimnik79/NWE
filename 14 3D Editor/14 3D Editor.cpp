//--------------------------------------------------------------------------------------
// File: 14 3D Editor.cpp
// Description: Sample of work with 3D editing.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


// This example shows how to work with 3D editing:
// - opening mesh;
// - adding triangle and rectangle;
// - deleting triangle; (yet not implemented)
// - adding and editing keyframe animation. (yet not implemented)

// In the next version of SDK, maybe, will be added axes (X - red, Y - green, Z - blue) and auto arrangement of texture coordinatesin in the next version of engine.


#include "..\Engine\NetWars.h"	// Engine

#include "..\Shared\Mesh.h"	// Class of Mesh

#include <math.h>	// sin, cos
#include <stdio.h>	// swprintf_s


CEngine	g_pAppl;	// Base Engine class
CTex	g_pTex;		// Texture class
CText	g_pText;	// Text class
C2D		g_p2D;		// 2D class
C3D		g_p3D;		// 3D class
CUI		g_pUI;		// UI class
CMesh	g_pMesh;	// Mesh class


#define	ID_BUTTON_OPEN		0
#define	ID_BUTTON_SAVE		1
#define	ID_BUTTON_TRIANGLE	2
#define	ID_BUTTON_RECTANGLE	3
#define	ID_EDITBOX_INDEX	4	// Index of vertex
#define	ID_EDITBOX_VX		5
#define	ID_EDITBOX_VY		6
#define	ID_EDITBOX_VZ		7
#define	ID_EDITBOX_NX		8
#define	ID_EDITBOX_NY		9
#define	ID_EDITBOX_NZ		10
#define	ID_EDITBOX_TX		11
#define	ID_EDITBOX_TY		12
#define	ID_CHECKBOX_ALL		13
#define	ID_CHECKBOX_NORMAL	14
#define	ID_RADIO_POINT		15
#define	ID_RADIO_LINE		16
#define	ID_RADIO_TRIANGLE	17

#define	ID_FONT_16	0

#define	ID_TEXTURE	0	// White texture

#define	GROUP_RENDER	0

enum state	// State machine
{
	DOTS,
	LINES,
	TRIANGLES
};
enum state g_eState = LINES;	// State machine


float	g_cClear[4] = { 0.25f, 0.7f, 0.9f, 1.0f };	// Light blue color for background

float	g_fHW = 512.0f;
float	g_fHH = 300.0f;

// Cube: position X, position Y, position Z, normal X, normal Y, normal Z, texture X, texture Y
float	g_fVertex[192] = {	// 24 * 8

	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 1.0,	// Forward
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 0.0,
	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 0.0,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 1.0,

	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0,	// Up
	0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0,
	1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0,

	1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,	// Back
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
	0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,

	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 1.0,	// Right
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.0,
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.0,
	1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0,

	0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 1.0,	// Left
	0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 0.0,
	0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 0.0,
	0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 1.0,

	0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0, 1.0,	// Down
	0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.0,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.0,
	1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0, 1.0, };

WORD	g_indices[36] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20, };

int		g_iSelectVertex = -1;
int		g_iSelectClick = -1;
int		g_xSelect = 0;
int		g_ySelect = 0;
float	g_vx, g_vy, g_vz;

int		g_iTriangle = 0;	// 0 - do not set the triangle, 1 - set the first vertex, 2 - second, 3 - third
int		g_iRectangle = 0;	// 0 - do not set the rectangle, 1 - set the first vertex, 2 - second, 3 - third, 4 - fourth

bool	g_bUpdateVertex = false;	// Flag to update vertex buffer
bool	g_bResetMesh = false;		// Flag to reset vertex and index buffers
int		g_iAdd2DPoint = 0;

bool	g_bLMouseClickLast = false;

ID3D11RasterizerState*	g_pRasWireFrame = NULL;	// Wireframe rasterizer mode


//--------------------------------------------------------------------------------------
// ENTRY POINT TO THE PROGRAM
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	g_pAppl.setCallbackInitialize();		// Set callback for Initialize
	g_pAppl.setCallbackGUI();				// Set callback for GUI event
	g_pAppl.setCallbackDeviceCreated();		// Set callback for Create Device
	g_pAppl.setCallbackSwapChainResized();	// Set callback for Resize
	g_pAppl.setCallbackFrameRender();		// Set callback for Render Frame
	g_pAppl.setCallbackMouse();				// Set callback for Mouse Event

	g_pTex.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_pText.setHModule(g_pAppl.getHModule());	// Set NetWars.dll
	g_p2D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_p3D.setHModule(g_pAppl.getHModule());		// Set NetWars.dll
	g_pUI.setHModule(g_pAppl.getHModule());		// Set NetWars.dll

	// Run Engine
	return g_pAppl.run(L"14 3D Editor", 1024, 600);
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
	g_pUI.addButton(-1, ID_FONT_16, L"Triangle", 150, 1, 0.8f);	// Return ID_BUTTON_TRIANGLE = 2
	g_pUI.addButton(-1, ID_FONT_16, L"Rectangle", 150, 1, 0.8f);	// Return ID_BUTTON_RECTANGLE = 3

	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_INDEX = 4
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_VX = 5
	g_pUI.setFloat(ID_EDITBOX_VX, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_VY = 6
	g_pUI.setFloat(ID_EDITBOX_VY, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_VZ = 7
	g_pUI.setFloat(ID_EDITBOX_VZ, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_NX = 8
	g_pUI.setFloat(ID_EDITBOX_NX, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_NY = 9
	g_pUI.setFloat(ID_EDITBOX_NY, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_NZ = 10
	g_pUI.setFloat(ID_EDITBOX_NZ, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_TX = 11
	g_pUI.setFloat(ID_EDITBOX_TX, 3, 0);
	g_pUI.addEditBox(-1, ID_FONT_16, L"", 150, 0.8f);	// Return ID_EDITBOX_TY = 12
	g_pUI.setFloat(ID_EDITBOX_TY, 3, 0);
	g_pUI.addCheckBox(-1, ID_FONT_16, L"All vertex", COLOR_GOLD, true, 0.8f);	// Return ID_CHECKBOX_ALL = 13
	g_pUI.addCheckBox(-1, ID_FONT_16, L"Auto normal", COLOR_GOLD, true, 0.8f);	// Return ID_CHECKBOX_NORMAL = 14
	
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Point", COLOR_GOLD, 0.8f);	// Return ID_RADIO_POINT = 15
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Line", COLOR_GOLD, 0.8f);	// Return ID_RADIO_LINE = 16
	g_pUI.addRadioButton(-1, ID_FONT_16, GROUP_RENDER, L"Triangle", COLOR_GOLD, 0.8f);	// Return ID_RADIO_TRIANGLE = 17
	g_pUI.setChecked(ID_RADIO_LINE, true);
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
			g_pMesh.read();
			g_bUpdateVertex = true;
			return;
		case ID_BUTTON_SAVE:
			g_pMesh.write();
			return;
		case ID_BUTTON_TRIANGLE:
			g_iTriangle = 1;
			return;
		case ID_BUTTON_RECTANGLE:
			g_iRectangle = 1;
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
		case ID_EDITBOX_INDEX:
		{
			wchar_t	s[16];
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &g_iSelectVertex);	// Index
			if (g_iSelectVertex < g_pMesh.getNVertex() && g_iSelectVertex >= 0)
			{
				swprintf_s(s, 16, L"%.03f", g_pMesh.getVX(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_VX, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getVY(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_VY, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getVZ(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_VZ, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getNX(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_NX, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getNY(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_NY, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getNZ(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_NZ, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getTX(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_TX, s);
				swprintf_s(s, 16, L"%.03f", g_pMesh.getTY(g_iSelectVertex));	g_pUI.text(ID_EDITBOX_TY, s);
			}
			else
			{
				g_pUI.text(ID_EDITBOX_VX, s);
				g_pUI.text(ID_EDITBOX_VY, s);
				g_pUI.text(ID_EDITBOX_VZ, s);
				g_pUI.text(ID_EDITBOX_NX, s);
				g_pUI.text(ID_EDITBOX_NY, s);
				g_pUI.text(ID_EDITBOX_NZ, s);
				g_pUI.text(ID_EDITBOX_TX, s);
				g_pUI.text(ID_EDITBOX_TY, s);
			}
		}	return;
		case ID_EDITBOX_VX:
		{	int id;
			float vx;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_VX), L"%f", &vx);	// X position
			g_pMesh.setVX(id, vx);
			g_bUpdateVertex = true;
			/// Add recalculation of normals
			//int x, y;
			//g_p3D.getScreenCoordFrom3DPoint(g_pMesh.getVX(id), g_pMesh.getVY(id), g_pMesh.getVZ(id), &x, &y);
			//g_pMesh.setX(id, x);
			//g_pMesh.setY(id, y);
			//g_p2D.updateF(id, 0.0f, 1.0f, 0.0f, 1.0f, x - 3.0f, x + 3.0f, y - 3.0f, y + 3.0f, 0.8f);
		}	return;
		case ID_EDITBOX_VY:
		{	int id;
			float vy;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_VY), L"%f", &vy);	// Y position
			g_pMesh.setVY(id, vy);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_VZ:
		{	int id;
			float vz;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_VZ), L"%f", &vz);	// Z position
			g_pMesh.setVZ(id, vz);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_NX:
		{	int id;
			float nx;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_NX), L"%f", &nx);	// X normal
			g_pMesh.setNX(id, nx);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_NY:
		{	int id;
			float ny;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_NY), L"%f", &ny);	// Y normal
			g_pMesh.setNY(id, ny);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_NZ:
		{	int id;
			float nz;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_NZ), L"%f", &nz);	// Z normal
			g_pMesh.setNZ(id, nz);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_TX:
		{	int id;
			float tx;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_TX), L"%f", &tx);	// X texture coordinate
			g_pMesh.setTX(id, tx);
			g_bUpdateVertex = true;
		}	return;
		case ID_EDITBOX_TY:
		{	int id;
			float ty;
			swscanf_s(g_pUI.getText(ID_EDITBOX_INDEX), L"%d", &id);	// Index
			swscanf_s(g_pUI.getText(ID_EDITBOX_TY), L"%f", &ty);	// Y texture coordinate
			g_pMesh.setTY(id, ty);
			g_bUpdateVertex = true;
		}	return;
		}
		break;
	}
}


//--------------------------------------------------------------------------------------
// MOUSE EVENT HANDLING
//--------------------------------------------------------------------------------------
void CALLBACK onMouse(bool bLeftButtonDown, bool bRightButtonDown, bool, bool, bool, int, int xPos, int yPos)
{
	//float fX = (float)xPos;
	//float fY = (float)yPos;
	if (g_iTriangle)	// If building triangle
	{
		if (bLeftButtonDown && !g_bLMouseClickLast)	// LMouse click
		{
			float x, y, z;	// The direction vector
			D3DXVECTOR3	vNor;
			switch (g_iTriangle)	// New triangle
			{
			case 1:
				g_pMesh.addMemory(3, 3);
				g_iAdd2DPoint = 3;
				//if (g_p3D.get3DCoordFromScreenPoint(v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z, xPos, yPos, &x, &y, &z))
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex1(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					//int xx, yy;
					//g_p3D.getScreenCoordFrom3DPoint(x, y, z, &xx, &yy);	// Test control
					g_iTriangle = 2;
					g_bResetMesh = true;
				}
				break;
			case 2:
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex2(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iTriangle = 3;
					g_bResetMesh = true;
				}
				break;
			case 3:
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex3(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iTriangle = 0;
					g_bResetMesh = true;
				}
				break;
			}
		}
	}
	else if (g_iRectangle)	// If building rectangle
	{
		if (bLeftButtonDown && !g_bLMouseClickLast)	// LMouse click
		{
			float x, y, z;	// The direction vector
			D3DXVECTOR3	v[3], vNor;
			switch (g_iRectangle)
			{
			case 1:
				g_pMesh.addMemory(4, 6);
				g_iAdd2DPoint = 4;
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex1(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iRectangle = 2;
					g_bResetMesh = true;
				}
				break;
			case 2:
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex2(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iRectangle = 3;
					g_bResetMesh = true;
				}
				break;
			case 3:
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex3(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iRectangle = 4;
					g_bResetMesh = true;
				}
				break;
			case 4:
				if (g_p3D.get3DCoordForPerpPlane(xPos, yPos, 10.0f, &x, &y, &z, &vNor))
				{
					g_pMesh.addVertex4(xPos, yPos, x, y, z, -vNor.x, -vNor.y, -vNor.z, 0.0f, 0.0f);
					g_iRectangle = 0;
					g_bResetMesh = true;
				}
				break;
			}
		}
	}

	else
	{
		if (bLeftButtonDown)	// LMouse click
		{
			//g_iSelectVertex = -1;
			if (g_iSelectClick == -1)
				for (int i = 0; i < g_pMesh.getNVertex(); i++)	// Edit points
				{
				if (xPos > g_pMesh.getX(i) - 4 && xPos < g_pMesh.getX(i) + 4 && yPos > g_pMesh.getY(i) - 4 && yPos < g_pMesh.getY(i) + 4)
				{
					int iV1, iV2;
					float fX, fY, fZ;
					g_pMesh.findTriangle(i, &iV1, &iV2);
					if (g_p3D.get3DCoordFromScreenPoint(g_pMesh.getVX(i), g_pMesh.getVY(i), g_pMesh.getVZ(i),
						g_pMesh.getVX(iV1), g_pMesh.getVY(iV1), g_pMesh.getVZ(iV1), g_pMesh.getVX(iV2), g_pMesh.getVY(iV2), g_pMesh.getVZ(iV2), g_pMesh.getX(i), g_pMesh.getY(i), &fX, &fY, &fZ))
					{
						g_iSelectClick = i;
						g_iSelectVertex = i;
						g_xSelect = xPos;
						g_ySelect = yPos;
						g_vx = g_pMesh.getVX(i);	// Remember begin coordinate
						g_vy = g_pMesh.getVY(i);
						g_vz = g_pMesh.getVZ(i);
						break;
					}
				}
			}

			wchar_t	s[16];
			if (g_iSelectClick > -1)	// Check vertices reposition
			{
				int iV1, iV2;
				float fX, fY, fZ;
				g_pMesh.findTriangle(g_iSelectClick, &iV1, &iV2);
				if (g_p3D.get3DCoordFromScreenPoint(g_pMesh.getVX(g_iSelectClick), g_pMesh.getVY(g_iSelectClick), g_pMesh.getVZ(g_iSelectClick),
					g_pMesh.getVX(iV1), g_pMesh.getVY(iV1), g_pMesh.getVZ(iV1), g_pMesh.getVX(iV2), g_pMesh.getVY(iV2), g_pMesh.getVZ(iV2), g_xSelect, g_ySelect, &fX, &fY, &fZ))
				{
					g_pMesh.setVX(g_iSelectClick, fX);
					g_pMesh.setVY(g_iSelectClick, fY);
					g_pMesh.setVZ(g_iSelectClick, fZ);
					g_bUpdateVertex = true;

					g_xSelect = xPos;
					g_ySelect = yPos;

					// After changing recalculate the normals (normal when you move do not change)

					swprintf_s(s, 16, L"%d", g_iSelectClick);					g_pUI.text(ID_EDITBOX_INDEX, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getVX(g_iSelectClick));	g_pUI.text(ID_EDITBOX_VX, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getVY(g_iSelectClick));	g_pUI.text(ID_EDITBOX_VY, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getVZ(g_iSelectClick));	g_pUI.text(ID_EDITBOX_VZ, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getNX(g_iSelectClick));	g_pUI.text(ID_EDITBOX_NX, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getNY(g_iSelectClick));	g_pUI.text(ID_EDITBOX_NY, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getNZ(g_iSelectClick));	g_pUI.text(ID_EDITBOX_NZ, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getTX(g_iSelectClick));	g_pUI.text(ID_EDITBOX_TX, s);
					swprintf_s(s, 16, L"%.03f", g_pMesh.getTY(g_iSelectClick));	g_pUI.text(ID_EDITBOX_TY, s);
				}
				else
				{
					g_pMesh.setVX(g_iSelectClick, g_vx);	// Restore begin coordinate
					g_pMesh.setVY(g_iSelectClick, g_vy);
					g_pMesh.setVZ(g_iSelectClick, g_vz);
					g_iSelectClick = -1;
				}
			}
		}
		else
		{
			g_iSelectClick = -1;
		}
	}

	// If you have released a number of point(s), they are equal

	if (bRightButtonDown)
	{
		g_iSelectVertex = -1;

		g_iTriangle = 0;
		g_iRectangle = 0;
	}

	g_bLMouseClickLast = bLeftButtonDown;
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DO NOT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onDeviceCreated()
{
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 0, add space for fps
	g_pText.addSpace(ID_FONT_16, TEXT_NORMAL, 100, DT_LEFT, 0.5f);	// Return 1, add space for timer

	g_pTex.addDynamic(1, 1);	// Return ID_TEXTURE = 0
	UINT map[1] = { 0xff000000 };	// White texture
	g_pTex.setMap(ID_TEXTURE, map);	// Set texture map for 3D object

	g_p3D.init(12.0f, 0.0f, 0.3f, 1.0f, 50.0f, 0.1f, 1.5f);	// 3D initialization

	// Square: position X, position Y, position Z, normal X, normal Y, normal Z, texture X, texture Y
	float	fVertex[6][8] = {
			{ -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 },
			{ -5.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0 },
			{ 5.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
			{ 5.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0 },
			{ 5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0 },
			{ -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0 }, };
	g_p3D.addVertex(fVertex, sizeof(fVertex));	// Return 0

	g_pMesh.setMesh(g_fVertex, 24, g_indices, 36);

	g_pUI.setInt(ID_EDITBOX_INDEX, g_pMesh.getNVertex());

	// Create vertex buffer
	g_p3D.addVertex(g_pMesh.getVertex(), 32 * g_pMesh.getNVertex());	// Return 1

	// Create index buffer
	g_p3D.addIndex(g_pMesh.getIndex(), 2 * g_pMesh.getNIndex());	// Return 2

	// Create line index buffer
	g_pMesh.createLineIndex();
	g_p3D.addIndex(g_pMesh.getIndexLine(), 2 * g_pMesh.getNIndexLine());	// Return 3

	//g_pMesh.write();
}


//--------------------------------------------------------------------------------------
// CREATE RESOURCES THAT DEPEND ON THE BACK BUFFER
//--------------------------------------------------------------------------------------
void CALLBACK onSwapChainResized(int, int)
{
	g_pText.pos(0, 5, 2);	// fps
	g_pText.pos(1, 105, 2);	// timer

	g_pUI.pos(ID_BUTTON_OPEN, 5, 20);
	g_pUI.pos(ID_BUTTON_SAVE, 5, 50);
	g_pUI.pos(ID_BUTTON_TRIANGLE, 5, 80);
	g_pUI.pos(ID_BUTTON_RECTANGLE, 5, 110);
	g_pUI.pos(ID_EDITBOX_INDEX, 5, 140);
	g_pUI.pos(ID_EDITBOX_VX, 5, 170);
	g_pUI.pos(ID_EDITBOX_VY, 5, 200);
	g_pUI.pos(ID_EDITBOX_VZ, 5, 230);
	g_pUI.pos(ID_EDITBOX_NX, 5, 260);
	g_pUI.pos(ID_EDITBOX_NY, 5, 290);
	g_pUI.pos(ID_EDITBOX_NZ, 5, 320);
	g_pUI.pos(ID_EDITBOX_TX, 5, 350);
	g_pUI.pos(ID_EDITBOX_TY, 5, 380);
	g_pUI.pos(ID_CHECKBOX_ALL, 5, 410);
	g_pUI.pos(ID_CHECKBOX_NORMAL, 5, 435);
	g_pUI.pos(ID_RADIO_POINT, 5, 460);
	g_pUI.pos(ID_RADIO_LINE, 5, 480);
	g_pUI.pos(ID_RADIO_TRIANGLE, 5, 500);

	// Panel sprites
	//g_p2D.setTextureSize(1, 1);	// For the correct calculation of texture coordinates of sprites
	float*	fVertex = g_pMesh.getVertex();
	int x, y;
	for (int i = 0; i < g_pMesh.getNVertexNew(); i++)	// Edit points
	{
		g_p3D.getScreenCoordFrom3DPoint(fVertex[8*i], fVertex[8*i+1], fVertex[8*i+2], &x, &y);
		g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, x - 3.0f, x + 3.0f, y - 3.0f, y + 3.0f, 0.8f);	// Return i
	}
}


//--------------------------------------------------------------------------------------
// RENDER THE SCENE
//--------------------------------------------------------------------------------------
void CALLBACK onFrameRender(double, float)
{
	g_pText.fps(ID_FONT_16, 0, DT_LEFT, COLOR_ORANGE);	// fps
	g_pText.timer(ID_FONT_16, 1, DT_LEFT, COLOR_ORANGE);	// timer

	if (g_eState != DOTS)
	{
		if (g_iAdd2DPoint)
		{
			for (int i = 0; i < g_iAdd2DPoint; i++)
				g_p2D.addF(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.8f);	// 
			g_p2D.resetBuffer();	// Reset sprite buffer
			g_iAdd2DPoint = 0;
		}

		float*	fVertex = g_pMesh.getVertex();
		int x, y;
		for (int i = 0; i < g_pMesh.getNVertex(); i++)	// Edit points
		{
			g_p3D.getScreenCoordFrom3DPoint(fVertex[8 * i], fVertex[8 * i + 1], fVertex[8 * i + 2], &x, &y);
			g_pMesh.setX(i, x);
			g_pMesh.setY(i, y);
			if (i==24)
				g_p2D.updateF(i, 0.0f, 1.0f, 0.0f, 1.0f, x - 3.0f, x + 3.0f, y - 3.0f, y + 3.0f, 0.8f);
			else
				g_p2D.updateF(i, 0.0f, 1.0f, 0.0f, 1.0f, x - 3.0f, x + 3.0f, y - 3.0f, y + 3.0f, 0.8f);
		}
		g_p2D.updateBuffer();

		if (g_bUpdateVertex)	// If need to update vertex buffer
		{
			g_p3D.updateVertex(1, g_pMesh.getVertex(), 32 * g_pMesh.getNVertex());
			g_bUpdateVertex = false;	// Vertex update completed
		}

		if (g_bResetMesh)	// If need to reset vertex and index buffers
		{
			g_p3D.updateVertex(1, g_pMesh.getVertex(), 32 * g_pMesh.getNVertex());
			g_p3D.updateIndex(2, g_pMesh.getIndex(), 2 * g_pMesh.getNIndex());
			g_p3D.updateIndex(3, g_pMesh.getIndexLine(), 2 * g_pMesh.getNIndexLine());
			g_bResetMesh = false;	// Vertex update completed
		}
	}

	g_pAppl.clearScreen(g_cClear);

	g_p3D.setRender3D();	// Set 3D render

	g_pTex.render(ID_TEXTURE);
	switch (g_eState)
	{
	case DOTS:	// Rendering of points
		//g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		g_p3D.setPrimitiveTopology(TOPOLOGY_POINTLIST);
		g_p3D.renderIndex(1, 2, 0.0f, 0.0f, 0.0f);
		break;
	case LINES:	// Rendering of lines
		//g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		g_p3D.setPrimitiveTopology(TOPOLOGY_LINELIST);
		g_p3D.renderIndex(1, 3, 0.0f, 0.0f, 0.0f);	// With line indexes
		break;
	case TRIANGLES:	// Rendering of triangles
		//g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_p3D.setPrimitiveTopology(TOPOLOGY_TRIANGLELIST);
		g_p3D.renderIndex(1, 2, 0.0f, 0.0f, 0.0f);
		break;
	}

	// Render mesh vertex
	g_pAppl.setRenderSprites(RENDER_ONLY_TEXTURE, ID_TEXTURE);
	g_p2D.draw(0, g_pMesh.getNVertex());

	//g_pAppl.getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// Rendering of triangles
	g_p3D.setPrimitiveTopology(TOPOLOGY_TRIANGLELIST);	// Rendering of triangles

	// Render UI
	g_pUI.render();

	// Render text
	g_pAppl.setRenderTexts();
	g_pText.draw(0, 2);
}

