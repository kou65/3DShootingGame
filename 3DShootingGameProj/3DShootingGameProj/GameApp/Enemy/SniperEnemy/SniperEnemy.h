#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../Factory/ObjectFactory/ObjectFactory.h"
#include"../../Player/Player.h"
#include"../../../Factory/EffectFactory/EffectFactory.h"



/**
* @brief 自機方向に弾を撃つ敵クラス
*/
class SniperEnemy : public EnemyBase{
public:

	//! 撃つ間隔
	const float INTERVAL = 60.f;

	//! 弾の速度
	const float BULLET_SPEED = 1.f;

public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 初期位置
	* @param[in] p_factory 工場ポインタ
	* @param[in] p_chara キャラクタポインタ
	*/
	SniperEnemy(
		const Vec3&pos,
		ObjectFactory *p_factory,
		std::weak_ptr<CharacterBase>p_chara,
		EffectFactory*p_effect
	);


	/**
	* @brief 更新
	*/
	void Update()override;


	/**
	* @brief 描画
	*/
	void Draw()override;

	// 衝突後処理
	/**
	* @brief 衝突後処理
	* @param[in] type 衝突オブジェクト種類
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 球を返す
	* @return Sphere 球を返す
	*/
	Sphere GetSphere()override;

private:

	/**
	* @brief 打てるかどうかを返す
	* @return bool
	*/
	bool IsShot();


	/**
	* @brief 方向計算
	* @return Vec3 方向計算結果
	*/
	Vec3 CalcDirection();


	/**
	* @brief 撃つ
	*/
	void Shot();


	/**
	* @brief 時間計算
	*/
	void AddShotTimer();

private:

	//! 弾生成工場
	ObjectFactory *mp_obj_factory;

	//! エフェクト工場
	EffectFactory *mp_effect;

	//! プレイヤー
	std::weak_ptr<CharacterBase>mp_player;

	//! 打ち出す時間
	float m_shot_timer;
};