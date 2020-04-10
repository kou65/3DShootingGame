#pragma once
#include"../../Manager/FileObjectDataManager/FileObjectDataManager.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../ManagerBase/ManagerBase.h"
#include"../../GameApp/Enemy/EnemyBase/EnemyBase.h"
#include"../../GameApp/Block/Block.h"
#include<vector>
#include<unordered_map>


struct FileObject {

	FileObject() {
		is_active = true;
		create_line = 0;
		destory_line = 0;
	}

	bool is_active;
	Vec3 pos;
	float create_line;
	float destory_line;
};


// フィールドのオブジェクトを生成し、削除する管理者


class FiledObjectManager : public ManagerBase{
public:

	static constexpr float FRONT_LINE = 100.f;
	static constexpr float BACK_LINE = -20.f;

public:

	FiledObjectManager(
		FileObjectDataManager*p_manager,
		ObjectFactory*p_factory,
		ObjectData*p_player
	);

	// 更新
	void Update()override;

private:

	void Create();
	void Destory();

	void FileObjCreate(
		const Object3DTag&tag,
		Vec3&pos,
		int& array_num
	);

	void FileObjDestory(
		const Object3DTag&tag,
		Vec3&pos,
		int& array_num
	);

private:

	// 各配列(filedobject数と同じのサイズにする必要がある)
	std::vector<EnemyBase*>m_p_enemy_list;
	std::vector<Block*>m_p_cube_list;

	// プレイヤー
	ObjectData*m_p_chara;

	// 取得した位置リスト
	std::unordered_map<Object3DTag,std::vector<FileObject>>m_pos_list;

	// 工場
	ObjectFactory*m_p_factory;

};