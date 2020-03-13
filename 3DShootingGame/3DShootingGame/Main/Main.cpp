#include"../LoadResource/LoadResource.h"
#include"../RenderState/RenderState.h"
#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/DirectInput.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/D3DFont/D3DFont.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Lib/Lib.h"
#include"../Scene/SceneManager/SceneManager.h"
#include"../Scene/TitleScene/TitleScene.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"
#include"../Lib/3D/ShapeIndex/PlaneIndex/PlaneIndex.h"




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

	// リソース読み込み
	LoadResources::Load();

	// DirectX描画状態の設定
	RenderState::AllOn();
	RenderState::LightMode(false);
	//RenderState::CullMode(false);

	// ブレンドオン
	Graphics::GetInstance()->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_1WEIGHTS);

	//Fbx::GetInstance()->Load("Resource/3DModel/Spiderfbx/Spider_2.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/portal/Cube.fbx");
	Fbx::GetInstance()->Load("Resource/3DModel/taiki/taiki.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/humanoid.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Plane.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/CubePolygon4.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HelicopterLight_v001.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Lowpoly_Helicopter.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan_WALK00_L.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan.fbx");

	SceneManager scene_manager(new TitleScene,SceneType::TITLE);

	// デバッグモード
	bool is_debug_mode = false;

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

			Debugger::GetInstance().Update();


			// アニメーション更新
			Fbx::GetInstance()->Animate(5.f);
			Fbx::GetInstance()->Update();

		}
		else {

			// シーンの更新
			scene_manager.Update();
		}

		// 描画開始
		if (Graphics::GetInstance()->DrawStart() == true) {

			if (is_debug_mode == true) {

				Fbx::GetInstance()->Draw();

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
