#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Graphics/Graphics.h"
#include"../../VertexBuffer/VertexBuffer.h"




Fbx::Fbx() {

	// グラフィックス受け取り
	mp_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	mp_manager = FbxManager::Create();

	// fbxシーンの作成
	mp_fbx_scene = FbxScene::Create(mp_manager, "");

	// ボーン数
	m_bone_num = 0;

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


	// スキニング
	Skinning();

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

	// モーション読み込み
	LoadMotion(mp_fbx_scene);

	// メッシュ数を追加
	m_mesh_num = mesh_num;

	// メッシュ数分回す
	for (UINT i = 0; i < mesh_num;i++) {

		// メッシュを取得
		FbxMesh * p_mesh = mp_fbx_scene->GetSrcObject<FbxMesh>(i);
		
		// 頂点データ生成
		m_mesh_data_list.emplace_back();

		// インデックス読み込み
		LoadIndeces(
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

			// ボーン情報
		LoadBone(
			&m_mesh_data_list.back(),
			p_mesh
		);

		// メッシュデータのインデックスを代入
		m_mesh_data_list[i].materialIndex = i;

		AnimationCustomVertex*p_vertics = nullptr;

		// サイズ設定
		UINT size =
			(UINT)(m_mesh_data_list.back().vertexNum *
				sizeof(AnimationCustomVertex));

		// バッファをロックしてデータを書き込む
		m_mesh_data_list[i].vertex_buffer->Lock(
		0,size,(void**)&p_vertics,0);

		// 頂点追加
		m_p_vertics.emplace_back(new AnimationCustomVertex[size]);

		// メモリコピー
		memcpy(m_p_vertics[i], p_vertics, size);

		m_mesh_data_list[i].vertex_buffer->Unlock();
	}

	// モーションセット
	SetMotion("default");
}


void Fbx::SetMotion(std::string name)
{
	m_motion_name = name;
	m_frame = 0.0f;
}

void Fbx::Animate(float sec)
{
	//	モーション時間の更新
	m_frame += sec * 60.0f;

	//	ループチェック
	if (m_frame >= m_motion[m_motion_name].frame_num - 1)
	{
		// ループ
		m_frame = 0;
	}
}

//	ボーン行列の補間
static void MatrixInterporate(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}


void Fbx::Skinning() {

	// モーションがないならば
	if (m_motion.empty() == true) {
		return;
	}

	MotionData * p_motion = &m_motion[m_motion_name];

	// null
	if (p_motion == nullptr) {
		return;
	}

	// フレームがない
	if (p_motion->frame_num < 0) {
		return;
	}

	float frame = m_frame;
	
	// 配列用変数
	int f = (int)frame;

	// 行列準備
	D3DXMATRIX key_matrix[256];

	for (INT b = 0; b < (INT)m_bone_num; b++) {

		// キーフレームが入っていないならば
		if (p_motion->key_list[b].empty() == true) {
			continue;
		}

		// 行列補間
		D3DXMATRIX m;

		MatrixInterporate(
			m,
			p_motion->key_list[b][f],
			p_motion->key_list[b][f + 1],
			frame - (int)frame
		);
		
		// キーフレーム
		key_matrix[b] = m_bone_list[b].offset * m;
	}

	// 頂点変形
	for (UINT mesh_index = 0; mesh_index < m_mesh_num; mesh_index++) {

		// メッシュデータ受け取り
		FbxMeshData*p_mesh_data = &m_mesh_data_list[mesh_index];

		// 頂点データ受け取り
		AnimationCustomVertex*p_src_vertics = m_p_vertics[mesh_index];

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)(p_mesh_data->vertexNum *
				sizeof(AnimationCustomVertex));

		AnimationCustomVertex * p_vertics;

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mesh_index].vertex_buffer->Lock(
			0, size, (void**)&p_vertics, 0
		);

		for (UINT v = 0; v < p_mesh_data->vertexNum; v++) {


			// 頂点 * ボーン行列
			// b = v番目の頂点の影響ボーン[n]
			if (p_vertics[v].weight[0] <= 0.f) {
				continue;
			}

			p_vertics[v].vertex.x = 0.f;
			p_vertics[v].vertex.y = 0.f;
			p_vertics[v].vertex.z = 0.f;

			// 影響個数文ループ
			for (int n = 0; n < 4; n++) {

				// 重さがないなら頂点変更なし
				if (p_vertics[v].weight[n] <= 0) {
					break;
				}

				// ボーンインデックス
				int b = (int)p_vertics[v].bone_index[n];

				// 現在の頂点情報を位置として受け取る
				float x = p_src_vertics[v].vertex.x;
				float y = p_src_vertics[v].vertex.y;
				float z = p_src_vertics[v].vertex.z;

				// 座標を影響力分移動
				// 頂点x * 1行1列番目の行列...
				p_vertics[v].vertex.x 
					+= (x * key_matrix[b]._11 + y * key_matrix[b]._21 + z * key_matrix[b]._31 + 1 * key_matrix[b]._41)*p_vertics[v].weight[n];
				p_vertics[v].vertex.y 
					+= (x * key_matrix[b]._12 + y * key_matrix[b]._22 + z * key_matrix[b]._32 + 1 * key_matrix[b]._42)*p_vertics[v].weight[n];
				p_vertics[v].vertex.z
					+= (x * key_matrix[b]._13 + y * key_matrix[b]._23 + z * key_matrix[b]._33 + 1 * key_matrix[b]._43)*p_vertics[v].weight[n];

				// 法線情報受け取り
				float nx = p_src_vertics[v].normal.x;
				float ny = p_src_vertics[v].normal.y;
				float nz = p_src_vertics[v].normal.z;

				// 法線を影響力分変換
				p_vertics[v].normal.x
					+= (nx * key_matrix[b]._11 + ny * key_matrix[b]._21 + nz * key_matrix[b]._31)*p_vertics[v].weight[n];
				p_vertics[v].normal.y
					+= (nx * key_matrix[b]._12 + ny * key_matrix[b]._22 + nz * key_matrix[b]._32)*p_vertics[v].weight[n];
				p_vertics[v].normal.z
					+= (nx * key_matrix[b]._13 + ny * key_matrix[b]._23 + nz * key_matrix[b]._33)*p_vertics[v].weight[n];

			}
		}

		// アンロック、描画設定
		p_mesh_data->vertex_buffer->Unlock();
	}
}


