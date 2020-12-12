#include "pch.h"
#include "portalmanager.h"
#include "manager.h"


std::weak_ptr<Portal> PortalManager::m_bluePortal;
std::weak_ptr<Portal> PortalManager::m_orangePortal;
std::weak_ptr<RenderTexture> PortalManager::m_renderTexBlue;
std::weak_ptr<RenderTexture> PortalManager::m_renderTexOrange;


void PortalManager::CreatePortal(PortalType type, dx::XMFLOAT3 position, dx::XMFLOAT3 lookAt, dx::XMFLOAT3 up)
{
	auto portal = CManager::GetActiveScene()->AddGameObject<Portal>(1);
	portal->SetPosition(position);
	portal->SetLookAt(lookAt);
	portal->SetUp(up);
	
	switch (type)
	{
	case PortalType::Blue:
		if (auto oldPortal = m_bluePortal.lock())
			oldPortal->SetDestroy();

		if (auto renderTex = m_renderTexBlue.lock())
			portal->SetRenderTexture(renderTex);

		portal->SetColor({0,0,1,1});
		m_bluePortal = portal;
		break;
	case PortalType::Orange:
		if (auto oldPortal = m_orangePortal.lock())
			oldPortal->SetDestroy();

		if (auto renderTex = m_renderTexOrange.lock())
			portal->SetRenderTexture(renderTex);

		portal->SetColor({ 1,0.7f,0,1 });
		m_orangePortal = portal;
		break;
	}

	if (type == PortalType::Orange)
	{
		if (auto bluePortal = m_bluePortal.lock())
		{
			bluePortal->IsOtherPortalActive(true);
			portal->IsOtherPortalActive(true);
		}
	}
	
	if (type == PortalType::Blue)
	{
		if (auto orangePortal = m_orangePortal.lock())
		{
			orangePortal->IsOtherPortalActive(true);
			portal->IsOtherPortalActive(true);
		}
	}
}

dx::XMMATRIX PortalManager::GetProjectionMatrix(PortalType type)
{
	switch (type)
	{
	case PortalType::Blue:
		if (auto portal = m_bluePortal.lock())
			return portal->GetProjectionMatrix();
	case PortalType::Orange:
		if (auto portal = m_orangePortal.lock())
			return portal->GetProjectionMatrix();
	default:
		return dx::XMMATRIX();
	}
}

dx::XMMATRIX PortalManager::GetViewMatrix(PortalType type)
{
	switch (type)
	{
	case PortalType::Blue:
		if (auto portal = m_bluePortal.lock())
			return portal->GetViewMatrix();
	case PortalType::Orange:
		if (auto portal = m_orangePortal.lock())
			return portal->GetViewMatrix();
	default:
		return dx::XMMATRIX();
	}
}

void PortalManager::BindRenderTexture(PortalType type, const std::shared_ptr<RenderTexture>& renderTexture)
{
	switch (type)
	{
	case PortalType::Blue:
		m_renderTexBlue = renderTexture;
	case PortalType::Orange:
		m_renderTexOrange = renderTexture;
	}
}
