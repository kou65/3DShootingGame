#include"FBX.h"
#include"../../Window/Window.h"
#include<fbxsdk.h>



Fbx::Fbx() {

	// fbxマネージャオブジェクトの生成
	mp_manager = FbxManager::Create();

	// fbxインポータの作成
	mp_importer = FbxImporter::Create(mp_manager, "");

	// fbxシーンの作成
	mp_fbx_scene = FbxScene::Create(mp_manager, "");

	// nullチェック
	{
		if (mp_manager == NULL) {
			Window::TextMessageBox("FBXManagerの生成に失敗しました");
		}

		if (mp_importer == NULL) {
			Window::TextMessageBox("FBXImporterの生成に失敗しました");
		}

		if (mp_fbx_scene == NULL) {
			Window::TextMessageBox("FBXSceneの作成に失敗しました");
		}
	}

}


bool Fbx::FileOpen(std::string fbx_file_path) {

	// 初期化
	if (!mp_importer->Initialize(fbx_file_path.c_str())) {

		Window::TextMessageBox("FBXimporter初期化失敗");
		return false;
	}

	// ファイルからシーンを読み込む
	if (!mp_importer->Import(mp_fbx_scene)) {

		Window::TextMessageBox("シーン読み込みに失敗");
		return false;
	}

	// インポータはファイルを開いてシーンクラスとの橋渡し
	// をしてくれるだけなので、ここで破棄してもいい
	// インポータの明示的な破棄
	mp_importer->Destroy();

	// ここに処理を書いていく

	return true;

}


void Fbx::GetMesh(FbxNode *node) {

	// ノードがnullなら返す
	if (node == nullptr) {
		return;
	}

		// メッシュが存在しなかったら返す
	if (IsMeshSerch(node) == false) {
		return;
	}

	// メッシュ取得
	FbxMesh * mesh = node->GetMesh();

	// 頂点データリスト追加
	m_vertex_data_list.emplace_back();

	// 現在の要素数追加
	unsigned int current_num = m_vertex_data_list.size();

	// 頂点データ
	VertexData vertex_data = m_vertex_data_list[current_num];

	// バーっテックス情報埋め込み
	vertex_data.polygon_count = mesh->GetPolygonCount();
	vertex_data.vertex_count = mesh->GetControlPointsCount();
	vertex_data.index_count = mesh->GetPolygonVertexCount();
	vertex_data.vector = mesh->GetControlPoints();

	// メッシュがnullでないなら
	if (mesh != nullptr) {
	// メッシュが見つかったので必要な情報を取得させる

	}

	// 子ノードを再帰探査
	for (int i = 0; node->GetChildCount() > i; i++) {
		GetMesh(node->GetChild(i));
	}
}


void Fbx::RootNodeProbe() {

	// ルートノードを取得
	FbxNode * root_node = mp_fbx_scene->GetRootNode();

	if (root_node != NULL) {

		// 子ノードの数だけ探査する
		for (int i = 0; root_node->GetChildCount() > i; i++) {
			
			GetMesh(root_node->GetChild(i));
		}
	}
}


void Fbx::VertexLoad() {

}


bool Fbx::IsMeshSerch(FbxNode * node) {

	// 探索ノード
	if (node) {

		int attr_count = node->GetNodeAttributeCount();

		for (int i = 0; attr_count > i; i++) {

			FbxNodeAttribute::EType attr_type =
				node->GetNodeAttributeByIndex(i)->GetAttributeType();


			// ノードがメッシュにつながっているかチェック
			if (attr_type == FbxNodeAttribute::EType::eMesh) {
				// メッシュに繋がっている
				return true;
			}
		}
	}

	// メッシュには繋がっていない
	return false;
}


void Fbx::Release() {

	mp_manager->Destroy();
}