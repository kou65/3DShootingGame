#include"Taile.h"




Taile::Taile(const ObjParameter&data) {

	m_data = data;
}



void Taile::Update() {

}


void Taile::Draw() {

	Obj::GetInstance()->ShaderDraw(m_data);
}