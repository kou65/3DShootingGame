#pragma once
#include"../ColorShader/ColorShader.h"
#include"../LightData/LightData.h"



/**
* @brief ライトシェーダー
*/
class Light : public ColorShader{
public:


	/**
	* @brief コンストラクタ
	*/
	Light();

	/**
	* @brief 更新
	*/
	void UpdateLight();


	/**
	* @brief ライトデータセット
	*/
	void SetLightData(const LightData&light_data);



protected:


	/**
	* @brief マテリアルハンドルの初期化
	* @param[in] mt_ambient_name 環境光名
	* @param[in] mt_specular_name 反射光名
	* @param[in] mt_diffuse_name 拡散光名
	* @param[in] ambient_name マテリアル以外の環境光名
	*/
	void InitMateHandle(
		const std::string&mt_ambient_name,
		const std::string&mt_specular_name,
		const std::string&mt_diffuse_name,
		const std::string&ambient_name
	);


	/**
	* @brief カメラハンドル
	* @param[in] mt_eye_pos カメラ位置
	* @param[in] mt_eye_dir カメラの向いている方向
	*/
	void InitCameraHandle(
		const std::string&mt_eye_pos,
		const std::string&mt_eye_dir
	);


	/**
	* @brief 平行光源ライトハンドル
	* @param[in] mt_light_dir ライト方向
	* @param[in] mt_light_color ライトカラー
	*/
	void InitLightDirHandle(
		const std::string&mt_light_dir,
		const std::string&mt_light_color
	);


	/**
	* @brief 点光源ライトハンドル
	*/
	void InitPointLightHandle(
		const std::string&g_pos,
		const std::string&mt_diffuse,
		const std::string&mt_specular,
		const std::string&attenuate_name
	);


protected:

	/* ライト変数データ */
	LightData m_light_data;
	
	/* 各ハンドル */

	//! ワールドビュー射影
	D3DXHANDLE m_h_wvp;

	//! マテリアル
	D3DXHANDLE m_h_mt_ambient;
	D3DXHANDLE m_h_mt_specular;
	D3DXHANDLE m_h_mt_diffuse;
	D3DXHANDLE m_h_ambient;

	//! カメラ
	D3DXHANDLE m_h_eye_pos;
	D3DXHANDLE m_h_eye_dir;

	//! ライト系
	D3DXHANDLE m_h_light_dir;
	D3DXHANDLE m_h_light_color;

	//! 点光源(point_light = pl)
	D3DXHANDLE m_h_pl_pos;
	D3DXHANDLE m_h_pl_specular;
	D3DXHANDLE m_h_pl_diffuse;

	//! 減衰
	D3DXHANDLE m_h_attenuate;
};
