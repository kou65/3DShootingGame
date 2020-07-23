#pragma once
#include"../Shape3DBase/ShapeBase.h"
#include"../../../Lib/Utility/Collision/Collision.h"



/**
* @brief 3D矩形衝突用のキューブクラス
*/
struct Cube : public Shape3DBase {

	/**
	* @brief コンストラクタ
	*/
	Cube() {};

	//! 3D矩形
	Rect3D m_rect_3d;
};