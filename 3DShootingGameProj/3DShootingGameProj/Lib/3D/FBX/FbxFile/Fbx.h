#ifndef FBX_H
#define FBX_H


#include"../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"
#include"../../Model/Model.h"
#include"../../../Shader/ShaderFunc/VertexBlendShader/VertexBlendEffectFile.h"
#include"../../../Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"
#include"../FbxData/FbxData.h"
#include"../FbxMotion/FbxSampleMotion/FbxSampleMotion.h"
#include"../FbxMotion/FbxMotion/FbxMotion.h"




/**
* @brief fbxモデルを形成するクラス
*/
class Fbx : public Model{
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

		// マネージャの破壊
		m_fbx_module.mp_manager->Destroy();
	}
	

	/**
	* @brief 読み込み 
	* @param[in] fbx_file_path fbxのファイルパス
	* @return bool
	*/
	bool Load(const std::string &fbx_file_path);


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw(TextureData*td = nullptr);


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
	D3DXMATRIX GetFbxLocalD3DMatrix(int mesh_index);


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


	/**
	* @brief エフェクト描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	* @param[in] world_mat ワールド行列
	* @param[in] bone_mat_list ボーン行列
	* @param[in] max_bone_index 最大ボーンインデックス数
	*/
	void EffectDraw(
		const int&vertex_num,
		const int&polygon_num,
		const D3DXMATRIX&world_mat,
		D3DXMATRIX*bone_mat_list,
		const int&max_bone_index
	);

private:


	/**
	* @brief メッシュ読み込み
	*/
	void LoadMesh();


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
		MaterialInfo*p_material_info
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
	void FbxPolygon3Convert();


	/**
	* @brief モーション読み込み
	*/
	void LoadFbxMotion();

private:

	//! fbxsdk部品
	FbxModuleModel m_fbx_module;

	//! カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	//! メッシュローカル座標変換リスト
	std::vector<D3DXMATRIX>m_local_pos_list;

	//! ルートパス
	std::string m_root_path;

	//! 変更用頂点配列
	std::vector<SkinCustomVertex*>mp_vertics;

	//! シェーダー
	VertexBlendEffectFile m_effect;
	
	/* アニメーション関連 */

	//! 切り替え用
	bool m_is_shader;
	bool m_is_skinning;
	
	//! fbxモーション
	FbxMotion*mp_fbx_motion;
};

#endif