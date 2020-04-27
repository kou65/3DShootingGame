#pragma once
#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../Shape/Shape3DBase/ShapeBase.h"
#include"../CollisionObjectData/CollisionObjectData.h"
#include"../Shape/Sphere/Sphere.h"
#include"../Shape/Cube/Cube.h"



/**
* @brief 衝突用オブジェクト
*/
class CollisionObject3DBase : public Object3D{
public:

	/**
	* @brief コンストラクタ
	*/
	CollisionObject3DBase() : Object3D(){
		m_shape_type = ShapeType::NONE;
	}


	/*
	* brief 更新
	*/
	void Update()override {};


	/*
	* brief 描画
	*/
	void Draw()override {};


	/*
	* brief 衝突後の動作
	* param[in] obj_type 相手オブジェクト定数
	*/
	virtual void HitAction(
		const CollisionObjectType&obj_type
	){};


	/*
	* @brief 球情報を渡す
	* @brief 使う形状を仮想関数で設定する
	* @brief 形状は派生先で作る
	* @param[out] out_sphere 球形状を返す
	* @return bool 返せたかどうか
	*/
	virtual Sphere GetSphere() {
		Sphere s;

		s.vec.Init();
		s.radian = 0.f;
		return s;
	};


	/*
	* @brief キューブ情報を返す
	*/
	virtual Cube GetCube() {
		Cube c;
		c.vec.Init();
		return c;
	}


	/**
	* @brief 形状定数を返す
	* @return ShapeType
	*/
	ShapeType GetShapeType() {
		return m_shape_type;
	}

protected:

	//! 形状の種類
	ShapeType m_shape_type;
};