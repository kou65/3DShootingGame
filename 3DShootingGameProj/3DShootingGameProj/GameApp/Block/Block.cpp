#include"Block.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../Manager/CollisionManager/CollisionManager.h"
#include"../../Lib/RenderState/RenderState.h"



Block::Block(
	EffectFactory*p_factory,
	const Vec3&pos
) {

	mp_factory = p_factory;
	m_pos = pos;

	// タグを追加
	m_object_tag = Object3DTag::BLOCK;

	// 形を登録
	m_shape_type = ShapeType::SPHERE;
}


void Block::Update() {

}


void Block::Draw() {

	ObjParameter param;

	param.register_obj_file_name = Const::Obj::CUBE;
	param.pos = m_pos;

	param.scale.x = OBJ_SIZE;
	param.scale.y = OBJ_SIZE;
	param.scale.z = OBJ_SIZE;

	bool is = RenderState::IsAlphaEnable();
	RenderState::AlphaEnable(FALSE);

	// 一旦コメント
	//Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW,param);
	Obj::GetInstance()->Draw(DrawStatus::LIGHT, param,8);
	//Obj::GetInstance()->DrawObjByNormalShader(param);

	RenderState::AlphaEnable(is);
}


void Block::DrawZTexture() {

	ObjParameter param(DrawStatus::LIGHT_SHADOW);

	param.register_obj_file_name = Const::Obj::CUBE;
	param.pos = m_pos;

	// 拡縮
	param.scale.x = OBJ_SIZE;
	param.scale.y = OBJ_SIZE;
	param.scale.z = OBJ_SIZE;


	bool is = RenderState::IsAlphaEnable();

	RenderState::AlphaEnable(FALSE);

	// zテクスチャ描画
	Obj::GetInstance()->WriteZTexture(
		param,
		FuncZTexture::Const::Z_TEX_1024
	);


	RenderState::AlphaEnable(is);
}


void Block::HitAction(const CollisionObjectType&type) {


	switch (type) {
		// 破壊弾だったら消す
	case CollisionObjectType::BREAK_BULLET:
		//Exit();
		m_is_active = false;
		mp_factory->CreatePolygonEffect(m_pos);
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
