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

#include "Mesh.h"	// Class of Mesh

extern	C3D	g_p3D;	// 3D class


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CMesh::CMesh()
{
	m_nVertex = 0;		// Number of vertexes
	m_nVertexNew = 0;	// Number of vertexes (new, add)
	m_nIndex = 0;		// Number of indexes
	m_nTriangle = 0;	// Number of triangles
	m_nIndexLine = 0;	// Number of indexes for line rendering+

	m_vx = NULL;
	m_vy = NULL;
	m_vz = NULL;
	m_nx = NULL;
	m_ny = NULL;
	m_nz = NULL;
	m_tx = NULL;
	m_ty = NULL;
	m_fVertex = NULL;
	m_x = NULL;
	m_y = NULL;
	m_i = NULL;	// 2 bytes indexes
	m_i4 = NULL;	// 4 bytes indexes
}


//--------------------------------------------------------------------------------------
// DESTRUCTOR
//--------------------------------------------------------------------------------------
CMesh::~CMesh()
{
	SAFE_DELETE_ARRAY(m_vx);
	SAFE_DELETE_ARRAY(m_vy);
	SAFE_DELETE_ARRAY(m_vz);
	SAFE_DELETE_ARRAY(m_nx);
	SAFE_DELETE_ARRAY(m_ny);
	SAFE_DELETE_ARRAY(m_nz);
	SAFE_DELETE_ARRAY(m_tx);
	SAFE_DELETE_ARRAY(m_ty);
	SAFE_DELETE_ARRAY(m_fVertex);
	SAFE_DELETE_ARRAY(m_x);
	SAFE_DELETE_ARRAY(m_y);
	SAFE_DELETE_ARRAY(m_i);
	SAFE_DELETE_ARRAY(m_i4);
	SAFE_DELETE_ARRAY(m_il);
	SAFE_DELETE_ARRAY(m_il4);
}


//--------------------------------------------------------------------------------------
// ADD MEMORY TO VERTEX AND INDEX BUFFERS
//--------------------------------------------------------------------------------------
void CMesh::addMemory(int nVertex, int nIndex)
{
	// Allocate memory
	m_nVertexNew = m_nVertex + nVertex;
	float* m_vx0 = new float[m_nVertexNew];
	float* m_vy0 = new float[m_nVertexNew];
	float* m_vz0 = new float[m_nVertexNew];
	float* m_nx0 = new float[m_nVertexNew];
	float* m_ny0 = new float[m_nVertexNew];
	float* m_nz0 = new float[m_nVertexNew];
	float* m_tx0 = new float[m_nVertexNew];
	float* m_ty0 = new float[m_nVertexNew];
	float* m_fVertex0 = new float[8 * m_nVertexNew];
	int* m_x0 = new int[m_nVertexNew];
	int* m_y0 = new int[m_nVertexNew];

	WORD	*m_i0 = NULL,	*m_il0 = NULL;
	DWORD	*m_i40 = NULL,	*m_il40 = NULL;
	if (m_nVertexNew > 65536)
	{
		m_i40 = new DWORD[m_nIndex + nIndex];
		m_il40 = new DWORD[m_nIndexLine + 2*nIndex];
	}
	else
	{
		m_i0 = new WORD[m_nIndex + nIndex];
		m_il0 = new WORD[m_nIndexLine + 2*nIndex];
	}

	// Copy memory
	memcpy(m_vx0, m_vx, 4 * m_nVertex);
	memcpy(m_vy0, m_vy, 4 * m_nVertex);
	memcpy(m_vz0, m_vz, 4 * m_nVertex);
	memcpy(m_nx0, m_nx, 4 * m_nVertex);
	memcpy(m_ny0, m_ny, 4 * m_nVertex);
	memcpy(m_nz0, m_nz, 4 * m_nVertex);
	memcpy(m_tx0, m_tx, 4 * m_nVertex);
	memcpy(m_ty0, m_ty, 4 * m_nVertex);
	memcpy(m_fVertex0, m_fVertex, 32 * m_nVertex);
	memcpy(m_x0, m_x, 4 * m_nVertex);
	memcpy(m_y0, m_y, 4 * m_nVertex);

	if (m_nVertexNew > 65536)
	{
		if (m_nVertex > 65536)
		{
			memcpy(m_i40, m_i4, 4 * m_nIndex);
			memcpy(m_il40, m_il4, 4 * m_nIndexLine);
		}
		else
		{
			for (int i = 0; i < m_nIndex; i++)
				m_i40[i] = m_i[i];
			for (int i = 0; i < m_nIndexLine; i++)
				m_il40[i] = m_il[i];
		}
	}
	else
	{
		//memcpy(m_i0, m_i, 2 * m_nIndex);
		//memcpy(m_il0, m_il, 2 * m_nIndexLine);
		for (int i = 0; i < m_nIndex; i++)
			m_i0[i] = m_i[i];
		for (int i = 0; i < m_nIndexLine; i++)
			m_il0[i] = m_il[i];
	}

	// Free memory
	SAFE_DELETE_ARRAY(m_vx);
	SAFE_DELETE_ARRAY(m_vy);
	SAFE_DELETE_ARRAY(m_vz);
	SAFE_DELETE_ARRAY(m_nx);
	SAFE_DELETE_ARRAY(m_ny);
	SAFE_DELETE_ARRAY(m_nz);
	SAFE_DELETE_ARRAY(m_tx);
	SAFE_DELETE_ARRAY(m_ty);
	SAFE_DELETE_ARRAY(m_fVertex);
	SAFE_DELETE_ARRAY(m_x);
	SAFE_DELETE_ARRAY(m_y);
	SAFE_DELETE_ARRAY(m_i);
	SAFE_DELETE_ARRAY(m_i4);
	SAFE_DELETE_ARRAY(m_il);
	SAFE_DELETE_ARRAY(m_il4);

	// Set memory
	m_vx = m_vx0;
	m_vy = m_vy0;
	m_vz = m_vz0;
	m_nx = m_nx0;
	m_ny = m_ny0;
	m_nz = m_nz0;
	m_tx = m_tx0;
	m_ty = m_ty0;
	m_fVertex = m_fVertex0;
	m_x = m_x0;
	m_y = m_y0;
	m_i = m_i0;
	m_i4 = m_i40;
	m_il = m_il0;
	m_il4 = m_il40;
}


