﻿#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility.h"



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
			m_mesh_data_list[i].index_buffer
		);

		// 頂点情報受け取り
		//if (mp_vertex_data_list.size() > i) {
		//	continue;
		//}

		//VertexData*vertex_data = mp_vertex_data_list[i].get();
		MeshData vertex_data = m_mesh_data_list[i];
		
		std::string texture_name;

		// テクスチャ名受け取り
		if (vertex_data.material_info.texture_name.size() > 0) {
			texture_name =
				vertex_data.material_info.texture_name.back();
		}

		// テクスチャが0ではないなら
		if (TextureManager::GetInstance()->Find2DTexture(texture_name) == true) {
			mp_graphics->GetLpDirect3DDevice9()->SetTexture(
				0,
				TextureManager::GetInstance()->GetTextureData2D(texture_name)
			);
		}

		// どの情報を伝えるか
		mp_graphics->GetLpDirect3DDevice9()->SetFVF(FVF_FBX);

		// インデックス描画
		mp_graphics->GetLpDirect3DDevice9()->
			DrawIndexedPrimitive(
				// 頂点のつなぎ方
				D3DPT_TRIANGLELIST,
				// 頂点インデックスの一番最初までのオフセット値を指定
				0,
				// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
				0,
				// 上引数の最小以降の頂点数を指定
				(int)vertex_data.polygon_num * 3,
				// 描画を開始する頂点インデックスまでのオフセット値を指定
				vertex_data.start_index,
				// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
				vertex_data.polygon_num
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

	std::vector<std::string>split_file_path;

	split_file_path = Utility::SplitStr('/', fbx_file_path);

	// 最後の.fbxを除いたファイルパスを渡す0
	for (unsigned int i = 0; i < split_file_path.size() - 1; i++) {
		m_texture_file_path += split_file_path[i];
	}

	m_fbx_file_name = split_file_path.back();

	// nullチェック
	if (p_importer == NULL) {
		Window::TextMessageBox("FBXImporterの生成に失敗しました");
	}

	// インポータの初期化
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


	/* ここに処理を書いていく */
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


	fbxsdk::FbxGeometryConverter converter(mp_manager);
	// マテリアルと1メッシュ毎に分ける
	converter.SplitMeshesPerMaterial(mp_fbx_scene, true);
	// 全てを三角形にする
	converter.Triangulate(mp_fbx_scene, true);

	// メッシュ分のバッファを確保
	UINT mesh_num = (UINT)mp_fbx_scene->GetSrcObjectCount<FbxMesh>();
	// マテリアル分のバッファを確保
	UINT materialNum = mesh_num;

	for (UINT i = 0; i < mesh_num;i++) {

		FbxMesh * p_mesh = mp_fbx_scene->GetSrcObject<FbxMesh>(i);

		// 頂点データ生成
		mp_vertex_data_list.emplace_back(new MeshData);

		// 最初に描画するインデックス数を入れる
		mp_vertex_data_list.back().get()->start_index =
			(int)m_indices.size();

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

		// 子ノード取得
		FbxNode * p_child_node = p_root_node->GetChild(i);

		MaterialLoad(
			mp_vertex_data_list,
			p_child_node,
			p_mesh
		);

		// インデックス読み込み
		IndexInfoLoad(
			m_indices,
			mp_vertex_data_list,
			p_mesh
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


void Fbx::NodeSerch(
	std::vector<D3DXVECTOR4>&vertex_list,
	std::vector<D3DXVECTOR2>&uv_list,
	std::vector<std::unique_ptr<MeshData>>&p_vertex_data_list,
	FbxNode *p_node
) {

	// ノードがnullなら返す
	if (p_node == nullptr) {
		return;
	}

	// メッシュが存在したら処理にはいる
	if (SerchNodeType(p_node) == MESH) {

		// メッシュ取得
		FbxMesh * p_mesh = p_node->GetMesh();

		// 頂点データ生成
		p_vertex_data_list.emplace_back(new MeshData);

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

			MaterialLoad(
				p_vertex_data_list,
				p_node,
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
	std::vector<std::unique_ptr<MeshData>>&p_vertex_data_list,
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
		MeshData *vertex_data = p_vertex_data_list.back().get();

		// バーテックスデータ
		if (vertex_data == nullptr) {
			return;
		}
		
		// ポリゴン数加算
		vertex_data->polygon_num++;

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

	// メッシュのトランスフォーム
	FbxVector4 T = p_mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 R = p_mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 S = p_mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix TRS = FbxAMatrix(T, R, S);
	//	全頂点変換
	for (int v = 0; v < p_mesh->GetControlPointsCount(); v++)
	{
		vertex_pos_buffer[v] = TRS.MultT(vertex_pos_buffer[v]);
	}

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
	std::vector<std::unique_ptr<MeshData>>&p_vertex_data_list,
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
					//p_vertex_data_list.back().get()->
					//	material_list.back().pTextureFilename =
					//	p_element->GetName();
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
	std::vector<std::unique_ptr<MeshData>>&p_vertex_data_list,
	FbxNode*p_node,
	FbxMesh*p_mesh
) {

	bool can_get_texture = false;

	if (p_node == 0) {
		return;
	}

	// マテリアルの数
	m_material_num = p_node->GetMaterialCount();

	if (m_material_num == 0) {
		return;
	}

	for (int i = 0; i < m_material_num; i++) {

		// サーフェイス受け取り
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

			// テクスチャ読み込み
			TextureInfoLoad(
				p_mesh,
				&p_vertex_data_list.back()->material_info);
		}
	}
	
}


void Fbx::SetLambertInfo(
	FbxSurfaceLambert*lambert,
	D3DXMATERIAL*material_info
	) {


	if (lambert == nullptr) {
		return;
	}

	D3DXMATERIAL *p_material = material_info;

	// アンビエント
	p_material->MatD3D.Ambient.r = (float)lambert->Ambient.Get()[0];
	p_material->MatD3D.Ambient.g = (float)lambert->Ambient.Get()[1];
	p_material->MatD3D.Ambient.b = (float)lambert->Ambient.Get()[2];

	// ディフューズ
	p_material->MatD3D.Diffuse.r = (float)lambert->Diffuse.Get()[0];
	p_material->MatD3D.Diffuse.g = (float)lambert->Diffuse.Get()[1];
	p_material->MatD3D.Diffuse.b = (float)lambert->Diffuse.Get()[2];

	// エミッシブ
	p_material->MatD3D.Emissive.r = (float)lambert->Emissive.Get()[0];
	p_material->MatD3D.Emissive.g = (float)lambert->Emissive.Get()[1];
	p_material->MatD3D.Emissive.b = (float)lambert->Emissive.Get()[2];

	// バンプ
	//material->MatD3D.Power.x = (float)lambert->GetBump().Get()[0];
	//material->MatD3D.Power.y = (float)lambert->GetBump().Get()[1];
	//material->MatD3D..z = (float)lambert->GetBump().Get()[2];

	// 透過度
	float A = (float)lambert->TransparencyFactor.Get();
}


void Fbx::SetPhongInfo(
	FbxSurfacePhong*p_phong,
	D3DXMATERIAL*p_material_info
	) {

	D3DXMATERIAL *p_material = p_material_info;

	// スペキュラ
	p_material->MatD3D.Specular.r = (float)p_phong->Specular.Get()[0];
	p_material->MatD3D.Specular.g = (float)p_phong->Specular.Get()[1];
	p_material->MatD3D.Specular.b = (float)p_phong->Specular.Get()[2];

	// 光沢
	p_material->MatD3D.Power = (float)p_phong->Shininess.Get();

	// 反射
	//reflectivity_ = (float)phong->GetReflectionFactor().Get();

}


bool Fbx::TextureInfoLoad(
	FbxMesh*p_mesh,
	MaterialInfo*p_material_info
)
{
	FbxLayerElementMaterial * p_element_material =
		p_mesh->GetElementMaterial();

	if (p_element_material == nullptr) {
		return false;
	}

	// マテリアル解析
	int index = p_element_material->GetIndexArray().GetAt(0);

	// サーフェイスマテリアル取得
	FbxSurfaceMaterial* p_material =
		p_mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (p_material == nullptr) {
		return false;
	}

	// ディフューズプロパティを検索
	FbxProperty fbx_property = 
		p_material->FindProperty(FbxSurfaceMaterial::sDiffuse);

	// プロパティが持っているレイヤードテクスチャの枚数をチェック
	int layer_num = 
		fbx_property.GetSrcObjectCount<FbxLayeredTexture>();

	// テクスチャ数を取得
	int file_texture_count = 
		fbx_property.GetSrcObjectCount<FbxFileTexture>();

	std::string texture_file_name;
	std::string texture_name;

	if (file_texture_count > 0 && layer_num == 0) 
	{
		//--- テクスチャの数だけ繰り返す ---//

		for (int j = 0; file_texture_count > j; j++) {

			//--- テクスチャを取得 ---//
			FbxFileTexture* p_texture =
				fbx_property.GetSrcObject<FbxFileTexture>(j);

			// nullチェック
			if (p_texture == nullptr) {
				continue;
			}
			
			// ファイルパスを取得
			texture_file_name = p_texture->GetFileName();

			// テクスチャ名を取得
			texture_name = p_texture->GetName();

			//--- UVSet名を取得 ---//
			std::string uv_set_name =
				p_texture->UVSet.Get().Buffer();
		}

		// テクスチャファイル名に追加
		p_material_info->texture_name_list.
			emplace_back(texture_file_name);

	}
	// レイヤーがある場合(こちらはまだ機能しない)
	else if(layer_num > 0) {

		// テクスチャの数だけ繰り返す
		for (int i = 0; i < file_texture_count; i++) {

			// レイヤードのテクスチャを受け取り
			FbxLayeredTexture * layerd_texture =
				fbx_property.GetSrcObject<FbxLayeredTexture>(i);

			if (layerd_texture == nullptr) {
				continue;
			}

			int texture_count =
				layerd_texture->
				GetSrcObjectCount<FbxFileTexture>();

			// レイヤーから順にテクスチャを取得
			for (int k = 0; k < texture_count; k++) {
				FbxFileTexture * p_texture = fbx_property.GetSrcObject<FbxFileTexture>(k);

				if (p_texture == nullptr) {
					continue;
				}

				std::string texture_name =
					p_texture->GetRelativeFileName();
			}
		}
	}
	else {
		return false;
	}

	std::vector<std::string>split_str;
	
	// 階層で
	split_str = Utility::SplitStr('/', texture_file_name);

	if (split_str.size() == 1) {
		// バックスラッシュで
		split_str = Utility::SplitStr('\\', texture_file_name);
	}
	
	// 最後と次のファイル階層名取得
	//std::string fbx_texture_file_path = 
	//	split_str[(int)split_str.size() - 2] + 
	//	'/' + 
	//	split_str[(int)split_str.size() - 1];
	//
	//// テクスチャ名取得
	//texture_name = 
	//	m_texture_file_path + 
	//	m_fbx_file_name + 
	//	"/" + 
	//	fbx_texture_file_path;

	// テクスチャ読み込み
	if (texture_name.size() > 0) {
		TextureManager::GetInstance()->Load2D(
			texture_name.c_str(),
			texture_name.c_str()
		);
	}

	p_material_info->texture_name_list.back() = texture_name;

	//FbxFree(p_file_name);

	return false;
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


Fbx::NodeType Fbx::SerchNodeType(FbxNode*p_node) {

	FbxNodeAttribute * p_attrib = p_node->GetNodeAttribute();

	if (p_attrib == NULL) {
		return NONE;
	}

	// タイプを使う
	FbxNodeAttribute::EType type = p_attrib->GetAttributeType();
	
	// メッシュ
	if (type == FbxNodeAttribute::eMesh) {
		return MESH;
	}

	return NONE;
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