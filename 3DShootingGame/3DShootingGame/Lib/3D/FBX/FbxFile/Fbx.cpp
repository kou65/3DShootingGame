#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Graphics/Graphics.h"




Fbx::Fbx() {

	// グラフィックス受け取り
	mp_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	mp_manager = FbxManager::Create();

	// fbxシーンの作成
	mp_fbx_scene = FbxScene::Create(mp_manager, "");

	// マテリアル数初期化
	m_material_num = 0;

	// インデックス数初期化
	m_current_index_num = 0;

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
		SetRenderState(D3DRS_LIGHTING, FALSE);



	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 移動
	D3DXMatrixTranslation(&mat, 0.f, 0.f, 0.f);

	mp_graphics->GetInstance()->GetLpDirect3DDevice9()
		->SetTransform(D3DTS_WORLD, &mat);


	IDirect3DDevice9* pDevice =
		Graphics::GetInstance()->GetLpDirect3DDevice9();


	// メッシュインデックスを回す
	for (UINT meshIdx = 0; meshIdx < m_mesh_num; meshIdx++)
	{

		MaterialInfo* mate_info =
			&m_mesh_data_list[meshIdx].material_info;

		// テクスチャーの設定
		if (TextureManager::GetInstance()->
			FindTexture(mate_info->texture_name) == true) {
			pDevice->SetTexture(
				0,
				TextureManager::GetInstance()->GetTextureData
				(mate_info->texture_name)
			);
		}

		// マテリアルの設定
		pDevice->SetMaterial(
			&mate_info->material
		);

		FbxMeshData* p_mesh_data =
			&m_mesh_data_list[meshIdx];

		if (meshIdx != p_mesh_data->materialIndex)
		{
			continue;
		}

		// 頂点バッファの設定
		pDevice->SetStreamSource(
			0,
			p_mesh_data->vertex_buffer,
			0,
			p_mesh_data->vertexStride
		);

		// 頂点フォーマットの指定
		pDevice->SetFVF(p_mesh_data->fvf);

		// インデックス数
		if (p_mesh_data->indexNum)
		{
			// インデックスバッファの設定
			pDevice->SetIndices(
				p_mesh_data->index_buffer
			);

			// 描画
			pDevice->DrawIndexedPrimitive(
				p_mesh_data->primType,
				0,
				0,
				p_mesh_data->vertexNum,
				0,
				p_mesh_data->primNum);
		}

	}
}


bool Fbx::Load(
	const std::string &fbx_file_path
	) {

	// ルートパスを設定
	SetRootPath(fbx_file_path.c_str());

	// fbxインポータの作成
	FbxImporter * p_importer = FbxImporter::Create(mp_manager, "");


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
	LoadMesh();

	return true;
}


void Fbx::LoadMesh() {

	// 頂点情報
	std::vector<D3DXVECTOR3>vertex_list;
	// uv情報
	std::vector<D3DXVECTOR2>uv_list;
	// 法線情報
	std::vector<D3DXVECTOR3>normal_list;

	// ルートノードを取得
	FbxNode * p_root_node = mp_fbx_scene->GetRootNode();

	// メッシュ分のバッファを確保
	UINT mesh_num = (UINT)mp_fbx_scene->GetSrcObjectCount<FbxMesh>();
	// マテリアル分のバッファを確保
	UINT materialNum = mesh_num;


	// ポリゴン変換
	Polygon3Convert();

	// メッシュ数を追加
	m_mesh_num = mesh_num;

	for (UINT i = 0; i < mesh_num;i++) {

		// メッシュを取得
		FbxMesh * p_mesh = mp_fbx_scene->GetSrcObject<FbxMesh>(i);
		
		// 頂点データ生成
		m_mesh_data_list.emplace_back();

		// インデックス読み込み
		LoadIndeces(
			m_current_index_num,
			m_mesh_data_list,
			p_mesh
		);

		// 頂点情報読み込み
		LoadVertexInfo(
			vertex_list,
			p_mesh,
			m_mesh_data_list
		);

		// uv情報読み込み
		LoadUvInfo(
			uv_list,
			m_mesh_data_list,
			p_mesh
		);

		// 法線情報読み込み
		LoadNormal(
			normal_list,
			m_mesh_data_list,
			p_mesh
		);

		// 子ノード取得
		FbxNode * p_child_node = p_root_node->GetChild(i);
		
		// マテリアル情報
		LoadMaterial(
			m_mesh_data_list,
			p_child_node,
			p_mesh
		);

		// メッシュデータのインデックスを代入
		m_mesh_data_list[i].materialIndex = i;
	}
}


