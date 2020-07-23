#include"CollisionManager.h"
#include"../../Lib/Utility/Collision/Collision.h"
#include"../../CollisionSystem/Shape/Sphere/Sphere.h"



CollisionManager::CollisionManager() {

	SetCombination();
}


void CollisionManager::Update() {

	// グループごとに衝突する
	CollisionGroup();
}


void CollisionManager::AllRelease() {

	for (auto&obj : mp_collision_list) {
		for (auto obj2 : obj.second) {

			if (obj2.lock() != nullptr) {
				obj2.reset();
			}
		}
		obj.second.clear();
	}

	m_group_list.clear();

	for (auto&obj : m_group_list) {

		obj.second.clear();
	}

	mp_collision_list.clear();

	m_insert_type_list.clear();

	for (auto&obj : mp_insert_collision_list) {
		
		if (obj.lock() != nullptr) {
			obj.reset();
		}
	}

	mp_insert_collision_list.clear();

}

bool CollisionManager::Entry(
	const CollisionObjectType&type,
	std::weak_ptr<CollisionObject3DBase>obj
) {

	if (obj.lock() == nullptr){
		return false;
	}

	// 追加
	mp_collision_list[type].emplace_back(obj);

	return true;
}


void CollisionManager::Clear() {

	// ポインタは削除せず、配列だけ消す
	mp_collision_list.clear();
}


void CollisionManager::InsertToCollisionList() {

	// 衝突する種類
	for (auto&shape : m_insert_type_list) {
		
		// 衝突物代入
		for (auto &i_obj : mp_insert_collision_list) {

			// オブジェクトポインタをコピー
			mp_collision_list[shape].emplace_back(i_obj);
		}
	}
	
	// 代入専用オブジェクトが存在するなら配列を削除
	{
		if (m_insert_type_list.size() > 0) {
			m_insert_type_list.clear();
		}

		if (mp_insert_collision_list.size() > 0) {
			mp_insert_collision_list.clear();
		}
	}
}


void CollisionManager::SetCombination() {

	// プレイヤーと敵
	m_group_list[CollisionObjectType::PLAYER].
		push_back(CollisionObjectType::ENEMY);

	// プレイヤーと敵の弾
	m_group_list[CollisionObjectType::PLAYER].
		push_back(CollisionObjectType::ENEMY_BULLET);

	// プレイヤーとキューブ
	m_group_list[CollisionObjectType::PLAYER].
		push_back(CollisionObjectType::BLOCK);
	
	// プレイヤーの弾と敵
	m_group_list[CollisionObjectType::PLAYER_BULLET].
		push_back(CollisionObjectType::ENEMY);

	// プレイヤーの弾とキューブ
	m_group_list[CollisionObjectType::PLAYER_BULLET].
		push_back(CollisionObjectType::BLOCK);

	// 敵の弾とキューブ
	m_group_list[CollisionObjectType::ENEMY_BULLET].
		push_back(CollisionObjectType::BLOCK);
	
	// 敵とキューブ
	m_group_list[CollisionObjectType::ENEMY].
		push_back(CollisionObjectType::BLOCK);

	// 破壊弾とキューブ
	m_group_list[CollisionObjectType::BREAK_BULLET].
		push_back(CollisionObjectType::BLOCK);
}	

void CollisionManager::CollisionGroup(){


	// 衝突物1
	for (auto obj1 : m_group_list) {

		// objキーが存在するか
		if (mp_collision_list.count(obj1.first) == 0) {
			continue;
		}

		// 衝突物2
		for (auto obj2 : m_group_list[obj1.first]) {

			// 衝突物が入っているかどうか
			if (mp_collision_list.count(obj2) == 0) {
				continue;
			}

			// 現在の衝突物として登録
			m_current_obj_type1 = obj1.first;
			m_current_obj_type2 = obj2;

			// 組み合わせで当たり判定を開始
			StartCollisionComb(obj1.first, obj2);
		}
	}

	// 初期化
	m_current_obj_type1 = CollisionObjectType::NONE;
	m_current_obj_type2 = CollisionObjectType::NONE;
}


