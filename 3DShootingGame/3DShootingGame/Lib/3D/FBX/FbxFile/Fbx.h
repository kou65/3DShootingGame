#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>
#include<map>
#include"../../AnimationCustomVertex/AnimationCustomVertex.h"
#include"../../../Graphics/Graphics.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


#define FVF_FBX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)


struct MaterialInfo {
	D3DMATERIAL9 material;
	std::string texture_name;
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
		//fvf = FVF_FBX;
		//primType = D3DPT_TRIANGLELIST;
		//primNum = 0;
		//vertexNum = 0;
		//vertexStride = 0;
		//indexNum = 0;
		//materialIndex = 0;
	}

	
	// インデックスバッファ
	IDirect3DIndexBuffer9 * index_buffer;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * vertex_buffer;

	// マテリアル配列
	MaterialInfo material_info;

	UINT				fvf;			// フォーマット
	D3DPRIMITIVETYPE	primType;		// プリミティブの描画方法
	UINT				primNum;		// プリミティブ数
	UINT				vertexNum;		// 頂点数
	UINT				vertexStride;	// 1頂点辺りのサイズ
	UINT				indexNum;		// インデックス数
	UINT				materialIndex;	// マテリアル番号
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
		mp_manager->Destroy();
	}
	
	// 読み込み
	bool Load(const std::string &fbx_file_path);

	// 描画
	void Draw();

	// アニメーション更新
	void Animate(float sec = 1.0f / 60.0f);

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
		FbxNode*p_node,
		FbxMesh*p_mesh
	);

	// ランバートからマテリアル情報を取得
	void SetLambertInfo(
		FbxSurfaceLambert*lambert,
		D3DXMATERIAL*material_info
	);

	// phongからマテリアル情報を取得する
	void SetPhongInfo(
		FbxSurfacePhong*p_phong,
		D3DXMATERIAL*p_material_info
	);

	bool LoadTexture(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
	);

private:

	void LoadAnimationFrame(
		FbxTime*m_frame_time,
		FbxMesh*p_mesh,
		FbxScene*p_scene
	);

	void LoadBone(
		FbxMeshData*p_fbx_mesh_data,
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

	// すきにんぐ
	void Skinning();

private:

	// ポリゴン2分割
	std::vector<INT> SplitPolygon2(
		const std::vector<INT>&indices
	);

	// ルートパスを作成する
	void SetRootPath(const char*p_file_name);

	// 相対パス(CP932) → 絶対パス(UTF-8)
	std::string GetUTF8Path(const std::string& path);

	// ポリゴンを3つに分割する
	void Polygon3Convert();


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

private:

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	// シーンの作成
	FbxScene * mp_fbx_scene;

	// メッシュの数
	UINT m_mesh_num;

	// カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	// ルートパス
	char m_root_path[MAX_PATH];

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

	// モーション配列
	std::map<std::string, MotionData> m_motion;

	// ボーン数
	int m_bone_num;

	// ボーンデータ配列
	BoneData m_bone_list[BONE_MAX];
};