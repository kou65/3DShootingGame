LPDIRECT3D9 m_pdirect3d9 = NULL;
LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS m_d3dParameters;

D3DCAPS9 Caps;

//シーンのメッシュ
//DirectX SDK(December 2004) に添付されているDXUTMesh.cppファイルにあるヘルパークラス群
CDXUTMesh* m_pMeshBack = NULL;
CDXUTMesh* m_pMeshTeapot = NULL;

//2Dオブジェクト(表面化散乱(Subsurface Scattering) ページ参照)
D3D2DSQUARE* m_pSquObj = NULL;

//ランバート拡散照明クラスの宣言
LAMBERT1* m_pLambert1 = NULL;

//シーンのZ値を取得するクラスの宣言
LAMBERT3* m_pLambert3 = NULL;

//ソフトシャドークラスの宣言
SOFTSHADOW1* m_pSoftShadow = NULL;

//ブラーフィルタークラスの宣言
BLURFILTER2* m_pBlurFilter = NULL;

//シャドーマップ
LPDIRECT3DTEXTURE9    m_pZBufferTexture = NULL;
LPDIRECT3DSURFACE9    m_pZBufferSurface = NULL;

//Zバッファ
LPDIRECT3DSURFACE9    m_pZBuffer = NULL;

//影イメージ
LPDIRECT3DTEXTURE9    m_pShadowTexture = NULL;
LPDIRECT3DSURFACE9    m_pShadowSurface = NULL;

//ぼかした影イメージ
LPDIRECT3DTEXTURE9    m_pBlurTexture[2] = { NULL, NULL };
LPDIRECT3DSURFACE9    m_pBlurSurface[2] = { NULL, NULL };

//スクリーンの解像度
UINT nWidth = 1024;
UINT nHeight = 768;

//平行光源の方向ベクトル
D3DXVECTOR4 LightDir;

D3DXVECTOR4 LightEye = D3DXVECTOR4(0.0f, 0.0f, -1.0f, 1.0f);
D3DXVECTOR4 LightUp = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
D3DXVECTOR4 LightAt = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

bool RenderOK = false;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR     /*lpCmpLine*/,
	INT       /*nCmdShow*/)
{
	char* AppName = "Tutrial";

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	HWND hWnd = NULL;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(DWORD);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppName;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = hInstance;
	::RegisterClassEx(&wc);


	//****************************************************************
	//ここでウィンドウの作成処理
	//****************************************************************


	//****************************************************************
	//ここでDirect3Dの初期化を行う。
	//****************************************************************

	m_pd3dDevice->GetDeviceCaps(&Caps);

	//ランバート拡散照明クラスの初期化
	m_pLambert1 = new LAMBERT1(m_pd3dDevice);
	m_pLambert1->Load();

	//シーンのZ値を取得するクラスの初期化
	m_pLambert3 = new LAMBERT3(m_pd3dDevice);
	m_pLambert3->Load();

	//ソフトシャドークラスの初期化
	m_pSoftShadow = new SOFTSHADOW1(m_pd3dDevice);
	m_pSoftShadow->Load();

	//ブラーフィルタークラスの初期化
	m_pBlurFilter = new BLURFILTER2(m_pd3dDevice, 512, 512);
	m_pBlurFilter->Load();


	//背景
	m_pMeshBack = new CDXUTMesh();
	m_pMeshBack->Create(m_pd3dDevice, _T("res\\01.x"));
	m_pMeshBack->SetFVF(m_pd3dDevice, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//ティーポット
	m_pMeshTeapot = new CDXUTMesh();
	m_pMeshTeapot->Create(m_pd3dDevice, _T("res\\t-pot.x"));
	m_pMeshTeapot->SetFVF(m_pd3dDevice, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//2Ｄオブジェクトのロード
	m_pSquObj = new D3D2DSQUARE(m_pd3dDevice, &m_d3dParameters);
	m_pSquObj->Load();

	//平行光源関連の位置ベクトルを回転
	D3DXMATRIX LightMatrix, mx, my;
	D3DXMatrixRotationX(&mx, D3DXToRadian(30.0f));
	D3DXMatrixRotationY(&my, D3DXToRadian(40.0f));
	LightMatrix = mx * my;

	D3DXVec3TransformCoord((D3DXVECTOR3*)&LightEye, (D3DXVECTOR3*)&LightEye, &LightMatrix);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&LightUp, (D3DXVECTOR3*)&LightUp, &LightMatrix);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&LightAt, (D3DXVECTOR3*)&LightAt, &LightMatrix);

	//平行光源の方向ベクトル
	LightDir = LightAt - LightEye;

	RenderOK = true;

	//デバイス消失後にリストアする必要があるオブジェクトの初期化
	Restore();

	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);

	do
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			if (MainLoop(hWnd) == FALSE)
				::DestroyWindow(hWnd);
		}
	} while (msg.message != WM_QUIT);

	::UnregisterClass(AppName, hInstance);

	return msg.wParam;
}

