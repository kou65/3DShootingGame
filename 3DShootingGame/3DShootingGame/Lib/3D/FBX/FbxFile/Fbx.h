#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>
#include<map>
#include"../../AnimationCustomVertex/AnimationCustomVertex.h"
#include"../../../Graphics/Graphics.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"
#include"../../Model/Model.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


#define FVF_FBX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


struct MaterialInfo {
	D3DMATERIAL9 material;
	std::string texture_name;
};


struct FbxModuleModel {

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	// シーンの作成
	FbxScene * mp_fbx_scene;

	// インポーター
	FbxImporter*mp_importer;

};


struct BoneData {

	std::string bone_name;

	FbxMatrix bone_matrix;
};


struct WeightData {

	WeightData() {
		weight = 0.f;
		weight_index = 0;
	}

	// 重み
	float weight;

	// 重みを与えるインデックス
	int weight_index;

	// クラスターで受け取った行列
	D3DXMATRIX base_mat;
};


struct MotionData {

	// フレーム数
	UINT frame_num;

	// キーフレーム
	std::vector<FbxMatrix>animation_matrix;
};


struct FbxMeshData {

	FbxMeshData() {
		
		polygon_num = 0;
		vertex_num = 0;
		index_num = 0;
		p_index_buffer = nullptr;
		p_vertex_buffer = nullptr;
	}
	
	// インデックスバッファ
	IDirect3DIndexBuffer9 * p_index_buffer;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * p_vertex_buffer;

	// マテリアル配列
	MaterialInfo material_info;

	UINT				 polygon_num;	 // ポリゴン数
	UINT				 vertex_num;	 // 頂点数
	UINT				 index_num;		 // インデックス数
	std::string			 mesh_node_name; // メッシュ名
	std::vector<FbxMatrix>bone_list;      // ボーン配列
	std::vector<FbxMatrix>motion_list;   // モーション配列

	// 影響数を保持
	std::vector <std::vector<double>>m_weight_list;
	// 影響インデックス
	std::vector<std::vector<int>>m_weight_index_list;

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
		m_fbx_mod.mp_manager->Destroy();
	}
	
	// 読み込み
	bool Load(const std::string &fbx_file_path);

	// 描画(デフォルトで画像を入れれるようにする)
	void Draw(TextureData*td = nullptr);

	// アニメーション更新
	void Animate(
		float sec = 1.0f / 60.0f,
		float reset_frame = 0.f
	);

	// モーション情報をセットする
	void SetMotion(std::string name = "default");

	void LoadMotion(
		std::string name,
		const char* pFilename);

	// FBX関連削除
	void Release();

private:
	// ノードの種類を調べる
	NodeType SerchNodeType(FbxNode*fbx_node);

	// ノード探査
	void SerchNodeAttributes(FbxNode*p_parent_node);

private:

	// メッシュ //

	// メッシュ読み込み
	void LoadMesh();
	void LoadSkeleton();

	// インデックス読み込み
	void LoadIndeces(
		std::vector<FbxMeshData>&mp_vertex_data_list,
		FbxMesh*p_mesh
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

	// アニメーションを選択
	void SelectAnimation(
		int select_anim_num
	);

	// アニメーションをセット
	void LoadDefaultMotion(int anim_num);

	// 読み込みアニメーションフレーム姿勢
	void LoadAnimFrameAttitudeMatrix(
		D3DXMATRIX*p_out_mat,
		FbxMesh*p_current_mesh,
		FbxCluster*p_current_cluster,
		float frame
	);

	void LoadKeyFrame(
		std::string name,
		int bone,
		FbxNode*p_bone_node);

	void SetAnimation();

	// 姿勢すきにんぐ
	void AttitudeSkinning();

	void LoadWeight(
		FbxCluster*current_cluster,
		FbxMesh*p_mesh,
		std::vector<WeightData>&weight_data_list
	);

	void AnimationSkinning();

	void LoadModelInfo(
		std::vector<FbxMeshData>& p_vertex_data_list,
		FbxMesh* p_mesh
	);

	void LoadAnimationMatrix(
		FbxMesh* mesh,
		std::vector<FbxMatrix>& anim_mat_list
	);

	void Skinning();
	
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

	FbxAMatrix GetGeometry(FbxNode* pNode);

	
	void FbxMatLConvert(
		FbxMatrix&out_fbx_mat
	);

	void FbxMatLConvert(
		FbxAMatrix&out_fbx_mat
	);

private:

	// fbxsdk部品
	FbxModuleModel m_fbx_mod;
	
	// メッシュの数
	UINT m_mesh_num;

	// カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	// カレントパス
	std::string m_current_path;

	// グラフィックス
	Graphics * mp_graphics;

	// 変更用頂点配列
	std::vector<AnimationCustomVertex*>m_p_vertics;

	/* アニメーション関連 */
	
	// モーション名
	std::string m_motion_name;

	// フレーム
	double m_current_frame;

	// 最初のフレーム
	int m_start_frame;

	// 停止フレーム
	int m_stop_frame;

	// モーション配列
	std::map<std::string, MotionData> m_motion;

	// ボーン数
	int m_bone_num;

	FbxTime FrameTime, timeCount, start, stop;

};