void CMesh::addVertex1(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vx[m_nVertex] = m_fVertex[8 * m_nVertex] = vx;
	m_vy[m_nVertex] = m_fVertex[8 * m_nVertex + 1] = vy;
	m_vz[m_nVertex] = m_fVertex[8 * m_nVertex + 2] = vz;
	m_nx[m_nVertex] = m_fVertex[8 * m_nVertex + 3] = nx;
	m_ny[m_nVertex] = m_fVertex[8 * m_nVertex + 4] = ny;
	m_nz[m_nVertex] = m_fVertex[8 * m_nVertex + 5] = nz;
	m_tx[m_nVertex] = m_fVertex[8 * m_nVertex + 6] = tx;
	m_ty[m_nVertex] = m_fVertex[8 * m_nVertex + 7] = ty;
	m_x[m_nVertex] = x;
	m_y[m_nVertex] = y;

	/*if (m_nVertexNew > 65536)
	{
		m_i4[m_nIndex++] = m_nVertex;
		m_il4[m_nIndexLine++] = m_nVertex;
	}
	else
	{
		m_i[m_nIndex++] = m_nVertex;
		m_il[m_nIndexLine++] = m_nVertex;
	}*/

	m_nVertex++;	// Added vertex 1
}

void CMesh::addVertex2(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vx[m_nVertex] = m_fVertex[8 * m_nVertex] = vx;
	m_vy[m_nVertex] = m_fVertex[8 * m_nVertex + 1] = vy;
	m_vz[m_nVertex] = m_fVertex[8 * m_nVertex + 2] = vz;
	m_nx[m_nVertex] = m_fVertex[8 * m_nVertex + 3] = nx;
	m_ny[m_nVertex] = m_fVertex[8 * m_nVertex + 4] = ny;
	m_nz[m_nVertex] = m_fVertex[8 * m_nVertex + 5] = nz;
	m_tx[m_nVertex] = m_fVertex[8 * m_nVertex + 6] = tx;
	m_ty[m_nVertex] = m_fVertex[8 * m_nVertex + 7] = ty;
	m_x[m_nVertex] = x;
	m_y[m_nVertex] = y;

	if (m_nVertexNew > 65536)
	{
		m_il4[m_nIndexLine++] = m_nVertex - 1;
		m_il4[m_nIndexLine++] = m_nVertex;
	}
	else
	{
		m_il[m_nIndexLine++] = (WORD)m_nVertex - 1;
		m_il[m_nIndexLine++] = (WORD)m_nVertex;
	}

	m_nVertex++;	// Added vertex 2
}

