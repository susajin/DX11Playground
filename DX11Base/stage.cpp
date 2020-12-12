#include "pch.h"
#include "renderer.h"
#include "modelmanager.h"
#include "player.h"
#include "stage.h"
#include "modelmanager.h"
#include "manager.h"
#include "bullet.h"
#include "light.h"
#include "rendertexture.h"


void Stage::Init()
{
	GameObject::Init();

	// get the shader
	m_shader = CRenderer::GetShader<BasicLightShader>();

	ModelManager::GetModel(MODEL_STAGE, m_model);

	// init values
	m_position = dx::XMFLOAT3(0.0F, 0.0F, 0.0F);
	m_rotation = dx::XMFLOAT3(0.0F, 0.0F, 0.0F);
	m_scale = dx::XMFLOAT3(1.0F, 1.0F, 1.0F);

	m_enableFrustumCulling = false;

	// colliders for portal
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, {-25, 20, 39.9F }, {25, 20, 39.9F}, {25, 0, 39.9F}, {-25, 0, 39.9F}, 0, 0, -1, Wall);
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -25, 20, -39.9F }, { 25, 20, -39.9F }, { 25, 0, -39.9F }, { -25, 0, -39.9F }, 0, 0, 1, Wall);
	
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 39.9F, 20, 25 }, { 39.9F, 20, -25 }, { 39.9F, 0, -25 }, { 39.9F, 0, 25 }, -1, 0, 0, Wall);
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -39.9F, 20, 25 }, { -39.9F, 20, -25 }, { -39.9F, 0, -25 }, { -39.9F, 0, 25 }, 1, 0, 0, Wall);

	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 25, 19.9F, 25 }, { -25, 19.9F, 25 }, { -25, 19.9F, -25 }, { 25, 19.9F, -25 }, 0, -1, 0, Ceiling);
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 25, 0.1F, 25 }, { -25, 0.1F, 25 }, { -25, 0.1F, -25 }, { 25, 0.1F, -25 }, 0, 1, 0, Floor);
}

void Stage::Uninit()
{
	GameObject::Uninit();

	for (auto& collider : m_colliders)
		delete collider;

	m_colliders.clear();
}

void Stage::Update()
{
	GameObject::Update();

	for (auto collider : m_colliders)
		collider->Update();
}

void Stage::Draw(Pass pass)
{
	GameObject::Draw(pass);

	// set buffers
	dx::XMMATRIX world = GetWorldMatrix();
	m_shader->SetWorldMatrix(&world);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = dx::XMFLOAT4(1.0F, 1.0F, 1.0F, 1.0F);
	m_shader->SetMaterial(material);

	m_shader->SetShadowMapTexture(CRenderer::GetRenderTexture(2)->GetRenderTexture());
	m_shader->SetLightProjectionMatrix(&LightManager::GetDirectionalProjectionMatrix());
	m_shader->SetLightViewMatrix(&LightManager::GetDirectionalViewMatrix());

	if (pass == Pass::PortalBlue)
	{
		m_shader->SetViewMatrix(&PortalManager::GetViewMatrix(PortalType::Orange));
		m_shader->SetProjectionMatrix(&PortalManager::GetProjectionMatrix(PortalType::Orange));
	}
	else if (pass == Pass::PortalOrange)
	{
		m_shader->SetViewMatrix(&PortalManager::GetViewMatrix(PortalType::Blue));
		m_shader->SetProjectionMatrix(&PortalManager::GetProjectionMatrix(PortalType::Blue));
	}

	// draw the model
	CRenderer::DrawModel(m_shader, m_model);
	for (auto collider : m_colliders)
		collider->Draw();
}

void Stage::Draw(const std::shared_ptr<Shader>& shader, Pass pass)
{
	GameObject::Draw(shader, pass);

	// set shader buffers
	dx::XMMATRIX world = GetWorldMatrix();
	shader->SetWorldMatrix(&world);

	shader->SetProjectionMatrix(&LightManager::GetDirectionalProjectionMatrix());
	shader->SetViewMatrix(&LightManager::GetDirectionalViewMatrix());

	// draw the model
	CRenderer::DrawModel(shader, m_model);
}
