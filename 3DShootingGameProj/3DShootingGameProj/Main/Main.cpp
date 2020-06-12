#include"../LoadResource/LoadResource.h"
#include"../Lib/RenderState/RenderState.h"
#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/DirectInput.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/D3DFont/D3DFont.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Lib/Lib.h"
#include"../Scene/SceneManager/SceneManager.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"
#include"../Lib/3D/ShapeIndex/PlaneIndex/PlaneIndex.h"
#include"../Lib/Shader/ShaderFunc/ZTexture/FuncZTexture/FuncZTexture.h"
#include"../User/ZTextureUser/ZTextureUser.h"



/**
* @brief メインエントリーポイント
*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE, 
	_In_ LPSTR, 
	_In_ int) {


	// ライブラリの初期化
	if (Lib::Init() == false) {
		return false;
	}

	// 背景を青にする
	Graphics::GetInstance()->SetClearBackGroundColor(0x0000ff);

	// Zテクスチャ読み込み
	FuncZTexture::Create();

	// リソース読み込み
	LoadResources::Load();
	
	// デバッグ初期化
	Debugger::GetInstance().Init();

	// DirectX描画状態の設定
	RenderState::AllOn();
	RenderState::LightMode(false);

	// ブレンドオン
	Graphics::GetInstance()->
		GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_1WEIGHTS);

	// シーン管理者(タイトルから)
	SceneManager scene_manager(new TitleScene,SceneType::TITLE);

	// グラフィックデータ初期化
	Obj::GetInstance()->InitGrapicData();

	// デバッグモード
	bool is_debug_mode = true;

	// zテクスチャ使用者
	ZTextureUser user;
	
	// ループ
	while (Window::ProcessMessage() == true) {

		// 終了処理
		if (Debugger::GetInstance().IsEnd() == true) {
			break;
		}

		// ジョイスティック更新
		JoyStick::Update();

		// キーボード更新
		KeyBoard::Update();

		// デバッグテスト
		if (is_debug_mode == true) {

			// デバッガー更新
			Debugger::GetInstance().Update();
		}
		else {

			// シーンの更新
			scene_manager.Update();

			// zテクスチャ使用者の更新
			user.Update();

			// シーンが終了したならプログラム終了
			if (scene_manager.IsEnd() == true) {
				break;
			}
		}

		// 描画開始
		if (Graphics::GetInstance()->DrawStart() == true) {

			if (is_debug_mode == true) {

				// デバッグの描画
				Debugger::GetInstance().Draw();
			}
			else {

				// シーンの描画
				scene_manager.Draw();
			}
		}

		// 描画終了
		Graphics::GetInstance()->DrawEnd();
	}

	// 解放
	Lib::Release();

	return 0;
}
