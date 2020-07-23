#include"FileDataManager.h"
#include"../../Manager/CollisionManager/CollisionManager.h"
#include"../../Lib/Utility/File/File.h"
#include"../../Lib/Utility/Convert/Convert.h"
#include <algorithm>


/* フォーマット
res = リソース番号
o%d = オブジェクト番号
*/


FileDataManager::FileDataManager(
	std::shared_ptr<ObjectFactory>of,
	std::shared_ptr<Camera3D>c,
	std::shared_ptr<ObjectData>p_data
) {

	mp_mng = of;
	mp_camera = c;
	mp_object_data = p_data;


	// ファイルポインタ
	FILE*p_file = nullptr;

	// ファイル生成
	Utility::File::Open(&p_file, "ExternalFile/ObjectList.txt", "r");
	LoadVec3(p_file);
	Utility::File::Close(&p_file);

	p_file = nullptr;

	// ファイル生成
	Utility::File::Open(&p_file, "ExternalFile/MapObjectPiece.txt", "r");
	LoadRandomPosNum(p_file);
	Utility::File::Close(&p_file);

	// ファイルの情報から自機と敵を生成する
	CreateGameObject();
}


void FileDataManager::LoadVec3(FILE*p_file) {

	const int LOAD_BUFFER_SIZE = 256;

	std::string res_num_str;
	char load_str[LOAD_BUFFER_SIZE];

	if (p_file == nullptr) {
		return;
	}

	while (feof(p_file) == 0) {

		// リソース番号読み込み
		fscanf_s(p_file, "%s", (char*)load_str,LOAD_BUFFER_SIZE);

		// コメントなら
		if (Utility::Convert::IsStrInclude(load_str, "#") == true) {
			continue;
		}

		// リソースが存在するなら
		if (Utility::Convert::IsStrInclude(load_str,"res") == true) {

			// リソース番号を代入
			res_num_str = load_str;
		}
		

		// オブジェクト読み込み
		if (Utility::Convert::IsStrInclude(load_str, "o") == true) {

			Vec3 v;

			// 位置を読み込み
			Utility::File::LoadFloatVec3(p_file, v, ' ');

			// リソース番号付きで位置を代入
			m_obj_pos_list[res_num_str].emplace_back(v);
		}
	}
}


void FileDataManager::LoadRandomPosNum(FILE*p_file) {


	const int LOAD_BUFFER_SIZE = 256;

	std::string res_num_str;
	char load_str[LOAD_BUFFER_SIZE];

	if (p_file == nullptr) {
		return;
	}

	while (feof(p_file) == 0) {

		// リソース番号読み込み
		fscanf_s(p_file, "%s", (char*)load_str, LOAD_BUFFER_SIZE);

		// コメントなら
		if (Utility::Convert::IsStrInclude(load_str, "#") == true) {
			continue;
		}


		// 指定文字列が存在するなら
		if (Utility::Convert::IsStrInclude(load_str, "P") == true) {

			// リソース番号を代入
			res_num_str = load_str;
		}

		// 行追加
		m_piece_list.emplace_back();

		for (int j = 0; j < 3; j++) {

			// オブジェクト読み込み
			std::vector<int>out_num_list;

			// カンマ区切りを3行で読み込み
			if (Utility::File::LoadIntNum(p_file, out_num_list,3,",")
				== false) {
				continue;
			}

			// リソース番号付きで位置を代入
			for (int i = 0; i < 3; i++) {
				m_piece_list.back().emplace_back(out_num_list[i]);
			}
		}
	}

}


void FileDataManager::CreateGameObject() {

	// 共有用取得
	std::shared_ptr<ObjectFactory>p_factory = mp_mng.lock();
	std::shared_ptr<Camera3D>p_camera = mp_camera.lock();
	std::shared_ptr<ObjectData>p_data = mp_object_data.lock();

	// プレイヤー生成(プレイヤー参照値取得)
	for (auto &player_pos : m_obj_pos_list[Const::FileObj::PLAYER]) {

		p_factory->CreateSharedPlayer(
			player_pos,
			mp_camera,
			p_data->p_player
		);

		// 当たり判定にエントリー
		CollisionManager::GetInstance()->Entry(
			CollisionObjectType::PLAYER,p_data->p_player
		);
	}

	// 背景生成
	for (auto &back_ground : m_obj_pos_list[Const::FileObj::BACK_GROUND]) {
		p_factory->CreateBackGround(back_ground);
	}

	// ゴールオブジェクト生成
	for (auto &goal_obj : m_obj_pos_list[Const::FileObj::GOAL_OBJECT]) {

		// ゴールオブジェクト生成
		p_factory->CreateGoalObject(
			goal_obj,
			p_data->p_goal_object
		);
	}

}


int FileDataManager::GetMaxPosListNum(const std::string&res_name) {

	return (int)m_obj_pos_list[res_name].size();
}


Vec3 FileDataManager::GetPosList(const std::string&res_name, const int &i) {

	return m_obj_pos_list[res_name][i];
}


int FileDataManager::GetPieceNumListSize(
	const int &map_size_num
) {

	auto itr = m_piece_list.begin();

	// 指定番号まで回す
	for (int i = 0; i < map_size_num; i++) {

		// 最後まできていたら最後を返す
		if (itr == m_piece_list.end()) {
			break;
		}
		// イテレータ加算
		itr++;
	}

	// キーを返す
	return (int)itr->size();
}


int FileDataManager::GetPieceNum(
	const int&map_size_num,
	const int&vector_num
) {

	auto itr = m_piece_list.begin();

	// 指定番号まで回す
	for (int i = 0; i < map_size_num; i++) {

		// 最後まできていたら最後を返す
		if (itr == m_piece_list.end()) {
			break;
		}
		itr++;
	}

	// キーを返す
	return itr->at(vector_num);
}


int FileDataManager::GetPieceListSize() {

	return (int)m_piece_list.size();
}


void FileDataManager::Clear() {
	m_obj_pos_list.clear();
	m_piece_list.clear();
}


void FileDataManager::Sort() {

	// 位置のソートを行う
	for (auto &obj : m_obj_pos_list) {

		std::sort(obj.second.begin(), obj.second.end(),
			[](const Vec3 obj1, const Vec3 obj2) {
				return obj1 < obj2;
			});
	}
}


