#include"CollisionManager.h"
#include"../Collision/Collision.h"
#include"../Shape/Sphere/Sphere.h"



CollisionManager::CollisionManager() {

	SetCombination();
}


void CollisionManager::Update() {


	// 衝突物の代入
	//InsertToCollisionList();

	// グループごとに衝突する
	CollisionGroup();
}


bool CollisionManager::Entry(
	const CollisionObjectType&type,
	CollisionObject3DBase*obj
) {

	if (obj == nullptr){
		return false;
	}

	// 追加
	m_p_collision_list[type].emplace_back(obj);
}


void CollisionManager::Clear() {

	// ポインタは削除せず、配列だけ消す
	m_p_collision_list.clear();
}


void CollisionManager::InsertToCollisionList() {

	// 衝突する種類
	for (auto&shape : m_insert_type_list) {
		
		// 衝突物代入
		for (auto &i_obj : m_p_insert_collision_list) {

			// オブジェクトポインタをコピー
			m_p_collision_list[shape].emplace_back(i_obj);
		}
	}
	
	// 代入専用オブジェクトが存在するなら配列を削除
	{
		if (m_insert_type_list.size() > 0) {
			m_insert_type_list.clear();
		}

		if (m_p_insert_collision_list.size() > 0) {
			m_p_insert_collision_list.clear();
		}
	}
}


void CollisionManager::SetCombination() {
	
	m_group_list[CollisionObjectType::BULLET].
		push_back(CollisionObjectType::ENEMY);
}

void CollisionManager::CollisionGroup(){


	// 衝突物1
	for (auto obj1 : m_group_list) {

		// キーが存在するか
		if (m_p_collision_list.count(obj1.first) == 0) {
			continue;
		}

		// 衝突物2
		for (auto obj2 : m_group_list[obj1.first]) {

			// 衝突物が入っているかどうか
			if (m_p_collision_list.count(obj2) == 0) {
				continue;
			}

			// 組み合わせで当たり判定を開始
			StartCollisionComb(obj1.first, obj2);
		}
	}
}

void CollisionManager::StartCollisionComb(
	const CollisionObjectType&type1,
	const CollisionObjectType&type2
) {

	for (auto obj1 : m_p_collision_list[type1])
	{
		for (auto obj2 : m_p_collision_list[type2]) {

			// 組み合わせで当たり判定を行う
			SelectCollider(obj1, obj2);
		}
	}
}


void CollisionManager::SelectCollider(
	CollisionObject3DBase * obj1,
	CollisionObject3DBase * obj2
) {

	// 形状代入
	ShapeType type1 = obj1->GetShapeType();
	ShapeType type2 = obj2->GetShapeType();

	switch (type1)
	{

		// 球
	case ShapeType::SPHERE:

		switch (type2) {

			// 球
		case ShapeType::SPHERE:
			ChackSphereAndSphere(obj1, obj2);
		}
	}
}


void CollisionManager::ChackSphereAndSphere(
	CollisionObject3DBase*s1,
	CollisionObject3DBase*s2){

	Sphere sphere1;
	Sphere sphere2;

	// 球情報受け取り
	sphere1 = s1->GetSphere();
	sphere2 = s2->GetSphere();

	// 球の当たり判定
	if (Collision::HitSphereAndSphere(
		s1->GetPos(),
		s2->GetPos(),
		sphere1.GetRadian(),
		sphere2.GetRadian())
		 == true) {

		// 衝突したのでそれぞれの衝突処理に入る
		s1->HitAction();
		s2->HitAction();
	}
}