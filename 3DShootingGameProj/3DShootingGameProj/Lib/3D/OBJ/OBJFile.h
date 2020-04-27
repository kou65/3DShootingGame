#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include<iostream>
#include<vector>
#include"../Object3DCustomVertex/MeshCustomVertex.h"
#include<unordered_map>
#include"../../../Utility/Utility.h"
#include"../Model/Model.h"
#include"../../Vec3/Vec3.h"
#include"../../EffectFileShader/NormalShader/NormalShader.h"
#include"../../EffectFileShader/DirectionalLight/Light.h"
#include"../../EffectFileShader/ZTexture/ZTexture.h"
#include"../../EffectFileShader/DepthShadowShader/DepthShadowEffectFile.h"


// HACK リファクタリング対象(複数のstructなど)


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


// オブジェクトパラメータ
struct ObjParameter {


	/**
	* @brief コンストラクタ
	*/
	ObjParameter() {

		pos.x = pos.y = pos.z = 0.f;
		rota = pos;
		scale.x = scale.y = scale.z = 1.f;

		// カラー代入
		color = D3DXVECTOR4(1.f,1.f,1.f,1.f);
	}

	//! 位置
	Vec3 pos;

	//! 拡縮
	Vec3 scale;

	//! 回転値
	Vec3 rota;

	//! カラー
	D3DXVECTOR4 color;

	//! ライト情報
	LightData light_data;

	//! 影データ
	ShadowData shadow_data;

	//! obj登録名
	std::string register_obj_file_name;

	//! テクスチャ名
	std::string texture_name;
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
	IDirect3DVertexBuffer9 * m_p_vertex_buffer;

	//! インデックスバッファ
	IDirect3DIndexBuffer9 * m_p_index_buffer;
};



/**
* @brief objファイルシングルトンクラス
*/
class Obj : public  Model{
public:

	
	/**
	* @brief シングルトン
	* @return static Obj* オブジェポインタ
	*/
	static Obj *GetInstance(){
		static Obj obj_file;
		return &obj_file;
	}

public:


	/**
	* @brief 解放処理
	*/
	void Release() {};

	// 読み込み
	/**
	* @brief 読み込み
	* @param[in] obj_file_path 読み込むobjファイル名
	* @param[in] registr_name 登録obj名
	* @param[in] mtl_file_path objマテリアルファイル名
	* @return bool 読み込めたかどうかを返す
	*/
	bool Load(
		const std::string &obj_file_path,
		const std::string &registr_name,
		const std::string &mtl_file_path = ""
	);

	
	/**
	* @brief GPUシェーダー描画
	* @param[in] param objパラメータ構造体
	*/
	void ShaderDraw(
		const ObjParameter &param
	);


	/**
	* @brief zテクスチャ描画
	* @param[in] param objパラメータ構造体
	*/
	void ZTextureDraw(
		const ObjParameter&param
	);


	/**
	* @brief 影とライト付き描画
	* @param[in] param objパラメータ構造体
	*/
	void ShadowDraw(
		const ObjParameter &param
	);


	/**
	* @brief 固定機能描画
	* @param[in] param objパラメータ構造体
	*/
	void NormalDraw(
		const ObjParameter&param
	);


	/**
	* @brief ライト付きで描画
	* @param[in] param objパラメータ構造体
	*/
	void LightDraw(
		const ObjParameter &param
	);

private:


	/**
	* @brief シェーダーパラメータ描画
	* @param[in] param objパラメータ構造体
	* @param[in] shader シェーダーポインタ
	*/
	void ShaderParameterDraw(
		const ObjParameter &param,
		ShaderBase*shader
	);


	/**
	* @brief ライト用の更新
	* @param[in] param objパラメータ構造体
	*/
	void UpdateLight(const ObjParameter&param);

	
	/**
	* @brief 通常更新
	* @param[in] param objパラメータ構造体
	* @param[in] shader 変換用シェーダーポインタ 
	*/
	void Update(
		const ObjParameter&param,
		StandardTransformShader*shader
	);


	/**
	* @brief テクスチャの読み込み
	* @param[in] mtl_texture_name マテリアルから読み込んだテクスチャ名
	* @param[in] obj_param_file_name 外部から入ってきたテクスチャ名
	* @return bool 読み込めたかどうか
	*/
	bool LoadTexture(
		std::string&mtl_texture_name,
		std::string&obj_param_file_name
	);


	/**
	* @brief 使うシェーダーパスを選択
	* @param[in] type シェーダーの種類
	*/
	UINT GetUsePass(const ShaderType&type);

private:


	/**
	* @brief コンストラクタ
	*/
	Obj();


	/**
	* @brief メッシュ情報読み込み
	* @param[in] file_path objファイル名
	* @param[in] registr_name obj登録名
	* @param[out] out_material_name マテリアル名を外部に参照
	* @param[out] out_total_material_num 外部にマテリアル数を参照
	* @param[out] indices インデックスリスト
	* @param[out] mesh_list メッシュカスタムバーテックスリスト
	*/
	bool MeshLoad(
		const std::string &file_path,
		const std::string &registr_name,
		std::string&out_material_name,
		int &out_total_material_num,
		std::vector<INT>&indices,
		std::vector<MeshCustomVertex>&mesh_list
	);


