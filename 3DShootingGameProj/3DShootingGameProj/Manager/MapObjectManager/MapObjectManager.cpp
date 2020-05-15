#include"../MapObjectManager/MapObjectManager.h"
#include"../../GameApp/Filed/ClearBlock/ClearBlock.h"
#include"../../GameApp/Enemy/HomingBulletEnemy/HomingBulletEnemy.h"
#include"../../GameApp/Enemy/ShotgunEnemy/ShotgunEnemy.h"
#include"../../Lib/Utility/Utility.h"



MapObjectManager::MapObjectManager(
	std::shared_ptr<FileDataManager>p_manager,
	std::shared_ptr<ObjectFactory>p_factory,
	std::shared_ptr<ObjectData>p_player
) {

	// マップ初期化
	InitMap();

	// ピース情報を渡す
	InsertPieceFileData(
		p_manager
	);

	// ランダム生成
	RandomNormalCreate();

	// ソートして整理
	Sort();

	// 位置ファイルデータ挿入
	InsertPosFileData(
		p_manager
	);

	// ファクトリー
	m_p_factory = p_factory;

	// キャラ代入
	m_p_chara = p_player;
}


void MapObjectManager::InitMap() {


	// 奥
	m_map.resize(MAX_FRONT_BLOCK);

	for (UINT i = 0; i < m_map.size(); i++) {

		// 縦
		m_map[i].resize(MAX_HEIGHT_BLOCK);

		for (UINT j = 0; j < m_map[i].size(); j++) {

			// 横
			for (UINT k = 0; k < MAX_WIDHT_BLOCK; k++) {

				// 横
				// 空白ブロックを入れておく
				m_map[i][j].emplace_back();
			}
		}
	}
}


void MapObjectManager::InsertPosFileData2(
	std::shared_ptr<FileDataManager>p_manager
) {

	// フィールドオブジェクト
	FileObject f_obj;

	f_obj.is_active = false;

	// 生成ライン設定
	f_obj.create_line = FRONT_LINE;
	f_obj.destory_line = BACK_LINE;

	// 弾を撃つ敵に追加
	FiledObjPosInsert(
		p_manager,
		f_obj,
		Const::FileObj::BULLET_ENEMY,
		Object3DTag::BULLET_ENEMY
	);

}


void MapObjectManager::InsertPosFileData(
	std::shared_ptr<FileDataManager>p_manager
) {

	// フィールドオブジェクト
	FileObject f_obj;

	f_obj.is_active = false;

	// 生成ライン設定
	f_obj.create_line = FRONT_LINE;
	f_obj.destory_line = BACK_LINE;

	// 弾を撃つ敵に追加
	FiledObjPosInsert(
		p_manager,
		f_obj,
		Const::FileObj::BULLET_ENEMY,
		Object3DTag::BULLET_ENEMY
	);

	if (m_world_pos_list[Object3DTag::BULLET_ENEMY].size() != 0) {
		// 撃つ敵リスト
		m_p_h_enemy_list.resize(
			m_world_pos_list[Object3DTag::BULLET_ENEMY].size()
		);
	}

	// 弾を撃つ敵に追加
	FiledObjPosInsert(
		p_manager,
		f_obj,
		Const::FileObj::CUBE,
		Object3DTag::BLOCK
	);

	if (m_world_pos_list[Object3DTag::BLOCK].size() != 0) {
		// キューブ
		m_p_cube_list.resize(
			m_world_pos_list[Object3DTag::BLOCK].size()
		);
	}

	// 弾を撃つ敵に追加
	FiledObjPosInsert(
		p_manager,
		f_obj,
		Const::FileObj::SHOTGUN_ENEMY,
		Object3DTag::SHOTGUN_ENEMY
	);

	if (m_world_pos_list[Object3DTag::SHOTGUN_ENEMY].size() != 0) {
		// キューブ
		m_p_shotgun_enemy.resize(
			m_world_pos_list[Object3DTag::SHOTGUN_ENEMY].size()
		);
	}
}


