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

/*
LPDIRECT3D9 pD3d = NULL;
LPDIRECT3DDEVICE9 pDevice = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3d(HWND);
VOID Render();



INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	HWND hWnd = NULL;
	MSG msg;

	static char szAppName[] = "Direct3D最小コード ";
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	RegisterClassEx(&wndclass);

	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, 400, 300, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	if (FAILED(InitD3d(hWnd)))
	{
		return 0;
	}

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	pDevice->Release();
	pD3d->Release();
	return (INT)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
//
//HRESULT InitD3d(HWND hWnd)
//ダイレクト3Dの初期化
HRESULT InitD3d(HWND hWnd)
{
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1; InitD3d;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &pDevice)))
				{
					MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}

VOID Render()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	if (SUCCEEDED(pDevice->BeginScene()))
	{

		pDevice->EndScene();
	}
	pDevice->Present(NULL, NULL, NULL, NULL);
}*/


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
	bool is_debug_mode = true;

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
