//--------------------------------------------------------------------------------------
// File: Mesh.cpp
// Description: Class of work with mesh files.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include "..\Engine\NetWars.h"	// NetWars Engine
#include "D3Dcompiler.h"
//#include "D3DX10Math.h"
#include "D3DX11async.h"	// D3DX11CompileFromFileW - Library D3DX11.lib
#include <stdio.h>	// swprintf_s

#include "Map.h"	// Class of Mesh

extern	C3D		g_p3D;		// 3D class
extern	CPhys	g_pPhys;	// Physics class


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CMap::CMap()
{
	m_lenght = 1024.0f;	// Map lenght = 1 km 24 m
	m_width = 1024.0f;	// Map width = 1 km 24 m
	m_nWalls = 0;		// Number of walls
	m_nPits = 0;		// Number of pits

	m_wx = NULL;	// Walls, X position
	m_wz = NULL;	// Walls, Z position
	m_wl = NULL;	// Walls, lenght (X)
	m_ww = NULL;	// Walls, width (Z)
	m_wh = NULL;	// Walls, height (Y)

	m_px = NULL;	// Pits, X position
	m_pz = NULL;	// Pits, Z position
	m_pl = NULL;	// Pits, lenght (X)
	m_pw = NULL;	// Pits, width (Z)
	m_ph = NULL;	// Pits, height (Y)
}


//--------------------------------------------------------------------------------------
// DESTRUCTOR
//--------------------------------------------------------------------------------------
CMap::~CMap()
{
	free();	// Free memory
}


//--------------------------------------------------------------------------------------
// FREE MEMORY
//--------------------------------------------------------------------------------------
void CMap::free()
{
	SAFE_DELETE_ARRAY(m_wx);
	SAFE_DELETE_ARRAY(m_wz);
	SAFE_DELETE_ARRAY(m_wl);
	SAFE_DELETE_ARRAY(m_ww);
	SAFE_DELETE_ARRAY(m_wh);

	SAFE_DELETE_ARRAY(m_px);
	SAFE_DELETE_ARRAY(m_pz);
	SAFE_DELETE_ARRAY(m_pl);
	SAFE_DELETE_ARRAY(m_pw);
	SAFE_DELETE_ARRAY(m_ph);
}

//--------------------------------------------------------------------------------------
// RESET
//--------------------------------------------------------------------------------------
void CMap::reset()
{
	free();	// Free memory

	m_wx = new float[m_nWalls];
	m_wz = new float[m_nWalls];
	m_wl = new float[m_nWalls];
	m_ww = new float[m_nWalls];
	m_wh = new float[m_nWalls];

	m_px = new float[m_nPits];
	m_pz = new float[m_nPits];
	m_pl = new float[m_nPits];
	m_pw = new float[m_nPits];
	m_ph = new float[m_nPits];
}