	/**
	* @brief 面情報からカスタムバーテックス作成
	* @param[out] p_file ファイルポインタ
	* @param[in] register_name 外部からきた登録名
	* @param[out] indices インデックスリスト
	* @param[out] mesh_list メッシュリスト
	* @param[out] vertex_list 頂点リスト
	* @param[out] uv_list uvリスト
	* @param[out] normal_list 法線リスト
	*/
	void FaceInfoLoad(
		FILE*p_file,
		const std::string &register_name,
		std::vector<INT>&indices,
		std::vector<MeshCustomVertex>&mesh_list,
		const std::vector<D3DXVECTOR3>&vertex_list,
		const std::vector<D3DXVECTOR2>&uv_list,
		const std::vector<D3DXVECTOR3>&normal_list
		);


	/**
	* @brief 頂点情報読み込み
	* @param[out] p_file ファイルポインタ
	* @param[in] front_str 最初の文字列
	* @param[out] out_vertex_list 頂点リスト
	* @param[out] out_uv_list uvリスト
	* @param[out] out_normal_list 法線リスト
	*/
	void VertexInfoLoad(
		FILE *p_file,
		const char*front_str,
		std::vector<D3DXVECTOR3>&out_vertex_list,
		std::vector<D3DXVECTOR2>&out_uv_list,
		std::vector<D3DXVECTOR3>&out_normal_list
	);


	/**
	* @brief マテリアルファイル読み込み
	* @param[in] mtl_file_name マテリアルファイル名
	* @param[in] obj_file_name objファイル名
	* @param[in] texture_file_path テクスチャファイルパス名
	*/
	bool MaterialFileLoad(
		const std::string &mtl_file_name,
		const std::string &obj_file_name,
		const std::string &texture_file_path
	);


	// usemtl読み込み
	/**
	* @brief usemtl読み込み
	* @param[out] p_file ファイルポインタ
	* @param[in] obj_file_name objファイル名
	* @param[in] out_total_material_num 総マテリアル数
	* @param[in] indices インデックスリスト
	* @param[in] front_str 前方文字列
	* @param[in] load_buffer 読み込みバッファ
	*/
	void UseMaterialInfoLoad(
		FILE*p_file,
		const std::string &obj_file_name,
		int &out_total_material_num,
		std::vector<INT>&indices,
		char*front_str,
		int load_buffer
	);


	/**
	* @brief 面情報代入
	* @param[in] face_info_str 面情報文字列
	* @param[out] custom_vertex カスタムバーテックス
	* @param[out] m_indices インデックスリスト 
	* @param[in] vertex_list 頂点リスト
	* @param[in] uv_list uvリスト
	* @param[in] normal_list 法線リスト
	* @param[out] out_draw_sub_set サブセット構造体
	*/
	void InsertFaceList(
		const std::vector<std::vector<std::string>>&face_info_str,
		std::vector<MeshCustomVertex>&custom_vertex,
		std::vector<INT>&m_indices,
		const std::vector<D3DXVECTOR3>&vertex_list,
		const std::vector<D3DXVECTOR2>&uv_list,
		const std::vector<D3DXVECTOR3>&normal_list,
		std::vector<ObjectSubset>&out_draw_sub_set
	);


	/**
	* @brief 整数値変換
	* @param[out] axis 軸数
	* @param[in] string 文字列を元に頂点算出
	*/
	bool Vector3ConversionByString(
		int &axis,
		const std::string &string
	);


	/**
	* @brief 頂点バッファ生成
	* @param[in] vertex_num 頂点数
	* @param[in] obj_file_name objファイル名
	* @param[in] mesh_vertex_list メッシュ頂点リスト
	*/
	void VertexBufferCreate(
		const int &vertex_num,
		const std::string &obj_file_name,
		const std::vector<MeshCustomVertex>&mesh_vertex_list
	);


	/**
	* @brief 面情報から頂点情報埋め込み
	* @param[in] face_num 面数
	* @param[in] obj_file_name objファイル名
	* @param[in] indices インデックスリスト
	*/
	bool IndexBufferCreateFaceBase(
		const int &face_num,
		const std::string &obj_file_name,
		const std::vector<INT>&indices
	);


	/**
	* @brief 4面を3面にする
	* @param[in] vertex4_polygon_list 頂点4リスト
	* @return std::vector<INT> 分割済みのリスト
	*/
	std::vector<INT> Vertex4CutToVertex3Face(
		const std::vector<INT>&vertex4_polygon_list
	);


	/**
	* @brief 頂点ファイル読み込み
	* @param[in] p_file ファイルポインタ
	* @param[in] vec3 ファイルから読み込んだ頂点数
	*/
	void VertexFscanfLoad(
		FILE*p_file,
		D3DXVECTOR3 &vec3
	);


	/**
	* @brief 変換行列を返す
	* @param[in] param objパラメータ
	* @return D3DXMATRIX 変換後の行列
	*/
	D3DXMATRIX GetTransformMatrix(const ObjParameter&param);

private:

	//! オブジェファイルリスト
	std::unordered_map<std::string, ObjFileData*>m_obj_file_data;

	//! 通常シェーダー
	NormalShader m_ns;

	//! ライト
	Light m_light_shader;

	//! z値計算用
	ZTexture m_z_tex;

	//! 影
	DepthShadowShader m_shadow_shader;

	//! ライトパスタイプ
	ShaderType m_pass_type;
};

#endif