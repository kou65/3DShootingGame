#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../Lib/Sprite2D/Sprite2DParameter/Sprite2DParameter.h"
#include"../Lib/3D/Sprite3D/Sprite3DParameter/Sprite3DParameter.h"
#include"../Lib/3D/Sprite3D/Sprite3D/Sprite3DUser.h"
#include"../RenderState/RenderState.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"
#include"../Lib/Texture/TextureManager/TextureManager.h"
#include"../Lib/EffectFileShader/Declation/VertexDeclation.h"
#include"../GameApp/GameConstant/GameConstant.h"



Debugger::Debugger() {

	// デバイス取得
	m_p_device = Graphics::GetInstance()->GetDevice();

	// fps生成
	m_p_fps = new FPS(60);

	// ライト生成
	light = new DefaultLight(Graphics::GetInstance());

	// データ
	CameraData data;

	// デバッグモードオン
	data.is_debug = true;

	// カメラ
	m_p_camera = new Camera3D(Camera3D::FPS,data);
	m_p_camera->AddPos(D3DXVECTOR3(0.f, 0.f, -30.f));

	// シェーダー初期化
	InitShader();

	// 立方体と板オブジェクトの読み込み
	ID3DXBuffer *cpMatBuf;
	
	// xファイル読み込み
	if (FAILED(D3DXLoadMeshFromX(
		TEXT("Resource/3DModel/Cube2.x"),
		D3DXMESH_MANAGED,
		m_p_device,
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
		m_p_device,
		NULL,
		&cpMatBuf,
		NULL,
		&dwMatNum_Plate,
		&cpMeshPlate
	))) {
		return;
	}

	//Fbx::GetInstance()->Load("Resource/3DModel/Spiderfbx/Spider_2.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/taiki/taiki.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/humanoid.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Plane.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HelicopterLight_v001.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Lowpoly_Helicopter.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan.fbx");


	// 背景を青にする
	Graphics::GetInstance()->SetClearBackGroundColor(0x0000ff);

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

	CameraRotation();

	CameraMove();

	m_p_camera->Update();

	// アニメーション更新
	//Fbx::GetInstance()->Animate(5.f);
	//Fbx::GetInstance()->Update();
}


void Debugger::Draw() {

	// ライトオン
	//light->On();

	// 影
	//ZTextureDraw();
	//ShadowDraw();

	m_p_fps->DebugDraw(Vec2(256.f, 256.f), 3000);
	m_p_camera->TransformDraw();

	// 地面
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

	LightDebugDraw();

	// fbx描画
		//Fbx::GetInstance()->Draw();

	}


void Debugger::LightDebugDraw() {

	// オブジェシェーダー描画
	ObjParameter param;
	param.register_obj_file_name = Const::Obj::SPEHER;

	D3DXVECTOR3 eye_pos;

	D3DXVec3Normalize(
		&eye_pos,
		&m_p_camera->GetEyePos());

	// 正規化注視点ベクトル
	param.eye_pos = eye_pos;

	param.pos.y = 0.f;
	param.pos.x = 0.f;
	param.pos.z = 0.f;

	Obj::GetInstance()->ShaderDraw(param);
}


void Debugger::InitShader() {

	// 深度とzテクスチャを初期化
	m_shadow_effect.Init("Lib/ShaderFile/DepthShadow.fx");

	// zテクスチャ初期化
	m_z_tex_effect.Init(
		"Lib/ShaderFile/ZTexture.fx",
		// 解像度
		1000,
		1000
	);

	// ビュー・射影変換行列を初期化して固定情報として登録する
	D3DXMATRIX CameraView, CameraProj;
	D3DXMATRIX LightView, LightProj;

	float LightScale = 1.5f;

	// カメラ射影
	D3DXMatrixPerspectiveFovLH(&CameraProj,
		D3DXToRadian(45), 640.0f / 480.0f, 10.0f, 1000.0f);

	// ライト射影
	D3DXMatrixPerspectiveFovLH(&LightProj,
		D3DXToRadian(40), 1.0f, 40.0f, 300.0f);

	// ライトビュー
	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(
		LightScale * 20,
		LightScale * 100,
		LightScale * 0),
		&D3DXVECTOR3(0, -60, 0),
		&D3DXVECTOR3(0, 1, 0)
	);

	float f = 0.20f;

	// 深度シャドウセット
	m_shadow_effect.SetLightViewMatrix(&LightView);
	m_shadow_effect.SetLightProjMatrix(&LightProj);

	// zテクスチャのテクスチャをシャドウにセット
	m_p_shadow_tex = m_z_tex_effect.GetZTexture();
	m_shadow_effect.SetShandowMap(m_p_shadow_tex);

}


