//--------------------------------------------------------------------------------------
// File: Effect.cpp
// Description: Class for effect.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include "..\Engine\NetWars.h"	// NetWars Engine
#include "D3Dcompiler.h"
#include "D3DX10Math.h"

#include "Effect.h"	// Class of Effect


CHAR sEffect[] = \
"cbuffer cbPerObject : register(b1)"\
"{	matrix mWVP;"\
"	matrix mWVPLight;"\
"	matrix worldMatrix;"\
"};"\
""\
"cbuffer cbPerFrame : register(b2)"\
"{	float3	vLight : packoffset(c0);"\
"};"\
""\
"cbuffer cbPerObject : register(b3)"\
"{	float	alpha;"\
"	float3	padding;"\
"};"\
""\
"Texture2D		txDiffuse : register(t0);"\
"SamplerState	samAnisot : register(s0);"\
""\
"struct VS_INPUT"\
"{	float4	vPos : POSITION;"\
"	float3	vNor : NORMAL;"\
"	float2	vTex : TEXCOORD0;"\
"};"\
""\
"struct VS_OUTPUT"\
"{	float3	vNor : NORMAL;"\
"	float2	vTex : TEXCOORD0;"\
"	float4	vPos : SV_POSITION;"\
"};"\
""\
"struct PS_INPUT"\
"{	float3	vNor : NORMAL;"\
"	float2	vTex : TEXCOORD0;"\
"};"\
""\
"VS_OUTPUT VS(VS_INPUT Input)"\
"{	VS_OUTPUT Output;"\
"	Output.vPos = mul(Input.vPos, mWVP);"\
"	Output.vNor = Input.vNor;"\
"	Output.vTex = Input.vTex;"\
"	return Output;"\
"};"\
""\
"float4 PS(PS_INPUT Input) : SV_TARGET"\
"{	if (alpha<1.0f)"\
"	{"\
"		float4 vDiffuse = 0.5f*txDiffuse.Sample(samAnisot, Input.vTex)*(1.0f + saturate(dot(vLight, Input.vNor)));"\
"		vDiffuse.a = alpha;"\
"		return vDiffuse;"\
"	}"\
"	float4 vDiffuse = txDiffuse.Sample(samAnisot, Input.vTex);"\
"	float fAlpha = vDiffuse.a;"\
"	vDiffuse *= 0.5f*(1.0f + saturate(dot(vLight, Input.vNor)));"\
"	vDiffuse.a = fAlpha;"\
"	return vDiffuse;"\
"}"\
""\
"float4 AS(PS_INPUT Input) : SV_TARGET"\
"{	float4 vDiffuse = txDiffuse.Sample(samAnisot, Input.vTex);"\
"	float fAlpha = vDiffuse.a;"\
"	vDiffuse *= 0.5f*(1.0f + saturate(dot(vLight, Input.vNor)));"\
"	vDiffuse.a = fAlpha;"\
"	return vDiffuse;"\
"}"\
""\
"float4 TS(PS_INPUT Input) : SV_TARGET"\
"{	float4 vDiffuse = 0.5f*txDiffuse.Sample(samAnisot, Input.vTex)*(1.0f + saturate(dot(vLight, Input.vNor)));"\
"	vDiffuse.a = alpha;"\
"	return vDiffuse;"\
"}";
const UINT uEffect = sizeof(sEffect);

struct CB4VS_PER_OBJECT	{ D3DXMATRIX mWVP; };					// World-View-Project Matrix
struct CB4PS_PER_FRAME	{ D3DXVECTOR4 lightDirection; };		// Light direction
struct CB4PS_PER_OBJECT	{ float alpha; D3DXVECTOR3 padding; };	// Transparency


