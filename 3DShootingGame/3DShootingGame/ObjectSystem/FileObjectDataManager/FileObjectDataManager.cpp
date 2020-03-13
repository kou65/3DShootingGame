#include"FileObjectDataManager.h"
#include"../../Utility/Utility.h"
#include <algorithm>


/* フォーマット
res = リソース番号
o%d = オブジェクト番号
*/


FileObjectDataManager::FileObjectDataManager(
	ObjectFactory*of,
	Camera3D*c,
	ObjectData*p_data
) {

	m_p_mng = of;
	m_p_camera = c;
	m_p_object_data = p_data;
}


void FileObjectDataManager::Load(FILE*p_file) {

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
		if (Utility::IsStrInclude(load_str, "#") == true) {
			continue;
		}

		// リソースが存在するなら
		if (Utility::IsStrInclude(load_str,"res") == true) {

			// リソース番号を代入
			res_num_str = load_str;
		}
		

		// オブジェクト読み込み
		if (Utility::IsStrInclude(load_str, "o") == true) {

			Vec3 v;

			// 位置を読み込み
			Utility::LoadFloatVec3(p_file, v, ' ');

			// リソース番号付きで位置を代入
			m_obj_pos[res_num_str].emplace_back(v);
		}
	}
}


void FileObjectDataManager::CreatePlayerAndEnemy() {


	// プレイヤー(プレイヤー参照値取得)
	for (auto &player_pos : m_obj_pos["res1"]) {
		m_p_mng->CreatePlayer(
			player_pos,
			m_p_camera,
			m_p_object_data
		);
	}

	// 敵
	for (auto &enemy_pos : m_obj_pos["res2"]) {
		m_p_mng->CreateEnemy(enemy_pos, 2.f);
	}

	// 背景
	for (auto &back_ground : m_obj_pos["res3"]) {
		m_p_mng->CreateBackGround(back_ground);
	}

}


void FileObjectDataManager::Sort() {

	// 位置のソートを行う
	for (auto &obj : m_obj_pos) {

		std::sort(obj.second.begin(), obj.second.end(),
			[](const Vec3 obj1, const Vec3 obj2) {
				return obj1 < obj2;
			});
	}
}


