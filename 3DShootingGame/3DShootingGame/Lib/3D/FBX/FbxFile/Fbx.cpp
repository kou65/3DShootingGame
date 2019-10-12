#include"FBX.h"
#include"../../../Window/Window.h"




Fbx::Fbx() {

	// グラフィックス受け取り
	mp_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	mp_manager = FbxManager::Create();

	// fbxシーンの作成
	mp_fbx_scene = FbxScene::Create(mp_manager, "");

	// nullチェック
	{
		if (mp_manager == NULL) {
			Window::TextMessageBox("FBXManagerの生成に失敗しました");
		}

		if (mp_fbx_scene == NULL) {
			Window::TextMessageBox("FBXSceneの作成に失敗しました");
		}
	}

}


void Fbx::Draw() {

	{
		// ワールド座標初期化
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		// 移動
		D3DXMatrixTranslation(&mat, 0.f, 0.f, 0.f);

		mp_graphics->GetInstance()->GetLpDirect3DDevice9()
			->SetTransform(D3DTS_WORLD, &mat);

		if (mp_vertex_buffer == nullptr) {
			return;
		}

		// ストリームをセット
		mp_graphics->GetLpDirect3DDevice9()->SetStreamSource(
			0,
			mp_vertex_buffer,
			0,
			sizeof(FbxCustomVertex)
		);

		// インデックス番号をデバイスに設定する
		mp_graphics->GetLpDirect3DDevice9()->SetIndices(
			mp_index_buffer
		);

		// インデックスをオフセットする
		int BASE_VERTEX_INDEX = 0;

		// 描画に使用する最小のインデックス番号
		int MIN_INDEX = 0;

		// 途中から描画したい場合に有効
		int START_INDEX = 0;

		// 三角ポリゴン頂点数
		int NUM_VERTEX = (int)m_custom_vertex_list.size() * 3;

		// START_INDEXを先頭として描画するポリゴンの数を指定する
		unsigned int PRIMITIVE_COUNT = m_indices.size();

		// どの情報を伝えるか
		mp_graphics->GetLpDirect3DDevice9()->SetFVF(FVF_FBX);
		
		// インデックス描画
		mp_graphics->GetLpDirect3DDevice9()->
			DrawIndexedPrimitive(
			// 頂点のつなぎ方
			D3DPT_TRIANGLELIST,
			// 頂点インデックスの一番最初までのオフセット値を指定
			BASE_VERTEX_INDEX,
			// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
			MIN_INDEX,
			// 上引数の最小以降の頂点数を指定
			NUM_VERTEX,
			// 描画を開始する頂点インデックスまでのオフセット値を指定
			START_INDEX,
			// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
			PRIMITIVE_COUNT
		);

		// セットテクスチャリセット
		mp_graphics->GetLpDirect3DDevice9()->SetTexture(
			0,
			NULL
		);
	}
}


bool Fbx::Load(std::string fbx_file_path) {

	// fbxインポータの作成
	FbxImporter * p_importer = FbxImporter::Create(mp_manager, "");

	// nullチェック
	if (p_importer == NULL) {
		Window::TextMessageBox("FBXImporterの生成に失敗しました");
	}

	// 初期化
	if (!p_importer->Initialize(fbx_file_path.c_str())) {

		Window::TextMessageBox("FBXimporter初期化失敗");
		return false;
	}

	// ファイルからシーンを読み込む
	if (!p_importer->Import(mp_fbx_scene)) {

		Window::TextMessageBox("シーン読み込みに失敗");
		return false;
	}

	// インポータはファイルを開いてシーンクラスとの橋渡し
	// をしてくれるだけなので、ここで破棄してもいい
	// インポータの明示的な破棄
	p_importer->Destroy();

	// ここに処理を書いていく
	RootNodeProbe();

	// バーテックスバッファ生成
	VertexBufferCreate(m_custom_vertex_list.size());

	// インデックスバッファ生成
	IndexBufferCreate(m_indices.size());

	return true;

}


void Fbx::RootNodeProbe() {

	// ルートノードを取得
	FbxNode * root_node = mp_fbx_scene->GetRootNode();

	if (root_node != NULL) {

		int count = root_node->GetChildCount();

		// 子ノードの数だけ探査する
		for (int i = 0; i < count; i++) {
			
			MeshSerch(root_node->GetChild(i));
		}
	}
}