//--------------------------------------------------------------------------------------
// SET STANDART MAP
//--------------------------------------------------------------------------------------
void CMap::setStandartMap()
{
	m_lenght = 1024.0f;	// Map lenght
	m_width = 1024.0f;	// Map width
	m_nWalls = 0;		// Number of walls
	m_nPits = 0;		// Number of pits

	reset();		// Reset
	setGround();	// Set ground

	for (int level = 0; level < 10; level++)
	{
		float delta = (942.0f - 80.0f*level - (11 - level)*2.0f) / (12 - level);	// 1024 - 2*41
		for (int i = 0; i < 12 - level; i++)
		{
			addWall(40.0f*level + 41.0f + i*(delta + 2.0f), 40.0f*level + 39.0f, delta, 1.0f, 5.0f);
			addWall(40.0f*level + 41.0f + i*(delta + 2.0f), 984.0f - 40.0f*level, delta, 1.0f, 5.0f);
			addWall(40.0f*level + 39.0f, 40.0f*level + 41.0f + i*(delta + 2.0f), 1.0f, delta, 5.0f);
			addWall(984.0f - 40.0f*level, 40.0f*level + 41.0f + i*(delta + 2.0f), 1.0f, delta, 5.0f);
		}

		delta = delta / 2.0f + 1.0f;
		for (int i = 0; i < 25 - 2 * level; i++)
		{
			addWall(40.0f*level + 40.0f + i*delta,       40.0f*level + 5.0f,  0.5f, 30.0f, 2.5f);
			addWall(40.0f*level + 45.0f + (i - 1)*delta, 40.0f*level + 10.0f, 30.0f, 0.5f, 2.5f);
			addWall(40.0f*level + 45.0f + (i - 1)*delta, 40.0f*level + 30.0f, 30.0f, 0.5f, 2.5f);

			addWall(989.0f - 40.0f*level,  40.0f*level + 40.0f + i*delta,       30.0f, 0.5f, 2.5f);
			addWall(994.0f - 40.0f*level,  40.0f*level + 45.0f + (i - 1)*delta, 0.5f, 30.0f, 2.5f);
			addWall(1014.0f - 40.0f*level, 40.0f*level + 45.0f + (i - 1)*delta, 0.5f, 30.0f, 2.5f);

			addWall(40.0f*level + 40.0f + i*delta, 989.0f - 40.0f*level,  0.5f, 30.0f, 2.5f);
			addWall(40.0f*level + 45.0f + i*delta, 994.0f - 40.0f*level,  30.0f, 0.5f, 2.5f);
			addWall(40.0f*level + 45.0f + i*delta, 1014.0f - 40.0f*level, 30.0f, 0.5f, 2.5f);

			addWall(40.0f*level + 5.0f,  40.0f*level + 40.0f + i*delta, 30.0f, 0.5f, 2.5f);
			addWall(40.0f*level + 10.0f, 40.0f*level + 45.0f + i*delta, 0.5f, 30.0f, 2.5f);
			addWall(40.0f*level + 30.0f, 40.0f*level + 45.0f + i*delta, 0.5f, 30.0f, 2.5f);
		}

		addWall(430.0f, 404.0f, 80.0f, 0.5f, 2.5f);
		addWall(514.0f, 404.0f, 80.0f, 0.5f, 2.5f);
		addWall(430.0f, 619.5f, 80.0f, 0.5f, 2.5f);
		addWall(514.0f, 619.5f, 80.0f, 0.5f, 2.5f);
		addWall(404.0f, 430.0f, 0.5f, 80.0f, 2.5f);
		addWall(404.0f, 514.0f, 0.5f, 80.0f, 2.5f);
		addWall(619.5f, 430.0f, 0.5f, 80.0f, 2.5f);
		addWall(619.5f, 514.0f, 0.5f, 80.0f, 2.5f);

		addWall(408.5f, 404.0f, 0.5f, 80.0f, 2.5f);
		addWall(540.0f, 408.5f, 80.0f, 0.5f, 2.5f);
		addWall(615.0f, 540.0f, 0.5f, 80.0f, 2.5f);
		addWall(404.0f, 615.0f, 80.0f, 0.5f, 2.5f);

		addWall(480.0f, 450.0f, 64.0f, 0.5f, 2.5f);
		addWall(483.5f, 453.5f, 60.5f, 0.5f, 2.5f);
		addWall(480.0f, 450.7f, 0.5f, 62.6f, 2.5f);
		addWall(483.5f, 454.2f, 0.5f, 55.6f, 2.5f);
		addWall(483.5f, 510.0f, 26.4f, 0.5f, 2.5f);
		addWall(480.0f, 513.5f, 29.9f, 0.5f, 2.5f);

		addWall(570.0f, 483.5f, 0.5f, 60.5f, 2.5f);
		addWall(573.5f, 480.0f, 0.5f, 64.0f, 2.5f);
		addWall(510.7f, 480.0f, 62.6f, 0.5f, 2.5f);
		addWall(514.2f, 483.5f, 55.6f, 0.5f, 2.5f);
		addWall(510.0f, 480.0f, 0.5f, 29.9f, 2.5f);
		addWall(513.5f, 483.5f, 0.5f, 26.4f, 2.5f);

		addWall(480.0f, 570.0f, 60.5f, 0.5f, 2.5f);
		addWall(480.0f, 573.5f, 64.0f, 0.5f, 2.5f);
		addWall(540.0f, 514.2f, 0.5f, 55.6f, 2.5f);
		addWall(543.5f, 510.7f, 0.5f, 62.6f, 2.5f);
		addWall(514.1f, 510.0f, 29.9f, 0.5f, 2.5f);
		addWall(514.1f, 513.5f, 26.4f, 0.5f, 2.5f);

		addWall(450.0f, 480.0f, 0.5f, 64.0f, 2.5f);
		addWall(453.5f, 480.0f, 0.5f, 60.5f, 2.5f);
		addWall(454.2f, 540.0f, 55.6f, 0.5f, 2.5f);
		addWall(450.7f, 543.5f, 62.6f, 0.5f, 2.5f);
		addWall(510.0f, 514.1f, 0.5f, 26.4f, 2.5f);
		addWall(513.5f, 514.1f, 0.5f, 29.9f, 2.5f);
	}
}


