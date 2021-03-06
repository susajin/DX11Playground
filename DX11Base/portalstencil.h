#pragma once

#include "portal.h"
#include "portalstencilshader.h"


class PortalStencil : public Portal
{
public:
	PortalStencil() {}
	~PortalStencil() {}

	void Awake() override;
	void Uninit() override;
	void Update() override;
	void Draw(Pass pass) override;
	void Draw(const std::shared_ptr<class Shader>& shader, Pass pass) override;

	// getters
	dx::XMMATRIX GetViewMatrix(bool firstIteration = false) override;

private:
	std::shared_ptr<PortalStencilShader> m_shader;
};
