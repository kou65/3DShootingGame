#include"Graphics.h"
#include"../Com_ptr/Com_ptr.h"



LPDIRECT3DDEVICE9 Graphics::GetLpDirect3DDevice9() {
	return m_p_d3d_device9;
}

void Graphics::SetClearBackGroundColor(D3DCOLOR color) {
	background_color = color;
}

bool Graphics::Init(
	UINT width_size,
	UINT height_size,
	BOOL windowed,
	UINT back_buffer_count) {

	// 背景色(黒色)
	background_color = 0x0000000;

	// ウィンドウハンドルのnullチェック
	if (Window::GetWindowHandle() == NULL) {

		// ウィンドウハンドルの取得に失敗
		MessageBoxA(0, "GetWindowHandle...Error/Place...Graphics>Init", TEXT("MessageBoxA"), MB_OK);

		// エラーを返す
		return false;
	}

	// ウィンドウハンドル取得
	HWND h_wnd = Window::GetWindowHandle();

	// LPDIRECT3D9は生成した後ほとんど使用しない
	m_p_direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	// DIRECT3D9のnullチェック
	if (m_p_direct3d9 == NULL) {

		// IDirect3D9の生成に失敗
		MessageBoxA(0, "IDirect3D9Create...Error/Place...Graphics>Init", TEXT("MessageBoxA"), MB_OK);

		// エラーを返す
		return false;
	}

	// パーセントパラメーターズのサイズを0にする
	ZeroMemory(&m_d3d_pp, sizeof(D3DPRESENT_PARAMETERS));

	/* 描画設定 */

	// 横の解像度
	m_d3d_pp.BackBufferWidth = width_size;
	// 縦の解像度
	m_d3d_pp.BackBufferHeight = height_size;
	// ディスプレイモード
	m_d3d_pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	// バックバッファの数
	m_d3d_pp.BackBufferCount = back_buffer_count;
	// マルチサンプルの数
	m_d3d_pp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	// マルチサンプルの品質レベル
	m_d3d_pp.MultiSampleQuality = 0;
	// フロントバッファとバックバッファの切り替え方法
	m_d3d_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 画面を描画するウィンドウハンドル
	m_d3d_pp.hDeviceWindow = h_wnd;
	// スクリーンモード
	m_d3d_pp.Windowed = windowed;
	// 深度ステンシルバッファがあるかどうか
	m_d3d_pp.EnableAutoDepthStencil = TRUE;
	// 深度バッファフォーマット
	m_d3d_pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	// バックバッファからフロントバッファへ転送時のオプション
	m_d3d_pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	// フルスクリーンでのリフレッシュレート
	m_d3d_pp.FullScreen_RefreshRateInHz = 0;
	// スワップエフェクトの書き換えタイミング
	m_d3d_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// デバイスの作成
	m_p_direct3d9->CreateDevice(
		// ディスプレイアダプターの種類
		D3DADAPTER_DEFAULT,
		// デバイスの種類を設定
		D3DDEVTYPE_HAL,
		// デバイスが割り当てられるウィンドウハンドル
		h_wnd,
		// デバイス制御の組み合わせ
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		// デバイスを設定するためのD3DPRESENT_PARAMETERS構造体のアドレスを渡す
		&m_d3d_pp,
		// LPDIRECT3DDEVICE9のアドレスを渡す
		&m_p_d3d_device9
	);


	// D3Dデバイスのnullチェック
	if (m_p_d3d_device9 == NULL) {

		// デバイスの生成に失敗 
		MessageBoxA(0, "d3d_device9Create...Error/Place...Graphics>Init", TEXT("MessageBoxA"), MB_OK);

		// エラーを返す
		return false;
	}

	// 深度バッファ変更
	//m_p_d3d_device9->SetDepthStencilSurface();

	// 正常終了
	return true;

	/* MEMO
	デバイスの作成に失敗した場合
	PCの端末がDirectXに対応していない場合がある
	*/
}


void Graphics::PresentParametersConfig(
	D3DPRESENT_PARAMETERS & d3d_pp,
	HWND window_handle,
	UINT width_size,
	UINT height_size,
	UINT back_buffer_count,
	bool windowed
) {

	/* 描画設定 */

	// 横の解像度
	m_d3d_pp.BackBufferWidth = width_size;
	// 縦の解像度
	m_d3d_pp.BackBufferHeight = height_size;
	// ディスプレイモード
	m_d3d_pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	// バックバッファの数
	m_d3d_pp.BackBufferCount = back_buffer_count;
	// マルチサンプルの数
	m_d3d_pp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	// マルチサンプルの品質レベル
	m_d3d_pp.MultiSampleQuality = 0;
	// フロントバッファとバックバッファの切り替え方法
	m_d3d_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 画面を描画するウィンドウハンドル
	m_d3d_pp.hDeviceWindow = window_handle;
	// スクリーンモード
	m_d3d_pp.Windowed = windowed;
	// 深度ステンシルバッファがあるかどうか
	m_d3d_pp.EnableAutoDepthStencil = TRUE;
	// ステンシルバッファのフォーマット
	m_d3d_pp.AutoDepthStencilFormat = D3DFMT_D24S8;// D3DFMT_D16フォーマットは深度バッファに対応しない
	// バックバッファからフロントバッファへ転送時のオプション
	m_d3d_pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	// フルスクリーンでのリフレッシュレート
	m_d3d_pp.FullScreen_RefreshRateInHz = 0;
	// スワップエフェクトの書き換えタイミング
	m_d3d_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

}