void CMesh::addVertex3(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vx[m_nVertex] = m_fVertex[8 * m_nVertex] = vx;
	m_vy[m_nVertex] = m_fVertex[8 * m_nVertex + 1] = vy;
	m_vz[m_nVertex] = m_fVertex[8 * m_nVertex + 2] = vz;
	m_nx[m_nVertex] = m_fVertex[8 * m_nVertex + 3] = nx;
	m_ny[m_nVertex] = m_fVertex[8 * m_nVertex + 4] = ny;
	m_nz[m_nVertex] = m_fVertex[8 * m_nVertex + 5] = nz;
	m_tx[m_nVertex] = m_fVertex[8 * m_nVertex + 6] = tx;
	m_ty[m_nVertex] = m_fVertex[8 * m_nVertex + 7] = ty;
	m_x[m_nVertex] = x;
	m_y[m_nVertex] = y;

	if (m_nVertexNew > 65536)
	{
		m_i4[m_nIndex++] = m_nVertex - 2;		// Add triangle
		m_i4[m_nIndex++] = m_nVertex - 1;
		m_i4[m_nIndex++] = m_nVertex;
		m_nTriangle++;
		m_il4[m_nIndexLine++] = m_nVertex - 1;	// Add line
		m_il4[m_nIndexLine++] = m_nVertex;
		m_il4[m_nIndexLine++] = m_nVertex;		// Add line
		m_il4[m_nIndexLine++] = m_nVertex - 2;
	}
	else
	{
		m_i[m_nIndex++] = (WORD)m_nVertex - 2;		// Add triangle
		m_i[m_nIndex++] = (WORD)m_nVertex - 1;
		m_i[m_nIndex++] = (WORD)m_nVertex;
		m_nTriangle++;
		m_il[m_nIndexLine++] = (WORD)m_nVertex - 1;	// Add line
		m_il[m_nIndexLine++] = (WORD)m_nVertex;
		m_il[m_nIndexLine++] = (WORD)m_nVertex;		// Add line
		m_il[m_nIndexLine++] = (WORD)m_nVertex - 2;
	}

	m_nVertex++;	// Added vertex 3
}

void CMesh::addVertex4(int x, int y, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vx[m_nVertex] = m_fVertex[8 * m_nVertex] = vx;
	m_vy[m_nVertex] = m_fVertex[8 * m_nVertex + 1] = vy;
	m_vz[m_nVertex] = m_fVertex[8 * m_nVertex + 2] = vz;
	m_nx[m_nVertex] = m_fVertex[8 * m_nVertex + 3] = nx;
	m_ny[m_nVertex] = m_fVertex[8 * m_nVertex + 4] = ny;
	m_nz[m_nVertex] = m_fVertex[8 * m_nVertex + 5] = nz;
	m_tx[m_nVertex] = m_fVertex[8 * m_nVertex + 6] = tx;
	m_ty[m_nVertex] = m_fVertex[8 * m_nVertex + 7] = ty;
	m_x[m_nVertex] = x;
	m_y[m_nVertex] = y;

	if (m_nVertexNew > 65536)
	{
		m_i4[m_nIndex++] = m_nVertex - 1;		// Add triangle
		m_i4[m_nIndex++] = m_nVertex;
		m_i4[m_nIndex++] = m_nVertex - 3;
		m_nTriangle++;
		m_il4[m_nIndexLine++] = m_nVertex - 1;	// Add line
		m_il4[m_nIndexLine++] = m_nVertex;
		m_il4[m_nIndexLine++] = m_nVertex;		// Add line
		m_il4[m_nIndexLine++] = m_nVertex - 3;
	}
	else
	{
		m_i[m_nIndex++] = (WORD)m_nVertex - 1;		// Add triangle
		m_i[m_nIndex++] = (WORD)m_nVertex;
		m_i[m_nIndex++] = (WORD)m_nVertex - 3;
		m_nTriangle++;
		m_il[m_nIndexLine++] = (WORD)m_nVertex - 1;	// Add line
		m_il[m_nIndexLine++] = (WORD)m_nVertex;
		m_il[m_nIndexLine++] = (WORD)m_nVertex;		// Add line
		m_il[m_nIndexLine++] = (WORD)m_nVertex - 3;
	}

	m_nVertex++;	// Added vertex 3
}


