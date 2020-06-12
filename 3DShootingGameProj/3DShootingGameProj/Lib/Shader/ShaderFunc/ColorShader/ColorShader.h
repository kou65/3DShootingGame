#pragma once
#include"../StandardTransformShader/StandardTransformShader.h"



/**
* @brief ノーマルな色を取り扱うシェーダークラス
*/
class ColorShader : public StandardTSShader{
public:


	/**
	* @brief コンストラクタ
	*/
	ColorShader();


	/**
	* @brief 更新
	*/
	void UpdateColor();


	/**
	* @brief カラーシェーダー初期化
	*/
	void InitColorHandle(
		const std::string&color_handle_name,
		const std::string&texture_handle_name
	);


	/**
	* @brief ポリゴン全体のカラー値
	*/
	void SetColor(const D3DXVECTOR4&color);


	/**
	* @brief テクスチャ
	*/
	void SetTexture(IDirect3DTexture9*p_tex);

private:

	/**
	* @brief 更新
	*/
	void UpdateColorPrivate();

protected:

	//! 全体のカラー
	D3DXVECTOR4 m_color;

	//! テクスチャ
	LPDIRECT3DTEXTURE9 mp_texture;

	//! カラーハンドル
	D3DXHANDLE m_h_color;

	//! テクスチャハンドル
	D3DXHANDLE m_h_texture;
};