bool Graphics::DrawStart()
{
	// シーンのクリア
	m_p_d3d_device9->Clear(
		// D3DRECT*の矩形の数
		0,
		// ビューポート全体をクリア
		NULL,
		// Zバッファとステンシルをクリア       
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		// クリアする色情報(背景色)
		background_color,
		// 深度バッファで使用(未使用なら0,f)
		1.0f,
		// ステンシルバッファで使用する値(未使用なら0)
		0
	);

	// シーン描画を開始する
	if (D3D_OK == m_p_d3d_device9->BeginScene())
	{
		// 正常終了
		return true;
	}

	// エラー
	return false;
}


void Graphics::DrawEnd()
{
	// シーン描画終了
	m_p_d3d_device9->EndScene();
	// バッファ転送(バックバッファに描画してあるものをフロントバッファに送る)
	m_p_d3d_device9->Present(
		NULL       // 転送元矩形
		, NULL	   // 転送先矩形
		, NULL	   // ウィンドウハンドル
		, NULL);   // 基本NULL
}


void Graphics::SetScreenMode(BOOL is_screen_mode) {

	// スクリーンモードの変更
	m_d3d_pp.Windowed = is_screen_mode;

	// デバイスの設定を変更する
	m_p_d3d_device9->Reset(&m_d3d_pp);
}


void Graphics::BackBufferReSize(const int&width_size, const int&height_size) {

	// 解像度変更
	m_d3d_pp.BackBufferWidth = width_size;
	m_d3d_pp.BackBufferHeight = height_size;

	// デバイスの設定を変更する
	m_p_d3d_device9->Reset(&m_d3d_pp);
}



void Graphics::Release() {

	// 生成した時と逆の順番で解放
	m_p_d3d_device9->Release();
	m_p_direct3d9->Release();
}

/* GetViewPortやSetViewPortなどもある*/

void Graphics::SetUpViewPort(DWORD x, DWORD y, DWORD width, DWORD height, FLOAT min_z, FLOAT max_z) {

	D3DVIEWPORT9 d3d_view_port9;

	// ビューポートの設定
	d3d_view_port9.X = x;           // X座標
	d3d_view_port9.Y = y;           // Y座標
	d3d_view_port9.Width = width;   // 幅
	d3d_view_port9.Height = height; // 高さ
	d3d_view_port9.MinZ = 0.f;	     // Z深度 : 最小
	d3d_view_port9.MaxZ = 1.f;	     // Z深度 : 最大

									 // ビューポート設定
	SetViewPort(d3d_view_port9);
}


void Graphics::SetViewPort(D3DVIEWPORT9 view_port) {

	// ビューポートをデバイスにセットできなかったとき
	if (m_p_d3d_device9->SetViewport(&view_port) != D3D_OK) {
		MessageBoxA(0, "SetViewPort...Error/Place...Graphics>SetViewPort", TEXT("MessageBoxA"), MB_OK);
	}
}


bool Graphics::GetViewPort(D3DVIEWPORT9 *d3d_view_port9) {

	// ビューポートのパラメータを受け取る
	HRESULT view_port_parameter = m_p_d3d_device9->GetViewport(d3d_view_port9);

	// 受け取れなかった場合
	if (view_port_parameter != D3D_OK) {
		MessageBoxA(0, "GetViewPort...Error/Place...Graphics>GetViewPort", TEXT("MessageBoxA"), MB_OK);
		return false;
	}
	// 正常終了
	return true;
}


void Graphics::SamplerStateUMirror() {

	// 描画Uを反転させる
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
}


void Graphics::SamplerStateVMirror() {
	// 描画Vを反転させる
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
}


void Graphics::SamplerStateClamp() {

	// 描画UVループテクスチャ描画なし
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}


void Graphics::SamplerStateWrap() {

	// 描画UVループテクスチャ描画あり
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}


void Graphics::SamplerStateBorderColor() {
	m_p_d3d_device9->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xffffff);
}


bool Graphics::GetSamplerState(DWORD sampler_stage_index, D3DSAMPLERSTATETYPE member_type, DWORD *get_state) {

	if (m_p_d3d_device9->GetSamplerState(sampler_stage_index, member_type, get_state) != MB_OK) {
		return false;
	}

	return true;
}