void Fbx::LoadIndeces(
	int &index_num,
	std::vector<FbxMeshData>&p_mesh_data_list,
	FbxMesh*p_mesh
) {

	// ポリゴン数を取得
	int polygon_count = p_mesh->GetPolygonCount();

	UINT size = (UINT)((polygon_count * 3) * sizeof(UINT16));

	// ないなら戻す
	if (p_mesh_data_list.size() <= 0) {
		return;
	}

	FbxMeshData *p_mesh_data = &p_mesh_data_list.back();

	p_mesh_data->fvf = FVF_FBX;
	p_mesh_data->primType = D3DPT_TRIANGLELIST;
	p_mesh_data->primNum = (UINT)polygon_count;
	p_mesh_data->indexNum = (UINT)(polygon_count * 3);

	Graphics::GetInstance()->
		CreateIndexBuffer16BitSize
		(&p_mesh_data->index_buffer, size);

	UINT16 * p_indeces;

	// バッファをロックしてデータを書き込む
	p_mesh_data->index_buffer->Lock(
		0,
		size,
		(void**)&p_indeces,
		0);

	// インデックス代入
	for (int poly_idx = 0; poly_idx < polygon_count; poly_idx++) {

		p_indeces[poly_idx * 3 + 0] = poly_idx * 3 + 2;
		p_indeces[poly_idx * 3 + 1] = poly_idx * 3 + 1;
		p_indeces[poly_idx * 3 + 2] = poly_idx * 3 + 0;

		// インディシーズ代入
		m_indeces.push_back(poly_idx * 3 + 0);
		m_indeces.push_back(poly_idx * 3 + 1);
		m_indeces.push_back(poly_idx * 3 + 2);

		int size = (int)m_indeces.size() - 1;

		// 反転
		int temp = m_indeces[size - 2];

		m_indeces[(size) - 2] = m_indeces[size - 0];
		m_indeces[(size) - 1] = m_indeces[size - 1];
		m_indeces[(size) - 0] = temp;

	}

	p_mesh_data->index_buffer->Unlock();
}


void Fbx::LoadVertexInfo(
	std::vector<D3DXVECTOR3>&vertex_list,
	FbxMesh*p_mesh,
	std::vector<FbxMeshData>&mesh_data_list
	) {


	int vertexNum = p_mesh->GetPolygonVertexCount();
	UINT size = (UINT)(vertexNum * sizeof(FbxCustomVertex));

	FbxVector4* source = p_mesh->GetControlPoints();
	// メッシュのトランスフォーム
	FbxVector4 T = p_mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 R = p_mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 S = p_mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix TRS = FbxAMatrix(T, R, S);
	//	全頂点変換
	for (int v = 0; v < p_mesh->GetControlPointsCount(); v++)
	{
		source[v] = TRS.MultT(source[v]);
	}

	// メッシュデータ受け取り
	FbxMeshData *p_mesh_data = &mesh_data_list.back();
	p_mesh_data->vertexNum = (UINT)vertexNum;
	p_mesh_data->vertexStride = sizeof(FbxCustomVertex);

	// バーテックスバッファ生成
	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		CreateVertexBuffer(
			size,
			0,
			0,
			D3DPOOL_MANAGED,
			&p_mesh_data->vertex_buffer,
			NULL
		);
	
	FbxCustomVertex* pVertices;

	// バッファをロックしてデータを書き込む
	p_mesh_data->vertex_buffer->Lock(
		0,
		size,
		(void**)&pVertices,
		0
	);

	int* pIndex = p_mesh->GetPolygonVertices();
	for (int vtxIdx = 0; vtxIdx < vertexNum; vtxIdx++)
	{
		int index = pIndex[vtxIdx];
		FbxVector4& pos = source[index];

		pVertices[vtxIdx].vertex.x = -(float)pos[0];
		pVertices[vtxIdx].vertex.y = (float)pos[1];
		pVertices[vtxIdx].vertex.z = (float)pos[2];
		pVertices[vtxIdx].normal.x = 0.0f;
		pVertices[vtxIdx].normal.y = 1.0f;
		pVertices[vtxIdx].normal.z = 0.0f;
		pVertices[vtxIdx].uv.x = 0.0f;
		pVertices[vtxIdx].uv.y = 0.0f;
	}

	// アンロック
	p_mesh_data->vertex_buffer->Unlock();
}


