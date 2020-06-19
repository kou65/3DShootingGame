#pragma once
#include"../../ShaderFunc/ColorShader/ColorShader.h"
#include"../../ShaderFunc//StandardTransformShader/StandardTransformShader.h"
#include"../../../Surface/Surface.h"




/**
* @brief ブラフィルター(ぼかし)クラス
*/
class BlurFilter : public ShaderBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] ブラフィルターバックバッファ解像度サイズ横
	* @param[in] ブラフィルターバックバッファ解像度サイズ縦
	*/
	BlurFilter(
		const int &width = Graphics::GetInstance()->GetBackBufferSize().x,
		const int &height = Graphics::GetInstance()->GetBackBufferSize().y ,
		const VertexDecl::Type&type = VertexDecl::Type::OBJ
	);


	/**
	* @brief 更新
	*/
	void Update()override;


	/**
	* @brief 描画の開始
	*/
	void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num = 0
	)override;


	/**
	* @brief 描画の終了
	*/
	void End()override;


	/**
	* @brief パス開始
	*/
	void BeginPass(
		const int &out_pass_num = 0
	)override;


	/**
	* @brief テクスチャセット
	*/
	void SetTexture(IDirect3DTexture9*p_tex);

	/**
	* @brief 変換情報セット
	*/
	void SetTransform(
		const D3DXMATRIX&world_mat,
		const D3DXMATRIX&view_mat,
		const D3DXMATRIX&proj_mat
	);

private:

	//! 総配列数
	static constexpr int ARRAY = 5;

	//! サーフェイスの数
	static constexpr int SURFACE_VALUE = 2;

private:


	/**
	* @brief 初期化
	*/
	void Init(
		const VertexDecl::Type&type = VertexDecl::Type::OBJ
	);

private:


	//! テクセルハンドル
	D3DXHANDLE m_h_tu;
	D3DXHANDLE m_h_tv;

	//! 変換ハンドル
	D3DXHANDLE m_h_world;
	D3DXHANDLE m_h_view;
	D3DXHANDLE m_h_proj;

	//! テクスチャハンドル
	D3DXHANDLE m_h_tex;

	//! バックバッファサーフェイス(パスは2個使うので2つ)
	Surface m_bf_surface_list[SURFACE_VALUE];

	//! デバイスサーフェイス
	Surface m_device_surface;

	//! デバイス
	IDirect3DDevice9*mp_device;

	//! 表示するサーフェイスバックバッファ解像度サイズx
	int m_width;

	//! 表示するサーフェイスバックバッファ解像度サイズx
	int m_height;
};