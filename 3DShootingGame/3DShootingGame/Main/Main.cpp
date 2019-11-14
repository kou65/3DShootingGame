#include"../LoadResource/LoadResource.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../DebugMode/DebugMode.h"
#include"../Lib/DirectInput/DirectInput.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/D3DFont/D3DFont.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Lib/Lib.h"
#include"../SceneManager.h"
#include"../TitleScene.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {


	// ライブラリの初期化
	if (Lib::Init() == false) {
		return false;
	}

	// 背景を青にする
	Graphics::GetInstance()->SetClearBackGroundColor(0x0000ff);

	// リソース読み込み
	Resource::LoadResource();

	// DirectX描画状態の設定
	SetRenderStateFile::Init();

	//Fbx::GetInstance()->
	//	Load("Resource/3DModel/Cube3Poly.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/60-glass-cube/glass cube.fbx");
	Fbx::GetInstance()->Load("Resource/3DModel/Spiderfbx/Spider_2.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/portal/Cube.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/taiki/taiki.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/cottage_fbx.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HandGun/Handgun_fbx_7.4_binary.fbx");

	//SceneManager scene_manager(new TitleScene,TITLE);
	GameScene game;
	SceneType st;

	while (Window::ProcessMessage() == true) {

		// 終了処理
		if (Debugger::GetInstance().IsEnd() == true) {
			break;
		}

		// デバッグテスト
		Debugger::GetInstance().Update();

		game.Update(st);
		

		// 描画開始
		if (Graphics::GetInstance()->DrawStart() == true) {

			Fbx::GetInstance()->Draw();
			// デバッグの描画
			Debugger::GetInstance().Draw();
			//game.Draw();
		
		}

		// 描画終了
		Graphics::GetInstance()->DrawEnd();
	}

	Lib::Release();

	return 0;
}