// 板のワールド変換行列生成
void GetPlateWorldMatrix(D3DXMATRIX *PlateWorld)
{
	float PlateScale = 1.0f;
	D3DXMATRIX Scale;
	D3DXMatrixIdentity(PlateWorld);
	D3DXMatrixScaling(&Scale, PlateScale, 1.0f, PlateScale);
	*PlateWorld *= Scale;
	PlateWorld->_42 = -60.0f;
}

// 立方体のワールド変換行列生成
void GetCubeWorldMatrix(float f, int x, int z, D3DXMATRIX *mat)
{
	D3DXMATRIX RotY, RotZ;
	D3DXMatrixIdentity(mat);
	D3DXMatrixRotationY(&RotY, D3DXToRadian(f));
	D3DXMatrixRotationZ(&RotZ, D3DXToRadian(f*2.353f));
	*mat *= RotY * RotZ;
	mat->_41 = x * 20.0f;  mat->_43 = z * 20.0f; mat->_42 = sin(f / 10) * 40;
}


void Debugger::ZTextureDraw() {


	D3DXMATRIX world_mat;

	D3DXMatrixIdentity(&world_mat);

	D3DXMATRIX view, proj;

	// ビュー行列
	m_p_device
		->GetTransform(D3DTS_VIEW, &view);

	// 射影行列 
	m_p_device
		->GetTransform(D3DTS_PROJECTION, &proj);

	m_z_tex_effect.SetProjMatrix(&proj);
	m_z_tex_effect.SetViewMatrix(&view);

	m_z_tex_effect.Begin();
	
	// ワールド座標変換
	GetCubeWorldMatrix(20,1,0,&world_mat);
	m_z_tex_effect.SetWorldMatrix(&world_mat);

	// キューブ描画
	for (UINT i = 0; i < dwMatNum; i++) {

		m_z_tex_effect.SetParameter();

		m_z_tex_effect.BeginPass();
		cpMeshCube->DrawSubset(i);
		m_z_tex_effect.EndPass();
	}


	GetPlateWorldMatrix(&world_mat);
	m_z_tex_effect.SetWorldMatrix(&world_mat);

	// プレート描画
	for (UINT i = 0; i < dwMatNum_Plate;i++) {
		
		m_z_tex_effect.SetParameter();
	
		m_z_tex_effect.BeginPass();
		cpMeshPlate->DrawSubset(i);
		m_z_tex_effect.EndPass();
	}

	m_z_tex_effect.End();
}


void Debugger::ShadowDraw() {


	// ワールド行列正規化
	D3DXMATRIX world_mat;
	D3DXMatrixIdentity(&world_mat);
	D3DXMATRIX view, proj;

	// ビュー行列
	m_p_device
		->GetTransform(D3DTS_VIEW, &view);

	// 射影行列 
	m_p_device
		->GetTransform(D3DTS_PROJECTION, &proj);

	// カメラ行列セット
	m_shadow_effect.SetCameraProjMatrix(&proj);
	m_shadow_effect.SetCameraViewMatrix(&view);

	m_shadow_effect.Begin();

	GetCubeWorldMatrix(0, 0, 0, &world_mat);
	m_shadow_effect.SetWorldMatrix(&world_mat);
	m_shadow_effect.SetParamToEffect();

	// xファイルキューブ
	for (UINT i = 0; i < dwMatNum; i++) {

		m_shadow_effect.SetParamToEffect();

		m_shadow_effect.BeginPass();
		cpMeshCube->DrawSubset(i);
		m_shadow_effect.EndPass();
	}

	// プレート
	float PlateScale = 1.0f;
	D3DXMATRIX Scale;
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&Scale, PlateScale, 1.0f, PlateScale);
	mat *= Scale;
	mat._42 = -60.0f;

	m_shadow_effect.SetWorldMatrix(&mat);
	m_shadow_effect.SetParamToEffect();

	// xファイルプレート
	for (UINT i = 0; i < dwMatNum_Plate; i++) {
		m_shadow_effect.BeginPass();
		cpMeshPlate->DrawSubset(i);
	}

	m_shadow_effect.End();
}




void Debugger::CameraMove() {

	// キーボード操作
	{
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			m_p_camera->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			m_p_camera->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			m_p_camera->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			m_p_camera->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}

	// ジョイスティック操作
	{
		if (JoyStick::IsBottomPushing(0)) {
			m_p_camera->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (JoyStick::IsBottomPushing(2)) {
			m_p_camera->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (JoyStick::IsBottomPushing(1)) {
			m_p_camera->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (JoyStick::IsBottomPushing(3)) {
			m_p_camera->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}
}


void Debugger::CameraRotation() {

	if (KeyBoard::IsKeyPushing(DIK_D)) {
		m_p_camera->AddRotation(D3DXVECTOR3(1.f, 0.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_A)) {
		m_p_camera->AddRotation(D3DXVECTOR3(-1.f, 0.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_W)) {
		m_p_camera->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_S)) {
		m_p_camera->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}


void Debugger::Release() {


}
