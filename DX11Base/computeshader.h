#pragma once


class ComputeShader
{
public:
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Uninit();

protected:
	ID3D11ComputeShader*		m_shader = nullptr;

	HRESULT CreateComputeShader(const char* pSrcFile);

	HRESULT CreateStructuredBuffer(UINT uElementSize, UINT uCount, void* pInitData, ID3D11Buffer** ppBufOut, bool resultBuffer);

	HRESULT CreateBufferSRV(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);
	HRESULT CreateBufferUAV(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut);

	void RunComputeShader(UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView* pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);

	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Buffer* pBuffer);

	template<class T>
	void WriteToBuffer(ID3D11Buffer* pBuffer, T* pData)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		T* data = (T*)msr.pData;

		memcpy(data, pData, sizeof(T));

		CRenderer::GetDeviceContext()->Unmap(pBuffer, 0);
	}
};