bool Graphics::CreateIndexBuffer16BitSize(
	LPDIRECT3DINDEXBUFFER9 *p_index_buffer,
	const int &index_size) {

	// インデックスバッファ作成
	GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		(index_size),
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX16,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		p_index_buffer,
		// 現在使用されていないので基本NULL
		NULL
	);

	// nullチェック
	if (p_index_buffer == nullptr) {
		return false;
	}

	return true;
}


unsigned short* Graphics::LockIndexBuffer16BitSize(
	LPDIRECT3DINDEXBUFFER9*p_index_buffer
) {

	WORD *size_16bit;

	if (FAILED((*p_index_buffer)->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&size_16bit,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		0
	)
	)
		) {
		size_16bit = NULL;
		return size_16bit;
	}

	return size_16bit;
}


bool Graphics::UnlockIndexBuffer(
	LPDIRECT3DINDEXBUFFER9*p_index_buffer
) {
	(*p_index_buffer)->Unlock();
	return true;
}


bool Graphics::CreateIndexBuffer32BitSize(
	LPDIRECT3DINDEXBUFFER9 *p_index_buffer,
	const int &index_size
) {

	// インデックスバッファ作成
	GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		(index_size),
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX32,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		p_index_buffer,
		// 現在使用されていないので基本NULL
		NULL
	);

	// nullチェック
	if (p_index_buffer == nullptr) {
		return false;
	}

	return true;
}


int *Graphics::LockIndexBuffer32BitSize(
	LPDIRECT3DINDEXBUFFER9*p_index_buffer
) {

	int *size_32bit;

	if (FAILED((*p_index_buffer)->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&size_32bit,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		0
	)
	)
		) {
		size_32bit = NULL;
		return size_32bit;
	}

	return size_32bit;
}



bool Graphics::CreateVertexBuffer9(
	LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer,
	const UINT&buffer_size
){

	// 頂点バッファ作成
	GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		buffer_size,
		// リソースの使用法
		0,
		// 柔軟な頂点フォーマットの型を指定する
		0,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		p_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	return true;
}


void Graphics::LockVertexBuffer(
	LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer,
	const UINT&buffer_size,
	void **pp_data
) {
	// ロック
	(*p_vertex_buffer)->Lock(
		0,
		buffer_size,
		(void**)&pp_data,
		0
	);
}


void Graphics::UnlockVertexBuffer(
	LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer
) {
	(*p_vertex_buffer)->Unlock();
}

/*
Zテクスチャを作成する理由は既存の深度バッファと
Z値計算用深度バッファを分けて描画する為

*/

void Graphics::SetUpSurface9() {

	// 深度バッファの幅と高さを取得
	UINT width;
	UINT height;

	IDirect3DSurface9*p_suf;
	D3DSURFACE_DESC suf_desc;

	// 深度バッファサーフェイスを取得
	m_p_d3d_device9->GetDepthStencilSurface(&p_suf);

	// サーフェイス情報取得
	p_suf->GetDesc(&suf_desc);

	width = suf_desc.Width;
	height = suf_desc.Height;

	// 解放を忘れない
	p_suf->Release();

	IDirect3DTexture9*p_tex;

	// Zテクスチャの作成
	D3DXCreateTexture(
		m_p_d3d_device9,
		width,
		height,
		// ミップマップレベルは必ず1に指定
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&p_tex
	);

	IDirect3DSurface9 * p_z_buffer_surf;

	// テクスチャサイズが2のべき乗になっていないか注意

	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する必要がある
	m_p_d3d_device9->CreateDepthStencilSurface(
		// Zテクスチャの実質のサイズ
		width,
		height,

		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		FALSE,
		// もう一度受け取る
		&p_z_buffer_surf,
		NULL
	);

	IDirect3DSurface9 * p_dev_back_surf;
	IDirect3DSurface9 * p_dev_z_buf;

	// レンダリングターゲットを切り替える
	m_p_d3d_device9->GetRenderTarget(0, &p_dev_back_surf);

	m_p_d3d_device9->SetRenderTarget(0, p_z_buffer_surf);

	// エフェクトを作成
	Com_ptr<ID3DXEffect>cpEffect;

	// シェーダーファイル作成
	D3DXCreateEffectFromFile(
		m_p_d3d_device9,
		TEXT("PixelShader.hlsl"),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		cpEffect.ToCreator(),
		NULL
	);

	// エフェクト内のワールドビュー射影変換行列を設定
	D3DXMATRIX mat, view, proj;

	// プロジェクション
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DXToRadian(45),
		640.f / 480.f,
		20.f,
		300.f
	);

	// ビュー
	D3DXMatrixLookAtLH(
		&view,
		&D3DXVECTOR3(30, 20, 30),
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		&D3DXVECTOR3(0.f, 1.f, 0.f)
	);

	D3DXMatrixIdentity(&mat);

	// ワールドビュー射影行列作成
	mat = mat * view * proj;

	// ワールドビュー射影変換行列設定
	cpEffect.GetPtr()->SetMatrix("matWorldViewProj", &mat);
	
}