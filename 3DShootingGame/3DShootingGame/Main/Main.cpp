#include"../LoadResource/LoadResource.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../DebugMode/DebugMode.h"
#include"../Lib/DirectInput/DirectInput.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/D3DFont/D3DFont.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {


	// 初期化
	if (Window::Init() == false) {
		return -1;
	}
	if (D3D9::Init() == false) {
		return -1;
	}

	DirectInput::Init();

	Sound::Init();

	D3DFont::Init();

	JoyStick::Init();

	// 背景を青にする
	D3D9::SetClearBackGroundColor(0x0000ff);

	// リソース読み込み
	Resource::LoadResource();

	// DirectX描画状態の設定
	//SetRenderStateFile::Init();

	while (Window::ProcessMessage() == true) {

		// 終了処理
		if (DebugMode::GetInstance().IsEnd() == true) {
			break;
		}

		// デバッグテスト
		DebugMode::GetInstance().Update();

		// 描画開始
		if (D3D9::DrawStart() == true) {

			// デバッグの描画
			DebugMode::GetInstance().Draw();

		}

		// 描画終了
		D3D9::DrawEnd();
	}

	// 解放処理
	DebugMode::GetInstance().Release();
	Sound::Release();
	DirectInput::Release();
	D3D9::Release();

	return 0;
}