//デバイスのリセット前に開放すべきオブジェクト
void Invalidate()
{
	m_pLambert1->Invalidate();
	m_pLambert3->Invalidate();
	m_pSoftShadow->Invalidate();
	m_pBlurFilter->Invalidate();

	SafeRelease(m_pZBufferTexture);
	SafeRelease(m_pZBufferSurface);

	SafeRelease(m_pZBuffer);

	SafeRelease(m_pShadowTexture);
	SafeRelease(m_pShadowSurface);

	for (int i = 0; i < 2; i++)
	{
		SafeRelease(m_pBlurTexture[i]);
		SafeRelease(m_pBlurSurface[i]);
	}
}

//デバイスのリセット後に初期化すべきオブジェクト
void Restore()
{
	//シャドーマップ作成時に使用するレンダーターゲットサーフェイスのサイズ
	//精度を上げるため適当に大きくする
	DWORD SurfaceWidth = nWidth * 2;
	DWORD SurfaceHeight = nHeight * 2;

	m_pLambert1->Restore();
	m_pLambert3->Restore();
	m_pSoftShadow->Restore();
	m_pBlurFilter->Restore();

	//レンダーターゲットサーフェイスとZバッファの縦横のサイズは一致させる
	D3DXCreateTexture(m_pd3dDevice,
		SurfaceWidth,
		SurfaceHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pZBufferTexture);
	m_pZBufferTexture->GetSurfaceLevel(0, &m_pZBufferSurface);

	m_pd3dDevice->CreateDepthStencilSurface(SurfaceWidth,
		SurfaceHeight,
		D3DFMT_D16,
		m_d3dParameters.MultiSampleType,
		m_d3dParameters.MultiSampleQuality,
		TRUE,
		&m_pZBuffer,
		NULL);

	D3DXCreateTexture(m_pd3dDevice,
		nWidth,
		nHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pShadowTexture);
	m_pShadowTexture->GetSurfaceLevel(0, &m_pShadowSurface);

	for (int i = 0; i < 2; i++)
	{
		D3DXCreateTexture(m_pd3dDevice,
			m_pBlurFilter->GetWidth(),
			m_pBlurFilter->GetHeight(),
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pBlurTexture[i]);
		m_pBlurTexture[i]->GetSurfaceLevel(0, &m_pBlurSurface[i]);
	}


	//固定機能パイプラインライティングを設定する
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(LightDir.x, LightDir.y, LightDir.z);
	Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetLight(0, &Light);
	m_pd3dDevice->LightEnable(0, TRUE);

	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(Material));
	Material.Diffuse.r = 1.0f;
	Material.Diffuse.g = 1.0f;
	Material.Diffuse.b = 1.0f;
	Material.Diffuse.a = 1.0f;
	m_pd3dDevice->SetMaterial(&Material);
}

