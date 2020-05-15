#ifndef FBX_H
#define FBX_H

#include<string>
#include<vector>
#include<fbxsdk.h>
#include<map>
#include"../../SkinCustomVertex/SkinCustomVertex.h"
#include"../../../Graphics/Graphics.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"
#include"../../Model/Model.h"
#include"../../../Shader/ShaderFunc/VertexBlendShader/VertexBlendEffectFile.h"
#include"../../../Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"


#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")



/**
* @brief fbxで使う部品モデル構造体
*/
struct FbxModuleModel {

	//! SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	//! シーンの作成
	FbxScene * mp_fbx_scene;

	//! インポーター
	FbxImporter*m_p_importer;

};

//! 最大ボーン数
const int MAX_BONE_MATRIX_NUM = 64;


/**
* @brief ボーン構造体
*/
struct Bone {

	// ボーン行列
	D3DXMATRIX bone_list[MAX_BONE_MATRIX_NUM];// 64
};


struct MotionData {

	// フレーム数
	UINT frame_num;

	// キーフレーム
	// フレーム数 > ボーン数
	std::vector<std::vector<FbxMatrix>>animation_matrix;

	// d3d用(GPUスキニングにする場合はこちらにする)
	std::vector<Bone>d3d_animation_mat;
};


struct FbxMeshData {

	FbxMeshData() {
		
		polygon_num = 0;
		vertex_num = 0;
		p_index_buffer = nullptr;
		p_vertex_buffer = nullptr;
	}
	
	// インデックスバッファ
	IDirect3DIndexBuffer9 * p_index_buffer;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * p_vertex_buffer;

	// マテリアル配列
	MaterialInfo material_info;

	UINT polygon_num;	 // ポリゴン数
	UINT vertex_num;	 // 頂点数
	UINT bone_num;       // ボーン数
	

	std::vector<FbxMatrix>bone_list;      // ボーン配列
	std::vector<FbxMatrix>motion_list;   // モーション配列

	// ボーン行列
	std::vector<D3DXMATRIX>d3d_bone_mat_list;
	// アニメーション行列
	std::vector<D3DXMATRIX>d3d_anim_mat_list;
	
	// 影響数を保持
	std::vector <std::vector<double>>weight_list;
	// 影響インデックス
	std::vector<std::vector<int>>weight_index_list;

};


class Fbx : public Model{
public:

	enum class NodeType {
		NONE,
		MESH,
		TOTAL,
	};

public:

	static Fbx *GetInstance() {
		static Fbx fbx;
		return &fbx;
	}

public:

	Fbx();

	~Fbx() {

		// マネージャの破壊
		m_fbx_module.mp_manager->Destroy();
	}
	
	// 読み込み
	bool Load(const std::string &fbx_file_path);

	// 更新
	void Update();

	// 描画(デフォルトで画像を入れれるようにする)
	void Draw(TextureData*td = nullptr);

	// アニメーション更新
	void Animate(
		const float &frame = 20.f
	);

	// モーション情報をセットする
	void SetMotion(std::string name = "default");

	// モーション読み込み
	void LoadFileMotion(
		std::string name,
		const char* pFilename);

	// FBX関連削除
	void Release();

private:

	D3DXMATRIX GetFbxWorldD3DMatrix(int mesh_index);

	void NormalDraw(
		const int&vertex_num,
		const int&polygon_num,
		D3DXMATRIX&world_mat
	);


	void EffectDraw(
		const int&vertex_num,
		const int&polygon_num,
		const D3DXMATRIX&world_mat,
		D3DXMATRIX*bone_mat_list,
		const int&max_bone_index
	);

private:

	// メッシュ //

	// メッシュ読み込み
	void LoadMesh();

	// インデックス読み込み
	void LoadIndeces(
		std::vector<FbxMeshData>&mp_vertex_data_list,
		FbxMesh*p_mesh
	);

	// 頂点の初期化
	void InitVertexInfo(
		FbxMesh*p_mesh,
		FbxMeshData*mesh_data,
		const UINT &size
	);