void MapObjectManager::InsertPieceFileData(
	std::shared_ptr<FileDataManager>p_manager
) {

	// サイズ受け取り
	int piece_list_size = 
		p_manager->GetPieceListSize();

	// mapピースサイズ分全て渡す
	for (int i = 0; i < piece_list_size; i++) {

		// サイズ受け取り
		int piece_num_size =
			p_manager->GetPieceNumListSize(i);

		// ピースサイズ数量分
		for (int j = 0; j < piece_num_size; j++) {

			// ピースを返す
			int ist = p_manager->GetPieceNum(i,j);

			// タグより大きいならなし
			if (ist > (int)PieceTag::TOTAL) {
				continue;
			}

			PieceTag tag = (PieceTag)i;

			// ピースを代入
			m_piece_list[tag].emplace_back(ist);
		}
	}
	
}


void MapObjectManager::Update() {

	// ワールド生成と削除
	CreateWorldObject();
	DestoryWorldObject();

	// 現在ブロック位置計算
	CalcCurrentBlock();

	// マップ生成と削除
	MapCreate();
	MapDestory();
}


void MapObjectManager::FiledObjPosInsert(
	std::shared_ptr<FileDataManager>p_manager,
	FileObject &f_obj,
	const std::string&res_name,
	const Object3DTag&tag
) {

	// 弾を撃つ敵に追加
	for (int i = 0; i < p_manager->GetMaxPosListNum(
		res_name
	); i++) {

		f_obj.pos = (p_manager->GetPosList(
			res_name, i)
			);

		// リソース位置追加
		m_world_pos_list[tag].emplace_back(
			f_obj
		);

	}
}


void MapObjectManager::CreateWorldObject() {

	int array_num = 0;

	std::shared_ptr<ObjectData>p_data =
		m_p_chara.lock();

	// キャラクターを代入
	CharacterBase m_chara = *p_data->p_player;

	// 全てのタグを検索
	for (auto&tag : m_world_pos_list) {

		array_num = 0;

		// 生成
		for (auto&filed_obj : m_world_pos_list[tag.first])
		{

			FileObject obj = filed_obj;

			Vec3 pos =
				filed_obj.pos;

			// 前方の生成位置を確定
			float front_pos = m_chara.GetPos().z +
				obj.create_line;

			float back_pos = m_chara.GetPos().z +
				obj.create_line - 10.f;

			// 範囲に入ったら生成
			if (front_pos >= pos.z &&
				back_pos <= pos.z) {

				// メモリが入っていないのを確認したら
				if (obj.is_active == false) {

					// タグによる生成
					FileObjCreate
					(tag.first, pos, array_num);
					// 生成された
					filed_obj
						.is_active = true;
				}
			}

			// 挿入要素数加算
			array_num++;
		}
	}
}


void MapObjectManager::DestoryWorldObject(
) {

	int array_num = 0;

	std::shared_ptr<ObjectData>p_data = 
		m_p_chara.lock();

	// キャラクターを代入
	CharacterBase m_chara = *p_data->p_player;

	// 全ての位置を回す
	for (auto&tag : m_world_pos_list) {

		array_num = 0;

		for (auto&filed_obj : m_world_pos_list[tag.first])
		{
		
	
			Vec3 pos = filed_obj.pos;

			// 前方の削除位置を確定
			float front_pos = m_chara.GetPos().z +
				filed_obj.destory_line;

			// 後方の削除位置を確定
			float back_pos = m_chara.GetPos().z + 
				filed_obj.destory_line - 20.f;
	
			// 削除
			if (front_pos >= pos.z &&
				back_pos <= pos.z) {
	
				// 生成されているなら
				if (filed_obj.is_active == true) {
	
					// 削除
					FileObjDestory(
						tag.first, pos, array_num
					);
					
					// 活動停止
					filed_obj.is_active = false;
				}
			}

			// 挿入要素数加算
			array_num++;
		}
	}
}