void Fbx::LoadUvInfo(
	std::vector<D3DXVECTOR2>&uv_list,
	std::vector<FbxMeshData>&p_mesh_data_list,
	FbxMesh*p_mesh) {

	FbxStringList uv_set_name;
	p_mesh->GetUVSetNames(uv_set_name);

	FbxArray<FbxVector2>uv_sets;

	// uvの名前からuvの頂点を取り出す
	p_mesh->GetPolygonVertexUVs(
		uv_set_name.GetStringAt(0),
		uv_sets
	);

	// 現在のメッシュデータ受け取り
	FbxMeshData*p_mesh_data = &p_mesh_data_list.back();

	UINT size = p_mesh_data->vertexNum * sizeof(FbxCustomVertex);

	FbxCustomVertex*p_vertices;

	// 再度ロックして読み込み可能にする
	p_mesh_data->vertex_buffer->Lock(
		0,
		size,
		(void**)&p_vertices,
		0
	);

	for (int vertex_index = 0;
		vertex_index < uv_sets.Size();
		vertex_index++) {

		// uv座標受け取り
		FbxVector2 &uv_set = uv_sets[vertex_index];

		// 右手系なので左手系にする
		p_vertices[vertex_index].uv.x = (float)uv_set[0];
		p_vertices[vertex_index].uv.y = (float)(1.f - uv_set[1]);
		
	}

	// アンロック
	p_mesh_data->vertex_buffer->Unlock();
}


void Fbx::LoadNormal(
	std::vector<D3DXVECTOR3>&normal_list,
	std::vector<FbxMeshData>&p_mesh_data_list,
	FbxMesh*p_mesh
) {

	// 現在のメッシュデータ受け取り
	FbxMeshData*p_mesh_data = &p_mesh_data_list.back();

	// 法線情報受け取り用
	FbxArray<FbxVector4>normals;

	p_mesh->GetPolygonVertexNormals(normals);

	// 頂点分のCustomVertexサイズを取り出す
	UINT size = p_mesh_data->vertexNum * sizeof(FbxCustomVertex);

	IDirect3DVertexBuffer9* p_vertex_buffer = p_mesh_data->vertex_buffer;

	FbxCustomVertex * p_vertices;

	// バッファをロックしてデータを書き込む
	p_vertex_buffer->Lock(0, size, (void**)&p_vertices, 0);

	for (int index = 0; index < normals.Size(); index++) {

		FbxVector4&normal = normals[index];

		// それぞれxyz代入
		p_vertices[index].normal.x = -(float)normal[0];
		p_vertices[index].normal.y = -(float)normal[1];
		p_vertices[index].normal.z = -(float)normal[2];
	}

	p_vertex_buffer->Unlock();
}