void CMesh::updateVertex(int id, float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vx[id] = m_fVertex[8 * id] = vx;
	m_vy[id] = m_fVertex[8 * id + 1] = vy;
	m_vz[id] = m_fVertex[8 * id + 2] = vz;
	m_nx[id] = m_fVertex[8 * id + 3] = nx;
	m_ny[id] = m_fVertex[8 * id + 4] = ny;
	m_nz[id] = m_fVertex[8 * id + 5] = nz;
	m_tx[id] = m_fVertex[8 * id + 6] = tx;
	m_ty[id] = m_fVertex[8 * id + 7] = ty;
}


void CMesh::updateWall(int id, float x, float z, float l, float w, float h)
{
	int i = 20 * id;
	float ht = h / 1.5f;
	float lt = l / 1.5f;
	updateVertex(i++, x,     0.0f, z,      0.0f, 0.0f, -1.0f, 0.0f, 0.0f);	// Forward
	updateVertex(i++, x,     h,    z,      0.0f, 0.0f, -1.0f, 0.0f, ht);
	updateVertex(i++, x + l, h,    z,      0.0f, 0.0f, -1.0f, lt, ht);
	updateVertex(i++, x + l, 0.0f, z,      0.0f, 0.0f, -1.0f, lt, 0.0f);
	updateVertex(i++, x + l, 0.0f, z,      1.0f, 0.0f,  0.0f, lt, 0.0f);	// Right
	updateVertex(i++, x + l, h,    z,      1.0f, 0.0f,  0.0f, lt, ht);
	lt += w / 1.5f;
	updateVertex(i++, x + l, h,    z + w,  1.0f, 0.0f,  0.0f, lt, ht);
	updateVertex(i++, x + l, 0.0f, z + w,  1.0f, 0.0f,  0.0f, lt, 0.0f);
	updateVertex(i++, x + l, 0.0f, z + w,  0.0f, 0.0f,  1.0f, lt, 0.0f);	// Back
	updateVertex(i++, x + l, h,    z + w,  0.0f, 0.0f,  1.0f, lt, ht);
	lt += l / 1.5f;
	updateVertex(i++, x,     h,    z + w,  0.0f, 0.0f,  1.0f, lt, ht);
	updateVertex(i++, x,     0.0f, z + w,  0.0f, 0.0f,  1.0f, lt, 0.0f);
	updateVertex(i++, x,     0.0f, z + w, -1.0f, 0.0f,  0.0f, lt, 0.0f);	// Left
	updateVertex(i++, x,     h,    z + w, -1.0f, 0.0f,  0.0f, lt, ht);
	lt += w / 1.5f;
	updateVertex(i++, x,     h,    z,     -1.0f, 0.0f,  0.0f, lt, ht);
	updateVertex(i++, x,     0.0f, z,     -1.0f, 0.0f,  0.0f, lt, 0.0f);

	updateVertex(i++, x,     h,    z,      0.0f, 1.0f,  0.0f, 0.0f, ht);	// Up
	updateVertex(i++, x,     h,    z + w,  0.0f, 1.0f,  0.0f, 0.0f, ht + w / 1.5f);
	updateVertex(i++, x + l, h,    z + w,  0.0f, 1.0f,  0.0f, l / 1.5f, ht + w / 1.5f);
	updateVertex(i,   x + l, h,    z,      0.0f, 1.0f,  0.0f, l / 1.5f, ht);
}

