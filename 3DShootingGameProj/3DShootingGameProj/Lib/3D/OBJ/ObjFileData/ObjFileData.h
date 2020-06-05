
#ifndef OBJ_FILE_DATA_H
#define OBJ_FILE_DATA_H

#include"../../../Graphics/Graphics.h"
#include"../../../Vec3/Vec3.h"
#include"../../../Shader/ShaderFunc/LightData/LightData.h"
#include"../../../Shader/ShaderFunc/ShadowData/ShadowData.h"
#include"../../Model/Model.h"
#include<vector>
#include<unordered_map>



/**
* @file オブジェファイルのデータ集
*/


/**
* @enum 描画状態まとめ列挙体
*/
enum class DrawStatus {
	NORMAL,
	LIGHT,
	SHADOW,
};


/**
* @brief オブジェクトパラメータ
*/
struct ObjParameter {


	/**
	* @brief コンストラクタ
	* @param state 描画状態
	* @param is_z_tex_write zテクスチャ書き込み
	*/
	ObjParameter(
		const DrawStatus &state = DrawStatus::NORMAL
	) {

		pos.x = pos.y = pos.z = 0.f;
		rota = pos;
		scale.x = scale.y = scale.z = 1.f;

		// 描画状態
		draw_status = state;

		texture_name = "";
	
		// カラー代入
		color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	}

	//! 位置
	Vec3 pos;

	//! 拡縮
	Vec3 scale;

	//! 回転値
	Vec3 rota;

	//! カラー
	D3DXVECTOR4 color;

	//! 描画状態
	DrawStatus draw_status;

	//! obj登録名
	std::string register_obj_file_name;

	//! テクスチャ名
	std::string texture_name;
};


/**
* @brief Drawサブセット用構造体
*/
struct ObjectSubset {

	//! インデックス
	unsigned int material_index;

	//! 始めの面の位置
	unsigned int face_start;

	//! 総面数
	unsigned int face_count;
};


/**
* @brief オブジェファイルで保存するデータ
*/
struct ObjFileData {

	//! マテリアル数
	unsigned int material_num;

	//! Objの方のマテリアル名配列
	std::vector<std::string>m_usemtl_name_list;

	//! 描画を入れ替えるサブセット
	std::vector<ObjectSubset>m_object_sub_set_list;

	//! マテリアルデータ配列(マテリアル名,マテリアルデータ)
	std::unordered_map<std::string, MaterialInfo>m_material_data_list;

	//! バーテックスバッファ
	IDirect3DVertexBuffer9 * mp_vertex_buffer;

	//! インデックスバッファ
	IDirect3DIndexBuffer9 * mp_index_buffer;
};



#endif