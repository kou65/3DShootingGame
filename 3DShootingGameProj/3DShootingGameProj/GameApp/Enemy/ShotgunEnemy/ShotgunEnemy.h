#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../ObjectSystem/ObjectFactory/ObjectFactory.h"



class ShotgunEnemy : public EnemyBase{
public:

	//! 弾の速度
	const float BULLET_SPEED = 2.f;

	//! 弾の撃つ間隔
	const float INTERVAL = 30.f;

public:


	/**
	* @brief コンストラクタ
	* @param[in] create_pos 初期位置
	* @param[in] p_factory 工場ポインタ
	* @param[in] p_chara キャラクターポインタ
	*/
	ShotgunEnemy(
		const Vec3&create_pos,
		ObjectFactory *p_factory,
		std::weak_ptr<CharacterBase>p_chara
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief 衝突後処理
	* @param[in] type 衝突物オブジェクト種類
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 球を返す
	* @return Sphere 球情報構造体
	*/
	Sphere GetSphere()override;

private:

	/**
	* @brief 撃てるかどうかを返す
	* @return bool
	*/
	bool IsShot();


	/**
	* @brief 撃つ
	*/
	void Shot();

private:

	// !弾生成工場
	ObjectFactory *mp_obj_factory;

	// !プレイヤー
	std::weak_ptr<CharacterBase>mp_player;

	// !打ち出す時間
	float m_shot_timer;
};