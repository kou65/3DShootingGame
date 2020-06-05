#include"GoalObject.h"
#include"../../Lib/3D/OBJ/OBJFile.h"



GoalObject::GoalObject(const Vec3&pos) {
	m_pos = pos;
}


void GoalObject::Update() {

}


void GoalObject::Draw() {

	ObjParameter param(DrawStatus::NORMAL);

	// 板
	param.register_obj_file_name = Const::Obj::PLANE;

	// 板のテクスチャ
	//param.texture_name = Const::Graph::TAILE;
	param.pos = m_pos;

	// 縦向き
	param.rota.y = 180.f;
	param.rota.x = 90.f;

	param.scale.x = 32.f;
	param.scale.y = 32.f;

	// 赤カラー
	param.color.y = 1.f;

	// obj描画
	Obj::GetInstance()->DrawObjByNormalShader(param);
}


void GoalObject::Destory() {

	m_is_active = false;
}
