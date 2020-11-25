#include "pch.h"
#include "manager.h"
#include "modelmanager.h"
#include "renderer.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "math.h"
#include "input.h"
#include "main.h"
#include "bullet.h"
#include "fpscamera.h"
#include "frustumculling.h"
#include "light.h"
#include "rendertexture.h"


void Enemy::Awake()
{
	GameObject::Awake();

	// init player stuff
	m_shader = CRenderer::GetShader<BasicLightShader>();

	ModelManager::GetModel(MODEL_ENEMY, m_model);

	m_position = dx::XMFLOAT3(0.0F, 1.0F, 10.0F);
	m_rotation = dx::XMFLOAT3(0.0F, 45, 0.0F);
	m_scale = dx::XMFLOAT3(2.0F, 20.0F, 2.0F);

	m_moveSpeed = 0.02F;

	obb.Init((GameObject*)this, 3, 5, 8);
}

void Enemy::Init()
{
	GameObject::Init();
	
	auto player = CManager::GetActiveScene()->GetGameObjects<Player>(0);
	if(!player.empty())
		m_rangeObject = player.front();
}

void Enemy::Uninit()
{
	GameObject::Uninit();
}

void Enemy::Update()
{
	GameObject::Update();
	obb.Update();

	//m_rotation.y += 0.3F;
	//m_rotation.z += 0.4F;
	m_rotation.x += 0.1F;
}

void Enemy::Draw(UINT renderPass)
{
	GameObject::Draw(renderPass);

	if (renderPass == 1)
	{
		// set buffers
		m_shader->SetDirectionalLight(LightManager::GetDirectionalLight());

		dx::XMMATRIX world = GetWorldMatrix();
		m_shader->SetWorldMatrix(&world);

		MATERIAL mat = {};
		mat.Diffuse = { 1,1,1,1 };
		mat.Specular = { 1,1,1,1 };
		m_shader->SetMaterial(mat);

		m_shader->SetShadowMapTexture(CRenderer::GetRenderTexture(2)->GetRenderTexture());
		m_shader->SetLightProjectionMatrix(&LightManager::GetDirectionalProjectionMatrix());
		m_shader->SetLightViewMatrix(&LightManager::GetDirectionalViewMatrix());

		// draw the model
		CRenderer::DrawModel(m_shader, m_model);
		obb.Draw();
	}
}

void Enemy::Draw(const std::shared_ptr<Shader>& shader, UINT renderPass)
{
	GameObject::Draw(shader, renderPass);

	// set shader buffers
	dx::XMMATRIX world = GetWorldMatrix();
	shader->SetWorldMatrix(&world);

	shader->SetProjectionMatrix(&LightManager::GetDirectionalProjectionMatrix());
	shader->SetViewMatrix(&LightManager::GetDirectionalViewMatrix());

	// draw the model
	CRenderer::DrawModel(shader, m_model);
}

void Enemy::Movement()
{
	if (auto player = m_rangeObject.lock())
	{
		dx::XMVECTOR direction = dx::XMVectorSubtract(player->GetPosition(), GetPosition());
		dx::XMStoreFloat3(&m_lookDirection, dx::XMVector3Normalize(direction));
		m_position += m_lookDirection * m_moveSpeed;
	}
}
