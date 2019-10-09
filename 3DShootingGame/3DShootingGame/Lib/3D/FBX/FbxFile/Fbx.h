#pragma once

#include<string>
#include<vector>
#include<fbxsdk.h>


#pragma comment(lib,"libfbxsdk.lib")
#pragma comment(lib,"libfbxsdk-md.lib")


struct VertexData {

	// ポリゴンの数
	int polygon_count;

	// 頂点の数
	int vertex_count;

	// インデックスバッファの数
	int index_count;

	// 頂点座標
	FbxVector4*vector;
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
	
	bool FileOpen(std::string fbx_file_path);

	// FBX関連削除
	void Release();

private:

	void Polygon3Convert();

	// ノード探査関数
	void MeshSerch(FbxNode *node);

	// 根のノードを探査する
	void RootNodeProbe();

	// メッシュかどうか調べる
	bool IsMeshSerch(FbxNode * fbx_node);

	// 頂点の読み込み
	void VertexLoad();


private:

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * mp_manager;

	// FBXファイルを読み込んでその情報をプログラムから読み込める形に管理してくれるクラス
	FbxImporter * mp_importer;

	// シーンの作成
	FbxScene * mp_fbx_scene;

	// カスタムバーテックスの配列
	std::vector<VertexData>m_vertex_data_list;
};