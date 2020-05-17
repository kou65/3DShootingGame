#include"BackGround.h"
#include"../../../Lib/3D/Sprite3D/Sprite3D/Sprite3DUser.h"




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

	Obj::GetInstance()->DrawObjByNormalShader(m_param);
	DrawWideTaile();
}



void BackGround::DrawWideTaile() {

	Sprite3DParameter ground(0.f, 0.f, 0.f, "ground");

	ground.scale_width = 10000.f;
	ground.scale_height = 10000.f;
	ground.polygon_dir = FLOOR;
	ground.pos.y = -5.f;
	ground.ofset.x = 0.5f;
	ground.ofset.y = 0.5f;

	Sprite3DUser sprite_obj;


	sprite_obj.BoardDraw(ground);
}
