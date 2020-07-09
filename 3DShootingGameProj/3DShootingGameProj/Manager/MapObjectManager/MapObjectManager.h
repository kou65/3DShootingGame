#pragma once
#include"../../Manager/FileDataManager/FileDataManager.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../ManagerBase/ManagerBase.h"
#include"../../GameApp/Enemy/EnemyBase/EnemyBase.h"
#include"../../GameApp/Block/Block.h"
#include"../../GameApp/Filed/MapData/MapData.h"
#include<vector>
#include<unordered_map>
#include<map>




/**
* @enum PieceTag ピースを識別するタグ
*/
enum class PieceTag {
	CLEAR,
	UP_BLOCK,
	CENTER_BLOCK,
	DOWN_BLOCK,
	ENEMY1,
	ENEMY2,
	TOTAL,
};


/**
* @brief ファイル用オブジェクト
*/
struct FileObject {


	FileObject() {
		is_active = false;
		create_line = 0;
		destory_line = 0;
	}

	//! 活動しているか
	bool is_active;

	//! 位置
	Vec3 pos;

	//! 生成ライン
	float create_line;

	//! 削除ライン
	float destory_line;
};


/**
* @brief フィールドのオブジェクトを生成し、削除する管理者
*/
class MapObjectManager : public ManagerBase{
public:

	//! 生成範囲
	static constexpr float FRONT_LINE = 120.f;
	static constexpr float BACK_LINE = -20.f;

	//! 前方タイルの最大数
	static constexpr int MAX_FRONT_BLOCK = 1000;
	//! 横タイルの最大数
	static constexpr int MAX_WIDHT_BLOCK = 3;
	//! 縦タイルの最大数
	static constexpr int MAX_HEIGHT_BLOCK = 3;

	//! マップ範囲
	//! 生成範囲
	static constexpr int CREATE_FRONT_BLOCK = 10;
	static constexpr int CREATE_BACK_BLOCK = 9;
	//! 削除範囲
	static constexpr int DESTORY_FRONT_BLOCK = -3;
	static constexpr int DESTORY_BACK_BLOCK = -2;
	

	//! 乱数パーセント
	//! 空白(50%)
	static constexpr int CLEAR_PER = 50;
	//! ブロックだけ(20%)
	static constexpr int BLOCK_PER = 70;
	//! 敵だけ(20%)
	static constexpr int ENEMY_PER = 90;
	//! ブロックと敵(10%)
	static constexpr int BLOCK_ENEMY_PER = 100;
	

public:


	/**
	* @brief コンストラクタ
	* @param[in] p_manager FileObjectDataのポインタ
	* @param[in] p_factory 工場のポインタ
	* @param[in] p_data オブジェクトデータのポインタ
	*/
	MapObjectManager(
		std::shared_ptr<FileDataManager> p_manager,
		std::shared_ptr<ObjectFactory>p_factory,
		std::shared_ptr<ObjectData>p_data
	);


	/**
	* @brief 更新
	*/
	void Update()override;

private:


	/**
	* @brief マップの初期化
	*/
	void InitMap();


	/**
	* @brief 生成する
	*/
	void CreateWorldObject();


	/**
	* @brief 削除する
	*/
	void DestoryWorldObject();


	/**
	* @brief ファイルから受け取ったオブジェクトを生成する
	* @param[in] pos 位置
	* @param[in] array_num 配列数
	*/
	void FileObjCreate(
		const Object3DTag&tag,
		Vec3&pos,
		int& array_num
	);


	/**
	* @brief tag オブジェクトタグを設定する
	* @param[in] pos 位置
	* @param[in] array_num 配列数
	*/
	void FileObjDestory(
		const Object3DTag&tag,
		Vec3&pos,
		int& array_num
	);


	/**
	* @brief ファイルから受け取った位置をFiledObjectに代入する
	* @param[in] p_manager フィールドオブジェクトデータ管理者
	* @param[out] f_obj FiledObject
	* @param[in] res_name リソース名
	* @param[in] tag 識別する用のオブジェクトタグ
	*/
	void FiledObjPosInsert(
		std::shared_ptr<FileDataManager>p_manager,
		FileObject &f_obj,
		const std::string&res_name,
		const Object3DTag&tag
	);


