#include"FBX.h"
#include"../Window/Window.h"



FBX::FBX() {

	// fbxマネージャオブジェクトの生成
	m_manager = FbxManager::Create();

	// fbxインポータの作成
	m_importer = FbxImporter::Create(m_manager, "");

	// fbxシーンの作成
	m_fbx_scene = FbxScene::Create(m_manager, "");

	// nullチェック
	{
		if (m_manager == NULL) {
			Window::TextMessageBox("FBXManagerの生成に失敗しました");
		}

		if (m_importer == NULL) {
			Window::TextMessageBox("FBXImporterの生成に失敗しました");
		}

		if (m_fbx_scene == NULL) {
			Window::TextMessageBox("FBXSceneの作成に失敗しました");
		}
	}

}


bool FBX::FileOpen(std::string fbx_file_path) {

	// 初期化
	if (!m_importer->Initialize(fbx_file_path.c_str())) {

		Window::TextMessageBox("FBXimporter初期化失敗");
		return false;
	}

	// ファイルからシーンを読み込む
	if (!m_importer->Import(m_fbx_scene)) {

		Window::TextMessageBox("シーン読み込みに失敗");
		return false;
	}

	// インポータの明示的な破棄
	//m_importer->Destroy();

	// ここに処理を書いていく

	return true;

}


void FBX::NordProbe(FbxNode *node) {

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

	// メッシュがnullでないなら
	if (mesh != nullptr) {
	// ここに処理を書いていく

	}

	// 子ノードを再帰探査
	for (int i = 0; node->GetChildCount() > i; i++) {
		NordProbe(node->GetChild(i));
	}
}


void FBX::RootNodeProbe() {

	FbxNode * root_node = m_fbx_scene->GetRootNode();

	if (root_node) {

		for (int i = 0; root_node->GetChildCount() > i; i++) {
			
			NordProbe(root_node->GetChild(i));
		}
	}
}


bool FBX::IsMeshSerch(FbxNode * node) {

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


