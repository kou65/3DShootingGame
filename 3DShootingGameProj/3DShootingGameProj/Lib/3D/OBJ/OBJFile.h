#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include<iostream>
#include<vector>
#include"../Object3DCustomVertex/MeshCustomVertex.h"
#include<unordered_map>
#include"../../../Lib/Utility/Utility.h"
#include"../Model/Model.h"
#include"../../Vec3/Vec3.h"
#include"../../Shader/ShaderFunc/NormalShader/NormalShader.h"
#include"../../Shader/ShaderFunc/DirectionalLight/Light.h"
#include"../../Shader/ShaderFunc/ZTexture/ZTexture.h"
#include"../../Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"
#include"../../Shader/ShaderFunc/ZTexture/FuncZTexture/FuncZTexture.h"
#include"ObjFileData/ObjFileData.h"




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
	* @brief objを描画
	* @param[in] state どの状態で描画するか
	* @param[in] param オブジェクトパラメータ
	*/
	void Draw(
		const DrawStatus&state,
		const ObjParameter&param
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
		const ObjParameter&param
	);


	/**
	* @brief 保存されてたパラメータ描画開始
	*/
	void DrawSavedObj();


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

private:


	/**
	* @brief ライトや影などのデータを代入
	*/
	void SetGrapicData(
		const LightData&data1,
		const ShadowData&data2
	);


	/**
	* @brief zテクスチャ書き込み
	* @param[in] param objパラメータ構造体
	* @param[in] ZTexture* zテクスチャポインタ
	*/
	void WriteZTexture(
		const ObjParameter&param,
		ZTexture*p_tex
	);


	/**
	* @brief zテクスチャを名前付きでパラメータを書き込み
	* @param[in] z_tex_name ZTexで保存した名前を入れる
	*/
	void ParamWriteZTextureByName(
		const std::string &z_tex_name
	);


	/**
	* @brief パラメータ
	*/
	void ResetParamList();


	/**
	* @brief 現在のテクスチャをチェックする
	* @param[in] チェックするテクスチャ
	*/
	bool CheckTexture(const std::string &tex);


	/**
	* @brief シェーダーパラメータ描画
	* @param[in] param objパラメータ構造体
	* @param[in] shader シェーダーポインタ
	*/
	void ShaderParameterDraw(
		const ObjParameter &param,
		ShaderBase*p_shader
	);


	/**
	* @brief beginなしシェーダーパラメータ描画
	*/
	void DrawShader(
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
		StandardTSShader*shader
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

	//! オブジェクトパラメータリストを登録(後で一気に描画する、毎回削除)
	std::vector<ObjParameter>m_param_list;

	//! 通常シェーダー
	NormalShader m_ns;

	//! ライト
	LightShader m_light_shader;

	//! 影
	DepthShadowShader m_shadow;

	//! ライトパスタイプ
	ShaderType m_pass_type;

	//! 影データ
	ShadowData m_shadow_data;

	//! ライトデータ
	LightData m_light_data;

	//! 唯一ひとつのライトを使うかどうか
	bool m_is_world_light;

};

#endif