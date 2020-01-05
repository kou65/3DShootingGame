// 完全ホワイトボックスなスキンメッシュアニメーションテストプログラム
//  作成者: IKD
//  HP    : http://marupeke296.com

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

_TCHAR gName[100] = _T("完全ホワイトボックスなスキンメッシュアニメーションテストプログラム");

int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev);


// 最低限の頂点情報
//  座標と各ボーンの重みそしてボーン行列番号があればスキンメッシュはできます！
struct Vertex {
	D3DXVECTOR3 coord;
	D3DXVECTOR3 weight;
	unsigned char matrixIndex[4];
};

// ボーン構造体
struct Bone {
	int id;                        // ボーンID（通し番号）
	Bone *firstChild;            // 第1子ボーン
	Bone *sibling;                // 次の兄弟ボーン
	D3DXMATRIX offsetMat;        // ボーンオフセット行列
	D3DXMATRIX initMat;            // 初期姿勢行列
	D3DXMATRIX boneMat;            // ボーン姿勢行列
	D3DXMATRIX *combMatAry;        // 合成姿勢行列配列へのポインタ

	Bone() : id(), firstChild(), sibling(), combMatAry() {
		D3DXMatrixIdentity(&initMat);
		D3DXMatrixIdentity(&offsetMat);
		D3DXMatrixIdentity(&boneMat);
	}
};

// 最低限のシェーダ
//  重み係数と頂点を動かすためのワールド変換行列の配列を渡します
const char *vertexShaderStr =
"float4x4 view : register(c0);"
"float4x4 proj : register(c4);"
"float4x4 world[12] : register(c8);"
"  "
"struct VS_IN {"
"    float3 pos : POSITION;"
"    float3 blend : BLENDWEIGHT;"
"    int4 idx : BLENDINDICES;"
"};"
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"VS_OUT main( VS_IN In ) {"
"    VS_OUT Out = (VS_OUT)0;"
"    float w[3] = (float[3])In.blend;"
"    float4x4 comb = (float4x4)0;"
"    for ( int i = 0; i < 3; i++ )"
"        comb += world[In.idx[i]] * w[i];"
"    comb += world[In.idx[3]] * (1.0f - w[0] - w[1] - w[2]);"
"    "
"    Out.pos = mul( float4(In.pos, 1.0f), comb );"
"    Out.pos = mul( Out.pos, view );"
"    Out.pos = mul( Out.pos, proj );"
"    return Out;"
"}";

// ピクセルシェーダは至って適当
// 好きなように点を穿って下さい。
const char *pixelShaderStr =
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"float4 main( VS_OUT In ) : COLOR {"
"    return float4(1.0f, 1.0f, 1.0f, 1.0f);"
"}"
"";

// メイン
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウプロシージャ
	struct WP {
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam) {
			if (mes == WM_DESTROY) { PostQuitMessage(0); return 0; }
			return DefWindowProc(hWnd, mes, wParam, lParam);
		}
	};

	// アプリケーションの初期化
	HWND hWnd;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WP::WndProc, 0, 0, hInstance, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, (_TCHAR*)gName, NULL };
	if (!RegisterClassEx(&wcex))
		return 0;
	RECT rect;
	const int width = 800, height = 600;
	SetRect(&rect, 0, 0, width, height);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	if (!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL)))
		return 0;

	// Direct3Dの初期化
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDev;
	if (!(g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return 0;
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = NULL;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev)))
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev))) {
			g_pD3D->Release(); return 0;
		}

	// ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);

	// ボーンを動かすぞ
	SkinMeshAppMain(g_pD3DDev);

	g_pD3DDev->Release();
	g_pD3D->Release();

	return 0;
}


