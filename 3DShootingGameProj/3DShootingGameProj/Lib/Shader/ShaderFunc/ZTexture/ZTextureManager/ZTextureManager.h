#pragma once
#include"../ZTexture.h"
#include"../../DepthShadowShader/DepthShadowEffectFile.h"
#include"../../../../Vec2/Vec2.h"
#include"../../../../Vec3/Vec3.h"
#include"../ZTextureData/ZTextureData.h"
#include"../../../../TransformData/TransformData2D.h"
#include<memory>
#include<map>



/**
* @brief Zテクスチャ管理シングルトンクラス
*/
class ZTextureManager {
public:

	/**
	* @brief アクセス権
	*/
	static ZTextureManager*GetInstance() {
		static ZTextureManager z_tex;
		return &z_tex;
	}

public:


	/**
	* @brief 初期化 右上から始点にカメラを向ける
	*/
	void Init(
		const Vec3&camera_pos = Vec3(150.f,150.f,150.f),
		const Vec3&look_pos = Vec3(0.f,0.f,0.f)
	);


	/**
	* @brief 全てのZテクスチャ更新
	*/
	void Update();


	/**
	* @brief 生成
	* @param[in] 登録名
	* @param[in] テクスチャサイズ
	*/
	void Create(
		const std::string&register_name,
		const Vec2 &tex_size
	);


	/**
	* @brief 削除
	*/
	void Destory(
		const std::string&register_name
	);


	/**
	* @brief 解放処理
	*/
	void Release();


	/**
	* @brief Zテクスチャのポインタを返す 
	*/
	ZTexture *GetZTexturePtr(
		const std::string&register_name
	);


	/**
	* @brief 全て削除
	*/
	void AllDestory();


	/**
	* @brief カメラの更新
	*/
	void UpdateCamera();


	/**
	* @brief 位置セット
	*/
	void SetPos(const Vec3&pos);


	/**
	* @brief 視点位置セット
	*/
	void SetLookPos(const Vec3&pos);


	/**
	* @brief 位置加算 
	*/
	void AddPos(const Vec3&pos);


	/**
	* @brief 視点加算
	*/
	void AddLookPos(const Vec3&pos);


	/**
	* @brief データで設定したビュー行列セット
	*/
	void SetViewMatrix(const ZTextureData&data);


	/**
	* @brief データで設定したプロジェクション行列セット
	*/
	void SetProjMatrix(const ZTextureData&data);


	/**
	* @brief データを返す
	*/
	ZTextureData GetZTexData();


	/**
	* @brief zテクスチャスプライト描画
	*/
	void SpriteDraw(
		const std::string&str,
		const TransformData2D&data
	);


private:


	/**
	* @brief コンストラクタ
	*/
	ZTextureManager();


	/**
	* @brief zテクスチャスプライト描画
	*/
	void ZTextureSpriteDraw(
		IDirect3DTexture9*p_tex,
		const TransformData2D&data =
		TransformData2D(
			Vec2(0.f,0.f),
			Vec2(1000.f,1000.f),
			Vec2(0.f,0.f)
			)
	);


private:


	//! Zテクスチャまとめ
	std::map<std::string,ZTexture*>
		m_z_tex_list;

	//! ワールドzテクスチャデータ(ユニークデータ)
	ZTextureData m_unique_z_tex_data;

	//! ライト位置
	Vec3 m_pos;

	//! ライト視点
	Vec3 m_look;

};