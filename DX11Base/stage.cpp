#include "pch.h"
#include "renderer.h"
#include "modelmanager.h"
#include "player.h"
#include "stage.h"
#include "modelmanager.h"
#include "manager.h"
#include "light.h"
#include "rendertexture.h"


void Stage::Init()
{
	GameObject::Init();

	// get the shader
	m_shader = CRenderer::GetShader<BasicLightShader>();

	ModelManager::GetModel(MODEL_STAGE, m_model);

	// init values
	SetPosition(0.0F, 0.0F, 0.0F);
	SetRotation(0.0F, 0.0F, 0.0F);
	SetScale(1.0F, 1.0F, 1.0F);

	m_enableFrustumCulling = false;

	// colliders for portal
	// finish room
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 13, -1.0f, -20 }, { -2.8f, -1.0f, -20 }, { -2.8f, -1.0f, -41 }, { 13, -1.0f, -41 }); // ground
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, {-2.8f, 18, -40.9f }, {13.0f, 18, -40.9f }, {13.0f, -1, -40.9f }, {-2.8f, -1, -40.9f }); // wall back
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -2.8f, 18, -20.0f }, { -2.8f, 18, -40.9f }, { -2.8f, -1, -40.9f }, { -2.8f, -1, -20.0f }); // wall left
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 13.0f, 18, -40.9f }, { 13.0f, 18, -20.0f }, { 13.0f, -1, -20.0f }, { 13.0f, -1, -40.9f }); // wall right

	// box room
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -3.0f, -24.9f, -4.5f }, { -13.2f, -24.9f, -4.5f }, { -13.2f, -24.9f, -20.0f }, { -3.0f, -24.9f, -20.0f }); // ground
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -13.2f, 17.9f, -20.0f }, { -3.0f, 17.9f, -20.0f } , { -3.0f, -24.9f, -20.0f }, { -13.2f, -24.9f, -20.0f }); // ground back
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -3.0f, -11.7f, -20.0f }, { -3.0f, -11.7f, -4.3f }, { -3.0f, -24.9f, -4.3f }, { -3.0f, -24.9f, -20.0f }); // ground right
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -3.0f, -11.7f, -4.5f }, { -13.2f, -11.7f, -4.5f }, { -13.2f, -24.9f, -4.5f }, { -3.0f, -24.9f, -4.5f }); // ground front
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -13.2f, -11.7f, -4.5f }, { -13.2f, -11.7f, -20.0f }, { -13.2f, -24.9f, -20.0f }, { -13.2f, -24.9f, -4.5f }); // ground left

	// ceiling
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -40, 18.0f, 16.8f }, { 27.1f, 18.0f, 16.8f }, { 27.1f, 18.0f, -40 }, { -40, 18.0f, -40 });

	// main room
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -2.8f, -1, -20 }, { 13.0f, -1, -20 }, { 13.0f, -11.5f, -20 }, { -2.8f, -11.5f, -20 }); // finish room below
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 13.0f, 18, -20.0f }, { 13.0f, 18, -4.4f }, { 13.0f, -11.7f, -4.4f }, { 13.0f, -11.7f, -20.0f }); // right wall front of finish room
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 13.0f, 18, -4.2f }, { 27.0f, 18, -4.2f }, { 27.0f, -11.7f, -4.2f }, { 13.0f, -11.7f, -4.2f }); // back wall of right area
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 27.0f, 18, -4.2f }, { 27.0f, 18, 7.9f }, { 27.0f, -11.7f, 7.9f }, { 27.0f, -11.7f, -4.2f }); // right wall of right area
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 18.2f, 18, 16.7f }, { -40.5f, 18, 16.7f }, { -40.5f, -11.7f, 16.7f }, { 18.2f, -11.7f, 16.7f }); // big wall front
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -40.5f, 18, 16.7f }, { -40.5f, 18, 6.3f }, { -40.5f, -11.7f, 6.3f }, { -40.5f, -11.7f, 16.7f }); // left side small wall
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -40.5f, 18, 6.3f }, { -13.5f, 18, 6.3f }, { -13.5f, -11.7f, 6.3f }, { -40.5f, -11.7f, 6.3f }); // left side back small wall
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { -13.2f, 17.9f, 6.3f }, { -13.2f, 17.9f, -20.0f }, { -13.2f, -11.7f, -20.0f }, { -13.2f, -11.7f, 6.3f }); // left side box room top

	// main ground
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 27.1f, -11.7f, 16.8f }, { -40.5f, -11.7f, 16.8f }, { -40.5f, -11.7f, -4.3f }, { 27.1f, -11.7f, -4.3f }); // main 1
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 13, -11.7f, -4.3f }, { -2.8f, -11.7f, -4.3f }, { -2.8f, -11.7f, -20.2f }, { 13, -11.7f, -20.2f }); // main 2

	// angled surface
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 8.0f, -1.6f, 16.3f }, { -2.8f, -1.6f, 16.3f }, { -2.8f, -11.7f, 6.1f }, { 8.0f, -11.7f, 6.1f }); // x angled
	m_colliders.push_back(new PolygonCollider());
	m_colliders.back()->Init(this, { 27.1f, 18, 7.9f }, { 18.2f, 18, 16.8f }, { 18.2f, -11.7f, 16.8f }, { 27.1f, -11.7f, 7.9f }); // y angled
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
	if (!(pass == Pass::Default || pass == Pass::PortalBlue || pass == Pass::PortalOrange))
		return;

	GameObject::Draw(pass);

	// set buffers
	dx::XMMATRIX world = GetWorldMatrix();
	m_shader->SetWorldMatrix(&world);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = dx::XMFLOAT4(1.0F, 1.0F, 1.0F, 1.0F);
	m_shader->SetMaterial(material);

	if (pass == Pass::PortalBlue)
	{
		m_shader->SetViewMatrix(&PortalManager::GetViewMatrix(PortalType::Blue));
		m_shader->SetProjectionMatrix(&PortalManager::GetProjectionMatrix(PortalType::Blue));
	}
	else if (pass == Pass::PortalOrange)
	{
		m_shader->SetViewMatrix(&PortalManager::GetViewMatrix(PortalType::Orange));
		m_shader->SetProjectionMatrix(&PortalManager::GetProjectionMatrix(PortalType::Orange));
	}

	// draw the model
	CRenderer::DrawModel(m_shader, m_model);

	// draw the collider
	for (auto collider : m_colliders)
		collider->Draw();
}

void Stage::Draw(const std::shared_ptr<Shader>& shader, Pass pass)
{
	if (!(pass == Pass::Lightmap))
		return;

	GameObject::Draw(shader, pass);

	// set shader buffers
	dx::XMMATRIX world = GetWorldMatrix();
	shader->SetWorldMatrix(&world);

	shader->SetProjectionMatrix(&LightManager::GetProjectionMatrix());
	shader->SetViewMatrix(&LightManager::GetViewMatrix());

	// draw the model
	CRenderer::DrawModel(shader, m_model);
}