void Fbx::MeshSerch(FbxNode *node) {

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

	// 頂点データ生成
	mp_vertex_data_list.emplace_back(new VertexData);

	// 頂点データ受け取り
	VertexData *vertex_data = mp_vertex_data_list.back().get();

	// バーテックスデータ
	if (vertex_data == nullptr) {
		return;
	}

	// 頂点情報読み込み
	VertexLoad(
		vertex_data,
		mesh
	);

	// メッシュがnullでないなら
	if (mesh != nullptr) {
		// メッシュが見つかったので必要な情報を取得させる
	}

	int count = node->GetChildCount();

	// 子ノードを再帰探査
	for (int i = 0; i < count; i++) {
		MeshSerch(node->GetChild(i));
	}

}


void Fbx::VertexLoad(
	VertexData*vertex_data,
	FbxMesh*mesh
	) {

	// 総ポリゴン数
	vertex_data->polygon_count = mesh->GetPolygonCount();

	// 頂点数
	vertex_data->vertex_count = mesh->GetControlPointsCount();

	// インデックス数
	vertex_data->index_count = mesh->GetPolygonVertexCount();

	// 頂点座標配列
	vertex_data->vector = mesh->GetControlPoints();

	// 頂点の種類
	enum VertexType {
		X,           // x     = 0 
		Y,           // y     = 1 
		Z,           // z     = 2 
		VERTEX_TOTAL // TOTAL = 3 
	};


	// 頂点数分回す
	for (int i = 0; i < vertex_data->vertex_count; i++) {

		// カスタムバーテックス
		FbxCustomVertex custom_vertex;

		// i番目の頂点の座標

		// x座標
		custom_vertex.vertex.x = 
			(float)vertex_data->vector[i][X];

		// y座標
		custom_vertex.vertex.y = 
			(float)vertex_data->vector[i][Y];

		// z座標
		custom_vertex.vertex.z = 
			(float)vertex_data->vector[i][Z];

		// カスタムバーテックスリスト
		m_custom_vertex_list.emplace_back(custom_vertex);
	}

	// インデックス
	int * index = mesh->GetPolygonVertices();

	// 読み込み
	for (int i = 0; i < vertex_data->index_count; i++) {
		m_indices.push_back(index[i]);
	}
}


bool Fbx::IsMeshSerch(FbxNode * node) {

	// 探索ノード
	if (node) {

		int attr_count = node->GetNodeAttributeCount();

		for (int i = 0; i < attr_count; i++) {

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


void Fbx::Polygon3Convert() {

	// 全てのメッシュを三角ポリゴンに変換する
	FbxGeometryConverter geometry_converter(mp_manager);

	geometry_converter.Triangulate(mp_fbx_scene, true);
}


bool Fbx::IndexBufferCreate(int total_face_num) {

	// 頂点32bit
	int * index_buffer;

	// 32bitサイズのインデックスバッファを作成
	mp_graphics->CreateIndexBuffer32BitSize(
		&mp_index_buffer,
		(total_face_num * sizeof(int))
	);

	// ロック
	index_buffer = mp_graphics->LockIndexBuffer32BitSize(
		&mp_index_buffer
	);

	// nullチェック
	if (index_buffer == nullptr) {
		return false;
	}

	// 面数
	for (unsigned int i = 0; i < m_indices.size(); i++) {
		index_buffer[i] = m_indices[i];
	}

	return true;
}


bool Fbx::VertexBufferCreate(int total_vertex_num) {
	
	// 頂点バッファ作成
	mp_graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		(sizeof(FbxCustomVertex) * total_vertex_num),
		// リソースの使用法
		0,
		// 柔軟な頂点フォーマットの型を指定する
		0,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&mp_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	FbxCustomVertex *custom_vertex_list;

	// ロック
	mp_vertex_buffer->Lock(
		0,
		total_vertex_num * sizeof(FbxCustomVertex),
		(void**)&custom_vertex_list,
		0
	);

	// ここで頂点情報を代入させる
	int count = 0;

	const int OFFSET = 1;

	for (unsigned int i = 0; i < m_custom_vertex_list.size(); i++) {
		custom_vertex_list[i] = m_custom_vertex_list[i];
	}

	// アンロック
	mp_vertex_buffer->Unlock();

	return true;
}


void Fbx::Release() {

	mp_manager->Destroy();
}