//--------------------------------------------------------------------------------------
// SET GROUND
//--------------------------------------------------------------------------------------
void CMap::setGround()
{
	// Square: X, Y, Z position; X, Y, Z normal; X, Y texture
	float	fVertex[32] = {	// 4 * 8
		0.0f,     0.0f, 0.0f,    0.0f, 1.0f, 0.0f, 0.0f,            m_width / 4.0f,	// Ground
		0.0f,     0.0f, m_width, 0.0f, 1.0f, 0.0f, 0.0f,            0.0f,
		m_lenght, 0.0f, m_width, 0.0f, 1.0f, 0.0f, m_lenght / 4.0f, 0.0f,
		m_lenght, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f, m_lenght / 4.0f, m_width / 4.0f, };
	WORD	indices[6] = { 0, 1, 2, 2, 3, 0, };	// Ground
	m_pGround.setMesh(fVertex, 4, indices, 6);
}


//--------------------------------------------------------------------------------------
// LOADING MAP FILE (TEXT)
//--------------------------------------------------------------------------------------
void CMap::load(LPCWSTR sFile)	// L"test.txtmap"
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, sFile, L"r"))	// Open a text file for reading
	{
		wchar_t	s[256];
		fgetws(s, 256, pFile);	swscanf_s(s, L"%f", &m_lenght);	// Map lenght
		fgetws(s, 256, pFile);	swscanf_s(s, L"%f", &m_width);	// Map width
		fgetws(s, 256, pFile);	swscanf_s(s, L"%d", &m_nWalls);	// Number of walls
		fgetws(s, 256, pFile);	swscanf_s(s, L"%d", &m_nPits);	// Number of pits

		reset();	// Reset

		setGround();	// Set ground

		int i = 0, j = 0;
		float*	fV = new float[128 + 160 * m_nWalls];	// 16 vertices of 4 boundary walls + 20 vertices for the 5 faces of each wall (bottomless)
		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;	// West wall
		fV[i++] = 0.0f;		fV[i++] = 10.0f;	fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;
		fV[i++] = 0.0f;		fV[i++] = 10.0f;	fV[i++] = m_width;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 100.0f;	fV[i++] = 1.0f;
		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = m_width;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 100.0f;	fV[i++] = 0.0f;
		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = m_width;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;	// North wall
		fV[i++] = 0.0f;		fV[i++] = 10.0f;	fV[i++] = m_width;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 1.0f;
		fV[i++] = m_lenght;	fV[i++] = 10.0f;	fV[i++] = m_width;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 100.0f;	fV[i++] = 1.0f;
		fV[i++] = m_lenght;	fV[i++] = 0.0f;		fV[i++] = m_width;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 100.0f;	fV[i++] = 0.0f;
		fV[i++] = m_lenght;	fV[i++] = 0.0f;		fV[i++] = m_width;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;	// East wall
		fV[i++] = m_lenght;	fV[i++] = 10.0f;	fV[i++] = m_width;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;
		fV[i++] = m_lenght;	fV[i++] = 10.0f;	fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 100.0f;	fV[i++] = 1.0f;
		fV[i++] = m_lenght;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 100.0f;	fV[i++] = 0.0f;
		fV[i++] = m_lenght;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;	// South wall
		fV[i++] = m_lenght;	fV[i++] = 10.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;
		fV[i++] = 0.0f;		fV[i++] = 10.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 100.0f;	fV[i++] = 1.0f;
		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 100.0f;	fV[i++] = 0.0f;

		WORD*	ind = new WORD[24 + 30 * m_nWalls];
		ind[j++] = 0;	ind[j++] = 1;	ind[j++] = 2;	ind[j++] = 2;	ind[j++] = 3;	ind[j++] = 0;
		ind[j++] = 4;	ind[j++] = 5;	ind[j++] = 6;	ind[j++] = 6;	ind[j++] = 7;	ind[j++] = 4;
		ind[j++] = 8;	ind[j++] = 9;	ind[j++] = 10;	ind[j++] = 10;	ind[j++] = 11;	ind[j++] = 8;
		ind[j++] = 12;	ind[j++] = 13;	ind[j++] = 14;	ind[j++] = 14;	ind[j++] = 15;	ind[j++] = 12;

		// Reading Walls
		for (int n = 0; n < m_nWalls; n++)
		{
			fgetws(s, 256, pFile);
			swscanf_s(s, L"%f	%f	%f	%f	%f", &m_wx[n], &m_wz[n], &m_wl[n], &m_ww[n], &m_wh[n]);	// X & Z position, lenght (X), width (Z), height (Y)

			float x1 = m_wx[n];
			float x2 = m_wx[n] + m_wl[n];
			float x3 = m_wx[n] + m_wl[n];
			float x4 = m_wx[n];
			float z1 = m_wz[n];
			float z2 = m_wz[n];
			float z3 = m_wz[n] + m_ww[n];
			float z4 = m_wz[n] + m_ww[n];
			float y1 = 0.0f;
			float y2 = m_wh[n];
			float tx1 = m_wl[n] / 1.5f;
			float tx2 = tx1 + m_ww[n] / 1.5f;
			float tx3 = tx2 + tx1;
			float tx4 = tx2 + tx2;
			float ty1 = m_wh[n] / 1.5f;
			float ty2 = ty1 + m_ww[n] / 1.5f;
			// Do not forget to calculate 5 normal 

			fV[i++] = x1;	fV[i++] = y1;	fV[i++] = z1;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;	fV[i++] = 0.0f;	// South face of the wall
			fV[i++] = x1;	fV[i++] = y2;	fV[i++] = z1;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = 0.0f;	fV[i++] = ty1;
			fV[i++] = x2;	fV[i++] = y2;	fV[i++] = z2;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = tx1;	fV[i++] = ty1;
			fV[i++] = x2;	fV[i++] = y1;	fV[i++] = z2;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = -1.0f;	fV[i++] = tx1;	fV[i++] = 0.0f;

			fV[i++] = x2;	fV[i++] = y1;	fV[i++] = z2;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx1;	fV[i++] = 0.0f;	// East face of the wall
			fV[i++] = x2;	fV[i++] = y2;	fV[i++] = z2;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx1;	fV[i++] = ty1;
			fV[i++] = x3;	fV[i++] = y2;	fV[i++] = z3;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx2;	fV[i++] = ty1;
			fV[i++] = x3;	fV[i++] = y1;	fV[i++] = z3;	fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx2;	fV[i++] = 0.0f;

			fV[i++] = x3;	fV[i++] = y1;	fV[i++] = z3;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = tx2;	fV[i++] = 0.0f;	// North face of the wall
			fV[i++] = x3;	fV[i++] = y2;	fV[i++] = z3;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = tx2;	fV[i++] = ty1;
			fV[i++] = x4;	fV[i++] = y2;	fV[i++] = z4;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = tx3;	fV[i++] = ty1;
			fV[i++] = x4;	fV[i++] = y1;	fV[i++] = z4;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = tx3;	fV[i++] = 0.0f;

			fV[i++] = x4;	fV[i++] = y1;	fV[i++] = z4;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx3;	fV[i++] = 0.0f;	// West face of the wall
			fV[i++] = x4;	fV[i++] = y2;	fV[i++] = z4;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx3;	fV[i++] = ty1;
			fV[i++] = x1;	fV[i++] = y2;	fV[i++] = z1;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx4;	fV[i++] = ty1;
			fV[i++] = x1;	fV[i++] = y1;	fV[i++] = z1;	fV[i++] = -1.0f;	fV[i++] = 0.0f;		fV[i++] = 0.0f;		fV[i++] = tx4;	fV[i++] = 0.0f;

			fV[i++] = x1;	fV[i++] = y2;	fV[i++] = z1;	fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;	fV[i++] = ty1;	// The top of the wall
			fV[i++] = x4;	fV[i++] = y2;	fV[i++] = z4;	fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = 0.0f;	fV[i++] = ty2;
			fV[i++] = x3;	fV[i++] = y2;	fV[i++] = z3;	fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = tx1;	fV[i++] = ty2;
			fV[i++] = x2;	fV[i++] = y2;	fV[i++] = z2;	fV[i++] = 0.0f;		fV[i++] = 1.0f;		fV[i++] = 0.0f;		fV[i++] = tx1;	fV[i++] = ty1;

			WORD v = WORD(i / 8 - 20);
			for (int k = 0; k < 5; k++)
			{
				ind[j++] = v;	ind[j++] = v + 1;	ind[j++] = v + 2;	ind[j++] = v + 2;	ind[j++] = v + 3;	ind[j++] = v;
				v += 4;
			}
		}
		m_pWalls.setMesh(fV, 16 + 20 * m_nWalls, ind, 24 + 30 * m_nWalls);

		delete[] fV;
		delete[] ind;

		// Reading TV

		// Reading Pits
		for (int p = 0; p < m_nPits; p++)
		{
			fgetws(s, 256, pFile);
			swscanf_s(s, L"%f	%f	%f	%f	%f", &m_px[p], &m_pz[p], &m_pl[p], &m_pw[p], &m_ph[p]);	// X & Z position, lenght (X), width (Z), height (Y)
		}

		fclose(pFile);	// Cllose file
	}
}