void MapObjectManager::FileObjCreate(
	const Object3DTag&tag,
	Vec3&pos,
	int& array_num
) {

	std::shared_ptr<ObjectData>p_data = m_p_chara.lock();
	std::shared_ptr<ObjectFactory>p_factory = m_p_factory.lock();

	// タグによって生成するものを入れ替える
	switch (tag) {

	case Object3DTag::BULLET_ENEMY:

		// キャラクター情報を渡す
		p_factory->CreateHEnemy(
			pos,
			p_data->p_player,
			&m_p_h_enemy_list[array_num]
		);

		break;

	case Object3DTag::BLOCK:

		// キューブオブジェクト生成
		p_factory->CreateCube(
			pos, 
			&m_p_cube_list[array_num]
		);

		break;

	case Object3DTag::SHOTGUN_ENEMY:

		// キャラクター情報を渡す
		p_factory->CreateShotgunEnemy(
			pos,
			p_data->p_player,
			&m_p_shotgun_enemy[array_num]
		);
		break;
	}
}


void MapObjectManager::FileObjDestory(
	const Object3DTag&tag,
	Vec3&pos,
	int& array_num
) {

	// タグによって生成するものを入れ替える
	switch (tag) {

	case Object3DTag::BULLET_ENEMY:

		// 敵オブジェクト削除
		m_p_h_enemy_list[array_num]->Destory();
		break;

	case Object3DTag::BLOCK:

		// キューブオブジェクト削除
		m_p_cube_list[array_num]->Destory();

		break;

	case Object3DTag::SHOTGUN_ENEMY:

		m_p_shotgun_enemy[array_num]->Destory();

		break;
	}
}



PieceTag MapObjectManager::ResultRandomPieceTag() {

	// 最大数代入
	int min = 0;
	int max = 99;

	// 第一乱数数値取得
	int rand_num = 
		Utility::Random::GetIntRandom(min,max);

	PieceTag tag;

	// ランダムの結果からタグを選択する
	tag = SelectRandomTag((float)rand_num);

	// 乱数の結果を代入
	m_result_rand_list.push_back(rand_num);

	return tag;
}


PieceTag MapObjectManager::SelectRandomTag(
	const float &rand_num
) {

	PieceTag tag = PieceTag::CLEAR;

	// 空白
	if (rand_num < CLEAR_PER) {
		tag = PieceTag::CLEAR;
	}

	//ブロックだけ
	else if (rand_num < BLOCK_PER) {
		tag = PieceTag::CENTER_BLOCK;
	}

	// 敵だけ
	else if (rand_num < ENEMY_PER) {
		tag = PieceTag::ENEMY1;
	}

	// ブロックと敵
	else if (rand_num < BLOCK_ENEMY_PER) {
		tag = PieceTag::ENEMY2;
	}

	else {
		tag = PieceTag::CLEAR;
	}

	return tag;
}



void MapObjectManager::RandomNormalCreate() {

	// 設置パラメータ
	int front = MAX_FRONT_BLOCK;
	int width = MAX_WIDHT_BLOCK;
	int height = MAX_HEIGHT_BLOCK;

	int num = 0;
	Vec3 get_pos;
	MapData obj;
	
	// 前方
	for (int f = 0; f < front; f++) {

		// 縦からしたに
		for (int h = 0; h < height; h++) {

			// 左から横
			for (int w = 0; w < width; w++) {

				// 位置取得
				get_pos.x = 
					((Taile::TAILE_SIZE_X * 2) * w)
					+ 8.f;

				// 上から下に下げる
				get_pos.y = 
					((Taile::TAILE_SIZE_Y * 2) * height)-
					(Taile::TAILE_SIZE_Y * 2) * h;

				get_pos.y -= 4.f;

				// 前方へ進ませる
				get_pos.z = 
					((Taile::TAILE_SIZE_Z * 2) * f);

				// 位置
				obj.pos = get_pos;

				// 現在のブロック位置抽出
				int block = 
					(h * height) + w;

				PieceTag tag;
				tag = ResultRandomPieceTag();

				// ピース
				int p = m_piece_list
					[tag][block];

				// 数字で識別したオブジェクトに位置代入
				InsertClearObject(
					p,
					f,
					h,
					w,
					obj
				);
			}
		}
	}
}


