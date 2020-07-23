#include"../PolygonEffect/PolygonEffect.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../Lib/Utility/Random/Random.h"
#include<random>



PolygonEffect::PolygonEffect(
	const Vec3&pos,
	const Vec3&color
) {

	m_color = D3DXVECTOR4(color.x,color.y,color.z,1.f);
	m_alpha = 0.f;
	m_pos = pos;

	float rand = 0.f;
	rand = 
		Utility::Random::GetRealRandom(-1.f, 1.f);

	Vec3 l_pos;

	float len_x = 4.f;
	float len_y = 4.f;

	for (int i = 0; i < 3; i++) {

		l_pos.x = m_pos.x + rand * len_x;
		l_pos.y = m_pos.y + rand * len_y;
		l_pos.z = m_pos.z;

		m_pos_list[i] = l_pos;
	}
}


void PolygonEffect::Update() {

	if (m_alpha > 1.f) {
		m_is_active = false;
	}

	// α値
	m_alpha += 0.1f;
}


void PolygonEffect::Draw() {

	for (int i = 0; i < 3; i++) {

		ObjParameter param(DrawStatus::NORMAL);

		// 板
		param.register_obj_file_name = Const::Obj::PLANE;

		// 板のテクスチャ
		//param.texture_name = Const::Graph::TAILE;
		param.pos = m_pos_list[i];

		// 縦向き
		param.rota.x = -90.f;
		param.scale.x = 1.5f;
		param.scale.z = 1.5f;

		// 透過させる
		m_color.w = 1.f - m_alpha;

		param.color = m_color;


		// obj描画
		Obj::GetInstance()->DrawObjByNormalShader(param);
	}
}