﻿#include"Graphics.h"



LPDIRECT3DDEVICE9 Graphics::GetDevice() {
	return mp_d3d_device9;
}


void Graphics::SetClearBackGroundColor(D3DCOLOR color) {
	background_color = color;
}


bool Graphics::Init(
	UINT width_size,
	UINT height_size,
	BOOL windowed,
	UINT back_buffer_count) {

	// 背景色(黒色)デフォルト
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
	mp_direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	// DIRECT3D9のnullチェック
	if (mp_direct3d9 == NULL) {

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
	m_d3d_pp.AutoDepthStencilFormat = D3DFMT_D24S8;// D3DFMT_D16では描画に支障をきたした
	// バックバッファからフロントバッファへ転送時のオプション
	m_d3d_pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	// フルスクリーンでのリフレッシュレート
	m_d3d_pp.FullScreen_RefreshRateInHz = 0;
	// スワップエフェクトの書き換えタイミング
	m_d3d_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// デバイスの作成
	mp_direct3d9->CreateDevice(
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
		&mp_d3d_device9
	);


	// D3Dデバイスのnullチェック
	if (mp_d3d_device9 == NULL) {

		// デバイスの生成に失敗 
		MessageBoxA(0, "d3d_device9Create...Error/Place...Graphics>Init", TEXT("MessageBoxA"), MB_OK);

		// エラーを返す
		return false;
	}

	// 深度バッファ変更
	//mp_d3d_device9->SetDepthStencilSurface();

	// 正常終了
	return true;

	/* MEMO
	デバイスの作成に失敗した場合
	PCの端末がDirectXに対応していない場合がある
	*/
}



void Graphics::Release() {

	// 生成した時と逆の順番で解放
	mp_d3d_device9->Release();
	mp_direct3d9->Release();
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
	mp_d3d_device9->Clear(
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
	if (D3D_OK == mp_d3d_device9->BeginScene())
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
	mp_d3d_device9->EndScene();
	// バッファ転送(バックバッファに描画してあるものをフロントバッファに送る)
	mp_d3d_device9->Present(
		NULL       // 転送元矩形
		, NULL	   // 転送先矩形
		, NULL	   // ウィンドウハンドル
		, NULL);   // 基本NULL
}


void Graphics::SetScreenMode(BOOL is_screen_mode) {

	// スクリーンモードの変更
	m_d3d_pp.Windowed = is_screen_mode;

	// デバイスの設定を変更する
	mp_d3d_device9->Reset(&m_d3d_pp);
}


void Graphics::BackBufferReSize(const int&width_size, const int&height_size) {

	// 解像度変更
	m_d3d_pp.BackBufferWidth = width_size;
	m_d3d_pp.BackBufferHeight = height_size;

	// デバイスの設定を変更する
	mp_d3d_device9->Reset(&m_d3d_pp);
}


void Graphics::Clear(
	const D3DCOLOR&color
) {

	// カラー値代入
	D3DCOLOR col = color;

	if (col == -1) {
		col = background_color;
	}


	// シーンのクリア
	mp_d3d_device9->Clear(
		// D3DRECT*の矩形の数
		0,
		// ビューポート全体をクリア
		NULL,
		// Zバッファとステンシルをクリア       
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		// クリアする色情報(背景色)
		col,
		// 深度バッファで使用(未使用なら0,f)
		1.0f,
		// ステンシルバッファで使用する値(未使用なら0)
		0
	);
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
	if (mp_d3d_device9->SetViewport(&view_port) != D3D_OK) {
		MessageBoxA(0, "SetViewPort...Error/Place...Graphics>SetViewPort", TEXT("MessageBoxA"), MB_OK);
	}
}


bool Graphics::GetViewPort(D3DVIEWPORT9 *d3d_view_port9) {

	// ビューポートのパラメータを受け取る
	HRESULT view_port_parameter = mp_d3d_device9->GetViewport(d3d_view_port9);

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
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
}


void Graphics::SamplerStateVMirror() {
	// 描画Vを反転させる
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
}


void Graphics::SamplerStateClamp() {

	// 描画UVループテクスチャ描画なし
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}


void Graphics::SamplerStateWrap() {

	// 描画UVループテクスチャ描画あり
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}


void Graphics::SamplerStateBorderColor() {
	mp_d3d_device9->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xffffff);
}


bool Graphics::GetSamplerState(DWORD sampler_stage_index, D3DSAMPLERSTATETYPE member_type, DWORD *get_state) {

	if (mp_d3d_device9->GetSamplerState(sampler_stage_index, member_type, get_state) != MB_OK) {
		return false;
	}

	return true;
}


bool Graphics::CreateIndexBuffer16BitSize(
	LPDIRECT3DINDEXBUFFER9 *p_index_buffer,
	const int &index_size) {

	// インデックスバッファ作成
	GetDevice()->CreateIndexBuffer(
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
	GetDevice()->CreateIndexBuffer(
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
	GetDevice()->CreateVertexBuffer(
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


D3DXMATRIX Graphics::GetTSMatrix(
	const D3DTRANSFORMSTATETYPE&type
) {

	D3DXMATRIX mat;

	HRESULT hr = mp_d3d_device9
		->GetTransform(type, &mat);

	if (hr != S_OK) {
		return mat;
	}

	return mat;
}


void Graphics::SetRenderSurface(
	const int &render_index,
IDirect3DSurface9*p_suf
) {

	// 使うバックバッファセット
	mp_d3d_device9->SetRenderTarget(render_index,p_suf);
}


D3DXVECTOR2 Graphics::GetBackBufferSize()const {

	D3DXVECTOR2 vec2;

	vec2.x = (float)m_d3d_pp.BackBufferWidth;
	vec2.y = (float)m_d3d_pp.BackBufferHeight;

	return vec2;
}


void Graphics::CreateTexture(
	LPDIRECT3DTEXTURE9*p_tex,
	const int&width,
	const int&height,
	const D3DFORMAT&format
) {

	// テクスチャ作成
	mp_d3d_device9->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		format,
		D3DPOOL_DEFAULT,
		&*p_tex,
		NULL
	);

}

void Graphics::GetTexSurfaceLevel(
	LPDIRECT3DTEXTURE9&p_tex,
	LPDIRECT3DSURFACE9&p_sur
) {


	p_tex->GetSurfaceLevel(0, &p_sur);
}


LPDIRECT3DSURFACE9 Graphics::GetRenderSurfacePtr(
	const int &render_index
) {

	LPDIRECT3DSURFACE9 suf;
	mp_d3d_device9->GetRenderTarget(render_index,&suf);

	return suf;

}
