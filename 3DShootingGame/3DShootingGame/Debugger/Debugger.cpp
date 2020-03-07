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
#include"../Lib/EffectFileShader/VertexDeclation.h"



Debugger::Debugger() {

	// デバイス取得
	m_p_device = Graphics::GetInstance()->GetDevice();

	index_buffer = new IndexBuffer();
	index_buffer->Create16(16);

	light = new Light(Graphics::GetInstance());

	CameraData data;

	camera_3d = new Camera3D(Camera3D::FPS,data);

	camera_3d->AddPos(D3DXVECTOR3(0.f, 0.f, -30.f));

	light->On();

	fps = new FPS(60);

	s2d.animation_param.division_width = 9;
	s2d.animation_param.division_height = 6;

	Sprite2DUser::GetInstance()->GraphSizeConvertUvSize(
		s2d.scale_width,
	s2d.scale_height,
		s2d.animation_param.division_width,
		s2d.animation_param.division_height
	);

	InitShader();

	// 立方体と板オブジェクトの読み込み
	ID3DXBuffer *cpMatBuf;
	

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

	camera_3d->Update();

}


void Debugger::CameraMove() {

	// キーボード操作
	{
		if (KeyBoard::IsKeyPushing(DIK_UP)) {
			camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_DOWN)) {
			camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_RIGHT)) {
			camera_3d->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (KeyBoard::IsKeyPushing(DIK_LEFT)) {
			camera_3d->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}
	
	// ジョイスティック操作
	{
		if (JoyStick::IsBottomPushing(0)) {
			camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		if (JoyStick::IsBottomPushing(2)) {
			camera_3d->AddMove(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		if (JoyStick::IsBottomPushing(1)) {
			camera_3d->AddMove(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		if (JoyStick::IsBottomPushing(3)) {
			camera_3d->AddMove(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
	}
}


void Debugger::CameraRotation() {

	if (KeyBoard::IsKeyPushing(DIK_D)) {
		camera_3d->AddRotation(D3DXVECTOR3(1.f,0.f,0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_A)) {
		camera_3d->AddRotation(D3DXVECTOR3(-1.f,0.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_W)) {
		camera_3d->AddRotation(D3DXVECTOR3(0.f, -1.f, 0.f));
	}
	if (KeyBoard::IsKeyPushing(DIK_S)) {
		camera_3d->AddRotation(D3DXVECTOR3(0.f, 1.f, 0.f));
	}
}


void Debugger::Draw() {

	// ライトオン
	//light->On();


	//ZTextureDraw();
	//ShadowDraw();

	fps->DebugDraw(Vec2(256.f, 256.f), 3000);
	camera_3d->TransformDraw();

	// 地面
	Sprite3DParameter td(0.f, 0.f, 0.f, "ground");
	td.scale_width = 1000.f;
	td.scale_height = 1000.f;
	td.polygon_dir = FLOOR;
	td.pos.y = -5.f;
	td.ofset.x = 0.0f;
	td.ofset.y = 1.0f;
	
	Sprite3DUser sprite_3d;
	
	sprite_3d.BoardDraw(
		td
	);

		s2d.texture_name = "enemy1";

		s2d.animation_param.animation_frame = 20.f;
		
		s2d.animation_param.is_animation = true;

		s2d.x = 40.f;
		s2d.y = 40.f;

		//Sprite2DObject::GetInstance()->BoardDraw(s2d);
	}


void Debugger::InitShader() {


	// 深度とzテクスチャを初期化
	m_d_effect.Init();

	m_z_tex_effect.Init(
		"ZTexture.fx",
		1000,
		1000
	);

	// ビュー・射影変換行列を初期化して固定情報として登録する
	D3DXMATRIX CameraView, CameraProj;
	D3DXMATRIX LightView, LightProj;

	float LightScale = 1.5f;

	D3DXMatrixPerspectiveFovLH(&CameraProj,
		D3DXToRadian(45), 640.0f / 480.0f, 10.0f, 1000.0f);


	D3DXMatrixPerspectiveFovLH(&LightProj,
		D3DXToRadian(40), 1.0f, 40.0f, 300.0f);

	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(
		LightScale * 20,
		LightScale * 100,
		LightScale * 0),
		&D3DXVECTOR3(0, -60, 0),
		&D3DXVECTOR3(0, 1, 0)
	);

	float f = 0.20f;

	// 深度シャドウセット
	m_d_effect.SetLightViewMatrix(&LightView);
	m_d_effect.SetLightProjMatrix(&LightProj);

	// テクスチャセット
	m_p_shadow_tex = m_z_tex_effect.GetZTexture();
	m_d_effect.SetShandowMap(m_p_shadow_tex);

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


void Debugger::ShadowDraw() {

	// 木偶レーション
	//VertexDecl v_d;
	//v_d.CreateObjFileDecl();
	//v_d.Set();

	D3DXMATRIX world_mat;

	D3DXMatrixIdentity(&world_mat);

	D3DXMATRIX view, proj;

	// ビュー行列
	m_p_device
		->GetTransform(D3DTS_VIEW, &view);

	// 射影行列 
	m_p_device
		->GetTransform(D3DTS_PROJECTION, &proj);

	m_d_effect.SetCameraProjMatrix(&proj);
	m_d_effect.SetCameraViewMatrix(&view);

	m_d_effect.Begin();

	GetCubeWorldMatrix(0, 0, 0, &world_mat);
	m_d_effect.SetWorldMatrix(&world_mat);
	m_d_effect.SetParamToEffect();

	for (UINT i = 0; i < dwMatNum; i++) {

		m_d_effect.SetParamToEffect();

		m_d_effect.BeginPass();
		cpMeshCube->DrawSubset(i);
		m_d_effect.EndPass();
	}

	D3DXMATRIX mat;


	float PlateScale = 1.0f;
	D3DXMATRIX Scale;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&Scale, PlateScale, 1.0f, PlateScale);
	mat *= Scale;
	mat._42 = -60.0f;

	m_d_effect.SetWorldMatrix(&mat);
	m_d_effect.SetParamToEffect();

	for (UINT i = 0; i < dwMatNum_Plate; i++) {
		m_d_effect.BeginPass();
		cpMeshPlate->DrawSubset(i);
		m_d_effect.EndPass();
	}

	m_d_effect.End();
}



void Debugger::ZTextureDraw() {

	// 木偶レーション
	//VertexDecl v_d;
	//v_d.CreateObjFileDecl();
	//v_d.Set();

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

	for (UINT i = 0; i < dwMatNum; i++) {

		m_z_tex_effect.SetParameter();

		m_z_tex_effect.BeginPass();
		cpMeshCube->DrawSubset(i);
		m_z_tex_effect.EndPass();
	}


	GetPlateWorldMatrix(&world_mat);
	m_z_tex_effect.SetWorldMatrix(&world_mat);

	for (UINT i = 0; i < dwMatNum_Plate;i++) {
		
		m_z_tex_effect.SetParameter();

		m_z_tex_effect.BeginPass();
		cpMeshPlate->DrawSubset(i);
		m_z_tex_effect.EndPass();
		
	}

	m_z_tex_effect.End();
}


void Debugger::Release() {


}
