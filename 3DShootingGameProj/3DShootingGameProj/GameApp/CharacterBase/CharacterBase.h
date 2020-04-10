#pragma once
#include"../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"



class CharacterBase : public CollisionObject3DBase{
public:

	void Update()override {};
	void Draw()override {};

protected:

	// 死亡
	void Death();

	// ダメージ加算
	void AddDamage(const float&damage);

protected:
	
	// 体力
	float m_hp;
};