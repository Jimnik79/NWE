//--------------------------------------------------------------------------------------
// File: Effect.h
// Description: Class for effect.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


class CEffect
{
protected:

	ID3D11VertexShader*	m_pVS;		// Vertex shader for rendering scene
	ID3D11PixelShader*	m_pPS;		// Pixel shader for rendering scene
	ID3D11PixelShader*	m_pAS;		// Pixel shader for texture with an Alpha channel (elements highlight)
	ID3D11PixelShader*	m_pTS;		// Pixel shader for texture with partial Transparency (figure)

	ID3D11InputLayout*	m_pVL;		// Vertex layout for meshes

	ID3D11Buffer*		m_pcbVO;	// Buffer with parameters of the matrices
	ID3D11Buffer*		m_pcbPF;	// Buffer with the parameters of the light
	ID3D11Buffer*		m_pcbPO;	// Buffer with the parameter of the object's color (transparency)

	ID3D11SamplerState*	m_pAnisot;	// Filter type - ANISOTROPIC

	UINT	m_iCBVSPerObjectBind;	// In the shader, register for the matrices 
	UINT	m_iCBPSPerFrameBind;	// In the shader, register for the light
	UINT	m_iCBPSPerObjectBind;	// In the shader, register for the object's color (transparency)

public:
	CEffect();	// Constructor

	void	init(ID3D11Device* pD3D);	// Initialization
	HRESULT	resize(ID3D11Device* pD3D, float fW, float fH);	// Resize the output scene
	void	render(ID3D11DeviceContext* pContext, float fTime);	// Rendering scene
	void	free();	// Deinitialization
};