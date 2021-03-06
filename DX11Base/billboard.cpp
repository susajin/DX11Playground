#include "pch.h"
#include "renderer.h"
#include "billboard.h"
#include "topdowncamera.h"
#include "manager.h"


void Billboard::Awake()
{
	GameObject::Awake();

	// init the shader
	m_shader = CRenderer::GetShader<BasicLightShader>();

	// init the vertices
	VERTEX_3D vertex[4];

	vertex[0].Position = dx::XMFLOAT3(-1.0F, 1.0F, 0);
	vertex[0].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = dx::XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = dx::XMFLOAT3(1.0F, 1.0F, 0);
	vertex[1].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = dx::XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = dx::XMFLOAT3(-1.0F, -1.0F, 0);
	vertex[2].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = dx::XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = dx::XMFLOAT3(1.0F, -1.0F, 0);
	vertex[3].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = dx::XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/explosion.png",
		NULL,
		NULL,
		&m_texture,
		NULL);

	assert(m_texture);

	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);
}

void Billboard::Uninit()
{
	GameObject::Uninit();

	m_vertexBuffer->Release();
	m_texture->Release();
}

void Billboard::Update()
{
	GameObject::Update();

	m_count += 0.2F;

	if (m_count > 16)
	{
		SetDestroy();
		return;
	}
}

void Billboard::Draw(Pass pass)
{
	GameObject::Draw(pass);

	// update the UV for texture sheet animation
	float x = (int)m_count % 4 * (1.0F / 4);
	float y = (int)m_count / 4 * (1.0F / 4);
	
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = dx::XMFLOAT3(-1.0F, 1.0F, 0);
	vertex[0].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = dx::XMFLOAT2(x, y);
	
	vertex[1].Position = dx::XMFLOAT3(1.0F, 1.0F, 0);
	vertex[1].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = dx::XMFLOAT2(x + (1.0F / 4), y);
	
	vertex[2].Position = dx::XMFLOAT3(-1.0F, -1.0F, 0);
	vertex[2].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = dx::XMFLOAT2(x, y + (1.0F / 4));
	
	vertex[3].Position = dx::XMFLOAT3(1.0F, -1.0F, 0);
	vertex[3].Normal = dx::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = dx::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = dx::XMFLOAT2(x + (1.0F / 4), y + (1.0F / 4));
	
	CRenderer::GetDeviceContext()->Unmap(m_vertexBuffer, 0);
	
	// get the inverse of camera matrix to always face towards the camera
	auto camera = CManager::GetActiveScene()->GetMainCamera();
	dx::XMMATRIX view = camera->GetViewMatrix();
	
	// get the inverse view matrix
	//D3DXMatrixInverse(&invView, NULL, &view);
	//invView._41 = 0.0F;
	//invView._42 = 0.0F;
	//invView._43 = 0.0F;
	
	// transpose instead of inverse (faster)
	view = dx::XMMatrixTranspose(view);

	dx::XMFLOAT4X4 invView;
	dx::XMStoreFloat4x4(&invView, view);
	invView._14 = 0.0F;
	invView._24 = 0.0F;
	invView._34 = 0.0F;

	view = dx::XMLoadFloat4x4(&invView);
	
	//マトリックス設定
	dx::XMMATRIX scale, trans;
	scale = dx::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	trans = dx::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	
	m_shader->SetWorldMatrix(&(scale * view * trans));
	
	// set material
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = dx::XMFLOAT4(1.0F, 1.0F, 1.0F, 1.0F);
	m_shader->SetMaterial(material);
	
	//テクスチャ設定
	m_shader->SetTexture(m_texture);
	
	//ポリゴン描画
	CRenderer::DrawPolygon(m_shader, &m_vertexBuffer, 4);
}
