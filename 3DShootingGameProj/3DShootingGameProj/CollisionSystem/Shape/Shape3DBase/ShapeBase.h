#pragma once
#include"../ShapeData/ShapeData.h"
#include"../../../Lib/Vec3/Vec3.h"



/**
* @brief 形状基底クラス
*/
struct Shape3DBase {


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	*/
	Shape3DBase(const Vec3 &pos);


	/**
	* @brief コンストラクタ
	* @param[in] x x座標
	* @param[in] y y座標
	* @param[in] z z座標
	*/
	Shape3DBase(
		const float&x,
		const float&y,
		const float&z
	);

	
	/**
	* @brief デフォルトコンストラクタ
	*/
	Shape3DBase() {};


	/**
	* @brief 形状を返す
	*/
	ShapeType GetShapeType();


	/**
	* @brief 位置を返す
	*/
	Vec3 GetPos();


	//! 位置
	Vec3 vec;

	//! 形状の種類
	ShapeType m_shape_type;
};