//メッセージループからコールされる関数
BOOL MainLoop(HWND HWnd)
{
	HRESULT hr;

	//レンダリング不可能
	if (RenderOK == false)
	{
		hr = m_pd3dDevice->TestCooperativeLevel();
		switch (hr)
		{
			//デバイスは消失しているがReset可能
		case D3DERR_DEVICENOTRESET:

			//開放
			Invalidate();

			//デバイスをリセットする
			hr = m_pd3dDevice->Reset(&m_d3dParameters);

			switch (hr)
			{
				//デバイスロスト
			case D3DERR_DEVICELOST:
				break;

				//内部ドライバーエラー
			case D3DERR_DRIVERINTERNALERROR:
				return FALSE;
				break;

				//メソッドの呼び出しが無効です
			case D3DERR_INVALIDCALL:
				return FALSE;
				break;

			case S_OK:

				//初期化
				Restore();

				RenderOK = true;
			}
			break;
		}
	}

	//レンダリング可能
	else
	{
		D3DXMATRIX matSM, matProj, matView, matScale, matViewLight, matProjLight, matWorld, m;
		D3DXVECTOR3 LEye;

		m_pd3dDevice->BeginScene();

		//カメラ位置からの射影座標変換
		D3DXMatrixPerspectiveFovLH(&matProj,
			D3DX_PI / 4.0f,
			(float)nWidth / (float)nHeight,
			50.0f, 1000.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		//****************************************************************   
		// STEP1:ライト位置からシーンを眺めて深度マップを作成
		//****************************************************************   

		//ライト位置からのビュー座標系
		//ライトの視点を適当に伸ばす
		LEye = D3DXVECTOR3(LightEye.x, LightEye.y, LightEye.z) * 700.0f;
		D3DXMatrixLookAtLH(&matViewLight, &LEye, (D3DXVECTOR3*)&LightAt, (D3DXVECTOR3*)&LightUp);

		//ライト位置からの射影行列はゆがませないようにするため左手座標系正射影行列を作成する
		D3DXMatrixOrthoLH(&matProjLight,
			(float)nWidth / 2.5f, (float)nHeight / 2.5f,
			500.0f, 800.0f);

		//カメラのビュー行列 * カメラの射影行列
		matSM = matViewLight * matProjLight;

		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		//レンダーターゲットサーフェイスに切り替える
		LPDIRECT3DSURFACE9 OldSurface = NULL, OldZMap = NULL;
		m_pd3dDevice->GetRenderTarget(0, &OldSurface);
		m_pd3dDevice->SetRenderTarget(0, m_pZBufferSurface);

		//Zバッファも切り替える
		m_pd3dDevice->GetDepthStencilSurface(&OldZMap);
		m_pd3dDevice->SetDepthStencilSurface(m_pZBuffer);

		//クリア
		m_pd3dDevice->Clear(0L,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xFF000000,
			1.0f,
			0L
		);

		//ティーポットのＺ値を取得
		m_pLambert3->Begin();
		D3DXMATRIX matTeaPot[2], matRotation, matScaling, matTranslation;
		D3DXMatrixRotationX(&matRotation, D3DXToRadian(30.0f));
		D3DXMatrixScaling(&matScaling, 20.0f, 20.0f, 20.0f);
		D3DXMatrixTranslation(&matTranslation, 30.0f, 80.0f, 25.0);
		matTeaPot[0] = matRotation * matScaling * matTranslation;
		m = matTeaPot[0] * matSM;
		m_pLambert3->SetMatrix(&m);
		m_pLambert3->BeginPass(0);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pLambert3->EndPass();

		D3DXMatrixTranslation(&matTranslation, 0.0f, 90.0f, -25.0);
		matTeaPot[1] = matRotation * matScaling * matTranslation;
		m = matTeaPot[1] * matSM;
		m_pLambert3->SetMatrix(&m);
		m_pLambert3->BeginPass(0);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pLambert3->EndPass();
		m_pLambert3->End();


		//****************************************************************   
		// STEP2:影イメージをレンダリング
		//****************************************************************   

		//レンダーターゲットサーフェイスを切り替える
		m_pd3dDevice->SetRenderTarget(0, m_pShadowSurface);

		//Ｚバッファは戻す
		m_pd3dDevice->SetDepthStencilSurface(OldZMap);
		SafeRelease(OldZMap);

		//クリアする
		m_pd3dDevice->Clear(0L,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xFFFFFFFF,                            //影イメージを作成するときは背景を白とする
			1.0f,
			0L
		);

		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		//シャドーマップをサンプラ番号1にセットする
		m_pd3dDevice->SetTexture(1, m_pZBufferTexture);

		//SetShadowColor() と SetAmbient の値は基本的に同じにした方がよい
		m_pSoftShadow->SetBias(0.02f);
		m_pSoftShadow->SetShadowColor(0.15f);
		m_pSoftShadow->SetAmbient(0.15f);

		m_pSoftShadow->Begin();

		//ティーポットをレンダリングし、影イメージを作成
		m_pSoftShadow->SetMatrix(&matTeaPot[0], &matSM, &LightDir);
		m_pd3dDevice->SetTexture(0, m_pMeshTeapot->m_pTextures[0]);
		m_pSoftShadow->BeginPass(0);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pSoftShadow->EndPass();

		m_pSoftShadow->SetMatrix(&matTeaPot[1], &matSM, &LightDir);
		m_pSoftShadow->BeginPass(0);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pSoftShadow->EndPass();
		m_pSoftShadow->End();

		m_pd3dDevice->SetTexture(1, NULL);


		//****************************************************************   
		// STEP3:影イメージをぼかす
		//****************************************************************   

		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

		m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		//縮小サーフェイスを使用するのでビューポートを変更する
		D3DVIEWPORT9 OldViewport, NewViewport;
		m_pd3dDevice->GetViewport(&OldViewport);
		CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		NewViewport.Width = m_pBlurFilter->GetWidth();
		NewViewport.Height = m_pBlurFilter->GetHeight();
		m_pd3dDevice->SetViewport(&NewViewport);

		m_pd3dDevice->SetRenderTarget(0, m_pBlurSurface[0]);
		m_pd3dDevice->SetTexture(0, m_pShadowTexture);
		m_pBlurFilter->Render(0);

		m_pd3dDevice->SetRenderTarget(0, m_pBlurSurface[1]);
		m_pd3dDevice->SetTexture(0, m_pBlurTexture[0]);
		m_pBlurFilter->Render(1);

		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		m_pd3dDevice->SetViewport(&OldViewport);


		//****************************************************************   
		// STEP4:シーンのレンダリング
		//****************************************************************   

		m_pd3dDevice->SetRenderTarget(0, OldSurface);
		SafeRelease(OldSurface);

		//地面をランバート拡散照明によりレンダリング
		m_pLambert1->Begin();
		D3DXMatrixIdentity(&matWorld);
		m_pLambert1->SetMatrix(&matWorld, &LightDir);
		m_pLambert1->SetAmbient(0.1f);
		m_pd3dDevice->SetTexture(0, m_pMeshBack->m_pTextures[0]);
		m_pLambert1->BeginPass();
		m_pMeshBack->m_pLocalMesh->DrawSubset(0);
		m_pLambert1->EndPass();

		//空をランバート拡散照明によりレンダリング
		D3DXMatrixIdentity(&matWorld);
		m_pLambert1->SetMatrix(&matWorld, &LightDir);
		m_pLambert1->SetAmbient(1.0f);
		m_pd3dDevice->SetTexture(0, m_pMeshBack->m_pTextures[1]);
		m_pLambert1->BeginPass(0);
		m_pMeshBack->m_pLocalMesh->DrawSubset(1);
		m_pLambert1->EndPass();
		m_pLambert1->End();

		//影を適応しつつティーポットをレンダリング
		m_pSoftShadow->Begin();
		m_pSoftShadow->SetMatrix(&matTeaPot[0], &matSM, &LightDir);
		m_pd3dDevice->SetTexture(0, m_pMeshTeapot->m_pTextures[0]);
		m_pd3dDevice->SetTexture(1, m_pBlurTexture[1]);
		m_pSoftShadow->BeginPass(1);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pSoftShadow->EndPass();

		m_pSoftShadow->SetMatrix(&matTeaPot[1], &matSM, &LightDir);
		m_pSoftShadow->BeginPass(1);
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
		m_pSoftShadow->EndPass();
		m_pSoftShadow->End();

		m_pd3dDevice->SetTexture(0, NULL);

		m_pd3dDevice->EndScene();

		hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		//デバイスロストのチェック
		switch (hr)
		{
			//デバイスロスト
		case D3DERR_DEVICELOST:
			RenderOK = false;
			break;

			//内部ドライバーエラー
		case D3DERR_DRIVERINTERNALERROR:
			return FALSE;
			break;

			//メソッドの呼び出しが無効です
		case D3DERR_INVALIDCALL:
			return FALSE;
			break;
		}
	}

	return TRUE;
}
