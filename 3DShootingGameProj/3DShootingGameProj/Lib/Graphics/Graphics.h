#pragma once
#include"../Window/Window.h"


// ==================================
/**
* @file Graphics.h
* @brief DirectX9Graphicsファイル
* @author maekawa
* @2019/06/11
*/
// ==================================



class Graphics {
public:

	enum SamplerStateType {

		// つなぎ目なし
		CLAMP,
		// ワープ
		WARP,
		// 反転
		MIRROR,
	};


public:

	// シングルトン
	static Graphics *GetInstance() {
		static Graphics graphics;
		return &graphics;
	}

public:


	/**
	* @brief Graphicsの初期化
	* @param[in] (width_size) 横のバックバッファサイズ
	* @param[in] (height_size) 縦のバックバッファサイズ
	* @param[in] (windowed) ウィンドウモード
	* @param[in] (back_buffer_count) バックバッファの数
	* @return bool 成功すればtrue,失敗すればfalseを返す
	*/
	bool Init(
		UINT width_size = 1920,
		UINT height_size = 1080,
		BOOL windowed = TRUE,
		UINT back_buffer_count = 1
	);


	/**
	* @brief スクリーンモードの変更
	* @param[in] is_screen_mode スクリーンモードはtrue,フルスクリーンモードはfalse
	*/
	void SetScreenMode(BOOL is_screen_mode);


	/**
	* @brief 解像度の変更
	* @param[in] width_size 横のバックバッファサイズ
	* @param[in] height_size 縦のバックバッファサイズ
	*/
	void BackBufferReSize(const int&width_size, const int&height_size);


	/**
	* @brief 描画クリア関数
	*/
	void Clear(const D3DCOLOR&color = -1);


	/**
	* brief ビューポートの設定
	* @param[in] x 位置X
	* @param[in] y 位置Y
	* @param[in] width ビューポートサイズ横
	* @param[in] height ビューポートサイズ縦
	* @param[in] min_z Z深度最小
	* @param[in] max_z Z深度最大
	*/
	void SetUpViewPort(
		DWORD x,
		DWORD y,
		DWORD width,
		DWORD height,
		FLOAT min_z = 0.f,
		FLOAT max_z = 1.f);


	/**
	* brief 描画領域を伝える
	* @param[in] view_port D3DVIEWPORT9のポインタ
	*/
	void SetViewPort(D3DVIEWPORT9 view_port);


	/**
	* brief 現在DirectXに設定されているビューポートパラメータを受け取る
	* @param[out] d3d_view_port9 デバイスに現在設定されているビューポートパラメータを取得
	* @return bool パラメータの取得に成功すればtrue,失敗すればfalseを返します
	*/
	bool GetViewPort(D3DVIEWPORT9 *d3d_view_port9);


	/**
	* @brief SetSamplerStateでU軸をミラーに変更する
	*/
	void SamplerStateUMirror();


	/**
	* @brief SetSamplerStateでV軸をミラーに変更する
	*/
	void SamplerStateVMirror();


	/**
	* @brief SetSamplerStateをクランプに変更する
	*/
	void SamplerStateClamp();


	/**
	* @brief SetSamplerStateをワープに変更する
	*/
	void SamplerStateWrap();


	/**
	* @brief SetSamplerStateをボーダーカラーに変更する
	*/
	void SamplerStateBorderColor();


	/**
	* @brief DirectX上に設定してあるSamplerStateを取得する
	* @param[in] sampler_stage_index サンプラーステージのインデックス
	* @param[in] member_type 任意のパラメータをメンバにできる
	* @param[out] *get_state 取得する状態値、member_typeによって変わる
	* @return 取得に成功すればtrue、失敗すればfalseを返す
	*/
	bool GetSamplerState(
		DWORD sampler_stage_index,
		D3DSAMPLERSTATETYPE member_type,
		DWORD *get_state);


	/**
	* @brief デバイスのゲッター
	* @return LPDIRECT3DDEVICE9を返す
	*/
	LPDIRECT3DDEVICE9 GetDevice();


	/**
	* @brief 描画の開始
	* @return bool 描画に成功すればtrue,失敗すればfalseを返す
	*/
	bool DrawStart();


	/**
	* @brief 描画の終了
	*/
	void DrawEnd();


	/**
	* @brief グラフィックスの解放
	*/
	void Release();


	/**
	* @brief 背景色変更
	* @param[in] color カラー色
	*/
	void SetClearBackGroundColor(D3DCOLOR color);


	/**
	* @brief バーテックスバッファ16bit作成
	* @param[in] int index_size かならず16bit掛け算する(UWORD)
	* @param[out] IDirect3DVertexBuffer9
	* @return 生成できたらtrueできなかったらfalseを返す
	*/
	bool CreateIndexBuffer16BitSize(
		LPDIRECT3DINDEXBUFFER9 *p_index_buffer,
		const int &index_size);

