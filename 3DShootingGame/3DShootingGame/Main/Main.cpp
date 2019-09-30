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


	//SceneManager scene_manager(new TitleScene,TITLE);
	GameScene game;
	SceneType st;

	while (Window::ProcessMessage() == true) {

		// 終了処理
		if (Debugger::GetInstance().IsEnd() == true) {
			break;
		}

		// デバッグテスト
		//Debugger::GetInstance().Update();

		game.Update(st);

		// 描画開始
		if (Graphics::GetInstance()->DrawStart() == true) {

			// デバッグの描画
			//Debugger::GetInstance().Draw();
			game.Draw();
		}

		// 描画終了
		Graphics::GetInstance()->DrawEnd();
	}

	Lib::Release();

	return 0;
}
