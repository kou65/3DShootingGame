#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../Lib/Sprite2D/Sprite2DParameter/Sprite2DParameter.h"
#include"../Lib/3D/Sprite3D/Sprite3DParameter/Sprite3DParameter.h"
#include"../Lib/3D/Sprite3D/Sprite3D/Sprite3DUser.h"
#include"../Lib/RenderState/RenderState.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"
#include"../Lib/Texture/TextureManager/TextureManager.h"
#include"../GameApp/GameConstant/GameConstant.h"
#include"../Lib/Shader/ShaderFunc/ZTexture/ZTextureManager/ZTextureManager.h"
#include"../Lib/Math/Math.h"



Debugger::Debugger() {

}


void Debugger::Init() {

	// デバイス取得
	mp_device = Graphics::GetInstance()->GetDevice();

	// fps生成
	mp_fps = new FPS(60);

	// ライト生成
	light = new DefaultLight(Graphics::GetInstance());

	// データ
	CameraData data;

	// デバッグモードオン
	data.is_debug = true;

	// カメラ
	mp_camera = new Camera3D(Camera3D::FPS, data);
	mp_camera->AddPos(D3DXVECTOR3(0.f, 0.f, -30.f));

	// シェーダー初期化
	InitXFileShader();

	// 立方体と板オブジェクトの読み込み
	ID3DXBuffer *cpMatBuf;

	// xファイル読み込み
	if (FAILED(D3DXLoadMeshFromX(
		TEXT("Resource/3DModel/Cube2.x"),
		D3DXMESH_MANAGED,
		mp_device,
		NULL,
		&cpMatBuf,
		NULL,
		&dwMatNum,
		&cpMeshCube))) {
		return;
	}

	// xファイル読み込み
	if (FAILED(D3DXLoadMeshFromX(
		TEXT("Resource/3DModel/Plate.x"),
		D3DXMESH_MANAGED,
		mp_device,
		NULL,
		&cpMatBuf,
		NULL,
		&dwMatNum_Plate,
		&cpMeshPlate
	))) {
		return;
	}

	// スプライト生成
	CreateSprite();

	//Fbx::GetInstance()->Load("Resource/3DModel/Spiderfbx/Spider_2.fbx");
	Fbx::GetInstance()->Load("Resource/3DModel/taiki/taiki.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/humanoid.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Plane.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HelicopterLight_v001.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Lowpoly_Helicopter.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan.fbx");
}


void Debugger::InitXFileShader() {

	/*
	カメラのセットとライトのセットを行っている
	シャドウの初期化はほとんど行っている
	*/


	// シャドウの初期化
	mp_shadow =
		new DepthShadowShader();


	// ビュー・射影変換行列を初期化して固定情報として登録する
	D3DXMATRIX CameraView, CameraProj;
	D3DXMATRIX LightView, LightProj;

	float LightScale = 1.5f;

	// カメラ射影
	D3DXMatrixPerspectiveFovLH(&CameraProj,
		D3DXToRadian(45), 640.0f / 480.0f, 10.0f, 1000.0f);

	
	// デバイスのカメラ情報送信
	mp_shadow->CommitDeviceViewProj();

	// zテクスチャのテクスチャをシャドウにセット
	ZTexture *tex =
		ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		);

	mp_shadow->SetShandowMap(tex->GetZTexture());

	// シャドウ初期化
	mp_shadow->Init();

}