//--------------------------------------------------------------------------------------
// LOADING MESH FILE (BINARY)
//--------------------------------------------------------------------------------------
void CMap::loadb(LPCWSTR sFile)	// L"test.map"
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, sFile, L"rb"))	// Open a binary file for reading
	{
		fread(&m_lenght, 1, 4, pFile);	// Map lenght
		fread(&m_width, 1, 4, pFile);	// Map width
		fread(&m_nWalls, 1, 4, pFile);	// Number of walls
		fread(&m_nPits, 1, 4, pFile);	// Number of pits

		// Reading walls and pits
		fread(m_wx, 1, 4 * m_nWalls, pFile);	// Walls, X position
		fread(m_wz, 1, 4 * m_nWalls, pFile);	// Walls, Z position
		fread(m_wl, 1, 4 * m_nWalls, pFile);	// Walls, lenght (X)
		fread(m_ww, 1, 4 * m_nWalls, pFile);	// Walls, width (Z)
		fread(m_wh, 1, 4 * m_nWalls, pFile);	// Walls, height (Y)
		fread(m_px, 1, 4 * m_nPits, pFile);		// Pits, X position
		fread(m_pz, 1, 4 * m_nPits, pFile);		// Pits, Z position
		fread(m_pl, 1, 4 * m_nPits, pFile);		// Pits, lenght (X)
		fread(m_pw, 1, 4 * m_nPits, pFile);		// Pits, width (Z)
		fread(m_ph, 1, 4 * m_nPits, pFile);		// Pits, height (Y)

		fclose(pFile);	// Cllose file
	}
}


