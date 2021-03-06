//--------------------------------------------------------------------------------------
// File: CS.h
// Description: Class for work with compute shader.
// Copyright (©) Dmitry N. Blinnikov (Jimnik). All rights reserved.
//--------------------------------------------------------------------------------------


#include <windows.h>


class CCS
{
protected:

	ID3D11ComputeShader *m_pCS;	// Compute shader
	ID3D11Buffer *m_pBufBeg;	// Buffer start of move
	ID3D11Buffer *m_pBufEnd;	// Buffer end of move
	ID3D11Buffer *m_pBufWeight;	// Buffer weight of move
	ID3D11Buffer *m_pBufResult;	// Buffer result of move

	byte	m_iBestBeg;		// The best move: start
	byte	m_iBestEnd;		// The best move: start
	WORD	m_nMove;		// Number of moves
	int		iBeg[320];		// Start of move
	int		iEnd[320];		// End of move
	int		iWeight[320];	// Weight of move

public:
	CCS();	// Constructor

	void	init(ID3D11Device* pD3D);	// Initialization
	HRESULT	resize(ID3D11Device* pD3D, float fW, float fH);	// Resize the output scene

	HRESULT	CreateStructuredBufferOnGPU(ID3D11Device* pD3D, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut);	// Create structured buffers on the GPU
	HRESULT	CreateBufferSRV(ID3D11Device* pD3D, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);	// Create a resource of RAW or STRUCTURED buffer
	HRESULT	CreateBufferUAV(ID3D11Device* pD3D, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut);	// Create an unordered access to the RAW or STRUCTURED buffer
	void	RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ComputeShader* pComputeShader, UINT nNumViews,	// Run Compute Shader
		ID3D11ShaderResourceView** pShaderResourceViews, ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView, UINT X, UINT Y, UINT Z);
	ID3D11Buffer*	CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer);	// Create CPU-buffer and copy GPU-buffer to it 

	void	render(ID3D11Device *pD3D, ID3D11DeviceContext* pContext, float fTime);	// Rendering
	void	free();	// Deinitialization
};