void Debugger::UpdateLight() {

	// 視点方向
	D3DXVECTOR3 eye_dir;

	// 視点方向を出す
	D3DXVec3Normalize(
		&eye_dir,
		&mp_camera->GetEyePos());

	// ライト方向
	D3DXVECTOR4 light_dir(0.0f, 20.0f, 20.0f, 0.0f);
	D3DXVec4Normalize(&light_dir, &light_dir);

	LightData light_data;

	// ライトの平行方向代入
	light_data.direction = light_dir;

	// 視点座標代入
	light_data.eye_direction = D3DXVECTOR4(
		eye_dir.x,
		eye_dir.y,
		eye_dir.z,
		1.f
	);

	// 点ライト座標代入
	light_data.point_light.pos =
		D3DXVECTOR4(20.f, 0.f, 0.f, 0.f);

	// 減衰パラメータ
	light_data.point_light.attenuation =
		D3DXVECTOR4(1.0f, 0.0f, 0.2f, 0.0f);

	// 点光源
	light_data.point_light.diffuse
		= D3DXVECTOR4(0.f, 0.5f, 0.f, 0.f);
	light_data.point_light.specular
		= D3DXVECTOR4(0.f, 0.5f, 0.f, 0.f);

	// 環境光
	light_data.ambient
		= D3DXVECTOR4(1.f, 1.f, 0.f, 0.f);

	// カメラ位置
	light_data.eye_pos = D3DXVECTOR4(
		mp_camera->GetPos().x,
		mp_camera->GetPos().y,
		mp_camera->GetPos().z,
		1.f
	);

	// ライトカラー
	light_data.light_color = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);

	// マテリアルの初期化
	light_data.material.ambient
		= D3DXVECTOR4(0.f, 0.5f, 0.f, 0.f);
	light_data.material.diffuse
		= D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);
	light_data.material.specular
		= D3DXVECTOR4(1.f, 1.f, 1.f, 0.f);

	// ライトデータ
	Obj::GetInstance()->SetLightData(light_data);
}


void Debugger::CreateSprite() {
	// Z値テクスチャ描画用のスプライト生成
	D3DXCreateSprite(
		Graphics::GetInstance()->GetDevice(),
		&cpSprite
	);

}


Debugger& Debugger::GetInstance() {
	static Debugger instance;
	return instance;
}


bool Debugger::IsEnd() {

	// デバッグ用強制終了
	if (KeyBoard::IsKeyPush(DIK_ESCAPE)) {
		return true;
	}
	return false;
}


void Debugger::Update() {

	// カメラ回転
	CameraRotation();

	// カメラ移動
	CameraMove();

	// 更新
	mp_camera->Update();

	// ライト更新
	UpdateLight();

	// 影更新
	UpdateShadow();

	// アニメーション更新
	Fbx::GetInstance()->Animate(5.f);
	Fbx::GetInstance()->Update();
}


void Debugger::Draw() {

	// Xfile影
	{
		// objではない
	//tex->ResetDecl();
	//mp_shadow->ResetDecl();
		//XFileZTextureWrite();
		//XFileShadowDraw();
		
	}

	// Obj影
	{
		ObjShadowDraw();
	}

	// OBJライト
	//LightDebugDraw();

	// fbx描画
	//Fbx::GetInstance()->Draw();


	mp_fps->DebugDraw(Vec2(256.f, 256.f), 3000);
	mp_camera->TransformDraw();

	{
		// Z値テクスチャ
		ZTexture *p_tex = ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		);

		// お試し画像
		TextureData p_tex_data = TextureManager::GetInstance()->
			GetTextureData("ground");

		// z値描画
		//{
		//	
		//	ZTextureDraw(
		//		p_tex->GetZTexture(),
		//		Vec2(1000.f, 200.f)
		//	);
		//}

		// スプライトを使った
		{
			// 比率
			float Ratio = 1.f;
			//////////////////////////////////
			//■パス3 : Z値テクスチャを描画
			D3DXMATRIX SpriteScaleMat;
			D3DXMatrixScaling(&SpriteScaleMat, Ratio / 3, Ratio / 3, 1.0f);
			cpSprite->SetTransform(&SpriteScaleMat);
			cpSprite->Begin(0);
			cpSprite->Draw(p_tex->GetZTexture(), NULL, NULL, NULL, 0xffffffff);
			cpSprite->End();
		}
	}

	
	// 地面
	{
		Sprite3DParameter td(0.f, 0.f, 0.f, "ground");
		td.scale_width = 1000.f;
		td.scale_height = 1000.f;
		td.polygon_dir = FLOOR;
		td.pos.y = -300.f;
		td.ofset.x = 0.5f;
		td.ofset.y = 0.5f;

		Sprite3DUser sprite_3d;

		sprite_3d.BoardDraw(
			td
		);
	}
}



