#pragma once
#include"../Shape/Shape3DBase/ShapeBase.h"
#include"../CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../Lib/SingletonTemplate/SingletonTemplate.h"
#include<vector>
#include<queue>
#include<unordered_map>



class CollisionManager {
public:

	// シングルトン
	static CollisionManager *GetInstance() {
		static CollisionManager mng;
		return &mng;
	}

public:

	// 更新
	void Update();

	// 追加
	bool Entry(
		const CollisionObjectType&type,
		CollisionObject3DBase*obj
	);

	// 削除
	void Clear();

private:

	// コンストラクタ
	CollisionManager();

	// コンビネーションを選択
	void SetCombination();

	// 挿入
	void InsertToCollisionList();

	// グループごとに衝突させる
	void CollisionGroup();

	// 当たり判定
	void SelectCollider(
		CollisionObject3DBase * obj1,
		CollisionObject3DBase * obj2
	);

	// 組み合わせ同士当たり判定を行う
	void StartCollisionComb(
		const CollisionObjectType&type1,
		const CollisionObjectType&type2
	);

private:

	// 当たり判定

	// 球と球
	void ChackSphereAndSphere(
		CollisionObject3DBase*s1,
		CollisionObject3DBase*s2
	);

private:

	// 当たり判定オブジェクト
	std::unordered_map<CollisionObjectType,
		std::vector<CollisionObject3DBase*>>m_p_collision_list;

	// 組み合わせリスト
	std::unordered_map<CollisionObjectType,
		std::vector<CollisionObjectType>>m_group_list;

	// 代入用配列
	std::vector<CollisionObjectType>m_insert_type_list;
	std::vector<CollisionObject3DBase*>m_p_insert_collision_list;
};