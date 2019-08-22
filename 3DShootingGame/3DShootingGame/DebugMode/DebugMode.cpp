#include"DebugMode.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Texture/DrawPolygon2D.h"
#include"../Lib/Texture/TextureFormat2D.h"
#include"../Lib/Texture/TextureFormat3D.h"
#include"../Lib/Texture/DrawPolygon3D.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../Lib/XFile/XFile.h"
#include"../FBX.h"



DebugMode::DebugMode() {

	camera_3d = new Camera3D(Camera3D::TPS);
	m_is_program_stop = false;
	light.On();
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

	FBX::Scene();

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

	d3d_mesh.AllDraw();

	// Xファイルの描画
	XFile::GetInstance()->Draw(
		"Sample01.x",
		camera_3d->GetPos()
		);

	XFile::GetInstance()->Draw(
		"Sample01.x",
		D3DXVECTOR3(-30.f, 0.f, 0.f)
	);

		TextureFormat3D td(0.f,0.f,0.f,"ground");
		td.scale_width = 1000.f;
		td.scale_height = 1000.f;
		td.polygon_dir = FLOOR;
		td.pos.y = -5.f;

		DrawPolygon3D::GetInstance()->BoardDraw(
			td
		);

}


void DebugMode::Release() {


}
