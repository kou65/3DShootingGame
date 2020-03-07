
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include"../SceneBase/SceneBase.h"
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"
#include"../../Lib\3D\Camera3D\Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../ObjectSystem/FileObjectDataManager/FileObjectDataManager.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../../Lib/FPS/FPS.h"
#include<memory>



class GameScene : public SceneBase {
public:

	GameScene();

	~GameScene();

	void Update(SceneType&scene_type)override;

	void Draw()override;

private:

	// 初期化
	void Init();

	// 操作者生成
	void CreateOperator();

	// オブジェクト生成
	void CreateObject();

private:

	// オブジェクトデータ
	std::unique_ptr<ObjectData>m_p_object_data;

	// ファイルオブジェクト管理者
	std::unique_ptr<FileObjectDataManager>m_p_file_obj_mng;

	// カメラ
	std::unique_ptr<Camera3D>m_p_camera;

	// ファクトリー
	std::unique_ptr<ObjectFactory>m_p_obj_factory;

	// fps計測クラス
	FPS m_fps;
};


#endif