extern	CEngine	g_pAppl;	// Base Engine class
extern	CTex	g_pTex;		// Texture class
extern	C3D		g_p3D;		// 3D class
extern	CCam	g_pCam;		// Camera class


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CEffect::CEffect()
{
	m_pVS = NULL;		// Vertex shader for rendering scene
	m_pPS = NULL;		// Pixel shader for rendering scene
	m_pAS = NULL;		// Pixel shader for texture with an Alpha channel (elements highlight)
	m_pTS = NULL;		// Pixel shader for texture with partial Transparency (figure)

	m_pVL = NULL;		// Vertex layout for meshes

	m_pcbVO = NULL;		// Buffer with parameters of the matrices
	m_pcbPF = NULL;		// Buffer with the parameters of the light
	m_pcbPO = NULL;		// Buffer with the parameter of the object's color (transparency)

	m_pAnisot = NULL;	// Filter type - ANISOTROPIC

	m_iCBVSPerObjectBind = 1;	// In the shader, register for the matrices
	m_iCBPSPerFrameBind = 2;	// In the shader, register for the light
	m_iCBPSPerObjectBind = 3;	// In the shader, register for the object's color (transparency)
}


//--------------------------------------------------------------------------------------
// INITIALIZATION
//--------------------------------------------------------------------------------------
void CEffect::init(ID3D11Device* pD3D)
{
	char	sVS[20] = "vs_5_0";	// Version of vertex shader
	char	sPS[20] = "ps_5_0";	// Version of pixel shader

	ID3DBlob* pVSB = NULL;	// VertexShaderBuffer
	ID3DBlob* pPSB = NULL;	// PixelShaderBuffer, texture with no Alpha channel
	ID3DBlob* pVRB = NULL;	// VertexShaderBufferReflection
	ID3DBlob* pVDB = NULL;	// VertexShaderBufferDepth
	ID3DBlob* pPRB = NULL;	// PixelShaderBufferReflection
	ID3DBlob* pASB = NULL;	// AlphaShaderBuffer, texture with no Alpha channel (elements highlight)
	ID3DBlob* pTSB = NULL;	// TextureShaderBuffer, texture with partial Transparency (figure)

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows the shaders to be optimized and to run exactly the way they will run in the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	HRESULT hr;
	V(D3DCompile(sEffect, uEffect, "none", NULL, NULL, "VS", sVS, dwShaderFlags, 0, &pVSB, NULL));
	V(D3DCompile(sEffect, uEffect, "none", NULL, NULL, "PS", sPS, dwShaderFlags, 0, &pPSB, NULL));
	V(D3DCompile(sEffect, uEffect, "none", NULL, NULL, "AS", sPS, dwShaderFlags, 0, &pASB, NULL));
	V(D3DCompile(sEffect, uEffect, "none", NULL, NULL, "TS", sPS, dwShaderFlags, 0, &pTSB, NULL));

	// Create shaders
	V(pD3D->CreateVertexShader(pVSB->GetBufferPointer(), pVSB->GetBufferSize(), NULL, &m_pVS));
	V(pD3D->CreatePixelShader(pPSB->GetBufferPointer(), pPSB->GetBufferSize(), NULL, &m_pPS));
	V(pD3D->CreatePixelShader(pASB->GetBufferPointer(), pASB->GetBufferSize(), NULL, &m_pAS));
	V(pD3D->CreatePixelShader(pTSB->GetBufferPointer(), pTSB->GetBufferSize(), NULL, &m_pTS));

	// Create vertex layout for meshes
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	V(pD3D->CreateInputLayout(layout, ARRAYSIZE(layout), pVSB->GetBufferPointer(), pVSB->GetBufferSize(), &m_pVL));

	// Free shader buffers
	SAFE_RELEASE(pTSB);	
	SAFE_RELEASE(pASB);
	SAFE_RELEASE(pVDB);
	SAFE_RELEASE(pPRB);
	SAFE_RELEASE(pVRB);
	SAFE_RELEASE(pPSB);
	SAFE_RELEASE(pVSB);

	// Configure the buffers constants passed to the shader
	D3D11_BUFFER_DESC Desc;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.ByteWidth = sizeof(CB4VS_PER_OBJECT);	V(pD3D->CreateBuffer(&Desc, NULL, &m_pcbVO));	// Each object for vertex shader buffer
	Desc.ByteWidth = sizeof(CB4PS_PER_FRAME);	V(pD3D->CreateBuffer(&Desc, NULL, &m_pcbPF));	// Each frame for pixel shaderbuffer
	Desc.ByteWidth = sizeof(CB4PS_PER_OBJECT);	V(pD3D->CreateBuffer(&Desc, NULL, &m_pcbPO));	// Each object for pixel shader buffer

	// Create an anisotropic filter
	D3D11_SAMPLER_DESC SSDesc;
	ZeroMemory(&SSDesc, sizeof(D3D11_SAMPLER_DESC));
	SSDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SSDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	//D3D11_COMPARISON_ALWAYS;
	SSDesc.MaxAnisotropy = 16;
	SSDesc.MinLOD = 0;
	SSDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (pD3D->GetFeatureLevel() < D3D_FEATURE_LEVEL_9_3)
	{
		SSDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SSDesc.MaxAnisotropy = 0;
	}
	V(pD3D->CreateSamplerState(&SSDesc, &m_pAnisot));
}