//--------------------------------------------------------------------------------------
// SAVING MESH FILE (TEXT)
//--------------------------------------------------------------------------------------
void CMap::save(LPCWSTR sFile)	// L"test.txtmap"
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, sFile, L"w"))	// Open a text file for writing
	{
		fwprintf(pFile, L"%f	// Map height\n", m_lenght);		// Map lenght
		fwprintf(pFile, L"%f	// Map width\n", m_width);			// Map width
		fwprintf(pFile, L"%d	// Number of walls\n", m_nWalls);	// Number of walls
		fwprintf(pFile, L"%d	// Number of pits\n", m_nPits);		// Number of pits

		for (int i = 0; i < m_nWalls; i++)	// All Walls
			fwprintf(pFile, L"%f	%f	%f	%f	%f\n", m_wx[i], m_wz[i], m_wl[i], m_ww[i], m_wh[i]);	// X & Z position, lenght (X), width (Z), height (Y)
		for (int i = 0; i < m_nPits; i++)	// All Pits
			fwprintf(pFile, L"%f	%f	%f	%f	%f\n", m_px[i], m_pz[i], m_pl[i], m_pw[i], m_ph[i]);	// X & Z position, lenght (X), width (Z), height (Y)

		fclose(pFile);	// Close file
	}
}