	/**
	* @brief 位置ファイルデータを挿入
	* @param[in] p_manager ファイルデータ管理者ポインタ
	*/
	void InsertPosFileData(
		std::shared_ptr<FileDataManager>p_manager
	);


	/**
	* @brief 位置ファイルデータを挿入
	* @param[in] p_manager ファイルデータ管理者ポインタ
	*/
	void InsertPosFileData2(
		std::shared_ptr<FileDataManager>p_manager
	);


	/**
	* @brief ピースファイルデータを挿入
	* @param[in] p_manager ファイルデータ管理者ポインタ
	*/
	void InsertPieceFileData(
		std::shared_ptr<FileDataManager>p_manager
	);

	/**
	* @brief 位置をソートしてz座標が小さい方から前に
	* 持ってくる
	*/
	void Sort();

private:

	/**
	* @brief 乱数生成
	*/
	void RandomNormalCreate();


	/**
	* @brief 乱数結果を返す
	*/
	PieceTag ResultRandomPieceTag();


	/**
	* @brief ランダム結果を選択
	*/
	PieceTag SelectRandomTag(const float &rand_num);


	/**
	* @brief 乱数結果を調べ、乱数最大数最小数を変える
	*/
	void SerchPastResultRandom(
		float &out_min,
		float &out_max
	);


	/**
	* @brief 現在のブロック計算
	*/
	void CalcCurrentBlock();


	/**
	* @brief マップ用生成
	*/
	void MapCreate();


	/**
	* @brief マップ削除
	*/
	void MapDestory();


	/**
	* @brief マップオブジェクト生成
	*/
	void CreateMapObject(
		MapTag obj_num,
		int f_block,
		int h_block,
		int w_block,
		MapData data
	);


	/**
	* @brief マップオブジェクト削除
	*/
	void DestoryMapObject(
		MapTag obj_num,
		int f_block,
		int h_block,
		int w_block,
		MapData data
	);


	/**
	* @brief 識別オブジェクト生成
	*/
	void InsertClearObject(
		int obj_num,
		int front_block,
		int height_block,
		int width_block,
		MapData data
	);


	/**
	* @brief 基準のオブジェクト生成
	*/
	void CreateClearBlock(
		MapData &data,
		int f_block,
		int h_block,
		int w_block,
		MapTag tag
	);


	/**
	* @brief ブロック生成
	*/
	void CreateBlock(
		Vec3 &pos,
	int front_block,
	int height_block,
		int width_block
	);


	/**
	* @brief 追尾弾敵生成
	*/
	void CreateHomingEnemy(
		Vec3 &pos,
		int front_block,
		int height_block,
		int width_block
		);


	/**
	* @brief 拡散弾敵生成
	*/
	void CreateShotgunEnemy(
		Vec3 &pos,
		int front_block,
		int height_block,
		int width_block
		);

private:

	// 各配列(filedobject数と同じのサイズにする必要がある)
	//! ホーミング敵リスト
	std::vector<std::weak_ptr<EnemyBase>>mp_h_enemy_list;

	//! ショットガン敵リスト
	std::vector<std::weak_ptr<EnemyBase>>mp_shotgun_enemy;

	//! ブロックリスト
	std::vector<std::weak_ptr<Block>>mp_cube_list;

	//! オブジェクトデータ
	std::weak_ptr<ObjectData>mp_chara;

	//! 工場
	std::weak_ptr<ObjectFactory>mp_factory;

	//! 世界に直接設置する位置リスト
	std::unordered_map<
		Object3DTag,
		std::vector<FileObject>>m_world_pos_list;

	//! 奥縦横配列、この位置リストを基準に生成
	std::vector<
		std::vector<
		std::vector<std::shared_ptr<MapObjectBase>
		>
		>
	>m_map;

	//! ピースリスト
	std::map<PieceTag, std::vector<int>>m_piece_list;

	//! 乱数結果リスト
	std::vector<int>m_result_rand_list;

	//! キャラクターの現在ブロック位置
	int m_current_block;
};