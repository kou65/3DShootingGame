#pragma once
#include"../LightShader/LightShader.h"
#include"../ShadowData/ShadowData.h"



/**
* @brief ライト影シェーダー
*/
class LightShadowShader : public Light{
public:


	/**
	* @brief コンストラクタライト影
	*/
	LightShadowShader() {};


	/**
	* @brief 更新
	*/
	void Update()override;


	/**
	* @brief 初期化
	*/
	void Init(const VertexDecl::Type&type);


	/**
	* @brief シャドウデータ
	*/
	void SetShadowData(const ShadowData&shadow_data);


	/**
	* @brief テクスチャポインタ
	*/
	void SetShadowMap(IDirect3DTexture9*p_tex);


private:


	/**
	* @brief ハンドルの初期化
	*/
	void InitHandle();


	/**
	* @brief 影ハンドルの初期化
	*/
	void InitShadowHandle();


	/**
	* @brief 影のデータ送信
	*/
	void CommitShadowData();


private:

	//! 影データ
	ShadowData m_shadow_data;

	//! 各ハンドル
	D3DXHANDLE m_h_light_view;
	D3DXHANDLE m_h_light_proj;
	D3DXHANDLE m_h_shadow_map;
};
