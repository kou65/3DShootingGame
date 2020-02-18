#pragma once
#include"../Shape/Shape3DBase/ShapeBase.h"
#include"../Shape/Sphere/Sphere.h"
#include"../CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../../Lib/SingletonTemplate/SingletonTemplate.h"
#include<vector>



class CollisionManager {
public:

	void Update();

private:

	void SellectCollider();

	void ChackSphereAndSphere(
		Sphere*s1,
		Sphere*s2
	);

private:

	Singleton<CollisionManager>m_instance;

	std::vector<CollisionObject3DBase*>m_p_col_obj_list;
	std::vector<Shape3DBase*>m_p_shape_list;
};