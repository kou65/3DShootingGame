#pragma once
#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../Shape/Shape3DBase/ShapeBase.h"
#include"../Shape/Sphere/Sphere.h"


// 衝突用オブジェクト


class CollisionObject3DBase : public Object3D{
public:

	// 更新
	void Update()override {};

	// 描画
	void Draw()override {};

	virtual void HitAction(Shape3DBase*hit_shape) = 0;

	Shape3DBase GetShape() {
		return m_p_shape;
	}

	ShapeType GetShapeType() {
		return m_shape_t;
	}

protected:

	// 形状
	Shape3DBase m_p_shape;

	ShapeType m_shape_t;
};