// スキンメッシュするぞー！
int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev) {
	// ポリゴンの頂点定義
	// 頂点数は15個
	Vertex vtx[15] = {
		{ D3DXVECTOR3(-0.5000f, -2.2887f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 2, 0, 0, 0 } },
	{ D3DXVECTOR3(-0.5000f, -1.2887f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 1, 2, 0, 0 } },
	{ D3DXVECTOR3(-0.5000f, -0.2887f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 0, 0, 0, 0 } },
	{ D3DXVECTOR3(-1.3660f,  0.2113f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 3, 4, 0, 0 } },
	{ D3DXVECTOR3(-2.2321f,  0.7113f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 4, 0, 0, 0 } },
	{ D3DXVECTOR3(-1.7321f,  1.5774f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 4, 0, 0, 0 } },
	{ D3DXVECTOR3(-0.8660f,  1.0774f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 3, 4, 0, 0 } },
	{ D3DXVECTOR3(0.0000f,  0.5774f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 0, 0, 0, 0 } },
	{ D3DXVECTOR3(0.8660f,  1.0774f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 5, 6, 0, 0 } },
	{ D3DXVECTOR3(1.7321f,  1.5774f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 6, 0, 0, 0 } },
	{ D3DXVECTOR3(2.2321f,  0.7113f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 6, 0, 0, 0 } },
	{ D3DXVECTOR3(1.3660f,  0.2113f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 5, 6, 0, 0 } },
	{ D3DXVECTOR3(0.5000f, -0.2887f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 0, 0, 0, 0 } },
	{ D3DXVECTOR3(0.5000f, -1.2887f, 0.0f), D3DXVECTOR3(0.50f, 0.50f, 0.00f),{ 1, 2, 0, 0 } },
	{ D3DXVECTOR3(0.5000f, -2.2887f, 0.0f), D3DXVECTOR3(1.00f, 0.00f, 0.00f),{ 2, 0, 0, 0 } },
	};

	// 頂点インデックス
	// ポリゴン数は13枚で三角形リストなので13×3 = 39個あります
	WORD idx[39] = {
		0, 1, 14,
		1, 13, 14,
		1, 2, 13,
		2, 12, 13,
		2, 7, 12,
		2, 6, 7,
		2, 3, 6,
		3, 5, 6,
		3, 4, 5,
		7, 8, 12,
		8, 11, 12,
		8, 9, 11,
		9, 10, 11,
	};

	// インデックスを辿って三角形リストを作成
	Vertex v[39];
	for (int i = 0; i < 39; i++)
		v[i] = vtx[idx[i]];

	// 頂点宣言
	D3DVERTEXELEMENT9 declAry[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	D3DDECL_END()
	};
	IDirect3DVertexDeclaration9 *decl = 0;
	g_pD3DDev->CreateVertexDeclaration(declAry, &decl);


	//////////////////////////////////////////
	// ボーン情報の作成
	//  必要なのは
	// ・ボーンオフセット行列
	// ・ボーン行列
	//////////////

	Bone *bones = new Bone[7];    // ボーン確保

								  // 親子関係を構築します
								  // 今回は手でべたべたとやります
	bones[0].firstChild = &bones[1];
	bones[1].firstChild = &bones[2];
	bones[3].firstChild = &bones[4];
	bones[5].firstChild = &bones[6];
	bones[1].sibling = &bones[3];
	bones[3].sibling = &bones[5];

	// 初期姿勢の計算
	// まずはローカル姿勢を設定して
	// 最終的に自分の親からの相対姿勢に直します。
	D3DXMatrixRotationZ(&bones[0].initMat, D3DXToRadian(-90.0f));
	D3DXMatrixRotationZ(&bones[1].initMat, D3DXToRadian(-90.0f));
	D3DXMatrixRotationZ(&bones[2].initMat, D3DXToRadian(-90.0f));
	D3DXMatrixRotationZ(&bones[3].initMat, D3DXToRadian(150.0f));
	D3DXMatrixRotationZ(&bones[4].initMat, D3DXToRadian(150.0f));
	D3DXMatrixRotationZ(&bones[5].initMat, D3DXToRadian(30.0f));
	D3DXMatrixRotationZ(&bones[6].initMat, D3DXToRadian(30.0f));
	bones[0].initMat._41 = 0.0000f; bones[0].initMat._42 = 0.0000f;
	bones[1].initMat._41 = 0.0000f; bones[1].initMat._42 = -1.0000f;
	bones[2].initMat._41 = 0.0000f; bones[2].initMat._42 = -2.0000f;
	bones[3].initMat._41 = -0.6830f; bones[3].initMat._42 = 0.3943f;
	bones[4].initMat._41 = -1.5490f; bones[4].initMat._42 = 0.8943f;
	bones[5].initMat._41 = 0.6830f; bones[5].initMat._42 = 0.3943f;
	bones[6].initMat._41 = 1.5490f; bones[6].initMat._42 = 0.8943f;

	// ボーンオフセット行列を計算しておきます。
	// オフセット行列は各ボーンの「ローカル姿勢」の逆行列です。
	D3DXMATRIX *combMat = new D3DXMATRIX[7];    // 合成変換行列。これがシェーダに渡ります
	for (int i = 0; i < 7; i++) {
		bones[i].id = i;
		bones[i].combMatAry = combMat;
		D3DXMatrixInverse(&bones[i].offsetMat, 0, &bones[i].initMat);
	}

	// 初期姿勢を親の姿勢からの相対姿勢に直します。
	// まず子の末端まで下りて、自分のローカル空間での初期姿勢 × 親のボーンオフセット行列で相対姿勢が出ます
	// 親子関係を辿るので再起関数が必要です。
	struct CalcRelativeMat {
		static void run(Bone* me, D3DXMATRIX *parentoffsetMat) {
			if (me->firstChild)
				run(me->firstChild, &me->offsetMat);
			if (me->sibling)
				run(me->sibling, parentoffsetMat);
			if (parentoffsetMat)
				me->initMat *= *parentoffsetMat;
		}
	};
	CalcRelativeMat::run(bones, 0);


	/////////////////////////////////////////
	// シェーダのコンパイルとシェーダ作成
	//////
	ID3DXBuffer *shader, *error;
	IDirect3DVertexShader9 *vertexShader;
	IDirect3DPixelShader9 *pixelShader;
	HRESULT res = D3DXCompileShader(vertexShaderStr, (UINT)strlen(vertexShaderStr), 0, 0, "main", "vs_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreateVertexShader((const DWORD*)shader->GetBufferPointer(), &vertexShader);
	shader->Release();
	res = D3DXCompileShader(pixelShaderStr, (UINT)strlen(pixelShaderStr), 0, 0, "main", "ps_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreatePixelShader((const DWORD*)shader->GetBufferPointer(), &pixelShader);
	shader->Release();

	// 各種行列初期化
	D3DXMATRIX view, proj;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0.0f, -5.0f, -4.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(30), 64.0f / 48, 1.0f, 10000.0f);


	////////////////////////////////////
	// ボーン描画用オブジェクト
	//   本質ではないので気にしない～(^-^;
	/////////
	ID3DXMesh *boneObj[7];
	float len[7] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	for (int i = 0; i < 7; i++)
		D3DXCreateCylinder(g_pD3DDev, 0.0f, 0.1f, len[i], 16, 1, &boneObj[i], 0);


	////////////////////////////////////////////
	// 描画ループ部
	//   毎フレームのボーンの姿勢更新がここで行われます
	/////////
	MSG msg;
	float val = 0.0f;
	float a[7] = { 0 };
	do {
		val += 0.03f;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
		}
		g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(40, 40, 80), 1.0f, 0);
		g_pD3DDev->BeginScene();

		///////////////////////////////////////
		// ボーンの姿勢を更新
		//  最終的には、
		//   [ボーンオフセット行列] × [ワールド空間でのボーンの姿勢]
		//  を計算します。

		// 各ボーンの初期姿勢からの差分姿勢（親空間ベース）を更新
		//  これは適当にぐにぐに動かして構わない部分です
		D3DXMATRIX defBone[7];
		D3DXMatrixIdentity(&defBone[0]);
		for (int i = 1; i < 7; i++) {
			D3DXMATRIX tmp;
			D3DXMatrixRotationY(&defBone[i], D3DXToRadian(sinf(val) * 70.0f));
		}

		// 各ボーン行列の親空間ベースでの姿勢を更新
		// 差分姿勢×初期姿勢（共に親空間ベース）です。
		for (int i = 0; i < 7; i++)
			bones[i].boneMat = defBone[i] * bones[i].initMat;

		// 親空間ベースにある各ボーン行列をローカル空間ベースの姿勢に変換
		// ここは親子関係に従って行列を掛ける必要があります
		// 掛ける順番は 子 × 親 です。
		D3DXMATRIX global;
		D3DXMatrixRotationZ(&global, val * 0.1f);
		struct UpdateBone {
			static void run(Bone* me, D3DXMATRIX *parentWorldMat) {
				me->boneMat *= *parentWorldMat;
				me->combMatAry[me->id] = me->offsetMat * me->boneMat;
				if (me->firstChild)
					run(me->firstChild, &me->boneMat);
				if (me->sibling)
					run(me->sibling, parentWorldMat);
			};
		};
		UpdateBone::run(bones, &global);

		// シェーダ設定
		// 変数を書き込むレジスタ位置はシェーダに書いてありますよ。
		g_pD3DDev->SetVertexShader(vertexShader);
		g_pD3DDev->SetPixelShader(pixelShader);
		g_pD3DDev->SetVertexShaderConstantF(0, (const float*)&view, 4);
		g_pD3DDev->SetVertexShaderConstantF(4, (const float*)&proj, 4);
		g_pD3DDev->SetVertexShaderConstantF(8, (const float*)combMat, 4 * 7);

		// ポリゴン描画
		// ワイヤーフレーム描画でカリング無しで
		g_pD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		g_pD3DDev->SetVertexDeclaration(decl);
		g_pD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 13, &v, sizeof(Vertex));


		////////////////////////////////
		// ボーンオブジェクト描画
		//   本質ではないので気にしない部分です(^-^;
		///////
		{
			D3DMATERIAL9 material = { { 1.0f, 1.0f, 1.0f, 1.0f } }; material.Power = 10.0f;
			D3DLIGHT9 light = { D3DLIGHT_DIRECTIONAL,{ 1.0f, 0.7f, 0.5f, 1.0f } };
			light.Direction = (D3DVECTOR)D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			g_pD3DDev->SetLight(0, &light);
			g_pD3DDev->LightEnable(0, TRUE);
			g_pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
			g_pD3DDev->SetMaterial(&material);
			g_pD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			g_pD3DDev->SetVertexShader(0);
			g_pD3DDev->SetPixelShader(0);
			g_pD3DDev->SetTransform(D3DTS_VIEW, &view);
			g_pD3DDev->SetTransform(D3DTS_PROJECTION, &proj);
			D3DXMATRIX boneObjRot;
			D3DXMatrixRotationY(&boneObjRot, D3DXToRadian(-90.0f));
			boneObjRot._41 = 0.5f;
			for (int i = 0; i < 7; i++) {
				g_pD3DDev->SetTransform(D3DTS_WORLD, &(boneObjRot * bones[i].boneMat));
				boneObj[i]->DrawSubset(0);
			}
		}

		g_pD3DDev->EndScene();
		g_pD3DDev->Present(NULL, NULL, NULL, NULL);
	} while (msg.message != WM_QUIT);

	for (int i = 0; i < 7; i++)
		boneObj[i]->Release();
	vertexShader->Release();
	pixelShader->Release();
	decl->Release();

	delete[] bones;
	delete[] combMat;

	return 0;
}