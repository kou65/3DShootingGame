#pragma once
#include"../../Shape/Shape3DBase/ShapeBase.h"



/**
* @brief 球形状
*/
struct Sphere : public Shape3DBase{


	/**
	* @brief コンストラクタ
	*/
	Sphere() {};

	
	/**
	* @brief 半径取得
	*/
	float GetRadian();


	/**
	* @brief 半径
	*/
	float radian;
};