void CollisionManager::StartCollisionComb(
	const CollisionObjectType&type1,
	const CollisionObjectType&type2
) {

	for (auto obj1 : mp_collision_list[type1])
	{
		for (auto obj2 : mp_collision_list[type2]) {

			// 組み合わせで当たり判定を行う
			SelectCollider(obj1, obj2);
		}
	}
}


void CollisionManager::SelectCollider(
	std::weak_ptr<CollisionObject3DBase>obj1,
	std::weak_ptr<CollisionObject3DBase>obj2
) {

	// 形状代入
	ShapeType type1 = obj1.lock()->GetShapeType();
	ShapeType type2 = obj2.lock()->GetShapeType();

	switch (type1)
	{

		// 球
	case ShapeType::SPHERE:

		switch (type2) {

			// 球
		case ShapeType::SPHERE:
			ChackSphereAndSphere(obj1, obj2);
			break;
		}

	//// キューブ
	//case ShapeType::CUBE:
	//
	//	switch (type2) {
	//
	//	case ShapeType::CUBE:
	//		ChackCubeAndCube(obj1, obj2);
	//		break;
	//	}
	}
}


void CollisionManager::ChackSphereAndSphere(
	std::weak_ptr<CollisionObject3DBase>s1,
	std::weak_ptr<CollisionObject3DBase>s2){

	Sphere sphere1;
	Sphere sphere2;

	std::shared_ptr<CollisionObject3DBase>p_s2 =
		s2.lock();

	std::shared_ptr<CollisionObject3DBase>p_s1 =
		s1.lock();

	if (p_s2 == nullptr) {
		return;
	}
	if (p_s1 == nullptr) {
		return;
	}

	// 球情報受け取り
	sphere1 = p_s1->GetSphere();
	sphere2 = p_s2->GetSphere();

	// 球の当たり判定
	if (Collision::IsHitSphereAndSphere(
		p_s1->GetPos(),
		p_s2->GetPos(),
		sphere1.radian,
		sphere2.radian)
		 == true) {

		// 衝突したのでそれぞれの衝突処理に入る
		// それぞれのオブジェクト定数も送る
		p_s1->HitAction(m_current_obj_type2);
		p_s2->HitAction(m_current_obj_type1);
	}
}


void CollisionManager::ChackCubeAndCube(
	std::weak_ptr<CollisionObject3DBase>s1,
	std::weak_ptr<CollisionObject3DBase>s2
) {

	Cube cube1;
	Cube cube2;


	std::shared_ptr<CollisionObject3DBase>p_s2 =
		s2.lock();

	std::shared_ptr<CollisionObject3DBase>p_s1 =
		s1.lock();

	// 球情報受け取り
	cube1 = p_s1->GetCube();
	cube2 = p_s2->GetCube();

	// 球の当たり判定
	if (Collision::IsHitCubeAndCube(
		cube1.m_rect_3d,
		cube2.m_rect_3d
	) == true) {

		// 衝突したのでそれぞれの衝突処理に入る
		// それぞれのオブジェクト定数も送る
		p_s1->HitAction(m_current_obj_type2);
		p_s2->HitAction(m_current_obj_type1);
	}
}


void CollisionManager::CheckAndClear() {

	// 削除する前にIsActiveを調べてfalseなら
	// コンビネーションの削除とポインタ配列の削除

	// 削除
	for (auto get : mp_collision_list) {

		for (auto it = mp_collision_list[get.first].begin();
			it != mp_collision_list[get.first].end();) {

			// 活動していないなら
			if ((*it).lock()->IsActive() == false) {

				// 要素削除、次の要素受け取り
				it = mp_collision_list[get.first].erase(it);
			}
			else
			{
				it++;
			}

		}
	}		
}