void Fbx::LoadMaterial(
	std::vector<FbxMeshData>&p_vertex_data_list,
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
			LoadTexture(
				p_mesh,
				&p_vertex_data_list.back().material_info);
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


bool Fbx::LoadTexture(
	FbxMesh*p_mesh,
	MaterialInfo*p_material_info
)
{
	// エレメント受け取り
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

	if (p_material == nullptr){
		return false;
	}

	// ディフューズプロパティを検索
	FbxProperty fbx_property = 
		p_material->FindProperty(FbxSurfaceMaterial::sDiffuse);

	// テクスチャ数を取得
	int file_texture_count =
		fbx_property.GetSrcObjectCount<FbxFileTexture>();

	std::string file_name;

	// テクスチャが存在しないなら失敗
	if (file_texture_count > 0) {

		FbxFileTexture*p_file_tex = fbx_property.GetSrcObject
			<FbxFileTexture>(0);
		file_name = p_file_tex->GetFileName();
	}
	else {
		int num_layer = fbx_property.GetSrcObjectCount
			<FbxLayeredTexture>();
		if (num_layer > 0) {

			// レイヤー
			FbxLayeredTexture*p_layer_tex =
				fbx_property.GetSrcObject<FbxLayeredTexture>(0);

			// ファイルテックス
			FbxFileTexture*p_file_tex = 
				p_layer_tex->GetSrcObject<FbxFileTexture>(0);
			file_name = p_file_tex->GetFileName();
		}
	}

	if ((int)file_name.size() <= 0)
	{ 
		return false; 
	}

	//	テクスチャーの取得
	char* p_file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(file_name.c_str(), p_file_name, &size);

	// PSDが使用されている場合は読めないのでTGAで試す
	char* ext = (char*)strstr(p_file_name, ".psd");
	if (ext)
	{
		strcpy_s(ext,5,".tga");
	}


	char path[MAX_PATH];
	// ファイルパスが絶対パスなら
	if ( p_file_name = strstr(p_file_name, "\\"))
	{
		strcpy_s(path, p_file_name);
	}
	// ファイルパスが相対パスなら
	else
	{
		strcpy_s(path, m_root_path);
		strcat_s(path, "/texture/");
		strcat_s(path, p_file_name);
	}
	
	// ファイルパス分割
	std::vector<std::string>string_list;
	string_list = Utility::SplitStr('\\', p_file_name);


	// 追加
	//strcat_s(path, file_name.c_str());

	// テクスチャ読み込み
	TextureManager::GetInstance()->
		Load(path, file_name.c_str());

	// テクスチャ登録名を保存
	p_material_info->texture_name = file_name;

	//FbxFree(p_file_name);

	return false;
}


void Fbx::LoadAnimationFrame(
	FbxMesh*p_mesh,
	FbxScene*fbx_scene
) {
	int anim_stack_number = 0;
	
	// 文字列格納配列
	FbxArray<FbxString*>take_name_ary;
	// テイク名を受け取り
	fbx_scene->FillAnimStackNameArray(take_name_ary);
	// テイク数取得
	int take_num = take_name_ary.GetCount();


	// アニメーション(1コマ)が実行されていく時間情報を保持する
	m_frame_time.SetTime(
		0,
		0,
		0,
		1,
		0,
		mp_fbx_scene->GetGlobalSettings().GetTimeMode()
	);

	FbxTime start;
	FbxTime stop;
	bool is_take_exist = false;

	for (int i = 0; i < take_num; ++i) {

		// テイク名からテイク情報を取得
		FbxTakeInfo*p_current_take_info
			= fbx_scene->GetTakeInfo(*(take_name_ary[i]));

		if (p_current_take_info != nullptr) {

			// 最初と最後のフレーム数を取得
			start = p_current_take_info->mLocalTimeSpan.GetStart();
			stop = p_current_take_info->mLocalTimeSpan.GetStop();
			is_take_exist = true;
			break;
		}
	}

	// 1フレーム時間(period)で割ればフレーム数になります
	int start_frame = (int)(start.Get() / m_frame_time.Get());
	int stop_frame = (int)(stop.Get() / m_frame_time.Get());

	// アニメーション情報を探して保存
	FbxAnimStack * animation_stack =  
		fbx_scene->FindMember<FbxAnimStack>
		(take_name_ary[0]->Buffer);

	// メモリ解放
	FbxFree(take_name_ary);

}


void Fbx::LoadBone(FbxMesh*p_mesh) {

	// クラスター
	FbxCluster*p_cluster;

	// スキンの数を取得
	int skin_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < skin_count; i++) {

		// i番目のスキンを取得
		FbxSkin*p_skin = (FbxSkin*)p_mesh->GetDeformer(i, FbxDeformer::eSkin);

		// クラスタの数を取得
		int cluster_num = p_skin->GetClusterCount();

		// クラスタ取得
		for (int j = 0; j < cluster_num; j++) {
			p_cluster = p_skin->GetCluster(j);
		}
	}

	// 初期行列受け取り
	FbxAMatrix init_mat;
	p_cluster->GetTransformMatrix(init_mat);

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

	fbxsdk::FbxGeometryConverter converter(mp_manager);
	// マテリアルと1メッシュ毎に分ける
	converter.SplitMeshesPerMaterial(mp_fbx_scene, true);
	// 全てを三角形にする
	converter.Triangulate(mp_fbx_scene, true);
}