void CMesh::updatePit(int id, float x, float z, float l, float w, float h)
{
	int i = 20 * id;
	float ht = h / 1.5f;
	float lt = l / 1.5f;
	updateVertex(i++, x, 0.0f, z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);	// Forward
	updateVertex(i++, x, h, z, 0.0f, 0.0f, -1.0f, 0.0f, ht);
	updateVertex(i++, x + l, h, z, 0.0f, 0.0f, -1.0f, lt, ht);
	updateVertex(i++, x + l, 0.0f, z, 0.0f, 0.0f, -1.0f, lt, 0.0f);
	updateVertex(i++, x + l, 0.0f, z, 1.0f, 0.0f, 0.0f, lt, 0.0f);	// Right
	updateVertex(i++, x + l, h, z, 1.0f, 0.0f, 0.0f, lt, ht);
	lt += w / 1.5f;
	updateVertex(i++, x + l, h, z + w, 1.0f, 0.0f, 0.0f, lt, ht);
	updateVertex(i++, x + l, 0.0f, z + w, 1.0f, 0.0f, 0.0f, lt, 0.0f);
	updateVertex(i++, x + l, 0.0f, z + w, 0.0f, 0.0f, 1.0f, lt, 0.0f);	// Back
	updateVertex(i++, x + l, h, z + w, 0.0f, 0.0f, 1.0f, lt, ht);
	lt += l / 1.5f;
	updateVertex(i++, x, h, z + w, 0.0f, 0.0f, 1.0f, lt, ht);
	updateVertex(i++, x, 0.0f, z + w, 0.0f, 0.0f, 1.0f, lt, 0.0f);
	updateVertex(i++, x, 0.0f, z + w, -1.0f, 0.0f, 0.0f, lt, 0.0f);	// Left
	updateVertex(i++, x, h, z + w, -1.0f, 0.0f, 0.0f, lt, ht);
	lt += w / 1.5f;
	updateVertex(i++, x, h, z, -1.0f, 0.0f, 0.0f, lt, ht);
	updateVertex(i++, x, 0.0f, z, -1.0f, 0.0f, 0.0f, lt, 0.0f);

	updateVertex(i++, x, h, z, 0.0f, 1.0f, 0.0f, 0.0f, ht);	// Up
	updateVertex(i++, x, h, z + w, 0.0f, 1.0f, 0.0f, 0.0f, ht + w / 1.5f);
	updateVertex(i++, x + l, h, z + w, 0.0f, 1.0f, 0.0f, l / 1.5f, ht + w / 1.5f);
	updateVertex(i, x + l, h, z, 0.0f, 1.0f, 0.0f, l / 1.5f, ht);
}


//--------------------------------------------------------------------------------------
// CREATE INDEXES FOR LINE RENDERING
//--------------------------------------------------------------------------------------
void CMesh::createLineIndex()
{
	SAFE_DELETE_ARRAY(m_il);
	SAFE_DELETE_ARRAY(m_il4);
	m_nIndexLine = m_nIndex << 1;
	if (m_nVertex > 65536)
	{
		m_il4 = new DWORD[m_nIndexLine];
		for (int i = 0; i < m_nTriangle; i++)
		{
			m_il4[6 * i]     = m_il4[6 * i + 5] = m_i4[3 * i];
			m_il4[6 * i + 1] = m_il4[6 * i + 2] = m_i4[3 * i + 1];
			m_il4[6 * i + 3] = m_il4[6 * i + 4] = m_i4[3 * i + 2];

			/// ADD optimization on removing repetitive line, made ​​through an intermediate buffer
		}
	}
	else
	{
		m_il = new WORD[m_nIndexLine];
		for (int i = 0; i < m_nTriangle; i++)
		{
			m_il[6 * i]     = m_il[6 * i + 5] = m_i[3 * i];
			m_il[6 * i + 1] = m_il[6 * i + 2] = m_i[3 * i + 1];
			m_il[6 * i + 3] = m_il[6 * i + 4] = m_i[3 * i + 2];

			/// ADD optimization on removing repetitive line, made ​​through an intermediate buffer
		}
	}
}


//--------------------------------------------------------------------------------------
// VERTEX MEMORY RESET
//--------------------------------------------------------------------------------------
void CMesh::resetVertex()
{
}


//--------------------------------------------------------------------------------------
// INDEX MEMORY RESET
//--------------------------------------------------------------------------------------
void CMesh::resetIndex()
{
}


//--------------------------------------------------------------------------------------
// MESH MEMORY RESET
//--------------------------------------------------------------------------------------
void CMesh::resetMesh()
{
	SAFE_DELETE_ARRAY(m_vx);
	SAFE_DELETE_ARRAY(m_vy);
	SAFE_DELETE_ARRAY(m_vz);
	SAFE_DELETE_ARRAY(m_nx);
	SAFE_DELETE_ARRAY(m_ny);
	SAFE_DELETE_ARRAY(m_nz);
	SAFE_DELETE_ARRAY(m_tx);
	SAFE_DELETE_ARRAY(m_ty);
	SAFE_DELETE_ARRAY(m_fVertex);
	SAFE_DELETE_ARRAY(m_x);
	SAFE_DELETE_ARRAY(m_y);
	SAFE_DELETE_ARRAY(m_i);
	SAFE_DELETE_ARRAY(m_i4);
	SAFE_DELETE_ARRAY(m_il);
	SAFE_DELETE_ARRAY(m_il4);
	m_vx = new float[m_nVertex];
	m_vy = new float[m_nVertex];
	m_vz = new float[m_nVertex];
	m_nx = new float[m_nVertex];
	m_ny = new float[m_nVertex];
	m_nz = new float[m_nVertex];
	m_tx = new float[m_nVertex];
	m_ty = new float[m_nVertex];
	m_fVertex = new float[8 * m_nVertex];
	m_x = new int[m_nVertex];
	m_y = new int[m_nVertex];
	if (m_nVertex > 65536)
		m_i4 = new DWORD[m_nIndex];
	else
		m_i = new WORD[m_nIndex];
}


