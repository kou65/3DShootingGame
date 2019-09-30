#pragma once
#include"Lib\Graphics\Graphics.h"


class Object {
public:

	~Object() {};

	Object() {
		m_is_active = true;
		m_pos.x = m_pos.y = m_pos.z = 0.f;
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsActive() {
		return m_is_active;
	}

protected:

	D3DXVECTOR3 m_pos;
	bool m_is_active;
};