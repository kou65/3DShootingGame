#pragma once
#include"../LightShader/LightShader.h"
#include"../../MultRenderTarget/MultRenderTarget.h"


/* ソフトシャドウ描画の流れ
レンダーターゲットセット
ライトから見たz深度値算出
次のshaderのSetTextureでCreateTextureで受け取ったtextureをセット
レンダーターゲットセット
2パス目で輪郭算出
SetTextureでCreateTextureで受け取ったtextureをセット
レンダーターゲットセット
3パス目で横ぼかし
SetTextureでCreateTextureで受け取ったtextureをセット
レンダーターゲットセット
4パス目で縦ぼかし
SetTextureでCreateTextureで受け取ったtextureをセット
レンダーターゲットセット
5パス目で深度値比較した影付きモデル描画
*/

/**
* @brief ソフトシャドウ
*/
class SoftShadow : public Light{
public:

	static constexpr int MAX_RENDER_INDEX = 3;

public:


	/**
	* @brief コンストラクタ
	*/
	SoftShadow(
		const VertexDecl::Type&type = VertexDecl::Type::OBJ
	);


	/**
	* @brief 初期化
	*/
	void InitHandle();


	/**
	* @brief 更新
	*/
	void Update()override;


	/**
	* @brief 描画開始
	*/
	void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num
	)override;

	void End()override;

	void InitAndCreateSurface();

	void SetLightView(const D3DXMATRIX&mat);
	void SetLightProj(const D3DXMATRIX&mat);
	void SetModelTex(LPDIRECT3DTEXTURE9&tex);
	void SetShadowMap(LPDIRECT3DTEXTURE9&tex);
	void SetMapSize(const float &size);
	LPDIRECT3DTEXTURE9 GetTexture();

	// レンダーインデックスのセット
	void SetRenderIndex(const int&index);

	void DrawTex(LPDIRECT3DTEXTURE9 tex = nullptr);

private:

	// 逆転置テクスチャ変換行列
	D3DXMATRIX m_t;

	// ライト射影行列
	D3DXMATRIX m_light_proj;
	// ライトビュー行列
	D3DXMATRIX m_light_view;
	
	// シャドウマップ
	LPDIRECT3DTEXTURE9 mp_shadow_map;
	// オブジェクトテクスチャ
	LPDIRECT3DTEXTURE9 mp_model_tex;

	// シャドウマップサイズ
	float m_map_size;

	// 頂点変換行列
	D3DXMATRIX m_wvp;
	
	// シャドウマップを貼るため
	// ワールド * ライトビュー * ライト射影 * テクスチャ
	D3DXMATRIX m_wvpb;

	// 深度計算の為
	D3DXMATRIX m_wlp;

	// texhandle
	D3DXHANDLE m_h_model_tex;
	D3DXHANDLE m_h_shadow_map;

	D3DXHANDLE m_h_map_size;

	// 変換ハンドル
	D3DXHANDLE m_h_wvp;
	D3DXHANDLE m_h_wvpb;
	D3DXHANDLE m_h_wlp;

	// レンダーインデックス
	int m_r_index;

	// レンダーターゲット
	RenderTarget m_rt[MAX_RENDER_INDEX];

	// マルチレンダリング
	MultRenderTarget m_mrt;

	//! 固定スプライト
	ID3DXSprite* mp_sprite;
};