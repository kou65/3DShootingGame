#include"../../GameApp/Player/Player.h"
#include"../../Factory/ObjectFactory/ObjectFactory.h"
#include"../../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../../Lib\Sprite2D\Sprite2D\Sprite2DUser.h"
#include"GameScene.h"
#include"../../Manager/MapStructureManager/MapStructureManager.h"
#include"../../Manager/CollisionManager/CollisionManager.h"




GameScene::GameScene() {

	// 初期化
	Init();

	// シーンスタート
	m_is_scene_start = false;

	// 変更シーンはなし
	SceneType none = SceneType::NONE;

	// 最初に2回だけ更新する
	UtilityUpdate(none);
	UtilityUpdate(none);

	UpdateLight();

	z = 0.f;
}


GameScene::~GameScene() {
	
	CollisionManager::GetInstance()->AllRelease();

	// 全てのオブジェクト解放
	ObjectManager::GetInstance()->AllRelease();

}


void GameScene::Update(SceneType&scene_type) {

	if (KeyBoard::IsKeyExit(DIK_RETURN)) {
		m_is_scene_start = true;
	}

	if (m_is_scene_start == false) {
		return;
	}

	// 更新
	UtilityUpdate(scene_type);

	// ライト初期化
	UpdateLight();
}


void GameScene::UtilityUpdate(SceneType&scene_type) {

	// カメラ更新
	mp_camera->Update();

	// 更新
	m_z_tex_user.Update();

	// オブジェクト更新
	ObjectManager::GetInstance()->Update();

	// 当たり判定更新
	CollisionManager::GetInstance()->Update();

	std::shared_ptr<Player>p =
		mp_object_data->p_player.lock();


	if (mp_object_data->p_player.lock() == nullptr) {
		return;
	}

	if (mp_object_data->p_goal_object.lock() == nullptr) {
		return;
	}

	// 管理更新
	mp_map_obj_mng->Update();

	// ゲーム終了判定者更新(あとでjugerにする)
	mp_game_end_juge_mng->Update(scene_type);

	// 更新
	mp_map_object_mng->Update();

	// 最後のタイミングで当たり判定オブジェクトを削除させる
	CollisionManager::GetInstance()->CheckAndClear();
}


void GameScene::Draw() {

	// fps値表示
	m_fps.DebugDraw(Vec2(0.f,0.f),200);

	// カメラデバッグ描画
	mp_camera->TransformDraw();

	// zテクスチャ使用者
	m_z_tex_user.Draw();

	// オブジェクト描画
	ObjectManager::GetInstance()->Draw();

}


void GameScene::Init() {

	// 操作者生成
	CreateOperator();

	// 管理者生成
	//CreateManager();

	// オブジェクト生成
	CreateObject();

}


void GameScene::UpdateLight() {

	// wは必ず1.fにする

	// 視点方向
	D3DXVECTOR3 eye_dir;

	// 視点方向を出す
	D3DXVec3Normalize(
		&eye_dir,
		&mp_camera->GetEyePos());

	// ライト方向
	LightData light_data;

	// 視点座標代入
	light_data.eye_direction = D3DXVECTOR4(
		eye_dir.x,
		eye_dir.y,
		eye_dir.z,
		1.f
	);

	// ライト方向
	D3DXVECTOR4 light_dir;

	// 点ライト座標代入
	light_data.point_light.pos = 
		D3DXVECTOR4(10.f, 200.f, 200.f, 0.f);

	z += 0.5f;

	light_data.point_light.pos.z += z;

	// 点ライトの座標を正規化
	D3DXVec4Normalize(&light_dir, &light_data.point_light.pos);

	// ライトの平行方向代入
	light_data.direction = 
		-light_dir;

	// 減衰パラメータ
	light_data.point_light.attenuation = 
		D3DXVECTOR4(1.0f, 0.2f, 0.2f, 1.0f);

	// カメラ位置
	light_data.eye_pos = D3DXVECTOR4(
		mp_camera->GetPos().x,
		mp_camera->GetPos().y,
		mp_camera->GetPos().z,
		1.f
	);

	// ライトカラー
	light_data.light_color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// 環境光
	light_data.ambient
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	// マテリアルの初期化
	light_data.material.ambient
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// 拡散
	light_data.material.diffuse
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	light_data.point_light.diffuse
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// 鏡面
	light_data.material.specular
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	light_data.point_light.specular
		= D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// ライトデータ
	Obj::GetInstance()->SetLightData(light_data);
}


void GameScene::CreateOperator() {

	// ファクトリー生成
	mp_obj_factory =
		std::make_shared<ObjectFactory>();

	// オブジェクトデータ生成
	mp_object_data
		= std::make_shared<ObjectData>();

	// カメラのデータ
	CameraData set_data;

	// 20先に回転軸を置く
	set_data.axis.z = -20.f;
	set_data.pos.x = 16.f;
	set_data.pos.y = 10.f;
	set_data.pos.z = -16.f;

	set_data.is_debug = false;

	// カメラ生成
	mp_camera =
		std::make_shared<Camera3D>(Camera3D::FPS, set_data);

	// ファイル管理者生成
	mp_file_obj_mng =
		std::make_unique<FileDataManager>(
			mp_obj_factory,
			mp_camera,
			mp_object_data
			);


	// マップ管理者生成
	mp_map_obj_mng =
		std::make_unique<MapObjectManager>(
			mp_file_obj_mng,
			mp_obj_factory,
			mp_object_data
			);
	

	// 終了管理者生成
	mp_game_end_juge_mng =
		std::make_unique<GameEndJugeManager>(
			mp_object_data
	);


	// マップオブジェクト管理者生成
	mp_map_object_mng =
		std::make_shared<MapStructureManager>(
			mp_obj_factory,
			mp_object_data
	);
}


void GameScene::CreateObject(){

	// UI生成
	mp_obj_factory->CreateHPUI(mp_object_data);
}



/*
void GameScene::CreateManager() {

	// 生成
	mp_mng_factory = std::make_unique<ManagerFactory>();
	mp_contorl_ofc = std::make_unique<ManagerControlOffcer>();

	std::shared_ptr<FileObjectDataManager>p_file_mng;
	std::shared_ptr<MapObjectManager>p_map_mng;
	std::shared_ptr<MapCharacterManager>p_chara_mng;

	// 管理者生成
	mp_mng_factory->CreateFileObjectDataManager(
		mp_obj_factory,
		mp_camera,
		mp_object_data,
		p_file_mng
	);

	mp_mng_factory->CreateMapObjectManager(
		mp_obj_factory,
		mp_object_data,
		p_map_mng
	);

	mp_mng_factory->CreateMapCharacterManager(
		p_file_mng,
		mp_obj_factory,
		mp_object_data,
		p_chara_mng
	);
}*/