void Fbx::LoadIndeces(
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
	}

	p_mesh_data->index_buffer->Unlock();
}


void Fbx::LoadVertexInfo(
	std::vector<D3DXVECTOR3>&vertex_list,
	FbxMesh*p_mesh,
	std::vector<FbxMeshData>&mesh_data_list
	) {


	int vertexNum = p_mesh->GetPolygonVertexCount();

	// 全ての頂点サイズ(数)を出力
	UINT size = (UINT)(vertexNum * sizeof(AnimationCustomVertex));

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
	p_mesh_data->vertexStride = sizeof(AnimationCustomVertex);

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
	
	AnimationCustomVertex* pVertices;

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
		// インデックス受け取り
		int index = pIndex[vtxIdx];

		// 位置受け取り
		FbxVector4& pos = source[index];

		// 位置
		pVertices[vtxIdx].vertex.x = -(float)pos[0];
		pVertices[vtxIdx].vertex.y = (float)pos[1];
		pVertices[vtxIdx].vertex.z = (float)pos[2];

		// 法線
		pVertices[vtxIdx].normal.x = 0.0f;
		pVertices[vtxIdx].normal.y = 1.0f;
		pVertices[vtxIdx].normal.z = 0.0f;

		// テクセル
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

	UINT size = p_mesh_data->vertexNum * sizeof(AnimationCustomVertex);

	AnimationCustomVertex*p_vertices;

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
	UINT size = p_mesh_data->vertexNum * sizeof(AnimationCustomVertex);

	IDirect3DVertexBuffer9* p_vertex_buffer = p_mesh_data->vertex_buffer;

	AnimationCustomVertex * p_vertices;

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
	int material_num = p_node->GetMaterialCount();

	if (material_num == 0) {
		return;
	}

	for (int i = 0; i < material_num; i++) {

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

	// テクスチャ読み込み
	TextureManager::GetInstance()->
		Load(path, file_name.c_str());

	// テクスチャ登録名を保存
	p_material_info->texture_name = file_name;

	return false;
}


void Fbx::LoadAnimationFrame(
	FbxTime*m_frame_time,
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
	m_frame_time->SetTime(
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

	for (int i = 0; i < take_num; ++i){

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
	int start_frame = (int)(start.Get() / m_frame_time->Get());
	int stop_frame = (int)(stop.Get() / m_frame_time->Get());

	// メモリ解放
	FbxFree(take_name_ary);
	
}


void Fbx::LoadBone(
	FbxMeshData*p_fbx_mesh_data,
	FbxMesh*p_mesh
) {

	if (p_fbx_mesh_data == nullptr) {
		return;
	}

	if (p_mesh == nullptr) {
		return;
	}

	// クラスター
	FbxCluster*p_cluster = nullptr;

	// ボーン数初期化
	int bone_num = 0;

	// スキン
	FbxSkin*p_skin = nullptr;

	// アニメーションカスタムバーテックス
	AnimationCustomVertex *p_mesh_vertics = nullptr;

	// 頂点受け取り
	int vertex_num = p_fbx_mesh_data->vertexNum;

	// スキンの数を取得
	int skin_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	// バッファサイズ受け取り
	UINT size = (UINT)(vertex_num * sizeof(AnimationCustomVertex));

	// バーテックスバッファ受け取り
	IDirect3DVertexBuffer9*p_vertex_buffer = p_fbx_mesh_data->vertex_buffer;

	// 書き込み可能状態にする
	p_vertex_buffer->Lock(0, size, (void**)&p_mesh_vertics, 0);


	// スキンの数分回す
	for (int i = 0; i < skin_count; i++) {

		// i番目のスキンを取得
		 p_skin = (FbxSkin*)p_mesh->GetDeformer(i, FbxDeformer::eSkin);

		// クラスタの数を取得(ボーン数)
		bone_num = p_skin->GetClusterCount();
	}

	if (p_skin == nullptr) {
		return;
	}


	// 全てのボーン情報取得
	for (int bone = 0; bone < bone_num; bone++) {

		// それぞれのボーンクラスタ取得
		p_cluster = p_skin->GetCluster(bone);

		// 行列
		FbxAMatrix trans;

		// ボーン情報取得
		p_cluster->GetTransformMatrix(trans);

		// 右手系を左手系に反転
		trans.mData[0][1] *= -1;
		trans.mData[0][2] *= -1;
		trans.mData[1][0] *= -1;
		trans.mData[2][0] *= -1;
		trans.mData[3][0] *= -1;

		// ボーンデータ
		BoneData * p_bone;

		// ボーン名取得
		const char*p_name = p_cluster->GetLink()->GetName();

		int bone_num = FindBone(p_name);

		// 登録されているボーン名が一致していないならば
		if (bone_num != -1) {
			p_bone = &m_bone_list[bone_num];
		}
		else {

			// ボーンを増やす
			bone_num = m_bone_num;
			p_bone = &m_bone_list[bone_num];
			m_bone_num++;

			// ボーン名を本体にコピー
			strcpy_s(p_bone->name, p_name);

			// オフセット行列作成
			FbxAMatrix link_matrix;

			// ボーン初期行列受け取り
			p_cluster->GetTransformLinkMatrix(link_matrix);

			// 反転
			link_matrix.mData[0][1] *= -1;
			link_matrix.mData[0][2] *= -1;
			link_matrix.mData[1][0] *= -1;
			link_matrix.mData[2][0] *= -1;
			link_matrix.mData[3][0] *= -1;

			// ボーンオフセット行列作成(逆行列 * 変換行列)
			FbxAMatrix offset = link_matrix.Inverse() * trans;
			FbxDouble * offset_mat = (FbxDouble*)offset;

			// オフセット行列保存
			for (int matrix = 0; matrix < 16; matrix++) {

				p_bone->offset.m[matrix / 4][matrix % 4] =
					(float)offset_mat[matrix];
			}

			// キーフレーム読み込み
			LoadKeyFrame("default", bone_num, p_cluster->GetLink());
		}


		// クラスターがないならボーンもウェイトもない
		if (p_cluster == nullptr) {
			return;
		}

		// ウェイト読み込み
		int weight_num = p_cluster->GetControlPointIndicesCount();
		int * p_weight_index = p_cluster->GetControlPointIndices();
		double * weight_list = p_cluster->GetControlPointWeights();

		// ポリゴンインデックス数受け取り
		int *index = p_mesh->GetPolygonVertices();


		// それぞれの重さの分だけ回す
		for (int i = 0; i < weight_num; i++) {

			int weight_index2 = p_weight_index[i];

			// 重みがある頂点を探す
			for (int vertex_index = 0;
				vertex_index < vertex_num;
				vertex_index++) {

				if (index[vertex_index] != weight_index2) {
					continue;
				}

				int weight_count = 0;
				// 頂点にウェイト保存
				for (weight_count = 0;
					weight_count < 4;
					weight_count++
					) {

					// 重みがない場所から保存
					if (p_mesh_vertics[vertex_index].weight[weight_count]
						<= 0.f)
					{
						break;
					}

				}

				// 重みの数が4つ以上あるなら戻す
				if (4 <= weight_count) {
					continue;
				}

				// 重みを追加
				p_mesh_vertics[vertex_index].bone_index[weight_count]
					= bone_num;
				p_mesh_vertics[vertex_index].weight[weight_count]
					= (float)weight_list[i];
			}
		}

	}

	// ウェイトを正規化
	for (int vetex_index = 0;
		vetex_index < vertex_num;
		vetex_index++) {

		// 正規化
		float n = 0;
		int weight_count;

		// 頂点のウェイトの合計値
		for (weight_count = 0; weight_count < 4; weight_count++) {

			if (p_mesh_vertics[vetex_index].weight[weight_count]
				<= 0.f) {
				break;
			}
			n += p_mesh_vertics[vetex_index].weight[weight_count];
		}

		// 正規化
		for (weight_count = 0; weight_count < 4; weight_count++) {
			p_mesh_vertics[vetex_index].weight[weight_count] /= n;
		}
	}
	

	// 書き込み付加
	p_fbx_mesh_data->vertex_buffer->Unlock();
}


void Fbx::LoadKeyFrame(
	std::string name,
	int bone,
	FbxNode*p_bone_node
) {

	// キーフレーム読み込み

	// メモリ確保
	MotionData * p_motion = &m_motion[name];
	p_motion->key_list[bone].resize(p_motion->frame_num);

	// キーフレーム時間割り出し(0.016)
	double time = (double)m_start_frame * (1.f / 60);

	FbxTime fbx_time;

	// モーションのフレーム数分回す
	for (UINT f = 0; f < p_motion->frame_num; f++) {

		// doubleの秒を受け取り
		fbx_time.SetSecondDouble(time);

		// t秒の姿勢行列を取得
		FbxMatrix m = p_bone_node->EvaluateGlobalTransform(fbx_time);

		// 行列について調べる
		m.mData[0][1] *= -1;// _12
		m.mData[0][2] *= -1;// _13
		m.mData[1][0] *= -1;// _21
		m.mData[2][0] *= -1;// _31
		m.mData[3][0] *= -1;// _41

		FbxDouble * mat = (FbxDouble*)m;

		for (int i = 0; i < 16; i++) {
			p_motion->key_list[bone][f].m[i / 4][i % 4]
				= (float)mat[i];
		}

		time += 1.0 / 60.0;
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
		(sizeof(AnimationCustomVertex) * total_vertex_num),
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
	AnimationCustomVertex *custom_vertex_list;

	// ロック
	(*p_vertex_buffer)->Lock(
		0,
		total_vertex_num * sizeof(AnimationCustomVertex),
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
	//m_fbx_file_name = fbx_file_name.back();

	m_root_path[i] = '\0';
}


void Fbx::LoadMotion(
	FbxScene*p_scene
) {

	if (p_scene == nullptr) {
		return;
	}

	FbxArray<FbxString*>names;

	// 利用するアニメーションを選び設定を行う
	p_scene->FillAnimStackNameArray(names);

	int start_frame;

	if (names != nullptr) {

		// モーションが存在する時
		FbxTakeInfo*p_take = p_scene->GetTakeInfo(names[0]->Buffer());

		// アニメーション(1コマ)が実行されていく時間情報を保持する
		FbxLongLong start = p_take->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = p_take->mLocalTimeSpan.GetStop().Get();
		FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		start_frame = (int)(start / fps60);

		// モーションのフレーム値を代入
		m_motion["default"].frame_num =
			(int)((stop - start) / fps60);
	}

	m_start_frame = start_frame;
}


int Fbx::FindBone(const char*p_name) {

	for (INT bone_index = 0; bone_index < m_bone_num; bone_index++) {

		// 文字列が一致しているならば
		if (strcmp(m_bone_list[bone_index].name, p_name)) {
			return (int)bone_index;
		}
	}

	return -1;
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