//--------------------------------------------------------------------------------------
// READING MESH FILE (TEXT)
//--------------------------------------------------------------------------------------
void CMesh::read()
{
	// Load Texture from memory
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"test.txtmesh", L"r, ccs=UNICODE"))	// Open a text file for reading
	{
		wchar_t	s[256];
		fgetws(s, 256, pFile);	swscanf_s(s, L"%d", &m_nVertex);	// Number of vertexes
		fgetws(s, 256, pFile);	swscanf_s(s, L"%d", &m_nTriangle);	// Number of triangles
		m_nVertexNew = m_nVertex;
		m_nIndex = 3 * m_nTriangle;
		resetMesh();	// Mesh memory reset

		// Reading vertex buffer
		for (int i = 0; i < m_nVertex; i++)
		{
			fgetws(s, 256, pFile);
			swscanf_s(s, L"%f	%f	%f	%f	%f	%f	%f	%f", &m_vx[i], &m_vy[i], &m_vz[i], &m_nx[i], &m_ny[i], &m_nz[i], &m_tx[i], &m_ty[i]);
			m_fVertex[8 * i] = m_vx[i];
			m_fVertex[8 * i + 1] = m_vy[i];
			m_fVertex[8 * i + 2] = m_vz[i];
			m_fVertex[8 * i + 3] = m_nx[i];
			m_fVertex[8 * i + 4] = m_ny[i];
			m_fVertex[8 * i + 5] = m_nz[i];
			m_fVertex[8 * i + 6] = m_tx[i];
			m_fVertex[8 * i + 7] = m_ty[i];
			//getVertexScreenCoord
			g_p3D.getScreenCoordFrom3DPoint(m_vx[i], m_vy[i], m_vz[i], &m_x[i], &m_y[i]);
		}

		// Reading index buffer
		if (m_nVertex > 65536)	// 4 bytes index buffer
			for (int i = 0; i < m_nTriangle; i++)
			{
				fgetws(s, 256, pFile);
				swscanf_s(s, L"%d	%d	%d", &m_i4[3 * i], &m_i4[3 * i + 1], &m_i4[3 * i + 2]);
			}
		else	// 2 bytes index buffer
			for (int i = 0; i < m_nTriangle; i++)
			{
				fgetws(s, 256, pFile);
				int a, b, c;
				swscanf_s(s, L"%i	%i	%i", &a, &b, &c);
				m_i[3 * i] = (WORD)a;
				m_i[3 * i + 1] = (WORD)b;
				m_i[3 * i + 2] = (WORD)c;
			}

		fclose(pFile);	// Cllose file
	}
}

//--------------------------------------------------------------------------------------
// READING MESH FILE (BINARY)
//--------------------------------------------------------------------------------------
void CMesh::readb()
{
	// Load Texture from memory
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"test.mesh", L"rb"))	// Open a binary file for reading
	{
		//fseek(pFile, 0, SEEK_END);	// Move the pointer to the end of the file
		//SIZE_T size = ftell(pFile);	// Get the file size
		//fseek(pFile, 0, SEEK_SET);	// Move the pointer to the beginning of the file
		//m_nVertex = size / 32;
		fread(&m_nVertex, 1, 4, pFile);	// Read number of vertex
		fread(&m_nTriangle, 1, 4, pFile);	// Read number of index
		m_nVertexNew = m_nVertex;
		m_nIndex = 3 * m_nTriangle;
		resetMesh();	// Mesh memory reset

		// Reading vertex buffer
		fread(m_vx, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_vy, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_vz, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_nx, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_ny, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_nz, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_tx, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		fread(m_ty, 1, 4 * m_nVertex, pFile);	// Copy file to memory
		for (int i = 0; i < m_nVertex; i++)
		{
			m_fVertex[8 * i] = m_vx[i];
			m_fVertex[8 * i + 1] = m_vy[i];
			m_fVertex[8 * i + 2] = m_vz[i];
			m_fVertex[8 * i + 3] = m_nx[i];
			m_fVertex[8 * i + 4] = m_ny[i];
			m_fVertex[8 * i + 5] = m_nz[i];
			m_fVertex[8 * i + 6] = m_tx[i];
			m_fVertex[8 * i + 7] = m_ty[i];
			g_p3D.getScreenCoordFrom3DPoint(m_vx[i], m_vy[i], m_vz[i], &m_x[i], &m_y[i]);
		}

		// Reading index buffer
		if (m_nVertex > 65536)	// 4 bytes index buffer
			fread(m_i4, 1, 4 * m_nIndex, pFile);
		else	// 2 bytes index buffer
			fread(m_i, 1, 2 * m_nIndex, pFile);

		fclose(pFile);	// Cllose file
	}
}


