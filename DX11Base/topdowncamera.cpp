#include "pch.h"
#include "main.h"
#include "topdowncamera.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"


void TopDownCamera::Init()
{
	Camera::Init();

	m_offset = dx::XMFLOAT3(0, 15, -2);
}

void TopDownCamera::Uninit()
{
	Camera::Uninit();
}

void TopDownCamera::Draw()
{
	Camera::Draw();
}

void TopDownCamera::Update()
{
	Camera::Update();

	if(m_target)
		m_position = m_target->GetPosition() + m_offset;
}

void TopDownCamera::SetViewMatrix()
{
	// convert float4x4 to matrices
	dx::XMMATRIX view = dx::XMLoadFloat4x4(&m_mView);
	dx::XMVECTOR eye = dx::XMLoadFloat3(&m_position);

	dx::XMFLOAT3 fup = dx::XMFLOAT3(0, 1, 0);
	dx::XMVECTOR up = dx::XMLoadFloat3(&fup);

	dx::XMVECTOR direction = dx::XMVectorSet(0, -1, 0.1F, 1);

	// calculate and set the view matrix for each shader
	//view = dx::XMMatrixLookAtLH(eye, m_target->GetPosition(), up);
	view = dx::XMMatrixLookToLH(eye, direction, up);
	auto shaders = CRenderer::GetShaders();
	for (Shader* shader : shaders)
	{
		shader->SetViewMatrix(&view);
	}

	// load the view matrix back to member variable
	dx::XMStoreFloat4x4(&m_mView, view);
}
