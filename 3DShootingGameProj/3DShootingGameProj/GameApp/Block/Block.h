﻿#pragma once
#include"../Filed/MapObjectBase/MapObjectBase.h"
#include"../../Factory/EffectFactory/EffectFactory.h"


// 障害物用キューブ


/**
* @brief 障害物用キューブ
*/
class Block : public MapObjectBase{
public:

	//! オブジェクトサイズ
	static constexpr float OBJ_SIZE = 2.f;

public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	*/
	Block(
		EffectFactory*p_factory,
		const Vec3&pos
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画　仮想関数
	*/
	void Draw()override;


	/**
	* @brief zテクスチャ描画 仮想関数
	*/
	void Block::DrawZTexture()override;


	/**
	* @brief 衝突後処理 仮想関数
	* @param[in] type CollisionObjectType 衝突先のオブジェクト名定数
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 設定して球情報を返す 仮想関数
	* @return Sphere 球構造体
	*/
	Sphere GetSphere()override;


	/**
	* @brief 設定してキューブ情報を返す 仮想関数
	* @return Cube キューブ構造体
	*/
	Cube GetCube()override;

private:

	EffectFactory *mp_factory;
};