void MapObjectManager::InsertClearObject(
	int obj_num,
	int front_block,
	int height_block,
	int width_block,
	MapData data
) {

	CreateClearBlock(
		data,
		front_block,
		height_block,
		width_block,
		(MapTag)obj_num
	);

}


void MapObjectManager::CreateClearBlock(
	MapData &data,
	int f_block,
	int h_block,
	int w_block,
	MapTag tag
) {

	ClearBlock*c_b = new ClearBlock(tag, data);

	if (m_map[f_block][h_block][w_block] != nullptr) {
		delete m_map[f_block][h_block][w_block];
		m_map[f_block][h_block][w_block] = nullptr;
	}

	m_map[f_block][h_block][w_block] = c_b;
}


void MapObjectManager::SerchPastResultRandom(
	float &out_min,
	float &out_max
) {

	// 過去のリザルト結果を調べる
	for (auto itr : m_result_rand_list) {

		switch (itr) {

			// 空白ピース
		case 0:
			break;

		}
	}
}


void MapObjectManager::CalcCurrentBlock() {

	std::shared_ptr<ObjectData>p_data =
		m_p_chara.lock();

	// キャラクターを代入
	CharacterBase m_chara = *p_data->p_player;

	float z_t = Taile::TAILE_SIZE_Z * 2;

	// 現在のタイル数字を出す
	m_current_block =
		(int)((p_data->p_player->GetPos().z) / z_t);
}


void MapObjectManager::MapCreate() {

	// 前方位置割り出し
	int front_taile =
		m_current_block + CREATE_FRONT_BLOCK;
	// 後方
	int back_taile =
		m_current_block + CREATE_BACK_BLOCK;

	// 配列外なら戻す
	if (back_taile < 0) {
		back_taile = 0;
	}
	else if (front_taile > MAX_FRONT_BLOCK) {
		front_taile = MAX_FRONT_BLOCK;
	}

	// 奥(必要な箇所だけ回す)
	for (int f = back_taile;
		f < front_taile;
		f++) {

		// 縦
		for (int h = 0;
			h < MAX_HEIGHT_BLOCK;
			h++)
		{
			// 横
			for (int w = 0;
				w < MAX_WIDHT_BLOCK;
				w++) {

				// nullチェック(かなり重要)
				if (m_map[f][h][w] == nullptr) {
					continue;
				}

				// 空白ブロックでない、(空白ブロックは初期状態false)
				// もしくはすでにobjectが入っている場合
				// 生成しない
				if (m_map[f][h][w]->IsActive() == true){
					continue;
				}

				MapData data;
				// 空白ブロックに存在してある位置を取ってくる
				data.pos = m_map[f][h][w]->GetPos();

				CreateMapObject(
					m_map[f][h][w]->GetMapTag(),
					f,
					h,
					w,
					data
				);
			}
		}
	}
}


void MapObjectManager::MapDestory() {

	// 後方タイル
	int back_taile = m_current_block + DESTORY_FRONT_BLOCK;
	// 前方タイル
	int front_taile = m_current_block + DESTORY_BACK_BLOCK;

	// 配列外なら戻す
	if (back_taile <= 0) {
		back_taile = 0;
	}
	else if (front_taile >= MAX_FRONT_BLOCK) {
		front_taile = MAX_FRONT_BLOCK;
	}

	// 奥(必要な箇所だけ回す)
	for (int f = back_taile;
		f < front_taile;
		f++) {

		// 縦
		for (int h = 0;
			h < MAX_HEIGHT_BLOCK;
			h++)
		{
			// 横
			for (int w = 0;
				w < MAX_WIDHT_BLOCK;
				w++) {

				// nullチェック(かなり重要)
				if (m_map[f][h][w] == nullptr) {
					continue;
				}

				// 生成されていないなら
				if (m_map[f][h][w]->IsActive() == false) {
					continue;
				}

				MapData data;
				// 空白ブロックに存在してある位置を取ってくる
				data.pos = m_map[f][h][w]->GetPos();

				// 削除
				DestoryMapObject(
					m_map[f][h][w]->GetMapTag(),
					f,
					h,
					w,
					data
				);
			}
		}
	}

}


