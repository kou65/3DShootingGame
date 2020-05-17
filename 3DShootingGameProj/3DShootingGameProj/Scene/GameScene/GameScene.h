
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include"../SceneBase/SceneBase.h"
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"
#include"../../Lib\3D\Camera3D\Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Manager/FileDataManager/FileDataManager.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../../Lib/FPS/FPS.h"
#include"../../Manager/MapObjectManager/MapObjectManager.h"
#include"../../Manager/GameEndJugeManager/GameEndJugeManager.h"
#include"../../Manager/MapStructureManager/MapStructureManager.h"
#include"../../Manager/ManagerFactory/ManagerFactory.h"
#include"../../Manager/ManagerControlOffcer/ManagerControlOffcer.h"
#include<memory>



/**
* @brief ゲームシーン
*/
class GameScene : public SceneBase {
public:

	/**
	* @brief コンストラクタ
	*/
	GameScene();


	/**
	* @brief デストラクタ
	*/
	~GameScene();


	/**
	* @brief 更新
	*/
	void Update(SceneType&scene_type)override;


	/**
	* @brief 描画
	*/
	void Draw()override;

private:


	/**
	* @brief 初期化
	*/
	void Init();


	/**
	* @brief 操作者生成
	*/
	void CreateOperator();


	/**
	* @brief オブジェクト生成
	*/
	void CreateObject();

	// 管理者生成
	//void CreateManager();

private:

	//! オブジェクトデータ
	std::shared_ptr<ObjectData>mp_object_data;

	//! ファイルオブジェクト管理者
	std::shared_ptr<FileDataManager>mp_file_obj_mng;

	//! カメラ
	std::shared_ptr<Camera3D>mp_camera;

	//! ファクトリー
	std::shared_ptr<ObjectFactory>mp_obj_factory;

	//! フィールドobj管理者
	std::unique_ptr<MapObjectManager>mp_filed_obj_mng;

	//! ゲーム終了判定管理者
	std::unique_ptr<GameEndJugeManager>mp_game_end_juge_mng;

	//! マップ管理者
	std::shared_ptr<MapStructureManager>mp_map_object_mng;

	//std::unique_ptr<ManagerFactory>mp_mng_factory;
	//std::unique_ptr<ManagerControlOffcer>mp_contorl_ofc;


	//! 一時的にシーンタイプを保存する用
	SceneType*mp_scene_type;

	//! fps計測クラス
	FPS m_fps;
};


#endif