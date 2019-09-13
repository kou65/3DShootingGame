#include"DebugMode.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2D.h"
#include"../Lib/Sprite2D/Sprite2DData/Sprite2DData.h"
#include"../Lib/3D/Sprite3D/Sprite3DData/Sprite3DData.h"
#include"../Lib/3D/Sprite3D/Sprite3D/Sprite3D.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/FBX/FBX.h"




DebugMode::DebugMode() {

	index_buffer = new IndexBuffer(Graphics::GetInstance());

	index_buffer->Create(16);

	light = new Light(Graphics::GetInstance());

	camera_3d = new Camera3D(Camera3D::TPS);
	m_is_program_stop = false;
	light->On();

	// オブジェクト読み込み
	objfile.Load(
		"Resource/3DModel/UH60/uh60",
		"Resource/3DModel/UH60/",
		m_total_material_num
	);
}


DebugMode& DebugMode::GetInstance() {
	static DebugMode instance;
	return instance;
}


bool DebugMode::IsEnd() {

	// デバッグ用強制終了
	if (KeyBoard::IsKeyPush(DIK_ESCAPE)) {
		return true;
	}
	return false;
}


bool DebugMode::IsStopUpdate() {

	// 更新をストップする
	if (KeyBoard::IsKeyExit(DIK_S)) {
		m_is_program_stop = !m_is_program_stop;
	}

	return m_is_program_stop;
}


void DebugMode::Update() {

	if (IsStopUpdate() == true) {
		// 終了
		return;
	}

	CameraRotation();

	CameraMove();

	// ジョイスティックを列挙する
	JoyStick::Update();

	KeyBoard::Update();

	camera_3d->Update();
}


void DebugMode::CameraMove() {

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


void DebugMode::CameraRotation() {

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


void DebugMode::Draw() {


	camera_3d->TransformDraw();

	// ライトモードをファルスにする
	SetRenderStateFile::LightMode(FALSE);

	//d3d_mesh.AllDraw();

	// Xファイルの描画
	XFile::GetInstance()->Draw(
		"Sample01.x",
		camera_3d->GetPos()
		);

	//XFile::GetInstance()->Draw(
	//	"Sample01.x",
	//	D3DXVECTOR3(-30.f, 0.f, 0.f)
	//);

		Sprite3DData td(0.f,0.f,0.f,"ground");
		td.scale_width = 1000.f;
		td.scale_height = 1000.f;
		td.polygon_dir = FLOOR;
		td.pos.y = -5.f;
		td.ofset.x = 0.0f;
		td.ofset.y = 1.0f;

		Sprite3D sprite_3d;

		sprite_3d.BoardDraw(
			td
		);

		//index_buffer->Draw();

		//Graphics::GetInstance()->
		//	GetLpDirect3DDevice9()->
		//	SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//SetRenderStateFile::CullMode(FALSE);

		//for (int i = 0; i < m_total_material_num; i++) {
			objfile.DrawSubSet(0);
		//}
}


void DebugMode::Release() {


}
