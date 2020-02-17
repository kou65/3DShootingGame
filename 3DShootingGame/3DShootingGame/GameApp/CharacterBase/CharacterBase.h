#pragma once
#include"../../ObjectSystem/Object3D/Object3D.h"



class CharacterBase : public Object3D{
public:

	void Update()override {}
	void Draw()override {}

protected:

	// 死亡
	void Death();

protected:
	
	// 体力
	float m_hp;
};