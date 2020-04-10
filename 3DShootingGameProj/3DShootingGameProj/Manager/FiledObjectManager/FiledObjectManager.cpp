#include"../FiledObjectManager/FiledObjectManager.h"



FiledObjectManager::FiledObjectManager(
	FileObjectDataManager*p_manager,
	ObjectFactory*p_factory,
	ObjectData*p_player
) {

	// フィールドオブジェクト
	FileObject f_obj;

	f_obj.is_active = false;

	// 生成ライン設定
	f_obj.create_line = FRONT_LINE;
	f_obj.destory_line = BACK_LINE;

	// 弾を撃つ敵に追加
	for (int i = 0; i < p_manager->GetMaxPosListNum(
		Const::FileObj::BULLET_ENEMY
	); i++) {

		// 位置を戻す
		f_obj.pos = p_manager->OutPosList(
			Const::FileObj::BULLET_ENEMY, i
		);

		// リソース位置追加
		m_pos_list[Object3DTag::BULLET_ENEMY].emplace_back
		(
			f_obj
		);
	}

	if (m_pos_list[Object3DTag::BULLET_ENEMY].size() != 0) {
		// 撃つ敵リスト
		m_p_enemy_list.resize(
			m_pos_list[Object3DTag::BULLET_ENEMY].size()
		);
	}
	
	// 弾を撃つ敵に追加
	for (int i = 0; i < p_manager->GetMaxPosListNum(
		Const::FileObj::CUBE
	); i++) {

		f_obj.pos = (p_manager->OutPosList(
			Const::FileObj::CUBE, i));
	
		// リソース位置追加
		m_pos_list[Object3DTag::CUBE].emplace_back(
			f_obj
			);
	}
	
	if (m_pos_list[Object3DTag::CUBE].size() != 0) {
		// キューブ
		m_p_cube_list.resize(
			m_pos_list[Object3DTag::CUBE].size()
		);
	}

	// ファクトリー
	m_p_factory = p_factory;

	// キャラ代入
	m_p_chara = p_player;
}


void FiledObjectManager::Update() {

	// 敵生成と削除
	Create();
	Destory();
}


void FiledObjectManager::Create() {

	int i = 0;

	// キャラクターを代入
	CharacterBase m_chara = *m_p_chara->p_player;

	for (auto&tag : m_pos_list) {
	
		// 生成
		for (auto&filed_obj : m_pos_list[tag.first]) {
	
			Vec3 pos = filed_obj.pos;
	
			// 前方の生成位置を確定
			float front_pos = m_chara.GetPos().z + 
				filed_obj.create_line;

			float back_pos = m_chara.GetPos().z +
				filed_obj.create_line - 10.f;
	
			// 範囲に入ったら生成
			if (front_pos >= pos.z && back_pos <= pos.z) {

				// メモリが入っていないのを確認したら
				if (filed_obj.is_active == false) {
	
					// タグによる生成
					FileObjCreate(tag.first, pos, i);
					// 生成された
					filed_obj.is_active = true;
				}
			}
		}
	}
}


void FiledObjectManager::Destory() {


	int i = 0;

	// zサイズ
	const float enemy_size_z = 16.f;

	// キャラクターを代入
	CharacterBase m_chara = *m_p_chara->p_player;

	for (auto&tag : m_pos_list) {
	
		// 生成
		for (auto&filed_obj : m_pos_list[tag.first]) {
	
			Vec3 pos = filed_obj.pos;

			// 前方の削除位置を確定
			float front_pos = m_chara.GetPos().z +
				filed_obj.destory_line;

			// 後方の削除位置を確定
			float back_pos = m_chara.GetPos().z + 
				filed_obj.destory_line - 20.f;
	
			// 削除
			if (front_pos >= pos.z && back_pos <= pos.z) {
	
				// 生成されているなら
				if (filed_obj.is_active == true) {
	
					// 削除
					FileObjDestory(tag.first, pos, i);
	
					// 活動停止
					filed_obj.is_active = false;
				}
			}
		}
	}

}


void FiledObjectManager::FileObjCreate(
	const Object3DTag&tag,
	Vec3&pos,
	int& array_num
) {


	// タグによって生成するものを入れ替える
	switch (tag) {

	case Object3DTag::BULLET_ENEMY:

		// キャラクター情報を渡す
		m_p_factory->CreateHEnemy(
			pos,
			m_p_chara->p_player,
			&m_p_enemy_list[array_num]
		);
		break;

	case Object3DTag::CUBE:

		// キューブオブジェクト生成
		m_p_factory->CreateCube(
			pos, 
			&m_p_cube_list[array_num]
		);

		break;
	}

}



void FiledObjectManager::FileObjDestory(
	const Object3DTag&tag,
	Vec3&pos,
	int& array_num
) {

	// タグによって生成するものを入れ替える
	switch (tag) {

	case Object3DTag::BULLET_ENEMY:

		// 敵オブジェクト削除
		m_p_enemy_list[array_num]->Destory();
		break;

	case Object3DTag::CUBE:

		// キューブオブジェクト削除
		m_p_cube_list[array_num]->Destory();

		break;
	}
}