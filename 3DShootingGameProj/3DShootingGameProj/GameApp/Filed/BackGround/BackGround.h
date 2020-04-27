#ifndef BACK_GROUND_H
#define BACK_GROUND_H

#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../Lib/Vec3/Vec3.h"



/**
* @brief 背景クラス
*/
class BackGround : public Object3D{
public:


	/**
	* @brief コンストラクタ
	*/
	BackGround(const Vec3&pos);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;

private:

	/**
	* @brief 広い床を描画
	*/
	void DrawWideTaile();

private:

	//! オブジェクトパラメータ
	ObjParameter m_param;
};

#endif