bool Fbx::IndexBufferCreate(
	int total_face_num,
	std::vector<INT>indices,
	LPDIRECT3DINDEXBUFFER9 * buffer) {

	// 頂点32bit
	int * index_buffer;

	// 32bitサイズのインデックスバッファを作成
	mp_graphics->CreateIndexBuffer32BitSize(
		buffer,
		(total_face_num * sizeof(int))
	);

	// ロック
	index_buffer = mp_graphics->LockIndexBuffer32BitSize(
		buffer
	);

	// nullチェック
	if (index_buffer == nullptr) {
		return false;
	}

	// 面数
	for (unsigned int i = 0; i < indices.size(); i++) {
		index_buffer[i] = indices[i];
	}

	// アンロック
	(*buffer)->Unlock();

	return true;
}


bool Fbx::VertexBufferCreate(
	int total_vertex_num,
	LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
) {
	
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
		p_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	FbxCustomVertex *custom_vertex_list;

	// ロック
	(*p_vertex_buffer)->Lock(
		0,
		total_vertex_num * sizeof(FbxCustomVertex),
		(void**)&custom_vertex_list,
		0
	);

	// ここで頂点情報を代入させる
	int count = 0;

	const int OFFSET = 1;

	//for (unsigned int i = 0; i < m_custom_vertex_list.size(); i++) {
	//	custom_vertex_list[i] = m_custom_vertex_list[i];
	//}

	// アンロック
	(*p_vertex_buffer)->Unlock();

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


void Fbx::SetRootPath(const char*p_file_name) {

	strcpy_s(m_root_path, p_file_name);
	int i;
	for (i = strlen(m_root_path); 0 < i; i--)
	{
		if (m_root_path[i] == '/') {
			break; 
		}
	}

	std::vector<std::string>fbx_file_name;

	fbx_file_name = Utility::SplitStr('/', p_file_name);

	// fbxのファイル名を受け取り
	m_fbx_file_name = fbx_file_name.back();

	m_root_path[i] = '\0';
}


void Fbx::Release() {

	mp_manager->Destroy();
}


std::string Fbx::GetUTF8Path(const std::string& path)
{
	// 相対パス → 絶対パス
	char fullPath[_MAX_PATH];
	_fullpath(fullPath, path.c_str(), _MAX_PATH);

	// cp932 → UTF8
	char* path_utf8;
	FbxAnsiToUTF8(fullPath, path_utf8);

	// char* → std::string
	std::string coverted_path(path_utf8);
	// FBX SDK内部で確保されたメモリは専用の関数で解放
	FbxFree(path_utf8);

	return coverted_path;
}
