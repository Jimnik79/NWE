//--------------------------------------------------------------------------------------
// File: Map.h
// Description: Class of work with map files.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>

#include "Mesh.h"


class CMap
{
protected:

	float	m_lenght;
	float	m_width;
	int		m_nWalls;
	int		m_nPits;

	float*	m_wx;	// Walls, X position
	float*	m_wz;	// Walls, Z position
	float*	m_wl;	// Walls, lenght (X)
	float*	m_ww;	// Walls, width (Z)
	float*	m_wh;	// Walls, height (Y)

	float*	m_px;	// Pits, X position
	float*	m_pz;	// Pits, Z position
	float*	m_pl;	// Pits, lenght (X)
	float*	m_pw;	// Pits, width (Z)
	float*	m_ph;	// Pits, height (Y)

	//int m_nVertex;

	CMesh	m_pGround;	// Mesh class for ground
	CMesh	m_pWalls;	// Mesh class for walls
	CMesh	m_pPits;	// Mesh class for pits

public:
	CMap();	// Constructor
	~CMap();	// Destructor

	void	free();		// Free memory
	void	reset();	// Reset

	void	setStandartMap();	// Set Standart Map

	void	setGround();	// Set ground

	void	load(LPCWSTR sFile);	// Loading map file (text)
	void	loadb(LPCWSTR sFile);	// Loading map file (binary)
	void	save(LPCWSTR sFile);	// Saving map file (text)
	void	saveb(LPCWSTR sFile);	// Saving map file (binary)

	void	loadPhysics();	// Load physics

	int		getNWalls()	{ return m_nWalls; }	// Get number of walls
	int		getNPits()	{ return m_nPits; }		// Get number of pits
	void	addMemoryForWalls();
	void	addWall(float x, float z, float l, float w, float h);	// Add wall
	void	addPit(float x, float z, float l, float w, float h);	// Add pit

	void	setLenght(float lenght)	{ m_lenght = lenght; }	// Set map lenght
	void	setWidth(float width)	{ m_width = width; }	// Set map width
	float	getLenght()	{ return m_lenght; }	// Get map lenght
	float	getWidth()	{ return m_width; }		// Get map width

	float	getWX(int id)	{ return m_wx[id]; }	// Get X position for wall
	float	getWZ(int id)	{ return m_wz[id]; }	// Get Z position for wall
	float	getWL(int id)	{ return m_wl[id]; }	// Get lenght (X) for wall
	float	getWW(int id)	{ return m_ww[id]; }	// Get width (Z) for wall
	float	getWH(int id)	{ return m_wh[id]; }	// Get height (Y) for wall
	float	getPX(int id)	{ return m_px[id]; }	// Get X position for pit
	float	getPZ(int id)	{ return m_pz[id]; }	// Get Z position for pit
	float	getPL(int id)	{ return m_pl[id]; }	// Get lenght (X) for pit
	float	getPW(int id)	{ return m_pw[id]; }	// Get width (Z) for pit
	float	getPH(int id)	{ return m_ph[id]; }	// Get height (Y) for pit

	void	setWX(int id, float x)	{ m_wx[id] = x; }	// Set X position for wall
	void	setWZ(int id, float z)	{ m_wz[id] = z; }	// Set Z position for wall
	void	setWL(int id, float l)	{ m_wl[id] = l; }	// Set lenght (X) for wall
	void	setWW(int id, float w)	{ m_ww[id] = w; }	// Set width (Z) for wall
	void	setWH(int id, float h)	{ m_wh[id] = h; }	// Set height (Y) for wall
	void	setPX(int id, float x)	{ m_px[id] = x; }	// Set X position for pit
	void	setPZ(int id, float z)	{ m_pz[id] = z; }	// Set Z position for pit
	void	setPL(int id, float l)	{ m_pl[id] = l; }	// Set lenght (X) for pit
	void	setPW(int id, float w)	{ m_pw[id] = w; }	// Set width (Z) for pit
	void	setPH(int id, float h)	{ m_ph[id] = h; }	// Set height (Y) for pit

	int		getNVertexGround()	{ return m_pGround.getNVertex(); }	// Get number of vertexes of Ground
	int		getNVertexWalls()	{ return m_pWalls.getNVertex(); }	// Get number of vertexes of Walls

	int		getNIndexGround()	{ return m_pGround.getNIndex(); }	// Get number of indexes of Ground
	int		getNIndexWalls()	{ return m_pWalls.getNIndex(); }	// Get number of indexes of Walls
	int		getNIndexLineWalls(){ return m_pWalls.getNIndexLine(); }// Get number of indexes of Walls

	float*	getVertexGround()	{ return m_pGround.getVertex(); }	// Get vertexes of Ground
	float*	getVertexWalls()	{ return m_pWalls.getVertex(); }	// Get vertexes of Walls

	WORD*	getIndexGround()	{ return m_pGround.getIndex(); }	// Get indexes of Ground
	WORD*	getIndexWalls()		{ return m_pWalls.getIndex(); }		// Get indexes of Walls (2 bytes)
	DWORD*	getIndex4Walls()	{ return m_pWalls.getIndex4(); }	// Get indexes of Walls (4 bytes)

	WORD*	getIndexLineGround(){ return m_pGround.getIndexLine(); }// Get line indexes of Ground
	WORD*	getIndexLineWalls()	{ return m_pWalls.getIndexLine(); }	// Get line indexes of Walls (2 bytes)
	DWORD*	getIndexLine4Walls(){ return m_pWalls.getIndexLine4(); }// Get line indexes of Walls (4 bytes)

	void	updateWall(int id, float x, float z, float l, float w, float h)	{ m_pWalls.updateWall(id, m_wx[id] = x, m_wz[id] = z, m_wl[id] = l, m_ww[id] = w, m_wh[id] = h); }
	void	updatePit(int id, float x, float z, float l, float w, float h)	{ m_pPits.updatePit(id, m_wx[id] = x, m_wz[id] = z, m_wl[id] = l, m_ww[id] = w, m_wh[id] = h); }

};