	// 頂点読み込み
	void LoadVertexInfo(
		FbxMesh*p_mesh,
		std::vector<FbxMeshData>&mesh_data_list
	);

	// UV読み込み
	void LoadUvInfo(
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh);

	// 法線読み込み
	void LoadNormal(
		std::vector<FbxMeshData>&p_mesh_data_list,
		FbxMesh*p_mesh
	);

private:

	// マテリアル関係 //

	// マテリアル読み込み
	void LoadMaterial(
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh
	);


	bool LoadTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);

	// カラー読み込み
	bool LoadColor(
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh
	);

	// エントリのマテリアルテクスチャを読み込む
	void LoadEntryTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);

private:

	// アニメーション関係

	// アニメーションを選択
	bool SelectAnimation(
		int select_anim_num
	);

	// モーションの読み込み
	void LoadMotion(
		const std::string& name,
		const int &select_motion_num
	);

	// アニメーション行列読み込み
	void LoadKeyFrame(
		FbxMesh* mesh,
		const std::string& motion_name,
		std::vector<FbxMatrix>& bone_mat_list,
		const int& bone_num,
		const FbxTime& start,
		const FbxTime& stop,
		const FbxTime& frame_count
	);

	// アニメーション関連をセットする
	void SetAnimation(
		FbxTime&start,
		FbxTime&stop,
		FbxTime&frame_time,
		const int&anim_num
	);

	// モデル情報読み込み
	void LoadModelInfo(
		std::vector<FbxMeshData>& p_vertex_data_list,
		FbxMesh* p_mesh
	);

	// 現在のアニメーションを受け取る
	FbxMatrix GetAnimationMatrix(
		FbxMesh* mesh,
		FbxTime&count,
		int deformer_num,
		int cluster_num
	);

	// CPUのスキニング
	void CPUSkinning();

	// 最新バージョン
	void WeightSkinning(
		FbxMesh* mesh,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		const int &mi
	);

	void KeySkinning();

	// キーフレームアニメーション
	void KeyFrameSkinning(
		FbxMesh* mesh,
		FbxMeshData& mesh_data,
		SkinCustomVertex* vertices,
		const int &mi
	);

	void LoadWeightVertexPoint(
		std::vector<FbxMeshData>& p_mesh_data_list,
		FbxMesh* p_mesh
	);

	
private:

	// 相対パス(CP932) → 絶対パス(UTF-8)
	std::string GetUTF8Path(const std::string& path);

	// カレントパスの読み込み
	void LoadCurrentPath(const std::string &path_name);

	// Fbx関数でポリゴンを3つに分割する
	void FbxPolygon3Convert();

	// fbx行列をDirectXの行列に変換
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix&fbx_mat
	);

	// 位置を修正する行列取得
	FbxAMatrix GetGeometry(FbxNode* pNode);
	
	// 左手系変換
	void FbxMatLConvert(
		FbxMatrix&out_fbx_mat
	);

	// 左手系変換
	void FbxMatLConvert(
		FbxAMatrix&out_fbx_mat
	);

private:

	// fbxsdk部品
	FbxModuleModel m_fbx_module;

	// カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	// ルートパス
	std::string m_root_path;

	// 変更用頂点配列
	std::vector<SkinCustomVertex*>m_p_vertics;

	// シェーダー
	VertexBlendEffectFile m_effect;
	
	/* アニメーション関連 */
	
	// モーション名
	std::string m_current_motion_name;

	// フレーム
	double m_frame;

	// 停止数
	int m_stop_count;

	// カウント
	int m_count;

	// モーション文字列
	// モーション種類 > メッシュ 
	std::map<std::string,std::vector<MotionData>> m_motion;

	// シェーダーするかどうか
	bool m_is_shader;

	// アニメーションするかどうか
	bool m_is_skinning;

	// 現在のアニメーションフレーム数
	float m_key_frame;

};

#endif