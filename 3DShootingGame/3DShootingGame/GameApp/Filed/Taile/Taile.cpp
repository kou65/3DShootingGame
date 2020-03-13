#include"Taile.h"




Taile::Taile(
	const ObjParameter&data,
	const Direction &dir) {

	m_data = data;

	SetDirection(dir);
}


void Taile::Update() {

}


void Taile::Draw() {

	Obj::GetInstance()->ShaderDraw(m_data);
}


void Taile::SetDirection(const Direction&dir) {

	switch (dir) {

	case Direction::LEFT:


		m_data.rota.z = -90.f;
		m_data.rota.x = -90.f;

		m_data.scale.z = 8;
		m_data.pos.y += m_data.scale.z;
		m_data.pos.x += 4;
	

		break;

	case Direction::RIGHT:

		m_data.rota.z = 90.f;
		m_data.rota.x = -90.f;

		m_data.scale.z = 8;
		m_data.pos.y += m_data.scale.z;
		m_data.pos.x -= 4;
		m_data.scale.y = 8;

		break;
	}
}