#include"MapStructureManager.h"


// マップ構造
MapStructureManager::MapStructureManager(
	std::shared_ptr<ObjectFactory>m_p_factory
) {

	this->m_p_factory = m_p_factory;

	Init();
}


MapStructureManager::MapStructureManager(
	std::shared_ptr<ObjectFactory>m_p_factory, 
	std::shared_ptr<ObjectData>p_data
) {

	this->m_p_factory = m_p_factory;
	m_p_chara = p_data->p_player;

	Init();
}


void MapStructureManager::Update(){

	SerchCreateDeleteTaile();
}


void MapStructureManager::CreateAxis(CharacterBase*p_chara) {
	m_p_chara = p_chara;
}


void MapStructureManager::Init() {

	if (m_p_chara == nullptr) {
		return;
	}

	// 配列確保
	m_p_map_obj_list.resize(TOTAL_FRONT_OBJ);
	m_is_create_list.resize(TOTAL_FRONT_OBJ);

	// 配列確保
	for (auto &obj : m_p_map_obj_list){
		obj.resize(TOTAL_TAILE_OBJ);
	}

	// 配列確保
	for (auto&is : m_is_create_list) {
		is.resize(TOTAL_TAILE_OBJ);
	}

	for (int i = 0; i < TOTAL_FRONT_OBJ;i++) {
		for (int j = 0; j < TOTAL_TAILE_OBJ; j++) {
			m_is_create_list[i][j] = false;
		}
	}

}


void MapStructureManager::SerchCreateDeleteTaile() {


	// 自分の近くのオブジェクトを走査し、
	// 生成するものはnullptrでチェックし、生成
	// 削除するものはnullptrでチェックし、削除

	ObjParameter data;
	Vec3 scale;

	// 拡縮
	scale.x = Taile::TAILE_SIZE_X;
	scale.z = Taile::TAILE_SIZE_Y;

	// 板
	data.register_obj_file_name = Const::Obj::PLANE;
	// 板のテクスチャ
	data.texture_name = Const::Graph::TAILE;


	// 現在タイル位置を割り出す
	int back_taile = 
		(int)((int)m_p_chara->GetPos().z / (Taile::TAILE_SIZE_X * 2));

	// 後方のタイル範囲
	back_taile -= CREATE_RANGE_BACK;

	if (back_taile < 0) {
		back_taile = 0;
	}

	// 前方のタイル範囲
	int front_taile =
		(int)(back_taile + CREATE_RANGE_FRONT);

	if (front_taile < 0) {
		front_taile = 0;
	}
	if (front_taile > TOTAL_FRONT_OBJ) {
		front_taile = TOTAL_FRONT_OBJ;
	}

	// 共有ポインタへ変換
	std::shared_ptr<ObjectFactory>p_factory = 
		m_p_factory.lock();

	// 生成範囲を厳選
	for (int i = back_taile;
		i < front_taile;
		i++)
	{

		// 横のタイル生成
		for (int j = 0; j < TOTAL_TAILE_OBJ; j++) {

			const float FIX_X_POS = (-Taile::TAILE_SIZE_X * 2);

			// 横
			data.pos.x = (j + 1) * (Taile::TAILE_SIZE_X * 2)
				+ FIX_X_POS;

			// 縦
			data.pos.z = (i + 1) * (Taile::TAILE_SIZE_Y * 2);

			// 拡縮
			data.scale = scale;

			Object3D*p_map_obj = nullptr;

			// 生成されていないなら
			if (m_is_create_list[i][j] == false){


				switch (WIDHT_FILED_OBJ[j]) {

				case TaileType::TAILE:

					// 床生成,参照受け取り
					p_factory->CreateTaile(
						data,
						&m_p_map_obj_list[i][j]);
					break;


				case TaileType::LEFT_WALL:

					// 床生成,参照受け取り
					p_factory->CreateTaile(
						data,
						&m_p_map_obj_list[i][j],
						Taile::Direction::LEFT
					);
					break;

				case TaileType::RIGHT_WALL:

					// 床生成,参照受け取り
					p_factory->CreateTaile(
						data,
						&m_p_map_obj_list[i][j],
						Taile::Direction::RIGHT
					);
					break;
				}

				m_is_create_list[i][j] = true;
			}
		}
	}

	const int FRONT_BACK = 2;
	
	// 前方情報と後方情報を合成
	int front_back[FRONT_BACK] = {
		front_taile + 1,
		back_taile - 1,
	};

	for (int i = 0; i < FRONT_BACK; i++) {

		// 横
		for (int j = 0; j < TOTAL_TAILE_OBJ; j++) {

			// 前方の削除領域を出す
			int fb = front_back[i];

			// 範囲外は何もなし
			if (fb < 0 || fb > TOTAL_FRONT_OBJ) {
				break;
			}

			// 生成されているなら
			if (m_is_create_list[fb][j] == true){

				// オブジェクトリストから削除
				m_p_map_obj_list[fb][j]->NotActive();

				m_is_create_list[fb][j] = false;
			}
		}
	}
}