//--------------------------------------------------------------------------------------
// SAVING MESH FILE (BINARY)
//--------------------------------------------------------------------------------------
void CMap::saveb(LPCWSTR sFile)	// L"test.map"
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, sFile, L"wb"))	// Open a binary file for writing
	{
		fwrite(&m_lenght, 1, 4, pFile);	// Map lenght
		fwrite(&m_width, 1, 4, pFile);	// Map width
		fwrite(&m_nWalls, 1, 4, pFile);	// Number of walls
		fwrite(&m_nPits, 1, 4, pFile);	// Number of pits

		// Writing walls and pits
		fwrite(m_wx, 1, 4 * m_nWalls, pFile);	// Walls, X position
		fwrite(m_wz, 1, 4 * m_nWalls, pFile);	// Walls, Z position
		fwrite(m_wl, 1, 4 * m_nWalls, pFile);	// Walls, lenght (X)
		fwrite(m_ww, 1, 4 * m_nWalls, pFile);	// Walls, width (Z)
		fwrite(m_wh, 1, 4 * m_nWalls, pFile);	// Walls, height (Y)
		fwrite(m_px, 1, 4 * m_nPits, pFile);	// Pits, X position
		fwrite(m_pz, 1, 4 * m_nPits, pFile);	// Pits, Z position
		fwrite(m_pl, 1, 4 * m_nPits, pFile);	// Pits, lenght (X)
		fwrite(m_pw, 1, 4 * m_nPits, pFile);	// Pits, width (Z)
		fwrite(m_ph, 1, 4 * m_nPits, pFile);	// Pits, height (Y)

		fclose(pFile);	// Close file
	}
}


////// ADD FUNCTION - GENERATES PERSON/BOT POSITION, THAT NOT TO BE IN THE WALL.


//--------------------------------------------------------------------------------------
// LOAD PHYSICS
//--------------------------------------------------------------------------------------
void CMap::loadPhysics()
{
	for (int n = 0; n < m_nWalls; n++)
		g_pPhys.addQuad(m_wx[n], m_wz[n], m_wx[n] + m_wl[n], m_wz[n], m_wx[n] + m_wl[n], m_wz[n] + m_ww[n], m_wx[n], m_wz[n] + m_ww[n], 0.0f, m_wh[n]);
}


//--------------------------------------------------------------------------------------
// ADD MEMORY FOR WALLS
//--------------------------------------------------------------------------------------
void CMap::addMemoryForWalls()
{
	int nWallsBytes = m_nWalls << 2;
	
	// Allocate new memory
	m_nWalls++;
	float* m_wx0 = new float[m_nWalls];
	float* m_wz0 = new float[m_nWalls];
	float* m_wl0 = new float[m_nWalls];
	float* m_ww0 = new float[m_nWalls];
	float* m_wh0 = new float[m_nWalls];

	// Copy memory
	memcpy(m_wx0, m_wx, nWallsBytes);
	memcpy(m_wz0, m_wz, nWallsBytes);
	memcpy(m_wl0, m_wl, nWallsBytes);
	memcpy(m_ww0, m_ww, nWallsBytes);
	memcpy(m_wh0, m_wh, nWallsBytes);

	// Free old memory
	SAFE_DELETE_ARRAY(m_wx);
	SAFE_DELETE_ARRAY(m_wz);
	SAFE_DELETE_ARRAY(m_wl);
	SAFE_DELETE_ARRAY(m_ww);
	SAFE_DELETE_ARRAY(m_wh);

	// Set memory
	m_wx = m_wx0;
	m_wz = m_wz0;
	m_wl = m_wl0;
	m_ww = m_ww0;
	m_wh = m_wh0;
}