// 描画が上書きされてしまうのは多分ビギンをもう一回行っているから
void Debugger::ObjShadowDraw() {

	// 行列
	D3DXMATRIX mat_camera_view;
	D3DXMATRIX mat_light_view;

	// zテクスチャ
	ObjParameter cube_p(DrawStatus::NORMAL);
	// オブジェパラメータ
	ObjParameter taile_p(DrawStatus::SHADOW);

	// キューブ
	{
		// キューブ文字
		cube_p.register_obj_file_name = Const::Obj::CUBE;

		// 位置
		cube_p.pos.y = 60.f;
		cube_p.pos.x = 0.f;
		cube_p.pos.z = 0.f;

		cube_p.texture_name = Const::Graph::TAILE;

	}

	// 板
	{
		// 板文字
		taile_p.register_obj_file_name = Const::Obj::PLANE;

		// 位置
		taile_p.pos.y = 20.f;
		taile_p.scale.x = 30.f;
		taile_p.scale.z = 30.f;

		// テクスチャ代入
		taile_p.texture_name = Const::Graph::BACK_GROUND;
	}

	// 視点
	Vec3 look(
		0.f,
		-20.f,
		0.f
	);

	// ライト位置
	Vec3 light_pos(
		55 * 1.5f,
		100 * 1.5f,
		55 * 1.5f);

	// パラメータ
	ObjParameter light_obj(DrawStatus::NORMAL);

	// 影のライトとカメラの位置を描画
	{
		//// カメラオブジェクト
		//param.register_obj_file_name = 
		//	Const::Obj::PLAYER;
		//
		//// カメラ位置を描画
		//Obj::GetInstance()->EntryObjParam(param);
		//
		// ライトデータ代入
		light_obj.pos = light_pos;

		// ライトオブジェクト
		light_obj.register_obj_file_name =
			Const::Obj::PLAYER;

	}

	// ライトビュー
	D3DXMatrixLookAtLH(
		// ライトビュー行列
		&mat_light_view,
		// カメラの位置座標
		&light_pos,
		// 注視点座標ポインタ
		&look,
		// アップベクトル
		&D3DXVECTOR3(0, 1, 0)
	);

	// 射影
	D3DXMATRIX mat_light_proj;

	// ライト射影
	D3DXMatrixPerspectiveFovLH(
		// ライト射影行列
		&mat_light_proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		1.0f,
		// 視推台の最も近い距離
		10.0f,
		// 視推台の最も遠い距離
		300.0f
	);


	// 現在の行列の場合
	// 行列
	D3DXMATRIX mat_c_view, mat_c_proj;

	// ビュー行列
	mat_c_view =
		Graphics::GetInstance()->GetTSMatrix(D3DTS_VIEW);
	// 射影行列
	mat_c_proj =
		Graphics::GetInstance()->GetTSMatrix(D3DTS_PROJECTION);


	// こちらを変える(本体)
	D3DXMATRIX const_view = 
		mat_c_view;
	D3DXMATRIX const_proj = 
		mat_c_proj;// mat_light_proj
	

	// 影データ代入
	{
		// 影データ
		ShadowData shadow_data;

		// ライト行列
		shadow_data.light_view_mat = mat_light_view;
		shadow_data.light_proj_mat = mat_light_proj;

		// 現在のデバイスカメラセット(変更)
		shadow_data.camera_proj_mat = mat_c_proj;
		shadow_data.camera_view_mat = mat_c_view;

		// グラフィックデータ(影、光)
		Obj::GetInstance()->SetShadowData(
			shadow_data
		);
	}

	// 現在のカメラ情報を入れる
	ZTextureData z_data;
	
	z_data.mat_camera_view = const_view;// const_view
	z_data.mat_camera_proj = mat_light_proj;

	// zテクスチャにセット
	ZTextureManager::GetInstance()->SetViewMatrix(z_data);
	ZTextureManager::GetInstance()->SetProjMatrix(z_data);


	// zテクスチャ
	ZTexture*p_tex
		= ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		);

	UINT i;

	// 描画開始
	p_tex->Begin(i, 0);

	// zテクスチャに書き込み
	Obj::GetInstance()->
		WriteZTexture(cube_p, FuncZTexture::Const::Z_TEX_1024);
	Obj::GetInstance()->
		WriteZTexture(light_obj, FuncZTexture::Const::Z_TEX_1024);
	Obj::GetInstance()->
		WriteZTexture(taile_p, FuncZTexture::Const::Z_TEX_1024);
	
	// 描画終了
	p_tex->End();

	// 影と光描画
	Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW, cube_p);
	Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW, light_obj);
	Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW, taile_p);
}


