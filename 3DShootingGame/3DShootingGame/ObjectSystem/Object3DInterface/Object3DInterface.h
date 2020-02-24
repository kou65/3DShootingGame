#pragma once
#include"../Object3D/Object3D.h"


// オブジェクトを渡す時に間に挟んで隠蔽化を行うクラス


class Object3DInterface {
public:


	/**
	* @brief コンストラクタ
	*/
	Object3DInterface(Object3D*object);


	/**
	* @brief 位置を返す
	* @return D3DXVECTOR2
	*/
	Vec3 GetPos();


private:

	//! オブジェクト
	Object3D * m_p_object;
};