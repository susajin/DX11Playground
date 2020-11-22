#pragma once

#include <typeinfo>
#include <map>


// 頂点構造体
struct VERTEX_3D
{
    dx::XMFLOAT3 Position;
	dx::XMFLOAT3 Normal;
	dx::XMFLOAT4 Diffuse;
	dx::XMFLOAT2 TexCoord;
	dx::XMFLOAT3 Tangent;
	dx::XMFLOAT3 Binormal;
};

// マテリアル構造体
struct MATERIAL
{
	dx::XMFLOAT4 Diffuse;
	dx::XMFLOAT4 Specular;
	dx::XMFLOAT4 Emission;
};

enum RasterizerState
{
	RasterizerState_CullBack,
	RasterizerState_CullFront,
	RasterizerState_Wireframe
};

class Shader;
class ComputeShader;
class Model;
class GameObject;

static class CRenderer
{
private:
	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_D3DDevice;
	static ID3D11DeviceContext*    m_ImmediateContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11RasterizerState* m_rasterizerCullBack;
	static ID3D11RasterizerState* m_rasterizerCullFront;
	static ID3D11RasterizerState* m_rasterizerWireframe;

	static std::map<UINT, ID3D11RenderTargetView*> m_renderTargetViews;

	static std::vector<std::shared_ptr<Shader>> m_shaders;
	static std::vector<std::shared_ptr<ComputeShader>> m_computeShaders;
	static std::weak_ptr<Shader> m_activeShader;

public:
	static void Init();
	static void Uninit();
	static void Begin(std::vector<uint8_t> renderPass, bool clearBuffer);
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetShader(const std::shared_ptr<Shader>& shader);
	static void BindRenderTargetView(ID3D11RenderTargetView* renderTargetView, UINT renderPass);

	template <typename T>
	static std::shared_ptr<T> GetShader()
	{
		// search the shader in the list
		for (auto shader : m_shaders)
		{
			if (typeid(*shader) == typeid(T))
				return std::static_pointer_cast<T>(shader);
		}

		// not found, so init the shader and return the pointer to it
		m_shaders.push_back(std::shared_ptr<T>(new T()));
		m_shaders.back()->Init();

		return std::static_pointer_cast<T>(m_shaders.back());
	}

	static std::vector<std::shared_ptr<Shader>> GetShaders() { return m_shaders; }

	template <typename T>
	static std::shared_ptr<T> GetComputeShader()
	{
		// search the compute shader in the list
		for (auto shader : m_computeShaders)
		{
			if (typeid(*shader) == typeid(T))
				return std::static_pointer_cast<T>(shader);
		}

		// not found, so init the compute shader and return the pointer to it
		m_computeShaders.push_back(std::shared_ptr<T>(new T()));
		m_computeShaders.back()->Init();

		return std::static_pointer_cast<T>(m_computeShaders.back());
	}

	static ID3D11Device* GetDevice(){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext(){ return m_ImmediateContext; }
	static void SetRasterizerState(RasterizerState state);

	static void DrawLine(const std::shared_ptr<Shader> shader, ID3D11Buffer** vertexBuffer, UINT vertexCount);
	static void DrawModel(const std::shared_ptr<Shader> shader, const std::shared_ptr<Model> model);
	static void DrawPolygon(const std::shared_ptr<Shader> shader, ID3D11Buffer** vertexBuffer, UINT vertexCount);
	static void DrawPolygonIndexed(const std::shared_ptr<Shader> shader, ID3D11Buffer** vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
};