//--------------------------------------------------------------------------------------
// WRITING MESH FILE (TEXT)
//--------------------------------------------------------------------------------------
void CMesh::write()
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"test.txtmesh", L"w, ccs=UNICODE"))	// Open a text file for reading
	{
		fwprintf(pFile, L"%d	// Number of vertex\n", m_nVertex);	// Number of vertexes
		fwprintf(pFile, L"%d	// Number of triangles\n", m_nTriangle);	// Number of triangles
		for (int i = 0; i < m_nVertex; i++)	// Vertex buffer
			fwprintf(pFile, L"%f	%f	%f	%f	%f	%f	%f	%f\n", m_vx[i], m_vy[i], m_vz[i], m_nx[i], m_ny[i], m_nz[i], m_tx[i], m_ty[i]);
		if (m_nVertex > 65536)
			for (int i = 0; i < m_nTriangle; i++)	// 4 bytes index buffer
				fwprintf(pFile, L"%d	%d	%d\n", m_i4[3*i], m_i4[3*i+1], m_i4[3*i+2]);
		else
			for (int i = 0; i < m_nTriangle; i++)	// 2 bytes index buffer
				fwprintf(pFile, L"%d	%d	%d\n", m_i[3*i], m_i[3*i+1], m_i[3*i+2]);

		fclose(pFile);	// Close file
	}
}


//--------------------------------------------------------------------------------------
// WRITING MESH FILE (BINARY)
//--------------------------------------------------------------------------------------
void CMesh::writeb()
{
	FILE* pFile;
	if (NULL == _wfopen_s(&pFile, L"test.mesh", L"wb"))	// Open a text file for writing
	{
		fwrite(&m_nVertex, 1, 4, pFile);	// Number of vertexes
		fwrite(&m_nTriangle, 1, 4, pFile);	// Number of triangles
		fwrite(m_vx, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_vy, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_vz, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_nx, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_ny, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_nz, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_tx, 1, 4 * m_nVertex, pFile);	// 
		fwrite(m_ty, 1, 4 * m_nVertex, pFile);	// З
		if (m_nVertex > 65536)	// 4 bytes index buffer
			fwrite(m_i4, 1, 4 * m_nIndex, pFile);
		else	// 2 bytes index buffer
			fwrite(m_i, 1, 2 * m_nIndex, pFile);
		fclose(pFile);	// Close file
	}
}


//--------------------------------------------------------------------------------------
// SET MESH
//--------------------------------------------------------------------------------------
void CMesh::setMesh(float* fVertex, int nVertex, WORD* iIndex, int nIndex)
{
	m_nVertexNew = m_nVertex = nVertex;
	resetMesh();	// Mesh memory reset
	memcpy(m_fVertex, fVertex, 32 * nVertex);
	for (int i = 0; i < nVertex; i++)
	{
		m_vx[i] = fVertex[8 * i];
		m_vy[i] = fVertex[8 * i + 1];
		m_vz[i] = fVertex[8 * i + 2];
		m_nx[i] = fVertex[8 * i + 3];
		m_ny[i] = fVertex[8 * i + 4];
		m_nz[i] = fVertex[8 * i + 5];
		m_tx[i] = fVertex[8 * i + 6];
		m_ty[i] = fVertex[8 * i + 7];
		g_p3D.getScreenCoordFrom3DPoint(m_vx[i], m_vy[i], m_vz[i], &m_x[i], &m_y[i]);
	}

	m_nIndex = nIndex;
	m_nTriangle = nIndex / 3;
	SAFE_DELETE_ARRAY(m_i);
	m_i = new WORD[nIndex];
	memcpy(m_i, iIndex, 2 * nIndex);
}