	/**
	* @brief バーテックスバッファ32bit作成
	* @param[in] int index_size　必ず32bit掛け算する(int)
	* @param[out] IDirect3DVertexBuffer9
	* @return 生成できたらtrueできなかったらfalseを返す
	*/
	bool CreateIndexBuffer32BitSize(
		LPDIRECT3DINDEXBUFFER9 *p_index_buffer,
		const int &index_size
	);

	/**
	* @brief インデックスバッファ16bitのロック
	* @param[out] p_index_buffer インデックスバッファ
	* @return unsigned short
	*/
	unsigned short *LockIndexBuffer16BitSize(
		LPDIRECT3DINDEXBUFFER9*p_index_buffer
	);


	/**
	* @brief インデックスバッファ32bitのロック
	* @param[out] p_index_buffer インデックスバッファ
	* @return int
	*/
	int *LockIndexBuffer32BitSize(
		LPDIRECT3DINDEXBUFFER9*p_index_buffer
	);


	/**
	* @brief インデックスバッファのアンロック
	* @param[out] p_index_buffer インデックスバッファ
	* @return bool
	*/
	bool UnlockIndexBuffer(
		LPDIRECT3DINDEXBUFFER9*p_index_buffer
	);


	/**
	* @brief バーテックスバッファの生成
	* @param[out] バーテックスバッファ
	* @param[in] バッファのサイズ(sizeofで割り出せばいい)
	* sizeof(CustomVertex) * vertex_num 
	*/
	bool CreateVertexBuffer9(
		LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer,
		const UINT&buffer_size
	);


	/**
	* @brief バーテックスバッファのロック(書き込み可能)
	* @param[out] バーテックスバッファ
	* @param[in]  バッファのサイズ(sizeofで割り出せばいい)
	* sizeof(CustomVertex) * vertex_num
	* @param[out] 保存するデータ型を入れる
	* customvertexなど
	*/
	void LockVertexBuffer(
		LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer,
		const UINT&buffer_size,
		void **pp_data
	);


	/**
	@brief バーテックスバッファのアンロック
	(書き込み不可、描画可能状態に変更)
	*/
	void UnlockVertexBuffer(
		LPDIRECT3DVERTEXBUFFER9*p_vertex_buffer
	);


	/**
	@brief 現在デバイスに設定されている変換行列を受け取る
	@param[in] type x9が用意している変換行列定数 D3DTS_VIEWなど
	*/
	D3DXMATRIX GetTSMatrix(const D3DTRANSFORMSTATETYPE&type);


	/**
	* @brief バックバッファ設定
	*/
	void SetRenderSurface(
		const int &render_index,
		IDirect3DSurface9*p_suf
	);

	/**
	* @brief バックバッファ解像度サイズを返す
	* @return D3DXVECTOR2 縦サイズと横サイズ
	*/
	D3DXVECTOR2 GetBackBufferSize()const;


	/**
	* @brief サーフェイス用テクスチャ作成
	*/
	void CreateTexture(
		LPDIRECT3DTEXTURE9*p_tex,
		const int&width,
		const int&height,
		const D3DFORMAT&format = D3DFMT_A8R8G8B8
	);


	/**
	* @brief テクスチャサイズのサーフェイスを返す
	*/
	void Graphics::GetTexSurfaceLevel(
		LPDIRECT3DTEXTURE9&p_tex,
		LPDIRECT3DSURFACE9&sur
	);


	/**
	* @brief 現在設定してあるバックバッファ取得
	*/
	LPDIRECT3DSURFACE9 GetRenderSurfacePtr(
	const int&render_index
	);


private:

	/**
	* @brief パーセントパラメータズ設定
	* @param[out] パーセントパラメーターズ構造体 
	* @param[in] ウィンドウハンドル
	* @param[in] 横の解像度サイズ
	* @param[in] 縦の解像度サイズ
	* @param[in] バックバッファのカウント
	* @param[in] ウィンドウモード
	*/
	void PresentParametersConfig(
		D3DPRESENT_PARAMETERS & d3d_pp,
		HWND window_handle,
		UINT widht_size,
		UINT height_size,
		UINT back_buffer_count,
		bool windowed
	);


	/**
	@brief 自動検出してゲッターを行う
	*/
	operator IDirect3DDevice9*() const {
		return mp_d3d_device9;
	}


private:

	//! グラフィック関連のデバイスを管理
	IDirect3DDevice9 *mp_d3d_device9;     

	//! 環境を設定するためのインターフェース
	IDirect3D9 *mp_direct3d9;	     

	//! グラフィックス設定用
	D3DPRESENT_PARAMETERS m_d3d_pp = {};   

	//! 背景色
	D3DCOLOR background_color;
};
