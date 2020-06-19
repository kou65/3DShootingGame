#pragma once
#include"../LightShader/LightShader.h"



/**
* @brief ソフトシャドウ
*/
class SoftShadow : public Light{
public:


	/**
	* @brief コンストラクタ
	*/
	SoftShadow();


	/**
	* @brief 更新
	*/
	void Update()override {};


	/**
	* @brief 描画開始
	*/
};