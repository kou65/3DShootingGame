#include"CollisionManager.h"
#include"../Collision/Collision.h"



void CollisionManager::Update() {

}


void CollisionManager::SellectCollider() {

	for (auto &itr : m_p_col_obj_list) {

		Shape3DBase s = itr->GetShape();

		switch(itr->GetShapeType())
		{

		case ShapeType::SPHERE:

			ChackSphereAndSphere()
			itr->GetShape()

		default:
			break;
		}

	}
}


void CollisionManager::ChackSphereAndSphere(Sphere*s1,Sphere*s2){

	if(Collision::HitSphereAndSphere(
		s1->GetPos(),
		s2->GetPos(),
		s1->GetRadian(),
		s2->GetRadian())
		);
}