void CMesh::setMesh4(float* fVertex, int nVertex, DWORD* iIndex, int nIndex)
{
	m_nVertexNew = m_nVertex = nVertex;
	resetMesh();	// Mesh memory reset
	memcpy(m_fVertex, fVertex, 32 * nVertex);
	for (int i = 0; i < nVertex; i++)
	{
		m_vx[i] = fVertex[8 * i];
		m_vy[i] = fVertex[8 * i + 1];
		m_vz[i] = fVertex[8 * i + 2];
		m_nx[i] = fVertex[8 * i + 3];
		m_ny[i] = fVertex[8 * i + 4];
		m_nz[i] = fVertex[8 * i + 5];
		m_tx[i] = fVertex[8 * i + 6];
		m_ty[i] = fVertex[8 * i + 7];
	}

	m_nIndex = nIndex;
	m_nTriangle = nIndex / 3;
	SAFE_DELETE_ARRAY(m_i4);
	m_i4 = new DWORD[nIndex];
	memcpy(m_i4, iIndex, 4 * nIndex);
}


//--------------------------------------------------------------------------------------
// SET VERTEX BUFFER FOR MESH
//--------------------------------------------------------------------------------------
void CMesh::setVertex(float* fVertex, int nVertex)
{
	m_nVertexNew = m_nVertex = nVertex;
	SAFE_DELETE_ARRAY(m_fVertex);
	m_fVertex = new float[8 * nVertex];
	memcpy(m_fVertex, fVertex, 32 * nVertex);
}


//--------------------------------------------------------------------------------------
// SET 2 BYTES INDEX BUFFER FOR MESH
//--------------------------------------------------------------------------------------
void CMesh::setIndex(WORD* iIndex, int nIndex)
{
	m_nIndex = nIndex;
	m_nTriangle = nIndex / 3;
	SAFE_DELETE_ARRAY(m_i);
	m_i = new WORD[nIndex];
	memcpy(m_i, iIndex, 2 * nIndex);
}


//--------------------------------------------------------------------------------------
// SET 4 BYTES INDEX BUFFER FOR MESH
//--------------------------------------------------------------------------------------
void CMesh::setIndex4(DWORD* iIndex, int nIndex)
{
	m_nIndex = nIndex;
	m_nTriangle = nIndex / 3;
	SAFE_DELETE_ARRAY(m_i4);
	m_i4 = new DWORD[nIndex];
	memcpy(m_i4, iIndex, 4 * nIndex);
}


//--------------------------------------------------------------------------------------
// FIND OTHER INDEXES OF VERTEXES
//--------------------------------------------------------------------------------------
void CMesh::findTriangle(int iV0, int* iV1, int* iV2)	// Find other indexes of vertexes
{
	if (m_nVertex > 65536)	// 4 bytes index buffer
	{
		for (int i = 0; i < m_nTriangle; i++)	// All indexes
		{
			if (m_i4[3 * i] == (DWORD)iV0)
			{
				*iV1 = m_i4[3 * i + 1];
				*iV2 = m_i4[3 * i + 2];
				break;
			}
			if (m_i4[3 * i + 1] == (DWORD)iV0)
			{
				*iV1 = m_i4[3 * i + 2];
				*iV2 = m_i4[3 * i];
				break;
			}
			if (m_i4[3 * i + 2] == (DWORD)iV0)
			{
				*iV1 = m_i4[3 * i];
				*iV2 = m_i4[3 * i + 1];
				break;
			}
		}
	}
	else	// 2 bytes index buffer
	{
		for (int i = 0; i < m_nTriangle; i++)	// All indexes
		{
			if (m_i[3 * i] == (WORD)iV0)
			{
				*iV1 = m_i[3 * i + 1];
				*iV2 = m_i[3 * i + 2];
				break;
			}
			if (m_i[3 * i + 1] == (WORD)iV0)
			{
				*iV1 = m_i[3 * i + 2];
				*iV2 = m_i[3 * i];
				break;
			}
			if (m_i[3 * i + 2] == (WORD)iV0)
			{
				*iV1 = m_i[3 * i];
				*iV2 = m_i[3 * i + 1];
				break;
			}
		}
	}
}