void Debugger::LightDebugDraw() {

	// オブジェシェーダー描画
	ObjParameter param(DrawStatus::LIGHT);
	param.register_obj_file_name = Const::Obj::SPEHER;

	D3DXVECTOR3 eye_dir;

	// 正規化
	D3DXVec3Normalize(
		&eye_dir,
		&mp_camera->GetEyePos());

	// ライトの方向
	D3DXVECTOR4 light_dir(0.0f, -2.f, -1.0f, 0.0f);
	D3DXVec4Normalize(&light_dir, &light_dir);

	LightData light_data;

	// ライトの平行方向代入
	light_data.direction = light_dir;

	// 視点座標代入
	light_data.eye_direction = D3DXVECTOR4(
		eye_dir.x,
		eye_dir.y,
		eye_dir.z,
		1.f
	);

	// 点ライト座標代入
	light_data.point_light.pos = 
		D3DXVECTOR4(10.f, 20.f, 0.f, 0.f);

	// 減衰パラメータ
	light_data.point_light.attenuation = 
		D3DXVECTOR4(1.0f, 0.0f, 0.2f, 0.0f);

	// アンビエントカラー
	light_data.material.ambient = 
		D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	// スペキュラー
	light_data.material.specular = 
		D3DXVECTOR4(1.f, 1.5f, 0.f, 1.f);

	// 位置
	param.pos.y = 0.f;
	param.pos.x = 0.f;
	param.pos.z = 0.f;

	// カラー
	param.color = 
		D3DXVECTOR4(1.f, 0.f, 0.f, 0.f);

	// 描画
	Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW,param);
}


void Debugger::ZTextureDraw(
	IDirect3DTexture9*p_tex,
	Vec2 pos,
	Vec2 scale
) {

	Sprite2DParameter param;

	param.pos.x = pos.x;
	param.pos.y = pos.y;

	param.tex_size.x = scale.x;
	param.tex_size.y = scale.y;

	param.p_tex = p_tex;

	// Zテクスチャをスプライト表示
	Sprite2DUser::GetInstance()->BoardDraw(
		param
	);
}


void Debugger::XFileZTextureWrite() {


	// 初期位置決定
	m_cube_pos = Vec3(0.f, 40.f, 0.f);
	m_plate_pos = Vec3(0.f, 10.f, 0.f);

	ZTexture*p_z_tex
		= ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		);

	// デプスと同じライト情報を入れる
	{
		float LightScale = 1.5f;
		D3DXMATRIX LightProj, LightView;

		// ライト射影
		D3DXMatrixPerspectiveFovLH(&LightProj,
			D3DXToRadian(40), 1.0f, 40.0f, 300.0f);

		// ライトビュー
		D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(
			LightScale * 100,
			LightScale * 100,
			LightScale * 100),
			&D3DXVECTOR3(0, -60, 0),
			&D3DXVECTOR3(0, 1, 0)
		);

		// 現在のカメラ情報セット
		p_z_tex->SetViewMatrix(
			LightView
		);

		p_z_tex->SetProjMatrix(
			LightProj
		);

	}

	D3DXMATRIX world_mat;

	D3DXMatrixIdentity(&world_mat);

	UINT pass = 0;
	p_z_tex->Begin(pass,0);
	
	world_mat = Utility::Math::GetTransformMatrix(
		m_cube_pos,
		D3DXVECTOR3(1.f, 1.f, 1.f)
	);

	// ワールド座標セット
	p_z_tex->SetWorldMatrix(world_mat);


	// キューブ描画
	for (UINT i = 0; i < dwMatNum; i++) {

		p_z_tex->Update();

		p_z_tex->BeginPass();
		cpMeshCube->DrawSubset(i);
		p_z_tex->EndPass();
	}


	world_mat = Utility::Math::GetTransformMatrix(
		m_plate_pos,
		D3DXVECTOR3(1.f, 1.f, 1.f)
	);

	p_z_tex->SetWorldMatrix(world_mat);

	// プレート描画
	for (UINT i = 0; i < dwMatNum_Plate;i++) {
		
		p_z_tex->Update();
	
		p_z_tex->BeginPass();
		cpMeshPlate->DrawSubset(i);
		p_z_tex->EndPass();
	}

	p_z_tex->End();
}