//--------------------------------------------------------------------------------------
// RESIZE THE OUTPUT SCENE
//--------------------------------------------------------------------------------------
/*HRESULT	CEffect::resize(ID3D11Device* pD3D, float fW, float fH)	
{
	//m_fW = fW;	// Width
	//m_fH = fH;	// Height

	return S_OK;
}*/


//--------------------------------------------------------------------------------------
// RENDERING SCENE
//--------------------------------------------------------------------------------------
void CEffect::render(ID3D11DeviceContext* pContext, float fAlpha)	// D3DXMATRIX mVP,
{
	HRESULT	hr;
	D3DXMATRIX	mVP;	// Matrix View Projection
	g_pCam.getMatrixViewProj(&mVP);	// Matrix View Projection
	D3DXMATRIX	mWVP;	// Matrix World View Projection for rendering object
	D3DXMATRIX	mWorld;	// Matrix World

	// Remember current render state
	ID3D11SamplerState*	pSamplers = NULL;	pContext->PSGetSamplers(0, 1, &pSamplers);
	ID3D11InputLayout* pInputLayout = NULL;	pContext->IAGetInputLayout(&pInputLayout);
	ID3D11VertexShader* pVS = NULL;			pContext->VSGetShader(&pVS, NULL, 0);
	ID3D11PixelShader* pPS = NULL;			pContext->PSGetShader(&pPS, NULL, 0);
	ID3D11GeometryShader* pGS = NULL;		pContext->GSGetShader(&pGS, NULL, 0);
	ID3D11HullShader* pHS = NULL;			pContext->HSGetShader(&pHS, NULL, 0);
	ID3D11DomainShader* pDS = NULL;			pContext->DSGetShader(&pDS, NULL, 0);

	//pContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pAppl.clearDepthStencilView();

	// Set shaders and rendering state
	pContext->PSSetSamplers(0, 1, &m_pAnisot);	// Anisotropic filter
	pContext->IASetInputLayout(m_pVL);		// Layout
	pContext->VSSetShader(m_pVS, NULL, 0);	// Vertex shader
	pContext->PSSetShader(m_pPS, NULL, 0);	// Pixel shader for rendering texture with no alpha channel

	// PS The light direction (set each frame on the whole scene)
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	V(pContext->Map(m_pcbPF, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB4PS_PER_FRAME* pPSPerFrame = (CB4PS_PER_FRAME*)MappedResource.pData;
	pPSPerFrame->lightDirection = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.1f);	// float fAmbient = 0.1f;
	pContext->Unmap(m_pcbPF, 0);
	pContext->PSSetConstantBuffers(m_iCBPSPerFrameBind, 1, &m_pcbPF);

	// Set the opacity (default opaque)
	V(pContext->Map(m_pcbPO, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB4PS_PER_OBJECT* pPSPerObject = (CB4PS_PER_OBJECT*)MappedResource.pData;
	pPSPerObject->alpha = 1.0f;	// Transparency
	pContext->Unmap(m_pcbPO, 0);
	pContext->PSSetConstantBuffers(m_iCBPSPerObjectBind, 1, &m_pcbPO);

	// Board
	D3DXMATRIX	mWorldBoard;	// World matrix board
	D3DXMatrixTranslation(&mWorldBoard, 0.0f, 0.0f, 0.0f);	// World matrix board
	D3DXMatrixMultiply(&mWVP, &mWorldBoard, &mVP);
	V(pContext->Map(m_pcbVO, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB4VS_PER_OBJECT* pVSPerObject = (CB4VS_PER_OBJECT*)MappedResource.pData;
	D3DXMatrixTranspose(&pVSPerObject->mWVP, &mWVP);
	pContext->Unmap(m_pcbVO, 0);
	pContext->VSSetConstantBuffers(m_iCBVSPerObjectBind, 1, &m_pcbVO);
	//pContext->PSSetShaderResources(0, 1, &m_pTBoard);	// Board texture
	//RenderMesh(pd3dContext, &m_MeshBoard);	// Board with markings / no markup

	// Render static mesh
	g_pTex.render(0);	// ID_TEXTURE_BOARD
	g_p3D.renderMesh(0, 0.0f, 0.0f, 0.0f);

	mWVP = mWorldBoard * mVP;
	V(pContext->Map(m_pcbVO, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	pVSPerObject = (CB4VS_PER_OBJECT*)MappedResource.pData;
	D3DXMatrixTranspose(&pVSPerObject->mWVP, &mWVP);
	pContext->Unmap(m_pcbVO, 0);
	pContext->VSSetConstantBuffers(m_iCBVSPerObjectBind, 1, &m_pcbVO);

	// Render motion mesh
	g_pTex.render(1);	// ID_TEXTURE_KNIGHT
	g_p3D.renderMesh(1, 5.0f * cosf(fAlpha), 0.0f, 5.0f * sinf(fAlpha));

	//pd3dContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pAppl.clearDepthStencilView();

	// Set remember render state
	pContext->PSSetSamplers(0, 1, &pSamplers);	// 
	pContext->IASetInputLayout(pInputLayout);	// 
	pContext->VSSetShader(pVS, NULL, 0);		// 
	pContext->PSSetShader(pPS, NULL, 0);		// 
	pContext->GSSetShader(pGS, NULL, 0);		// 
	pContext->HSSetShader(pHS, NULL, 0);		// 
	pContext->DSSetShader(pDS, NULL, 0);		//

	SAFE_RELEASE(pSamplers);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pGS);
	SAFE_RELEASE(pHS);
	SAFE_RELEASE(pDS);
}


//--------------------------------------------------------------------------------------
// DEINITIALIZATION
//--------------------------------------------------------------------------------------
void CEffect::free()
{
	SAFE_RELEASE(m_pVS);	// Vertex shader for rendering scene
	SAFE_RELEASE(m_pPS);	// Pixel shader for rendering scene
	SAFE_RELEASE(m_pAS);	// Pixel shader for texture with an Alpha channel (elements highlight)
	SAFE_RELEASE(m_pTS);	// Pixel shader for texture with partial Transparency (figure)

	SAFE_RELEASE(m_pVL);	// Vertex layout for meshes

	SAFE_RELEASE(m_pcbVO);	// Buffer with parameters of the matrices
	SAFE_RELEASE(m_pcbPF);	// Buffer with the parameters of the light
	SAFE_RELEASE(m_pcbPO);	// Buffer with the parameter of the object's color (transparency)

	SAFE_RELEASE(m_pAnisot);// Filter Type - ANISOTROPIC
}