//--------------------------------------------------------------------------------------
// ADD WALL
//--------------------------------------------------------------------------------------
void CMap::addWall(float x, float z, float l, float w, float h)
{
	int nWalls = m_nWalls;
	addMemoryForWalls();

	// Add walls
	m_wx[nWalls] = x;
	m_wz[nWalls] = z;
	m_wl[nWalls] = l;
	m_ww[nWalls] = w;
	m_wh[nWalls] = h;

	//float y = 0.0f;
	m_pWalls.addMemory(20, 30);
	m_pWalls.addVertex1(0, 0, x,     0.0f, z,      0.0f, 0.0f, -1.0f, 0.0f,                    0.0f);
	m_pWalls.addVertex2(0, 0, x,     h,    z,      0.0f, 0.0f, -1.0f, 0.0f,                    h / 1.5f);
	m_pWalls.addVertex3(0, 0, x + l, h,    z,      0.0f, 0.0f, -1.0f, l / 1.5f,                h / 1.5f);
	m_pWalls.addVertex4(0, 0, x + l, 0.0f, z,      0.0f, 0.0f, -1.0f, l / 1.5f,                0.0f);
	m_pWalls.addVertex1(0, 0, x + l, 0.0f, z,      1.0f, 0.0f,  0.0f, l / 1.5f,                0.0f);
	m_pWalls.addVertex2(0, 0, x + l, h,    z,      1.0f, 0.0f,  0.0f, l / 1.5f,                h / 1.5f);
	m_pWalls.addVertex3(0, 0, x + l, h,    z + w,  1.0f, 0.0f,  0.0f, (l + w) / 1.5f,          h / 1.5f);
	m_pWalls.addVertex4(0, 0, x + l, 0.0f, z + w,  1.0f, 0.0f,  0.0f, (l + w) / 1.5f,          0.0f);
	m_pWalls.addVertex1(0, 0, x + l, 0.0f, z + w,  0.0f, 0.0f,  1.0f, (l + w) / 1.5f,          0.0f);
	m_pWalls.addVertex2(0, 0, x + l, h,    z + w,  0.0f, 0.0f,  1.0f, (l + w) / 1.5f,          h / 1.5f);
	m_pWalls.addVertex3(0, 0, x,     h,    z + w,  0.0f, 0.0f,  1.0f, (l + w + l) / 1.5f,      h / 1.5f);
	m_pWalls.addVertex4(0, 0, x,     0.0f, z + w,  0.0f, 0.0f,  1.0f, (l + w + l) / 1.5f,      0.0f);
	m_pWalls.addVertex1(0, 0, x,     0.0f, z + w, -1.0f, 0.0f,  0.0f, (l + w + l) / 1.5f,      0.0f);
	m_pWalls.addVertex2(0, 0, x,     h,    z + w, -1.0f, 0.0f,  0.0f, (l + w + l) / 1.5f,      h / 1.5f);
	m_pWalls.addVertex3(0, 0, x,     h,    z,     -1.0f, 0.0f,  0.0f, (l + w + l + w) / 1.5f,  h / 1.5f);
	m_pWalls.addVertex4(0, 0, x,     0.0f, z,     -1.0f, 0.0f,  0.0f, (l + w + l + w) / 1.5f,  0.0f);
	m_pWalls.addVertex1(0, 0, x,     h,    z,      0.0f, 1.0f,  0.0f, 0.0f,                    h / 1.5f);
	m_pWalls.addVertex2(0, 0, x,     h,    z + w,  0.0f, 1.0f,  0.0f, 0.0f,                    (h + w) / 1.5f);
	m_pWalls.addVertex3(0, 0, x + l, h,    z + w,  0.0f, 1.0f,  0.0f, l / 1.5f,                (h + w) / 1.5f);
	m_pWalls.addVertex4(0, 0, x + l, h,    z,      0.0f, 1.0f,  0.0f, l / 1.5f,                h / 1.5f);
}


//--------------------------------------------------------------------------------------
// ADD PIT
//--------------------------------------------------------------------------------------
void CMap::addPit(float x, float z, float l, float w, float h)
{
	// Allocate new memory
	int nPits = m_nPits + 1;
	float* m_px0 = new float[nPits];
	float* m_pz0 = new float[nPits];
	float* m_pl0 = new float[nPits];
	float* m_pw0 = new float[nPits];
	float* m_ph0 = new float[nPits];

	// Copy memory
	memcpy(m_px0, m_px, 4 * m_nPits);
	memcpy(m_pz0, m_pz, 4 * m_nPits);
	memcpy(m_pl0, m_pl, 4 * m_nPits);
	memcpy(m_pw0, m_pw, 4 * m_nPits);
	memcpy(m_ph0, m_ph, 4 * m_nPits);

	// Free old memory
	SAFE_DELETE_ARRAY(m_px);
	SAFE_DELETE_ARRAY(m_pz);
	SAFE_DELETE_ARRAY(m_pl);
	SAFE_DELETE_ARRAY(m_pw);
	SAFE_DELETE_ARRAY(m_ph);

	// Set memory
	m_px = m_px0;
	m_pz = m_pz0;
	m_pl = m_pl0;
	m_pw = m_pw0;
	m_ph = m_ph0;

	// Add Pits
	m_px[m_nPits] = x;
	m_pz[m_nPits] = z;
	m_pl[m_nPits] = l;
	m_pw[m_nPits] = w;
	m_ph[m_nPits] = h;
	m_nPits++;
}
