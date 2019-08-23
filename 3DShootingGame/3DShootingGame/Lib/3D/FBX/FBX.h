#pragma once

//#include<fbxsdk.h>
#include<string>

//#pragma comment(lib,"libfbxsdk.lib")
//#pragma comment(lib,"libfbxsdk-md.lib")

/*
class FBX {
public:

	static FBX GetInstance() {
		static FBX fbx;
		return fbx;
	}

public:

	FBX();

	~FBX() {

		// マネージャの破壊
		m_manager->Destroy();
	}

	bool FileOpen(std::string fbx_file_path);

	// ノード探査関数
	void NordProbe(FbxNode *node);

	// 根のノードを探査する
	void RootNodeProbe();

	// メッシュかどうか調べる
	bool IsMeshSerch(FbxNode * fbx_node);

private:

	// SDK全体を管理して各種オブジェクトの生成を行う
	FbxManager * m_manager;

	// FBXファイルを読み込んでその情報をプログラムから読み込める形に管理してくれるクラス
	FbxImporter * m_importer;

	// シーンの作成
	FbxScene * m_fbx_scene;

};*/