#ifndef ENEMY_H
#define ENEMY_H

#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../GameConstant/GameConstant.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"



/**
* @brief 通常敵クラス
*/
class Enemy : public CollisionObject3DBase{
public:


	//! 横移動
	const int WIDTH_MOVE_RIGHT = 200;
	const int WIDTH_MOVE_LEFT = 100;
	
public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	* @param[in] speed 速度
	*/
	Enemy(
		D3DXVECTOR3 pos,
		float speed);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief 衝突後の処理
	* @param[in] type 相手の衝突物オブジェクト種類
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 球情報を返す
	* @Sphere 球構造体を返す
	*/
	Sphere GetSphere()override;

private:

	/**
	* @brief 移動
	*/
	void Move() {

		// 左まで来たら
		if (m_pos.x <= WIDTH_MOVE_LEFT) {
			
			// 反転
			m_speed *= -1.f;
		}
		else if (m_pos.x <= WIDTH_MOVE_RIGHT) {
			// 反転
			m_speed *= -1.f;
		}

		// 移動加算
		m_pos.x += m_speed;
	}

private:

	//! 速度
	float m_speed;
};

#endif