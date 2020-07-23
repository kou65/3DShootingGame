#pragma once
#include"../../CollisionSystem/Shape/Shape3DBase/ShapeBase.h"
#include"../../CollisionSystem/CollisionObjectData/CollisionObjectData.h"
#include"../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include<vector>
#include<queue>
#include<unordered_map>
#include<memory>




/**
* @brief 当たり判定管理者クラス
*/
class CollisionManager {
public:


	/**
	* @brief シングルトン
	*/
	static CollisionManager *GetInstance() {
		static CollisionManager mng;
		return &mng;
	}

public:


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 追加
	* @param[in] type 衝突するオブジェクトの種類定数
	* @param[in] obj 衝突させるオブジェクトを入れる
	*/
	bool Entry(
		const CollisionObjectType&type,
		std::weak_ptr<CollisionObject3DBase>obj
	);


	/**
	* @brief 全て解放
	*/
	void AllRelease();


	/**
	* @brief 調べて配列削除(必ず全ての更新の最後に記述) 
	*/
	void CheckAndClear();


	/**
	* @brief 全ての配列を削除 
	*/
	void Clear();

private:


	/**
	* @brief コンストラクタ
	*/
	CollisionManager();


	/**
	* @brief コンビネーションを選択 
	*/
	void SetCombination();


	/**
	* @brief 挿入
	*/
	void InsertToCollisionList();


	/**
	* @brief グループごとに衝突させる 
	*/
	void CollisionGroup();


	/**
	* @brief 当たり判定
	* @param[in] obj1 衝突させるオブジェクト1
	* @param[in] obj2 衝突させるオブジェクト2
	*/
	void SelectCollider(
		std::weak_ptr<CollisionObject3DBase>obj1,
		std::weak_ptr<CollisionObject3DBase>obj2
	);


	/**
	* @brief 組み合わせ同士当たり判定を行う
	* @param[in] type1 組み合わせるオブジェクト定数1
	* @param[in] type2 組み合わせるオブジェクト定数2
	*/
	void StartCollisionComb(
		const CollisionObjectType&type1,
		const CollisionObjectType&type2
	);


private:

	// 当たり判定

	/**
	* @brief 球と球
	* @param[in] o1 衝突オブジェクト1
	＊@param[in] o2 衝突オブジェクト2
	*/
	void ChackSphereAndSphere(
		std::weak_ptr<CollisionObject3DBase>o1,
		std::weak_ptr<CollisionObject3DBase>o2
	);


	/**
	* @brief キューブとキューブ
	* @param[in] o1 衝突オブジェクト1
	* @param[in] o2 衝突オブジェクト2
	*/
	void ChackCubeAndCube(
		std::weak_ptr<CollisionObject3DBase>o1,
		std::weak_ptr<CollisionObject3DBase>o2
	);

private:

	//! 当たり判定オブジェクトリスト
	std::unordered_map<CollisionObjectType,
		std::vector<std::weak_ptr<CollisionObject3DBase>>>mp_collision_list;

	//! 組み合わせリスト
	std::unordered_map<CollisionObjectType,
		std::vector<CollisionObjectType>>m_group_list;

	//! 代入用配列
	std::vector<CollisionObjectType>m_insert_type_list;
	std::vector<std::weak_ptr<CollisionObject3DBase>>mp_insert_collision_list;

	//! 現在の衝突物の種類(衝突物の識別の為必要)
	CollisionObjectType m_current_obj_type1;
	CollisionObjectType m_current_obj_type2;
};