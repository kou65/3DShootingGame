#pragma once
#include"../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"



// 障害物用キューブ



class Block : public CollisionObject3DBase{
public:

	static constexpr float OBJ_SIZE = 2.f;

	void Destory();

public:

	Block(const Vec3&pos);

	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	// 衝突後
	void HitAction(const CollisionObjectType&type)override;

	// 球を返す
	Sphere GetSphere()override;

	// キューブを返す
	Cube GetCube()override;

};