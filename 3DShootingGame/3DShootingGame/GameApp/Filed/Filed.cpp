#include"Filed.h"
#include"../../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../../Lib/3D/Sprite3D/Sprite3D/Sprite3DUser.h"




Filed::Filed(ObjectFactory*m_p_factory) {

	this->m_p_factory = m_p_factory;

	CreateTaile();
}


Filed::Filed(ObjectFactory*m_p_factory, CharacterBase*p_chara) {

	this->m_p_factory = m_p_factory;
	m_p_chara = p_chara;

	CreateTaile();
}


void Filed::Update(){

}


void Filed::Draw() {

	DrawWideTaile();

}


void Filed::CreateAxis(CharacterBase*p_chara) {
	m_p_chara = p_chara;
}


void Filed::DrawWideTaile() {

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



void Filed::CreateRightWall() {

	const int FRONT = 6;

	for (int i = 0; i < FRONT; i++) {

		TransformData3D data;
		//data.pos.x = ;

		m_p_factory->CreateRightWall(data);
	}
}


void Filed::CreateLeftWall() {

}


void Filed::CreateTaile() {

	if (m_p_chara == nullptr) {
		return;
	}

	ObjParameter data;
	Vec3 scale;

	scale.x = TAILE_SIZE;
	scale.z = TAILE_SIZE;

	// 現在タイル位置を割り出す
	int current_taile_pos =
		(int)(m_p_chara->GetPos().z * (1.f / TAILE_SIZE));


	// 前方のタイル範囲
	int create_range_z = 
		static_cast<int>(
		(CREATE_RANGE_F_Z * (1.f / TAILE_SIZE))
			);

	// 板
	data.register_obj_file_name = Const::Obj::PLANE;
	data.texture_name = Const::Graph::TAILE;

	// 全て生成して、描画だけ制限する

		// 生成範囲を厳選
	for (int j = current_taile_pos;
		j < current_taile_pos + create_range_z;
		j++) {

		// 横のタイル生成
		for (int i = 0; i < TOTAL_TAILE_WIDTH; i++) {

			// 横
			data.pos.x = (i + 1) * (scale.x * 2) + 
				(-(TAILE_SIZE * 4));

			// 縦
			data.pos.z = (j + 1) + scale.z + (TAILE_SIZE * 3);

			// 拡縮
			data.scale = scale;

			MapObjectBase*p_map_obj;

			// 床生成
			m_p_factory->CreateTaile(data,p_map_obj);

			// オブジェクトに追加
			m_p_map_obj_list.emplace_back(p_map_obj);
		}
	}
}