void Debugger::XFileShadowDraw() {


	// 現在のカメラ情報セット
	mp_shadow->SetViewMatrix(
		Graphics::GetInstance()->GetTSMatrix(D3DTS_VIEW)
	);
	mp_shadow->SetProjMatrix(
		Graphics::GetInstance()->GetTSMatrix(D3DTS_PROJECTION)
	);

	float LightScale = 1.5f;
	D3DXMATRIX LightProj,LightView;
	// ライト射影
	D3DXMatrixPerspectiveFovLH(&LightProj,
		D3DXToRadian(40), 1.0f, 40.0f, 300.0f);

	// ライトビュー
	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(
		LightScale * 100,
		LightScale * 100,
		LightScale * 100),
		&D3DXVECTOR3(0, -60, 0),
		&D3DXVECTOR3(0, 1, 0)
	);

	// 深度シャドウセット
	mp_shadow->SetLightViewMatrix(LightView);
	mp_shadow->SetLightProjMatrix(LightProj);

	{
		// ワールド行列正規化
		D3DXMATRIX world_mat;
		D3DXMatrixIdentity(&world_mat);

		world_mat = Utility::Math::GetTransformMatrix(
			m_cube_pos,
			D3DXVECTOR3(1.f, 1.f, 1.f)
		);

		// ワールド位置セット
		mp_shadow->SetWorldMatrix(world_mat);


		UINT pass = 0;
		// 開始
		mp_shadow->Begin(pass, 0);

		// xファイルキューブ
		for (UINT i = 0; i < dwMatNum; i++) {

			mp_shadow->Update();
			mp_shadow->BeginPass();
			cpMeshCube->DrawSubset(i);
			mp_shadow->EndPass();
		}

		// プレート
		world_mat = Utility::Math::GetTransformMatrix(
			m_plate_pos,
			D3DXVECTOR3(1.f, 1.f, 1.f)
		);

		mp_shadow->SetWorldMatrix(world_mat);

		// xファイルプレート
		for (UINT i = 0; i < dwMatNum_Plate; i++) {

			mp_shadow->Update();
			mp_shadow->BeginPass();
			cpMeshPlate->DrawSubset(i);
			mp_shadow->EndPass();
		}

		mp_shadow->End();
	}
}


void Debugger::DrawLightShadow() {

	// オブジェシェーダー描画
	ObjParameter param(DrawStatus::LIGHT);
	param.register_obj_file_name = Const::Obj::SPEHER;

	// 影ライト描画
	Obj::GetInstance()->Draw(DrawStatus::LIGHT_SHADOW, param);
}



void Debugger::UpdateShadow() {


	// 現在の行列の場合
	// 行列
	D3DXMATRIX mat_c_view, mat_c_proj;

	// ビュー行列
	mat_c_view =
		Graphics::GetInstance()->GetTSMatrix(D3DTS_VIEW);
	// 射影行列
	mat_c_proj =
		Graphics::GetInstance()->GetTSMatrix(D3DTS_PROJECTION);


	// 射影
	D3DXMATRIX mat_light_proj;

	// ライト射影
	D3DXMatrixPerspectiveFovLH(
		// ライト射影行列
		&mat_light_proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		1.0f,
		// 視推台の最も近い距離
		40.0f,
		// 視推台の最も遠い距離
		300.0f
	);

	// こちらを変える(本体)
	D3DXMATRIX const_view =
		mat_c_view;
	D3DXMATRIX const_proj =
		mat_light_proj;

	// 影データ
	ShadowData shadow_data;

	// ライト行列
	shadow_data.light_view_mat = const_view;
	shadow_data.light_proj_mat = const_proj;

	// 現在のデバイスカメラセット(変更)
	shadow_data.camera_proj_mat = mat_c_proj;
	shadow_data.camera_view_mat = mat_c_view;


	// グラフィックデータ(影、光)
	Obj::GetInstance()->SetShadowData(
		shadow_data
	);
}



void Debugger::CameraMove() {

	// キーボード操作
	{
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			mp_camera->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			mp_camera->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			mp_camera->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			mp_camera->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}

	// ジョイスティック操作
	{
		if (JoyStick::IsBottomPushing(0)) {
			mp_camera->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (JoyStick::IsBottomPushing(2)) {
			mp_camera->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (JoyStick::IsBottomPushing(1)) {
			mp_camera->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (JoyStick::IsBottomPushing(3)) {
			mp_camera->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}
}


void Debugger::CameraRotation() {

	if (KeyBoard::IsKeyPushing(DIK_D)) {
		mp_camera->AddRotation(D3DXVECTOR3(1.f, 0.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_A)) {
		mp_camera->AddRotation(D3DXVECTOR3(-1.f, 0.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_W)) {
		mp_camera->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_S)) {
		mp_camera->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}


void Debugger::Release() {


}
