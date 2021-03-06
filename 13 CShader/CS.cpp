//--------------------------------------------------------------------------------------
// File: CS.cpp
// Description: Class for work with compute shader.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include "..\Engine\NetWars.h"	// NetWars Engine
#include "D3Dcompiler.h"
#include "D3DX10Math.h"
#include "D3DX11async.h"	// D3DX11CompileFromFileW - Library D3DX11.lib
#include <stdio.h>	// swprintf_s

#include "CS.h"	// Class of Compute Shader


//--------------------------------------------------------------------------------------
// CONSTRUCTOR
//--------------------------------------------------------------------------------------
CCS::CCS()
{
	m_nMove = 0;	// Number of moves
}


//--------------------------------------------------------------------------------------
// INITIALIZATION
//--------------------------------------------------------------------------------------
void CCS::init(ID3D11Device* pD3D)
{
	// Compile Compute Shader
	ID3D10Blob* pBlob;		// Shader buffer
	ID3D10Blob* pErrorBlob;	// Shader error buffer 
	D3DX11CompileFromFileW(L"..\\Content\\Shader\\CS.hlsl", NULL, NULL, "CSMain", "cs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL, NULL, &pBlob, &pErrorBlob, NULL);
	if (pErrorBlob)
	{
		pErrorBlob->GetBufferPointer();
		// then cast to a char* to see it in the locals window
	}

	// Create Compute Shader
	m_pCS = NULL;
	pD3D->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pCS);

	// Release the shader buffers
	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);
}


//--------------------------------------------------------------------------------------
// RESIZE THE OUTPUT SCENE
//--------------------------------------------------------------------------------------
/*HRESULT	CCS::resize(ID3D11Device* pD3D, float fW, float fH)
{
	//m_fW = fW;	// Width
	//m_fH = fH;	// Height

	return S_OK;
}*/


//--------------------------------------------------------------------------------------
// CREATE STRUCTURED BUFFER ON GPU
//-------------------------------------------------------------------------------------
HRESULT CCS::CreateStructuredBufferOnGPU(ID3D11Device* pD3D, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut)
{
	*ppBufOut = NULL;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = uElementSize * uCount;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = uElementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return pD3D->CreateBuffer(&desc, &InitData, ppBufOut);
	}
	else
		return pD3D->CreateBuffer(&desc, NULL, ppBufOut);
}


//--------------------------------------------------------------------------------------
// CREATE A RESOURCE OF RAW OR STRUCTURED BUFFER
//--------------------------------------------------------------------------------------
HRESULT CCS::CreateBufferSRV(ID3D11Device* pD3D, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)	// If Raw buffer
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)	// If Structured buffer
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
		return E_INVALIDARG;

	return pD3D->CreateShaderResourceView(pBuffer, &desc, ppSRVOut);
}


//--------------------------------------------------------------------------------------
// CREATE AN UNORDERED ACCESS TO THE RAW OR STRUCTURED BUFFER
//-------------------------------------------------------------------------------------- 
HRESULT CCS::CreateBufferUAV(ID3D11Device* pD3D, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)	// If Raw buffer
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;	// When creating a disordered access to buffer Raw its format must also be DXGI_FORMAT_R32_TYPELESS
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)	// If Structured buffer
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;		// When creating a disordered Structured access to its buffer size must also be DXGI_FORMAT_UNKNOWN
		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
		return E_INVALIDARG;

	return pD3D->CreateUnorderedAccessView(pBuffer, &desc, ppUAVOut);
}


//--------------------------------------------------------------------------------------
// RUN COMPUTE SHADER
//-------------------------------------------------------------------------------------- 
void CCS::RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ComputeShader* pComputeShader, UINT nNumViews,
	ID3D11ShaderResourceView** pShaderResourceViews, ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
	ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z)
{
	pd3dImmediateContext->CSSetShader(pComputeShader, NULL, 0);
	pd3dImmediateContext->CSSetShaderResources(0, nNumViews, pShaderResourceViews);
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pUnorderedAccessView, (UINT*)&pUnorderedAccessView);
	if (pCBCS)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		pd3dImmediateContext->Map(pCBCS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		memcpy(MappedResource.pData, pCSData, dwNumDataBytes);
		pd3dImmediateContext->Unmap(pCBCS, 0);
		ID3D11Buffer* ppCB[1] = { pCBCS };
		pd3dImmediateContext->CSSetConstantBuffers(0, 1, ppCB);
	}
	pd3dImmediateContext->Dispatch(X, Y, Z);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	pd3dImmediateContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, (UINT*)(&ppUAViewNULL));
	ID3D11ShaderResourceView* ppSRVNULL[3] = { NULL, NULL, NULL };
	pd3dImmediateContext->CSSetShaderResources(0, 3, ppSRVNULL);
	pd3dImmediateContext->CSSetConstantBuffers(0, 0, NULL);
}


