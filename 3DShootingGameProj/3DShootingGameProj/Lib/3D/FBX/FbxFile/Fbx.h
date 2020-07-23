#ifndef FBX_H
#define FBX_H


#include"../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"
#include"../../Model/Model.h"
#include"../../../Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"
#include"../FbxMeshData/FbxMeshData.h"
#include"../FbxMotion/FbxMotion.h"
#include<map>



// LP型作成
typedef class FbxScene *LP_FBX_SCENE;
typedef class FbxManager*LP_FBX_MANAGER;
typedef class FbxImporter*LP_FBX_IMPORTER;


/**
* @brief fbxモデルを形成するクラス
*/
class Fbx : public Model{
public:

	// エントリーで探すテクスチャエントリー
	const std::string DEFFUSE_TEX_NAME = "Maya|DiffuseTexture";
	const std::string NORMAL_TEX_NAME = "Maya|NormalTexture";
	const std::string SEPCULAR_TEX_NAME = "Maya|SpecularTexture";
	const std::string FALL_OFF_TEX_NAME = "Maya|SpecularTexture";
	const std::string REFLECTION_MAP_TEX_NAME = "Maya|ReflectionMapTexture";

public:


	/**
	* @brief インスタンスを返す
	*/
	static Fbx *GetInstance() {
		static Fbx fbx;
		return &fbx;
	}

public:


	/**
	* @brief コンストラクタ
	*/
	Fbx();


	/**
	* @brief デストラクタ
	*/
	~Fbx() {

	}
	

	/**
	* @brief 読み込み 
	* @param[in] fbx_file_path fbxのファイルパス
	* @return bool
	*/
	bool Load(
		const std::string &fbx_file_path,
		const std::string&register_name
	);


	/**
	* @brief 更新
	*/
	void Update(
		const std::string&mesh_name,
		const std::string&motion_name
	);


	/**
	* @brief 描画
	*/
	void Draw(
		const std::string&mesh_name,
		TextureData*td = nullptr,
		bool is_local_mat_on = true
	);


	/**
	* @brief モーション読み込み
	* @param[in] file_path ファイルパス
	* @param[in] mesh_name メッシュ名
	* @param[in] new_motion_name モーション名
	* @param[in] select_motion_num モーション読み込み番号
	*/
	void LoadMotion(
		const std::string&file_path,
		const std::string&mesh_name,
		const std::string&new_motion_name,
		const int&select_motion_num
	);


	/**
	* @brief ファイルから読み込む
	*/
	void LoadFileMotion(
		const std::string&file_path,
		const std::string&mesh_name,
		const std::string&new_motion_name,
		const int&select_motion_num
	);


	/**
	* @brief モーション
	*/
	void InitMotion(
		const std::string&mesh_name,
		const std::string&motion_name
	);


	/**
	* @brief モーションをアニメーションして入れ替え
	*/
	void ChangeMotion(
		const std::string&mesh_name,
		const std::string&motion_name
	);


	/**
	* @brief モーション終了を返す
	*/
	bool GetEndMotion(
		const std::string&mesh_name,
		const std::string&motion_name
	);


	/**
	* @brief 解放
	*/
	void Release();

private:


	/**
	* @brief ワールド変換行列を返す
	* @param[in] mesh_index
	* @return D3DXMATRIX 行列を返す
	*/
	D3DXMATRIX GetFbxLocalD3DMatrix(
		FbxScene*p_scene,
		FbxMesh*p_mesh
	);


	/**
	* @brief fbx行列をDirectXの行列に変換
	*/
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix&fbx_mat
	);


	/**
	* @brief 通常描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	* @param[in] world_mat ワールド行列
	*/
	void NormalDraw(
		const int&vertex_num,
		const int&polygon_num,
		D3DXMATRIX&world_mat
	);


private:


	/**
	* @brief メッシュ読み込み
	*/
	bool LoadMesh(
		FbxScene**p_scene,
		FbxManager**p_manager,
		FbxImporter**p_importer,
		const std::string&mesh_name
	);


	/**
	* @brief インデックス読み込み
	* @param[in] p_vertex_data_list
	* @p_mesh[in] p_mesh
	*/
	void LoadIndeces(
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh
	);

	/**
	* @brief 頂点情報初期化
	* @param[in] p_mesh メッシュ
	* @param[in] mesh_data メッシュデータ
	* @param[in] size サイズ
	*/
	void InitVertexInfo(
		FbxMesh*p_mesh,
		FbxMeshData*mesh_data,
		const UINT &size
	);


	/**
	* @brief 頂点読み込み
	* @param[out] p_mesh メッシュデータ
	* @param[in] mesh_data_list メッシュデータ配列
	*/
	void LoadVertexInfo(
		FbxMesh*p_mesh,
		std::vector<FbxMeshData>&mesh_data_list
	);


	/**
	* @brief uv読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh Fbxメッシュ 
	*/
	void LoadUvInfo(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief 法線読み込み
	* @param[in] p_mesh_data_list メッシュデータ配列
	* @param[in] p_mesh fbxメッシュ
	*/
	void LoadNormal(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);

private:


	/**
	* @brief マテリアル読み込み
	* @param[in] p_mesh_data_list fbxメッシュデータ
	* @param[out] p_mesh メッシュ
	*/
	void LoadMaterial(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief テクスチャ読み込み
	* @param[in] p_mesh fbxメッシュ
	* @param[in] p_material_info マテリアル情報
	* @return bool
	*/
	bool LoadTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);


	/**
	* @brief カラー読み込み
	* @param[in] fbxメッシュデータ
	* @param[in] p_mesh メッシュ
	*/
	bool LoadColor(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);


	/**
	* @brief エントリー済みのマテリアルテクスチャを読み込む
	* @param[in] p_mesh メッシュ
	* @param[out] p_material_info マテリアル情報
	*/
	void LoadEntryTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info,
		int entry_tex_index = 0
	);

	
private:


	/**
	* @brief 相対パス(CP932) → 絶対パス(UTF-8)
	* @param[in] path UTF8パス
	*/
	std::string GetUTF8Path(const std::string& path);


	/**
	* @brief カレントパスの読み込み
	*/
	void LoadCurrentPath(const std::string &path_name);


	/**
	* @brief fbx関数でポリゴンを3つに分割する
	*/
	void FbxPolygon3Convert(
		FbxManager*p_manager,
		FbxScene*p_scene
	);


private:

	//! カスタムバーテックスの配列
	std::map<std::string,std::vector<FbxMeshData>>m_mesh_data_list;

	//! ルートパス
	std::string m_root_path;

	
	/* アニメーション関連 */

	//! 切り替え用
	bool m_is_shader;
	bool m_is_skinning;
	
	//! fbxモーション
	FbxMotion m_fbx_motion;

};

#endif