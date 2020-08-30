#include "pch.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "enemy.h"
#include "math.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "bullet.h"


void Enemy::Awake()
{
	GameObject::Awake();

	// init player stuff
	m_shader = CRenderer::GetShader<RangeShader>();

	ModelManager::GetModel(MODEL_PLAYER, m_model);

	m_position = dx::XMFLOAT3(5.0F, 0.0F, 0.0F);
	m_rotation = dx::XMFLOAT3(0.0F, 0.0F, 0.0F);
	m_scale = dx::XMFLOAT3(1, 1, 1);

	m_moveSpeed = 0.05F;
}

void Enemy::Init()
{
	GameObject::Init();
	
	m_rangeObject = CManager::GetActiveScene()->GetGameObjects<Player>(0).front();
}

void Enemy::Uninit()
{
	GameObject::Uninit();
}

void Enemy::Update()
{
	GameObject::Update();

	Movement();
}

void Enemy::Draw()
{
	GameObject::Draw();

	// set the active shader
	CRenderer::SetShader(m_shader);

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

	dx::XMFLOAT4X4 t;
	dx::XMStoreFloat4x4(&t, world);

	dx::XMVECTOR up = dx::XMVectorSet(0, 1, 0, 1);
	dx::XMVECTOR zaxis = dx::XMVector3Normalize(dx::XMVectorSet(m_lookDirection.x, 0, m_lookDirection.z, 1));
	dx::XMVECTOR xaxis = dx::XMVector3Normalize(dx::XMVector3Cross(up, zaxis));
	dx::XMVECTOR yaxis = dx::XMVector3Cross(zaxis, xaxis);

	dx::XMFLOAT3 z, x, y;
	dx::XMStoreFloat3(&z, zaxis);
	dx::XMStoreFloat3(&x, xaxis);
	dx::XMStoreFloat3(&y, yaxis);

	t._11 = x.x; t._12 = x.y; t._13 = x.z;
	t._21 = y.x; t._22 = y.y; t._23 = y.z;
	t._31 = z.x; t._32 = z.y; t._33 = z.z;

	world = dx::XMLoadFloat4x4(&t);
	m_shader->SetWorldMatrix(&world);

	// set buffers
	auto bullets = CManager::GetActiveScene()->GetGameObjects<Bullet>(0);
	if(!bullets.empty())
		m_shader->PS_SetRangeBuffer(10, m_rangeObject->GetPosition(), 5, bullets.back()->GetPosition());
	else
		m_shader->PS_SetRangeBuffer(10, m_rangeObject->GetPosition(), -1, dx::XMVECTOR{0,0,0});

	m_shader->PS_SetValueBuffer(1, false, true);

	// draw the model
	m_model->Draw(m_shader);
}

void Enemy::Movement()
{
	dx::XMVECTOR direction = dx::XMVectorSubtract(m_rangeObject->GetPosition(), GetPosition());
	dx::XMStoreFloat3(&m_lookDirection, dx::XMVector3Normalize(direction));

	m_position += m_lookDirection * m_moveSpeed;
}
