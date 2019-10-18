#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>
#include<memory>
#include"../../../Graphics/Graphics.h"

#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


#define FVF_FBX (D3DFVF_XYZ)


struct FbxCustomVertex {

	FbxCustomVertex(){
		vertex.x = vertex.y = vertex.z = vertex.w= 0.f;
		normal.x = normal.y = normal.z = 0.f;
		uv.x = uv.y = 0.f;
	}

	// 頂点データ
	D3DXVECTOR4 vertex;

	// 法線ベクトル
	D3DXVECTOR3 normal;

	// テクセル
	D3DXVECTOR2 uv;
};


struct VertexData {

	VertexData() {
		polygon_num = 0;
		polygon_vertex_num = 0;
		index_count = 0;
		mp_vertex_buffer = NULL;
		mp_index_buffer = NULL;
	}

	// ポリゴンの数
	int polygon_num;

	// 頂点の数
	int polygon_vertex_num;

	// インデックスバッファの数
	int index_count;

	// バーテックスバッファカウント
	int *mp_vertex_buffer;

	// インデックスバッファカウント
	int *mp_index_buffer;
};


class Fbx {
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
	bool Load(std::string fbx_file_path);

	// 描画
	void Draw();

	// FBX関連削除
	void Release();

private:

	// ノード探査関数
	void NodeSerch(FbxNode *node);

	// 根のノードを探査する
	void RootNodeProbe();

	// メッシュかどうか調べる
	bool IsMeshSerch(FbxNode * fbx_node);

	// メッシュかどうか調べる2
	bool IsMeshSerch2(FbxNode*fbx_node);

	// ポリゴンを3つに分割する
	void Polygon3Convert();

	// 頂点読み込み
	void VertexLoad(
		VertexData*vertex_data,
		FbxMesh*mesh
	);

	// インデックスバッファ生成
	bool IndexBufferCreate(int total_index);

	// 頂点バッファ生成
	bool VertexBufferCreate(int total_vertex);

	// ポリゴン2分割
	std::vector<INT> SplitPolygon2(const std::vector<INT>&indices4);

private:

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	// シーンの作成
	FbxScene * mp_fbx_scene;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * mp_vertex_buffer;

	// インデックスバッファ
	IDirect3DIndexBuffer9 * mp_index_buffer;

	// 全てのインデックス
	std::vector<INT>m_indices;

	// 全ての頂点数
	std::vector<FbxCustomVertex>m_custom_vertex_list;
	
	// カスタムバーテックスの配列
	std::vector<std::unique_ptr<VertexData>>mp_vertex_data_list;

	// グラフィックス
	Graphics * mp_graphics;
};