#pragma once

#include "gameObject.h"
#include "basiclightshader.h"
#include "model.h"


class Bullet : public GameObject
{
private:
	BasicLightShader* m_shader;
	std::shared_ptr<CModel> m_model;
	dx::XMFLOAT3 m_direction;
	float m_speed;

	float m_timeToLive;
	float m_timeTillStart;

public:
	Bullet() {}
	~Bullet() {}

	void Awake() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetDirection(dx::XMFLOAT3 direction) { m_direction = direction; }
	void SetSpeed(float speed) { m_speed = speed; }
};
