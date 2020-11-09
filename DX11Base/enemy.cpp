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


void Enemy::Awake()
{
	GameObject::Awake();

	// init player stuff
	m_shader = CRenderer::GetShader<BasicLightShader>();

	ModelManager::GetModel(MODEL_ENEMY, m_model);

	m_position = dx::XMFLOAT3(0.0F, 1.0F, 10.0F);
	m_rotation = dx::XMFLOAT3(0.0F, 0.0F, 0.0F);
	m_scale = dx::XMFLOAT3(1.0F, 1.0F, 1.0F);

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

	m_rotation.y += 0.3F;
	m_rotation.x += 0.4F;
}

void Enemy::Draw(UINT renderPass)
{
	GameObject::Draw(renderPass);

	// lighting
	LIGHT light;
	light.Enable = false;
	light.Direction = dx::XMFLOAT4(0.5F, -1.0F, 0.0F, 0.0F);
	dx::XMStoreFloat4(&light.Direction, dx::XMVector4Normalize(dx::XMLoadFloat4(&light.Direction)));

	light.Ambient = dx::XMFLOAT4(.1F, .1F, .1F, 1.0F);
	light.Diffuse = dx::XMFLOAT4(1.0F, 1.0F, 1.0F, 1.0F);
	m_shader->SetLight(light);

	// set the world matrix for this object based on lookat vector
	dx::XMMATRIX world = GetWorldMatrix();

	//dx::XMFLOAT4X4 t;
	//dx::XMStoreFloat4x4(&t, world);
	//
	//dx::XMVECTOR up = dx::XMVectorSet(0, 1, 0, 1);
	//dx::XMVECTOR zaxis = dx::XMVector3Normalize(dx::XMVectorSet(m_lookDirection.x, 0, m_lookDirection.z, 1));
	//dx::XMVECTOR xaxis = dx::XMVector3Normalize(dx::XMVector3Cross(up, zaxis));
	//dx::XMVECTOR yaxis = dx::XMVector3Cross(zaxis, xaxis);
	//
	//dx::XMFLOAT3 z, x, y;
	//dx::XMStoreFloat3(&z, zaxis);
	//dx::XMStoreFloat3(&x, xaxis);
	//dx::XMStoreFloat3(&y, yaxis);
	//
	//t._11 = x.x * m_scale.x; t._12 = x.y * m_scale.y; t._13 = x.z * m_scale.z;
	//t._21 = y.x * m_scale.x; t._22 = y.y * m_scale.y; t._23 = y.z * m_scale.z;
	//t._31 = z.x * m_scale.x; t._32 = z.y * m_scale.y; t._33 = z.z * m_scale.z;
	//
	//world = dx::XMLoadFloat4x4(&t);
	m_shader->SetWorldMatrix(&world);

	// set buffers
	//auto bullets = CManager::GetActiveScene()->GetGameObjects<Bullet>(0);
	//if (auto player = m_rangeObject.lock())
	//{
	//	dx::XMVECTOR pos = player->GetPosition();
	//	pos = dx::XMVectorSetY(pos, 0);
	//
	//	if (!bullets.empty())
	//		m_shader->PS_SetRangeBuffer(player->GetSightRange(), pos, 5, bullets.back()->GetPosition());
	//	else
	//		m_shader->PS_SetRangeBuffer(player->GetSightRange(), pos, -1, dx::XMVECTOR{ 0,0,0 });
	//}
	//
	//m_shader->PS_SetValueBuffer(1, false, true);

	// draw the model
	CRenderer::DrawModel(m_shader, m_model);
	obb.Draw();
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