//--------------------------------------------------------------------------------------
// CREATE CPU-BUFFER AND COPY GPU-BUFFER TO IT
// THIS FEATURE IS VERY USEFUL FOR DEBUGGING COMPUTE SHADER
//-------------------------------------------------------------------------------------- 
ID3D11Buffer* CCS::CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer)
{
	ID3D11Buffer* debugbuf = NULL;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	pDevice->CreateBuffer(&desc, NULL, &debugbuf);
	pd3dImmediateContext->CopyResource(debugbuf, pBuffer);

	return debugbuf;
}


//--------------------------------------------------------------------------------------
// RENDERING
//--------------------------------------------------------------------------------------
void CCS::render(ID3D11Device *pD3D, ID3D11DeviceContext* pContext, float)
{
	m_nMove = 8;
	for (int i = 0; i < 320; i++)
	{
		iBeg[i] = 5;
		iEnd[i] = 6;
		iWeight[i] = 7;
	}

	// Input output registers
	m_pBufBeg = NULL;		// Buffer start of move
	m_pBufEnd = NULL;		// Buffer end of move
	m_pBufWeight = NULL;	// Buffer weight of move
	m_pBufResult = NULL;	// Buffer result of move
	CreateStructuredBufferOnGPU(pD3D, 4, (UINT)m_nMove, &iBeg[0], &m_pBufBeg);	// 4 - sizeof(int)
	CreateStructuredBufferOnGPU(pD3D, 4, (UINT)m_nMove, &iEnd[0], &m_pBufEnd);
	CreateStructuredBufferOnGPU(pD3D, 4, (UINT)m_nMove, &iWeight[0], &m_pBufWeight);
	CreateStructuredBufferOnGPU(pD3D, 4, (UINT)m_nMove, NULL, &m_pBufResult);

	ID3D11ShaderResourceView *pBufBeg_SRV = NULL;
	ID3D11ShaderResourceView *pBufEnd_SRV = NULL;
	ID3D11ShaderResourceView *pBufWeight_SRV = NULL;
	ID3D11UnorderedAccessView *pBufResult_UAV = NULL;

	CreateBufferSRV(pD3D, m_pBufBeg, &pBufBeg_SRV);
	CreateBufferSRV(pD3D, m_pBufEnd, &pBufEnd_SRV);
	CreateBufferSRV(pD3D, m_pBufWeight, &pBufWeight_SRV);
	CreateBufferUAV(pD3D, m_pBufResult, &pBufResult_UAV);

	// Run compute shader
	ID3D11ShaderResourceView* aRViews[3] = { pBufBeg_SRV, pBufEnd_SRV, pBufWeight_SRV };
	RunComputeShader(pContext, m_pCS, 3, aRViews, NULL, NULL, 0, pBufResult_UAV, 200, 1, 1);

	// Read the result from the GPU, and copy to the CPU buffer
	ID3D11Buffer* pBufResult = CreateAndCopyToDebugBuf(pD3D, pContext, m_pBufResult);
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (S_OK == pContext->Map(pBufResult, 0, D3D11_MAP_READ, 0, &MappedResource))
	{
		int *p = (int*)MappedResource.pData;
		for (UINT i = 0; i<(UINT)m_nMove; i++)
			iWeight[i] = p[i];
		pContext->Unmap(pBufResult, 0);
	}

	UINT iBest = 0;
	for (UINT i = 1; i<(UINT)m_nMove; i++)
		if (iWeight[i]>iWeight[iBest])
			iBest = i;

	m_iBestBeg = (byte)iBeg[iBest];
	m_iBestEnd = (byte)iEnd[iBest];

	SAFE_RELEASE(pBufResult);

	SAFE_RELEASE(pBufBeg_SRV);
	SAFE_RELEASE(pBufEnd_SRV);
	SAFE_RELEASE(pBufWeight_SRV);
	SAFE_RELEASE(pBufResult_UAV);

	SAFE_RELEASE(m_pBufBeg);
	SAFE_RELEASE(m_pBufEnd);
	SAFE_RELEASE(m_pBufWeight);
	SAFE_RELEASE(m_pBufResult);
}


//--------------------------------------------------------------------------------------
// DEINITIALIZATION
//--------------------------------------------------------------------------------------
void CCS::free()
{
	SAFE_RELEASE(m_pCS);
}