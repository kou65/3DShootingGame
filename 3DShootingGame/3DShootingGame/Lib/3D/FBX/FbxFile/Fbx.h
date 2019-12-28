#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>
#include<map>
#include"../../AnimationCustomVertex/AnimationCustomVertex.h"
#include"../../../Graphics/Graphics.h"
#include"../../../Texture/TextureData2D/TextureData2D.h"

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

	// とりあえず8ビット
	char name[64];

	// 姿勢オフセット行列
	D3DXMATRIX offset;

	// 変換行列
	D3DXMATRIX transform;
};


struct MotionData {


	// フレーム数
	UINT frame_num;

	// キーフレーム
	std::vector<D3DXMATRIX>key_list[256];
};


struct FbxMeshData {

	FbxMeshData() {
		fvf = FVF_FBX;
		prim_type = D3DPT_TRIANGLELIST;
		prim_num = 0;
		vertex_num = 0;
		vertex_stride = 0;
		index_num = 0;
		material_index = 0;
	}

	
	// インデックスバッファ
	IDirect3DIndexBuffer9 * index_buffer;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * vertex_buffer;

	// マテリアル配列
	MaterialInfo material_info;

	UINT				fvf;			// フォーマット
	D3DPRIMITIVETYPE	prim_type;		// プリミティブの描画方法
	UINT				prim_num;		// プリミティブ数
	UINT				vertex_num;		// 頂点数
	UINT				vertex_stride;	// 1頂点辺りのサイズ
	UINT				index_num;		// インデックス数
	UINT				material_index;	// マテリアル番号
};


class Fbx {
public:

	enum NodeType {
		NONE,
		MESH,
		TOTAL,
	};

	static const int MOTION_MAX = 256;
	static const int BONE_MAX = 256;

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

	// FBX関連削除
	void Release();

private:
	// ノードの種類を調べる
	NodeType SerchNodeType(FbxNode*fbx_node);


private:

	// メッシュ //

	// メッシュ読み込み
	void LoadMesh();

	// インデックス読み込み
	void LoadIndeces(
		std::vector<FbxMeshData>&mp_vertex_data_list,
		FbxMesh*p_mesh
	);

	// 頂点読み込み
	void LoadVertexInfo(
		std::vector<D3DXVECTOR3>&vertex_list,
		FbxMesh*p_mesh,
		std::vector<FbxMeshData>&mesh_data_list
	);

	// UV読み込み
	void LoadUvInfo(
		std::vector<D3DXVECTOR2>&uv_list,
		std::vector<FbxMeshData>&p_vertex_data_list,
		FbxMesh*p_mesh);

	// 法線読み込み
	void LoadNormal(
		std::vector<D3DXVECTOR3>&normal_list,
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
	void SetAnimation(int anim_num);

	// 読み込みアニメーションフレーム姿勢
	void LoadAnimFrameAttitudeMatrix(
		D3DXMATRIX*p_out_mat,
		FbxMesh*p_mesh,
		float frame
	);


	void LoadBone(
		FbxMeshData*p_fbx_mesh_data,
		FbxMesh*p_mesh
	);


	void LoadModelInfo(
		FbxMeshData*p_fbx_mesh_data,
		FbxMesh*p_mesh
	);

	void LoadLocalMatrix(
		FbxMesh*p_mesh
	);

	void LoadKeyFrame(
		std::string name,
		int bone,
		FbxNode*p_bone_node);

	// モーション読み込み
	void LoadMotion(
		FbxScene*p_scene
	);

	// ボーン検索
	int FindBone(const char*p_name);

	// 姿勢すきにんぐ
	void AttitudeSkinning();

	void RootNodeSample();

private:


	// 相対パス(CP932) → 絶対パス(UTF-8)
	std::string GetUTF8Path(const std::string& path);

	// カレントパスの読み込み
	void LoadCurrentPath(const std::string &path_name);

	// ポリゴンを3つに分割する
	void Polygon3Convert();

	// fbx行列をDirectXの行列に変換
	void FbxMatConvertD3DMat(
		D3DXMATRIX*p_out_mat,
		FbxMatrix&fbx_mat
	);

	// インデックスバッファ生成
	bool IndexBufferCreate(
		int total_face_num,
		std::vector<INT>indices,
		LPDIRECT3DINDEXBUFFER9 * buffer
	);

	// 頂点バッファ生成
	bool VertexBufferCreate(
		int total_vertex,
		LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
	);

	// ワールド座標変換
	D3DXMATRIX WorldTransform(
		D3DXVECTOR3 &pos,
		D3DXVECTOR3 &rot,
		D3DXVECTOR3 &scale);


	FbxAMatrix GetGeometry(FbxNode* pNode);

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

	// 頂点配列
	std::vector<AnimationCustomVertex*>m_p_vertics;


	/* アニメーション関連 */
	
	// モーション名
	std::string m_motion_name;

	// フレーム
	float m_frame;

	// 最初のフレーム
	int m_start_frame;

	// 停止フレーム
	int m_stop_frame;

	// モーション配列
	std::map<std::string, MotionData> m_motion;

	// ボーン数
	int m_bone_num;

	// ワールド座標行列
	std::vector<std::vector<D3DXVECTOR3>>m_world_trans_list;

	// ボーンデータ配列
	BoneData m_bone_data_list[BONE_MAX];

	// 二つ目のやり方
	// ボーンデータ配列2
	BoneData m_bone_data_list2[BONE_MAX];

	// 重み
	std::vector<float>m_weight_list;

	// 重みを与えるインデックス
	std::vector<int>m_weight_index_list;

	// ワールド行列配列
	std::vector<D3DXMATRIX>m_mat_trans_list;

	// クラスター数
	int m_cluster_num;

	// ずらし行列リスト
	std::vector<FbxMatrix>m_glo_bone_mat_list;
};