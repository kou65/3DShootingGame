#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DObject.h"
#include"../Lib/Sprite2D/Sprite2DData/Sprite2DData.h"
#include"../Lib/3D/Sprite3D/Sprite3DData/Sprite3DData.h"
#include"../Lib/3D/Sprite3D/Sprite3D/Sprite3DObject.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../Lib/3D/XFile/XFile.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"



Debugger::Debugger() {

	index_buffer = new IndexBuffer(Graphics::GetInstance());

	index_buffer->Create(16);

	light = new Light(Graphics::GetInstance());

	camera_3d = new Camera3D(Camera3D::FPS);
	camera_3d->AddPos(D3DXVECTOR3(0.f, 0.f, -30.f));
	m_is_program_stop = false;
	light->On();

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


bool Debugger::IsStopUpdate() {

	// 更新をストップする
	if (KeyBoard::IsKeyExit(DIK_S)) {
		m_is_program_stop = !m_is_program_stop;
	}

	return m_is_program_stop;
}


void Debugger::Update() {

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


	light->On();

	camera_3d->TransformDraw();

	// ライトモードをファルスにする
	//SetRenderStateFile::LightMode(FALSE);

	// Xファイルの描画
	//XFile::GetInstance()->Draw(
	//	"Sample01.x",
	//	camera_3d->GetPos()
	//	);


	{
		Sprite3DData td(0.f, 0.f, 0.f, "ground");
		td.scale_width = 1000.f;
		td.scale_height = 1000.f;
		td.polygon_dir = FLOOR;
		td.pos.y = -5.f;
		td.ofset.x = 0.0f;
		td.ofset.y = 1.0f;

		Sprite3DObject sprite_3d;

		sprite_3d.BoardDraw(
			td
		);

		td.scale_width = 1000.f;
		td.scale_height = 1000.f;
		td.polygon_dir = FRONT;
		td.pos.y = -5.f;
		td.ofset.x = 0.0f;
		td.ofset.y = 1.0f;
		td.pos.z = 1000.f;

		sprite_3d.BoardDraw(
			td
		);
	}


		SetRenderStateFile::LightMode(TRUE);

		//ObjFile::GetInstance()->DrawSubSet("1", 0.f, 0.f, 0.f);
}


void Debugger::Release() {


}
