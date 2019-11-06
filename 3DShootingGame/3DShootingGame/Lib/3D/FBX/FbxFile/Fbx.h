#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>
#include"../../../../FbxModel.h"
#include"../../../Graphics/Graphics.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


#define FVF_FBX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)


struct FbxCustomVertex {

	FbxCustomVertex(){
		vertex.x = vertex.y = vertex.z = 0.f;
		normal.x = normal.y = normal.z = 0.f;
		uv.x = uv.y = 0.f;
	}

	// 頂点データ
	D3DXVECTOR3 vertex;

	// 法線ベクトル
	D3DXVECTOR3 normal;

	// テクセル
	D3DXVECTOR2 uv;
};


struct MaterialInfo {
	D3DXMATERIAL material;
	std::string texture_name;
};


struct FbxMeshData {

	FbxMeshData() {
		polygon_num = 0;
		start_index = 0;
	}

	// ポリゴンの数
	int polygon_num;

	// インデックスバッファの数
	int start_index;

	// インデックスバッファ
	IDirect3DIndexBuffer9 * index_buffer;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * vertex_buffer;

	// マテリアル配列
	MaterialInfo material_info;
};


class Fbx {
public:

	enum NodeType {
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
		mp_manager->Destroy();
	}
	
	// 読み込み
	bool Load(const std::string &fbx_file_path);

	// 描画
	void Draw();

	// FBX関連削除
	void Release();

private:

	// ノード探査関数
	void NodeSerch(
		std::vector<D3DXVECTOR3>&vertex_list,
		std::vector<D3DXVECTOR2>&uv_list,
		std::vector<FbxMeshData>&mp_vertex_data_list,
		FbxNode *node
	);

	// 根のノードを探査する
	void RootNodeProbe();

	// ノードの種類を調べる
	NodeType SerchNodeType(FbxNode*fbx_node);

	// ポリゴンを3つに分割する
	void Polygon3Convert();

	// インデックス読み込み
	void LoadIndeces(
		int &index_num,
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


	// カスタムバーテックス作成
	void CustomVertexCreate(
		int index_num,
		std::vector<D3DXVECTOR3>&vertex_list,
		std::vector<D3DXVECTOR2>&uv_list,
		std::vector<D3DXVECTOR3>&normal_list
	);

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

	bool LoadTextureInfo(
		FbxMesh*p_mesh,
		MaterialInfo*p_material_info
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

	// ポリゴン2分割
	std::vector<INT> SplitPolygon2(
		const std::vector<INT>&indices
	);

private:

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	// シーンの作成
	FbxScene * mp_fbx_scene;

	// バーテックスバッファ
	//IDirect3DVertexBuffer9 * mp_vertex_buffer;

	// 全てのインデックス
	std::vector<INT>m_indices;

	// 現在のインデックス数
	int m_current_index_num;

	// 全ての頂点数
	std::vector<FbxCustomVertex>m_custom_vertex_list;
	
	// カスタムバーテックスの配列
	std::vector<FbxMeshData>m_mesh_data_list;

	// マテリアル数
	int m_material_num;

	// テクスチャファイルパス
	std::string m_texture_file_path;

	// fbxファイル名
	std::string m_fbx_file_name;

	// グラフィックス
	Graphics * mp_graphics;
};