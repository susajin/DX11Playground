#include "pch.h"
#include "main.h"
#include "renderer.h"
#include "rendertexture.h"
#include "sprite.h"


void Sprite::Awake()
{
	GameObject::Awake();

	m_shader = CRenderer::GetShader<MinimapShader>();
	m_renderTexture = new RenderTexture();

	CRenderer::BindRenderTargetView(m_renderTexture->GetRenderTargetView(), 2);
}

void Sprite::Uninit()
{
	GameObject::Uninit();

	SAFE_RELEASE(m_Texture);
	SAFE_RELEASE(m_VertexBuffer);
}

void Sprite::Update()
{
	GameObject::Update();
}

void Sprite::Draw(UINT renderPass)
{
	if (renderPass == 1)
	{
		GameObject::Draw(renderPass);

		// dont draw if vertex buffer is not initialized
		if (!m_VertexBuffer)
			return;

		// material
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = dx::XMFLOAT4(1.0F, 1.0F, 1.0F, 1.0F);
		m_shader->SetMaterial(material);

		// texture
		m_shader->PS_SetTexture(m_Texture);
		m_shader->PS_SetTexture(m_renderTexture->GetRenderTexture());
		m_shader->PS_SetSamplerState(m_renderTexture->GetSamplerState());
		CRenderer::DrawPolygon(m_shader, &m_VertexBuffer, 4);
	}
}

void Sprite::SetTexture(const char* path)
{
	CPolygon::LoadTexture(path, m_Texture);
}

void Sprite::CreatePlaneCenter(float centerX, float centerY, float width, float height, bool dynamic)
{
	SAFE_RELEASE(m_VertexBuffer);
	CPolygon::CreatePlaneCenter(centerX, centerY, width, height, m_VertexBuffer, dynamic);
}

void Sprite::CreatePlaneTopLeft(float topLeftX, float topLeftY, float width, float height, bool dynamic)
{
	SAFE_RELEASE(m_VertexBuffer);
	CPolygon::CreatePlaneTopLeft(topLeftX, topLeftY, width, height, m_VertexBuffer, dynamic);
}

void Sprite::SetPositionCenter(float centerX, float centerY, float width, float height)
{
	CPolygon::RemapDimensionsCenter(centerX, centerY, width, height, m_VertexBuffer);
}

void Sprite::SetPositionTopLeft(float topLeftX, float topLeftY, float width, float height)
{
	CPolygon::RemapDimensionsTopLeft(topLeftX, topLeftY, width, height, m_VertexBuffer);
}