void MapObjectManager::CreateMapObject(
	MapTag obj_num,
	int f_block,
	int h_block,
	int w_block,
	MapData data) {


	switch (obj_num)
	{
		// 空白
	case MapTag::NONE:
		break;

		// ブロック
	case MapTag::BLOCK:
		CreateBlock(data.pos, f_block,h_block, w_block);
		break;

		// 敵1
	case MapTag::ENEMY1:
		CreateHomingEnemy(data.pos, f_block, h_block,w_block);
		break;

		// 敵2
	case MapTag::ENEMY2:
		CreateShotgunEnemy(data.pos, f_block, h_block, w_block );
		break;

	default:
		break;
	}
}


void MapObjectManager::DestoryMapObject(
	MapTag obj_num,
	int f_block,
	int h_block,
	int w_block,
	MapData data
) {

	// objリストから削除する
	m_map[f_block][h_block][w_block]->Destory();

	// 参照を一旦消す
	m_map[f_block][h_block][w_block] = nullptr;

	// 新しく空白オブジェクトを代入
	m_map[f_block][h_block][w_block] = 
		new ClearBlock(obj_num,data);
}



void MapObjectManager::CreateBlock(
	Vec3 &pos,
	int f_block,
	int h_block,
	int w_block
) {

	Block *b = new Block(pos);
	ObjectManager::GetInstance()->Entry(b);

	if (m_map[f_block][h_block][w_block] != nullptr) {
		delete m_map[f_block][h_block][w_block];
		m_map[f_block][h_block][w_block] = nullptr;
	}

	m_map[f_block][h_block][w_block] = b;
}


void MapObjectManager::CreateHomingEnemy(
	Vec3 &pos,
	int f_block,
	int h_block,
	int w_block
	) {

	std::shared_ptr<ObjectData>d
		= m_p_chara.lock();

	std::shared_ptr<ObjectFactory>fac
		= m_p_factory.lock();


	HomingBulletEnemy *he =
		new HomingBulletEnemy(
			pos,
			fac.get(),
			d->p_player
		);

	// オブジェクトエントリー
	ObjectManager::GetInstance()->Entry(he);

	// マップに埋め込む
	if (m_map[f_block][h_block][w_block] != nullptr) {
		delete m_map[f_block][h_block][w_block];
		m_map[f_block][h_block][w_block] = nullptr;
	}

	m_map[f_block][h_block][w_block] = he;
}


void MapObjectManager::CreateShotgunEnemy(
	Vec3 &pos,
	int f_block,
	int h_block,
	int w_block
) {

	std::shared_ptr<ObjectData>d
		= m_p_chara.lock();

	std::shared_ptr<ObjectFactory>fac
		= m_p_factory.lock();

	ShotgunEnemy *he =
		new ShotgunEnemy(
			pos,
			fac.get(),
			d->p_player
		);

	// オブジェクトエントリー
	ObjectManager::GetInstance()->Entry(he);
	
	if (m_map[f_block][h_block][w_block] != nullptr) {
		delete m_map[f_block][h_block][w_block];
		m_map[f_block][h_block][w_block] = nullptr;
	}

	m_map[f_block][h_block][w_block] = he;
}


void MapObjectManager::Sort() {

	// 位置昇順のソートを行う
	for (auto &obj : m_world_pos_list) {

		std::sort(obj.second.begin(), obj.second.end(),
			[](const FileObject obj1, 
				const FileObject obj2) {
				return obj1.pos.z < obj2.pos.z;
			});
	}
}