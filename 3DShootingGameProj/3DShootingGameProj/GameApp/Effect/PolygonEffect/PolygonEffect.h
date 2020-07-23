#pragma once
#include"../../../ObjectSystem/Object3D/Object3D.h"



/**
* @brief ポリゴンが出て消えるエフェクト
*/
class PolygonEffect : public Object3D{
public:


	/**
	* @brief コンストラクタ
	*/
	PolygonEffect(
		const Vec3&pos,
		const Vec3&color = Vec3(1.f,1.f,1.f)
	);


	/**
	* @brief 更新
	*/
	void Update()override;


	/**
	* @brief 描画
	*/
	void Draw()override;

private:

	// α値
	float m_alpha;

	Vec3 m_pos_list[3];

	D3DXVECTOR4 m_color;
};