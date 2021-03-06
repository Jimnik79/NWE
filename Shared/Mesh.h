//--------------------------------------------------------------------------------------
// File: Mesh.h
// Description: Class of work with mesh files.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


class CMesh
{
protected:

	int m_nVertex;
	int m_nVertexNew;
	int m_nIndex;
	int m_nTriangle;
	int m_nIndexLine;
	float*	m_vx;
	float*	m_vy;
	float*	m_vz;
	float*	m_nx;
	float*	m_ny;
	float*	m_nz;
	float*	m_tx;
	float*	m_ty;
	float*	m_fVertex;
	int*	m_x;		// X screen coordinate
	int*	m_y;		// Y screen coordinate
	WORD*	m_i;		// 2 bytes indexes
	DWORD*	m_i4;		// 4 bytes indexes
	WORD*	m_il;		// 2 bytes indexes for line rendering
	DWORD*	m_il4;		// 4 bytes indexes for line rendering

public:
	CMesh();	// Constructor
	~CMesh();	// Destructor

	void	createLineIndex();
	void	resetVertex();	// Vertex memory reset
	void	resetIndex();	// Index memory reset
	void	resetMesh();	// Mesh memory reset
	void	read();		// Reading mesh file (text)
	void	readb();	// Reading mesh file (binary)
	void	write();	// Writing mesh file (text)
	void	writeb();	// Writing mesh file (binary)
	void	setMesh(float* fVertex, int nVertex, WORD* iIndex, int nIndex);	// 
	void	setMesh4(float* fVertex, int nVertex, DWORD* iIndex, int nIndex);	// 
	void	setVertex(float* fVertex, int nVertex);	// Set vertex buffer for mesh
	void	setIndex(WORD* iIndex, int nIndex);		// Set 2 bytes index buffer for mesh
	void	setIndex4(DWORD* iIndex, int nIndex);	// Set 4 bytes index buffer for mesh
	void	setX(int i, int x)	{ m_x[i] = x; }		// Set X screen coordinate
	void	setY(int i, int y)	{ m_y[i] = y; }		// Set Y screen coordinate

	void	findTriangle(int iV0, int* iV1, int* iV2);	// Find other indexes of vertexes

	void	addMemory(int nVertex, int nIndex);	// Add memory to vertex and index buffers
	void	addVertex1(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void	addVertex2(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void	addVertex3(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void	addVertex4(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void	updateVertex(int id, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void	updateWall(int id, float x, float z, float l, float w, float h);
	void	updatePit(int id, float x, float z, float l, float w, float h);

	int		getNVertex()	{ return m_nVertex; }	// Get number of vertexes
	int		getNVertexNew()	{ return m_nVertexNew ? m_nVertexNew : m_nVertex; }// Get number of vertexes (new)
	int		getNIndex()		{ return m_nIndex; }	// Get number of indexes
	int		getNTriangle()	{ return m_nTriangle; }	// Get number of triangles
	int		getNIndexLine()	{ return m_nIndexLine; }// Get number of indexes for line randering
	float*	getVertex()		{ return m_fVertex; }	// Get vertexes
	float	getVX(int id)	{ return m_vx[id]; }	// Get X position
	float	getVY(int id)	{ return m_vy[id]; }	// Get Y position
	float	getVZ(int id)	{ return m_vz[id]; }	// Get Z position
	float	getNX(int id)	{ return m_nx[id]; }	// Get X normal
	float	getNY(int id)	{ return m_ny[id]; }	// Get Y normal
	float	getNZ(int id)	{ return m_nz[id]; }	// Get Z normal
	float	getTX(int id)	{ return m_tx[id]; }	// Get X texture coordinate
	float	getTY(int id)	{ return m_ty[id]; }	// Get Y texture coordinate
	float*	getVX()	{ return m_vx; }	// Get X positions
	float*	getVY()	{ return m_vy; }	// Get Y positions
	float*	getVZ()	{ return m_vz; }	// Get Z positions

	void	setVX(int id, float vx)	{ m_vx[id] = m_fVertex[8*id  ] = vx; }	// Set X position
	void	setVY(int id, float vy)	{ m_vy[id] = m_fVertex[8*id+1] = vy; }	// Set Y position
	void	setVZ(int id, float vz)	{ m_vz[id] = m_fVertex[8*id+2] = vz; }	// Set Z position
	void	setNX(int id, float nx)	{ m_nx[id] = m_fVertex[8*id+3] = nx; }	// Set X normal
	void	setNY(int id, float ny)	{ m_ny[id] = m_fVertex[8*id+4] = ny; }	// Set Y normal
	void	setNZ(int id, float nz)	{ m_nz[id] = m_fVertex[8*id+5] = nz; }	// Set Z normal
	void	setTX(int id, float tx)	{ m_tx[id] = m_fVertex[8*id+6] = tx; }	// Set X texture coordinate
	void	setTY(int id, float ty)	{ m_ty[id] = m_fVertex[8*id+7] = ty; }	// Set Y texture coordinate

	WORD*	getIndex()		{ return m_i; }		// Get 2 bytes indexes
	DWORD*	getIndex4()		{ return m_i4; }	// Get 4 bytes indexes
	WORD*	getIndexLine()	{ return m_il; }	// Get 2 bytes indexes for line rendering
	DWORD*	getIndexLine4()	{ return m_il4; }	// Get 4 bytes indexes for line rendering
	int		getX(int i)		{ return m_x[i]; }	// Get X screen coordinate
	int		getY(int i)		{ return m_y[i]; }	// Get Y screen coordinate
};