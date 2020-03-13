#include"BackGround.h"



BackGround::BackGround(const Vec3&pos) {

	m_param.pos = pos;

	m_param.register_obj_file_name = Const::Obj::PLANE;
	m_param.texture_name = Const::Graph::BACK_GROUND;

	m_param.scale.x = 3000.f;
	m_param.scale.z = 1600.f;

	m_param.pos.y = m_param.scale.z;

	m_param.rota.x = 90.f;
	m_param.rota.y = 0.f;
	m_param.rota.z = 180.f;
}

void BackGround::Update() {

}


void BackGround::Draw() {

	Obj::GetInstance()->ShaderDraw(m_param);
}