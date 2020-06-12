#pragma once
#include"../ObjFileData/ObjFileData.h"
#include"../../../Shader/ShaderFunc/ShaderBase/EffectFileBase.h"



/**
* @brief オブジェ描画用クラス
*/
class ObjRenderer {
public:



	/**
	* @brief objを描画
	* @param[in] state どの状態で描画するか
	* @param[in] param オブジェクトパラメータ
	*/
	void Draw(
		const DrawStatus&state,
		const ObjParameter&param
	);


	/**
	* @brief シャドウカメラ更新
	*/
	void UpdateCameraDataInShadow();


	/**
	* @brief グラフィックデータの初期化
	*/
	void InitGrapicData();


	/**
	* @brief 影データセット
	*/
	void SetShadowData(
		ShadowData&data
	);


	/**
	* @brief ライトデータ
	*/
	void SetLightData(
		LightData&data
	);


	/**
	* @brief GPUシェーダー描画
	* @param[in] param objパラメータ構造体
	*/
	void DrawObjByNormalShader(
		const ObjParameter &param
	);


	/**
	* @brief 影とライト付き描画
	* @param[in] param objパラメータ構造体
	*/
	void DrawShadowObj(
		const ObjParameter &param
	);


	/**
	* @brief 固定機能描画
	* @param[in] param objパラメータ構造体
	*/
	void DrawNormal(
		const ObjParameter&param
	);


	/**
	* @brief ライト付きで描画
	* @param[in] param objパラメータ構造体
	*/
	void DrawLightObj(
		const ObjParameter &param
	);


	/**
	* @brief 描画用パラメータを追加
	*/
	void EntryObjParam(
		const std::string&param_name,
		const ObjParameter&param
	);


	/**
	* @brief 保存されてたパラメータ描画開始
	*/
	void DrawShadowParamList();


	/**
	* @brief シェーダーをパスごとに描画する
	*/
	void DrawBeginPassShader(
		ShaderBase*p_shader,
		ObjFileData*p_data,
		const ObjParameter&param,
		const UINT&current_pass,
		int sub_num
	);


	/**
	* @brief メッシュごとに描画する描画サブセット内部処理
	*/
	void DrawSubSet(
		const ObjParameter&param,
		const DrawStatus&state,
		int sub_set_number
	);


	/**
	* @brief メッシュごとに描画するサブセット外部シェーダー処理
	*/
	void DrawSubSet(
		const ObjParameter&param,
		ShaderBase*p_shader,
		int sub_set_number
	);


	/**
	* @brief 1024サイズのzテクスチャを書き込む
	*/
	void WriteZTexture(
		const ObjParameter&param,
		const std::string &register_name
	);

};