#pragma once

#include "gameObject.h"
#include "model.h"


class Bullet : public GameObject
{
private:
	std::shared_ptr<CModel> m_model;
	dx::XMFLOAT3 m_direction;
	float m_speed;

	float m_timeToLive;
	float m_timeTillStart;

public:
	Bullet() {}
	~Bullet() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetDirection(dx::XMFLOAT3 direction) { m_direction = direction; }
	void SetSpeed(float speed) { m_speed = speed; }
};
