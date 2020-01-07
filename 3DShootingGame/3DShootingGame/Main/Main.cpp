#include"../LoadResource/LoadResource.h"
#include"../SetRenderStateFile/SetRenderStateFile.h"
#include"../Debugger/Debugger.h"
#include"../Lib/DirectInput/DirectInput.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/D3DFont/D3DFont.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/DirectInput/JoyStick/JoyStick.h"
#include"../Lib/Lib/Lib.h"
#include"../SceneManager/SceneManager.h"
#include"../TitleScene/TitleScene.h"
#include"../Lib/3D/Fbx/FbxFile/Fbx.h"




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
	Resource::LoadResource();

	// DirectX描画状態の設定
	SetRenderStateFile::Init();

	//Fbx::GetInstance()->
	//	Load("Resource/3DModel/Cube3Poly.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/60-glass-cube/glass cube.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Spiderfbx/Spider_2.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/portal/Cube.fbx");
	Fbx::GetInstance()->Load("Resource/3DModel/taiki/taiki.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/humanoid.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Plane.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/CubePolygon4.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/cottage_fbx.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HandGun/Handgun_fbx_7.4_binary.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Character/StickFigurea.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Character/cavaleiro.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/dragon.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/HelicopterLight_v001.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/Lowpoly_Helicopter.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/FBX.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/FBX/Shachiku_chan_Ver2.0.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan_WALK00_L.fbx");
	//Fbx::GetInstance()->Load("Resource/3DModel/UnityChann/unitychan.fbx");

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

		// アニメーション更新
		Fbx::GetInstance()->Animate(1.f,-74.f);
		
		TextureData td = TextureManager::GetInstance()->GetTextureData("taiki_tex");

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
