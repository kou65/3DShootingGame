#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"



Fbx::Fbx() {

	// グラフィックス受け取り
	mp_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	mp_manager = FbxManager::Create();

	// fbxシーンの作成
	mp_fbx_scene = FbxScene::Create(mp_manager, "");

	// マテリアル数初期化
	m_material_num = 0;

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

	// ライトをオンにするかどうか
	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		SetRenderState(D3DRS_LIGHTING,FALSE);

	// マテリアル分描画する
	for (int i = 0; i < m_material_num; i++)
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

		// 頂点情報受け取り
		VertexData*vertex_data = mp_vertex_data_list[i].get();

		// テクスチャ名受け取り
		std::string texture_name = vertex_data->material_list.back().texture_name;

		// テクスチャが0ではないなら
		if (TextureManager::GetInstance()->Find2DTexture(texture_name) == true) {
			mp_graphics->GetLpDirect3DDevice9()->SetTexture(
				0,
				TextureManager::GetInstance()->GetTextureData2D(texture_name)
			);
		}

		// インデックスをオフセットする
		int BASE_VERTEX_INDEX = 0;

		// 描画に使用する最小のインデックス番号
		int MIN_INDEX = 0;

		// 途中から描画したい場合に有効
		int START_INDEX = vertex_data->start_index;

		// 三角ポリゴン頂点数
		int NUM_VERTEX = (int)vertex_data->polygon_num * 3;

		// START_INDEXを先頭として描画するポリゴンの数を指定する
		unsigned int PRIMITIVE_COUNT = vertex_data->polygon_num;

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


bool Fbx::Load(
	const std::string &fbx_file_path
	) {

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

	// 頂点情報
	std::vector<D3DXVECTOR4>vertex_list;
	// uv情報
	std::vector<D3DXVECTOR2>uv_list;
	// 法線情報
	std::vector<D3DXVECTOR3>normal_list;

	// ルートノードを取得
	FbxNode * p_root_node = mp_fbx_scene->GetRootNode();

	if (p_root_node != NULL) {

		int count = p_root_node->GetChildCount();

		// 子ノードの数だけ探査する
		for (int i = 0; i < count; i++) {
			
			// 子ノード取得
			FbxNode * p_child_node= p_root_node->GetChild(i);

			// ノード探査
			NodeSerch(
				vertex_list,
				uv_list,
				mp_vertex_data_list,
				p_child_node
			);
		}

		// カスタムバーテックス作成
		CustomVertexCreate(
			m_indices,
			vertex_list,
			uv_list,
			normal_list
		);
	}
}


void Fbx::NodeSerch(
	std::vector<D3DXVECTOR4>&vertex_list,
	std::vector<D3DXVECTOR2>&uv_list,
	std::vector<std::unique_ptr<VertexData>>&p_vertex_data_list,
	FbxNode *p_node
) {

	// ノードがnullなら返す
	if (p_node == nullptr) {
		return;
	}

	// メッシュが存在したら処理にはいる
	if (IsMeshSerch(p_node) == true) {

		// メッシュ取得
		FbxMesh * p_mesh = p_node->GetMesh();

		// 頂点データ生成
		p_vertex_data_list.emplace_back(new VertexData);

		// 最初に描画するインデックス数を入れる
		p_vertex_data_list.back().get()->start_index =
			(int)m_indices.size();

		// メッシュがnullでないなら
		if (p_mesh != NULL) {

			// メッシュが見つかったので必要な情報を取得させる

			// 頂点情報読み込み
			VertexInfoLoad(
				vertex_list,
				p_mesh
			);

			// uv情報読み込み
			UvInfoLoad(
				uv_list,
				mp_vertex_data_list,
				p_mesh
			);

			// インデックス読み込み
			IndexInfoLoad(
				m_indices,
				p_vertex_data_list,
				p_mesh
			);
		}
	}

	// さらに子ノードからその子ノードを探査
	int count = p_node->GetChildCount();

	// 子ノードを再帰探査
	for (int i = 0; i < count; i++) {
		NodeSerch(
			vertex_list,
			uv_list,
			p_vertex_data_list,
			p_node->GetChild(i)
		);
	}
}


void Fbx::IndexInfoLoad(
	std::vector<INT>&index_list,
	std::vector<std::unique_ptr<VertexData>>&p_vertex_data_list,
	FbxMesh*p_mesh
) {

	// 面数
	int polygon_count = p_mesh->GetPolygonCount();

	// 読み込み
	for (int p = 0; p < polygon_count; p++) {

		// 仮のインデックスリスト
		std::vector<INT>indices;

		// i番目のポリゴンの頂点数
		int vertex_num = p_mesh->GetPolygonSize(p);


		for (int i = 0; i < vertex_num; i++) {

			// 頂点インデックス
			int index_number = p_mesh->GetPolygonVertex(p, i);

			// 仮のインデックスバッファに代入
			indices.push_back(index_number);
		}

		// 頂点情報読み込み
		
		// 頂点データ受け取り
		VertexData *vertex_data = p_vertex_data_list.back().get();

		// バーテックスデータ
		if (vertex_data == nullptr) {
			return;
		}
		
		// ポリゴン数加算
		vertex_data->polygon_num++;

		// 頂点数が4つならポリゴンを半分に切る
		if (vertex_num == 4) {

			indices = SplitPolygon2(indices);
			// ポリゴン数加算
			vertex_data->polygon_num++;
		}

		// 総インデックス数を代入
		for (unsigned int j = 0; j < indices.size(); j++){
			m_indices.push_back(indices[j]);
		}
	}
}


void Fbx::VertexInfoLoad(
	std::vector<D3DXVECTOR4>&vertex_list,
	FbxMesh*p_mesh
	) {

	// 総ポリゴン数
	int polygon_num = p_mesh->GetPolygonCount();

	// 頂点数
	int polygon_vertex_num = p_mesh->GetControlPointsCount();

	// インデックス数
	int index_count = p_mesh->GetPolygonVertexCount();

	// 頂点インデックス配列
	int * verticse = p_mesh->GetPolygonVertices();


	// 頂点の種類
	enum VertexType {
		X,           // x     = 0 
		Y,           // y     = 1 
		Z,           // z     = 2 
		W,
		VERTEX_TOTAL // TOTAL = 3 
	};


	// 頂点座標配列
	FbxVector4 * vertex_pos_buffer = p_mesh->GetControlPoints();

	int vertex_buffer_num = p_mesh->GetControlPointsCount();

	// 頂点数分回す
	for (int i = 0; i < vertex_buffer_num; i++) {

		D3DXVECTOR4 vertex;

		// i番目の頂点の座標

		// x座標
		vertex.x = 
			(float)vertex_pos_buffer[i][X];

		// y座標
		vertex.y = 
			(float)vertex_pos_buffer[i][Y];

		// z座標
		vertex.z = 
			(float)vertex_pos_buffer[i][Z];

		// w
		vertex.w = 
			(float)vertex_pos_buffer[i][W];

		// 頂点配列追加
		vertex_list.emplace_back(vertex);
	}

}


void Fbx::UvInfoLoad(
	std::vector<D3DXVECTOR2>&uv_list,
	std::vector<std::unique_ptr<VertexData>>&p_vertex_data_list,
	FbxMesh*p_mesh) {

	// レイヤー数を取得
	int layer_count = p_mesh->GetLayerCount();

	for (int i = 0; i < layer_count; i++) {

		// レイヤー取得
		FbxLayer*p_layer = p_mesh->GetLayer(i);

		// UVを格納しているFbxLayerElementUVオブジェクトを取得
		FbxLayerElementUV * p_element = p_layer->GetUVs();

		// uv存在をチェック
		if (p_element == 0) {
			continue;
		}

		// uv数を取得
		int uv_num = p_element->GetDirectArray().GetCount();

		// uvインデックス数を取得
		int index_num = p_element->GetIndexArray().GetCount();

		int size = 0;

		// サイズを比較
		if (uv_num > index_num) {
			size = uv_num;
		}
		else {
			size = index_num;
		}
		
		D3DXVECTOR2 * buffer = new D3DXVECTOR2[size];

		// マッピングモード・リファレンスモード別にUV取得
		FbxLayerElement::EMappingMode mapping_mode = 
			p_element->GetMappingMode();
		FbxLayerElement::EReferenceMode ref_mode = 
			p_element->GetReferenceMode();
		
		D3DXVECTOR2 uv;

		bool is_enter = false;

		// マッピングモードの判別
		if (mapping_mode == FbxLayerElement::eByPolygonVertex) {

			// リファレンスモードの判別
			if (ref_mode == FbxLayerElement::eDirect) {

				// 直接取得
				for (int i = 0; i < size; i++) {

					uv.x =
						(float)p_element->GetDirectArray().GetAt(i)[0];
					uv.y =
						1.f - (float)p_element->GetDirectArray().GetAt(i)[1];

					uv_list.emplace_back(uv);

					// テクスチャ名受け取り
					p_vertex_data_list.back().get()->
						material_list.back().texture_name =
						p_element->GetName();
				}
				// 入った
				is_enter = true;

			}
			else if (ref_mode == FbxLayerElement::eIndexToDirect) {

				// インデックスから取得
				for (int i = 0; i < uv_num; i++) {

					// インデックス取得
					int index = p_element->GetIndexArray().GetAt(i);

					uv.x = (float)p_element->GetDirectArray().GetAt(index)[0];
					// blenderは左下からuv頂点がはじまる
					uv.y = 1.f - (float)p_element->GetDirectArray().GetAt(index)[1];

					uv_list.emplace_back(uv);
				}

				// 入った
				is_enter = true;
			}

		}
	}
}


void Fbx::MaterialLoad(
	std::vector<std::unique_ptr<VertexData>>&p_vertex_data_list,
	FbxNode*p_mesh
) {

	FbxNode*p_node = p_mesh;

	if (p_node == 0) {
		return;
	}

	// マテリアルの数
	m_material_num = p_node->GetMaterialCount();

	if (m_material_num == 0) {
		return;
	}

	for (int i = 0; i < m_material_num; i++) {

		// サーフェイス
		FbxSurfaceMaterial*p_material = p_node->GetMaterial(i);

		if (p_material != 0) {
			// マテリアル解析
			
			// LambertかPhongか
			if (p_material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {

				// Lambertにダウンキャスト
				FbxSurfaceLambert *lambert = (FbxSurfaceLambert*)p_material;
				
				// ランバートの読み込み
			}
			else if (p_material->GetClassId().Is(FbxSurfacePhong::ClassId)) {

				// Phongにダウンキャスト
				FbxSurfacePhong * phong = (FbxSurfacePhong*)p_material;
			}
		}
	}

	// テクスチャ読み込み
	TextureManager::GetInstance()->Load2D(
		file_name,
		texture_name
	);
}


void Fbx::SetLambertInfo(
	FbxSurfaceLambert*lambert,
	MaterialInfo*material_info
	) {


	if (lambert == nullptr) {
		return;
	}

	D3DXMATERIAL *material = &material_info->material;

	// アンビエント
	material->MatD3D.Ambient.r = (float)lambert->Ambient.Get()[0];
	material->MatD3D.Ambient.g = (float)lambert->Ambient.Get()[1];
	material->MatD3D.Ambient.b = (float)lambert->Ambient.Get()[2];

	// ディフューズ
	material->MatD3D.Diffuse.r = (float)lambert->Diffuse.Get()[0];
	material->MatD3D.Diffuse.g = (float)lambert->Diffuse.Get()[1];
	material->MatD3D.Diffuse.b = (float)lambert->Diffuse.Get()[2];

	// エミッシブ
	material->MatD3D.Emissive.r = (float)lambert->Emissive.Get()[0];
	material->MatD3D.Emissive.g = (float)lambert->Emissive.Get()[1];
	material->MatD3D.Emissive.b = (float)lambert->Emissive.Get()[2];

	// バンプ
	//material->MatD3D.Power.x = (float)lambert->GetBump().Get()[0];
	//material->MatD3D.Power.y = (float)lambert->GetBump().Get()[1];
	//material->MatD3D..z = (float)lambert->GetBump().Get()[2];

	// 透過度
	float A = (float)lambert->TransparencyFactor.Get();

}


void Fbx::SetPhongInfo(
	FbxSurfacePhong*p_phong,
	MaterialInfo*p_material_info
	) {

	// スペキュラ
	specular_.r = (float)phong->GetSpecularColor().Get()[0];
	specular_.g = (float)phong->GetSpecularColor().Get()[1];
	specular_.b = (float)phong->GetSpecularColor().Get()[2];

	// 光沢
	shininess_ = (float)phong->GetShininess().Get();

	// 反射
	reflectivity_ = (float)phong->GetReflectionFactor().Get();

}


void Fbx::CustomVertexCreate(
	std::vector<INT>&indeces,
	std::vector<D3DXVECTOR4>&vertex_list,
	std::vector<D3DXVECTOR2>&uv_list,
	std::vector<D3DXVECTOR3>&normal_list
) {

	FbxCustomVertex custom_vertex;


	// 現在の頂点分回す
	for (unsigned int i = 0; i < indeces.size(); i++) {

		unsigned int id = indeces[i] - 1;

		// 頂点データ代入
		if (vertex_list.size() > i) {
			custom_vertex.vertex = vertex_list[i];
		}
		
		// uvデータ代入
		if (uv_list.size() > i) {
			custom_vertex.uv = uv_list[i];
		}

		// 法線データ代入
		if (normal_list.size() > id) {
			custom_vertex.normal = normal_list[id];
		}

		// 本体に代入
		m_custom_vertex_list.emplace_back(custom_vertex);
	}
}


bool Fbx::IsMeshSerch(FbxNode*p_node) {

	FbxNodeAttribute * p_attrib = p_node->GetNodeAttribute();

	if (p_attrib == NULL) {
		return false;
	}

	// タイプを使う
	FbxNodeAttribute::EType type = p_attrib->GetAttributeType();
	
	// メッシュ
	if (type == FbxNodeAttribute::eMesh) {
		return true;
	}

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

	// アンロック
	mp_index_buffer->Unlock();

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


std::vector<INT> Fbx::SplitPolygon2(
	const std::vector<INT>&indices4
) {

	std::vector<INT>indices;

	const int NEW_VERTEX_NUM = 6;

	// 分割要素
	int split_number_list[NEW_VERTEX_NUM] =
	{
		// 1面
		0,1,3,
		// 2面
		3,1,2
	};

	for (int i = 0; i < NEW_VERTEX_NUM; i++) {

		int split_number = split_number_list[i];

		// 分割する要素で代入
		indices.push_back(indices4[split_number]);
	}

	return indices;
}


void Fbx::Release() {

	mp_manager->Destroy();
}