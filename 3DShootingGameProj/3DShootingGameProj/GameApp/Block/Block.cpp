#include"Block.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"



Block::Block(const Vec3&pos) {

	m_pos = pos;

	// タグを追加
	m_object_tag = Object3DTag::BLOCK;

	// 衝突に追加
	CollisionManager::GetInstance()->Entry(
		CollisionObjectType::BLOCK, this
	);

	// 形を登録
	m_shape_type = ShapeType::SPHERE;
}


void Block::Update() {

}


void Block::Draw() {


	ObjParameter param(DrawStatus::NORMAL,true);

	param.register_obj_file_name = Const::Obj::CUBE;
	param.pos = m_pos;

	param.scale.x = OBJ_SIZE;
	param.scale.y = OBJ_SIZE;
	param.scale.z = OBJ_SIZE;

	Obj::GetInstance()->DrawObjByNormalShader(param);
}


void Block::HitAction(const CollisionObjectType&type) {


	switch (type) {
		// 破壊弾だったら消す
	case CollisionObjectType::BREAK_BULLET:
		Exit();
		break;
	}
}


Sphere Block::GetSphere() {

	Sphere s;

	s.vec = m_pos;
	s.radian = 20.f;

	return s;
}


Cube Block::GetCube() {

	Cube c;

	c.m_rect_3d.x = m_pos.x;
	c.m_rect_3d.y = m_pos.y;
	c.m_rect_3d.z = m_pos.z;

	c.m_rect_3d.width = 1;
	c.m_rect_3d.height = 1;
	c.m_rect_3d.depth = 1;

	return c;
}
