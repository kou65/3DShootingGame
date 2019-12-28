#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Graphics/Graphics.h"
#include"../../VertexBuffer/VertexBuffer.h"
#include"../../../D3DFont/D3DFont.h"



Fbx::Fbx() {

	// グラフィックス受け取り
	mp_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	m_fbx_mod.mp_manager = FbxManager::Create();

	// fbxシーンの作成
	m_fbx_mod.mp_fbx_scene = FbxScene::Create(m_fbx_mod.mp_manager, "");

	// ボーン数
	m_bone_num = 0;

	// nullチェック
	{
		if (m_fbx_mod.mp_manager == NULL) {
			Window::TextMessageBox("FBXManagerの生成に失敗しました");
		}

		if (m_fbx_mod.mp_fbx_scene == NULL) {
			Window::TextMessageBox("FBXSceneの作成に失敗しました");
		}
	}
}


void Fbx::Draw(TextureData*td) {


	// ライトをオンにするかどうか
	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 移動
	D3DXMatrixTranslation(&mat, 0.f, 0.f, 0.f);

	// ワールド座標変換
	mp_graphics->GetInstance()->GetLpDirect3DDevice9()
		->SetTransform(D3DTS_WORLD, &mat);

	// デバイス
	IDirect3DDevice9* pDevice =
		Graphics::GetInstance()->GetLpDirect3DDevice9();


	// スキニング
	AttitudeSkinning();

	// メッシュインデックスを回す
	for (UINT meshIdx = 0; meshIdx < m_mesh_num; meshIdx++)
	{

		MaterialInfo* mate_info = 
			&m_mesh_data_list[meshIdx].material_info;

		if (td != nullptr) {

			pDevice->SetTexture(
				0,
				td->p_texture_buffer
			);
		}
		// テクスチャーの設定
		else if (TextureManager::GetInstance()->
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

		if (meshIdx != p_mesh_data->material_index)
		{
			continue;
		}
		

		// 頂点バッファの設定
		pDevice->SetStreamSource(
			0,
			p_mesh_data->vertex_buffer,
			0,
			p_mesh_data->vertex_stride
		);

		// 頂点フォーマットの指定
		pDevice->SetFVF(p_mesh_data->fvf);

		// インデックス数
		if (p_mesh_data->index_num)
		{
			// インデックスバッファの設定
			pDevice->SetIndices(
				p_mesh_data->index_buffer
			);

			// 描画
			pDevice->DrawIndexedPrimitive(
				p_mesh_data->prim_type,
				0,
				0,
				p_mesh_data->vertex_num,
				0,
				p_mesh_data->prim_num
			);
		}
	}

	// アニメーションデバッグ
	D3DFont::Draw(500.f, 20.f, 200,"anim_num=>%f", m_frame);
}


bool Fbx::Load(
	const std::string&fbx_file_path
	) {

	LoadCurrentPath(fbx_file_path);

	// fbxインポータの作成
	m_fbx_mod.mp_importer = FbxImporter::Create
	(m_fbx_mod.mp_manager, "");


	// nullチェック
	if (m_fbx_mod.mp_importer == NULL) {
		Window::TextMessageBox("FBXImporterの生成に失敗しました");
	}

	// インポータの初期化
	if (!m_fbx_mod.mp_importer->Initialize(fbx_file_path.c_str())) {

		Window::TextMessageBox("FBXimporter初期化失敗");
		return false;
	}

	// ファイルからシーンを読み込む
	if (!m_fbx_mod.mp_importer->Import(m_fbx_mod.mp_fbx_scene)) {

		Window::TextMessageBox("シーン読み込みに失敗");
		return false;
	}

	
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
	FbxNode * p_root_node = m_fbx_mod.mp_fbx_scene->GetRootNode();

	// メッシュ分のバッファを確保
	UINT mesh_num = (UINT)m_fbx_mod.mp_fbx_scene->GetSrcObjectCount<FbxMesh>();
	// マテリアル分のバッファを確保
	UINT materialNum = mesh_num;

	// ポリゴン変換
	Polygon3Convert();

	SelectAnimation(1);

	SetAnimation(1);

	// モーション読み込み
	LoadMotion(m_fbx_mod.mp_fbx_scene);

	// メッシュ数を追加
	m_mesh_num = mesh_num;

	// メッシュ数分回す
	for (UINT i = 0; i < mesh_num;i++) {

		// メッシュを取得
		FbxMesh * p_mesh = 
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(i);

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

		// カラー情報読み込み
		LoadColor(
			m_mesh_data_list,
			p_mesh
		);

		// 子ノード取得
		//FbxNode * p_child_node = p_root_node->GetChild(i);
		
		// マテリアル情報
		LoadMaterial(
			m_mesh_data_list,
			//p_child_node,
			p_mesh
		);

		// ボーン情報
		LoadBone(
			&m_mesh_data_list.back(),
			p_mesh
		);

		// モデル情報読み込み
		LoadModelInfo(
			&m_mesh_data_list.back(),
			p_mesh
		);

		// ローカル座標を受け取り
		LoadLocalMatrix(
			p_mesh
		);

		// メッシュデータのインデックスを代入
		m_mesh_data_list[i].material_index = i;

		AnimationCustomVertex*p_vertics = nullptr;

		// サイズ設定
		UINT size =
			(UINT)(m_mesh_data_list.back().vertex_num *
				sizeof(AnimationCustomVertex));

		// バッファをロックしてデータを書き込む
		m_mesh_data_list[i].vertex_buffer->Lock(
		0,size,(void**)&p_vertics,0);

		// 頂点追加
		m_p_vertics.emplace_back(new AnimationCustomVertex[size]);

		// メモリコピー
		memcpy(m_p_vertics[i], p_vertics, size);

		if (p_vertics == nullptr) {
			return;
		}

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


void Fbx::Animate(float sec,float reset_frame)
{
	//	モーション時間の更新
	m_frame += sec / 60.0f;

	if (m_frame >= (m_stop_frame + reset_frame)) {
		m_frame = m_start_frame + m_frame - m_stop_frame;
	}

	//	ループチェック
	//if (m_frame >= m_motion[m_motion_name].frame_num - 1)
	//{
	//	// ループ
	//	m_frame = 0;
	//}

}


//	ボーン行列の補間
static void MatrixInterporate(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}


void Fbx::LoadIndeces(
	std::vector<FbxMeshData>&p_mesh_data_list,
	FbxMesh*p_mesh
) {

	// ポリゴン数を取得
	int polygon_count = p_mesh->GetPolygonCount();

	UINT size = (UINT)((polygon_count * 3) * sizeof(UINT32));

	// ないなら戻す
	if (p_mesh_data_list.size() <= 0) {
		return;
	}

	FbxMeshData *p_mesh_data = &p_mesh_data_list.back();

	p_mesh_data->fvf = FVF_FBX;
	p_mesh_data->prim_type = D3DPT_TRIANGLELIST;
	p_mesh_data->prim_num = (UINT)polygon_count;
	p_mesh_data->index_num = (UINT)(polygon_count * 3);


	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		// インデックスバッファ作成
		CreateIndexBuffer(
			// インデックスバッファのサイズをバイト単位で指定
		(size),
			// 頂点バッファをどのように使用するか
			D3DUSAGE_WRITEONLY,
			// 一つのインデックスバッファのサイズをフラグで表す
			D3DFMT_INDEX32,
			// 頂点インデックスをどのメモリに置くか指定
			D3DPOOL_MANAGED,
			// IDirect3DIndexBuffer9インターフェースが返る
			&p_mesh_data->index_buffer,
			// 現在使用されていないので基本NULL
			NULL
		);

	// nullチェック
	if (p_mesh_data->index_buffer == nullptr) {
		return;
	}

	UINT32 * p_indeces;

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

	// メッシュデータ受け取り
	FbxMeshData *p_mesh_data = &mesh_data_list.back();
	p_mesh_data->vertex_num = (UINT)vertexNum;
	p_mesh_data->vertex_stride = sizeof(AnimationCustomVertex);

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

	int* p_index = p_mesh->GetPolygonVertices();
	
	for (int vtxIdx = 0; vtxIdx < vertexNum; vtxIdx++)
	{
		// インデックス受け取り
		int index = p_index[vtxIdx];

		// もう一つの受け取り方
		FbxVector4 pos = p_mesh->GetControlPointAt(p_index[vtxIdx]);

		// 位置
		pVertices[vtxIdx].vertex.x = -(float)pos[0];
		pVertices[vtxIdx].vertex.y = (float)pos[1];
		pVertices[vtxIdx].vertex.z = (float)pos[2];

		D3DXVECTOR3 vec(-(float)pos[0], (float)pos[1], (float)pos[2]);

		// 法線
		pVertices[vtxIdx].normal.x = 0.0f;
		pVertices[vtxIdx].normal.y = 1.0f;
		pVertices[vtxIdx].normal.z = 0.0f;

		// テクセル
		pVertices[vtxIdx].uv.x = 0.0f;
		pVertices[vtxIdx].uv.y = 0.0f;

		// 頂点カラー
		pVertices[vtxIdx].color = 100;
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

	UINT size = p_mesh_data->vertex_num * sizeof(AnimationCustomVertex);

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
	UINT size = p_mesh_data->vertex_num * sizeof(AnimationCustomVertex);

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
	FbxMesh*p_mesh
) {

	bool can_get_texture = false;

	FbxNode*p_node = p_mesh->GetNode();

	// マテリアルの数
	int material_num = p_node->GetMaterialCount();


	if (material_num == 0) {

		// テクスチャ読み込み
		LoadTexture(
			p_mesh,
			&p_vertex_data_list.back().material_info
		);

		// エントリー用のテクスチャ読み込み
		LoadEntryTexture(
			p_mesh,
			&p_vertex_data_list.back().material_info
		);


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


			LoadEntryTexture(
				p_mesh,
				&p_vertex_data_list.back().material_info
			);
		}
	}
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

		// レイヤー数
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

	// ルートパス
	char root_path[MAX_PATH];

	// 現在のパス
	char path[MAX_PATH];

	// ファイルパスが絶対パスなら
	if (strstr(p_file_name, "\\"))
	{
		strcpy_s(path, p_file_name);
	}
	// ファイルパスが相対パスなら
	else
	{
		if (p_file_name == nullptr) {
			return false;
		}

		strcpy_s(path, root_path);
		strcat_s(path, "/texture/");
		strcat_s(path, p_file_name);
	}
	
	std::string str_path = path;

	// Dルート
	if (p_file_name[0] == 'D') {
		// ファイルパス分割
		std::vector<std::string>string_list;
		string_list = Utility::SplitStr('\\', p_file_name);

		// テクスチャ登録名を保存
		str_path = 
			m_current_path + string_list.back();
	}
	

	// テクスチャ読み込み
	TextureManager::GetInstance()->
		Load(str_path.c_str(), file_name.c_str());

	// テクスチャ登録名を保存
	p_material_info->texture_name = file_name;

	return true;
}



void Fbx::LoadEntryTexture(
	FbxMesh*p_mesh,
	MaterialInfo*p_material_info
) {

	FbxNode*p_node = p_mesh->GetNode();

	int material_count = p_node->GetMaterialCount();

	std::vector<FbxSurfaceMaterial*>mte_list;

	for (int i = 0; i < material_count; i++) {
		mte_list.emplace_back(p_node->GetMaterial(i));
	}

	FbxObject *p_obj = mte_list.back();
	// インプリメンテーションのルートテーブルを取得
	const FbxImplementation *p_imple =
		GetImplementation(
			p_obj,
			FBXSDK_IMPLEMENTATION_CGFX
		);

	// ルートテーブル取得
	const FbxBindingTable*p_root_table =
		p_imple->GetRootTable();

	// ルートカウント
	int entry_count = p_root_table->GetEntryCount();

	const FbxBindingTableEntry *entry = nullptr;

	for (int i = 0; i < entry_count; i++) {

		entry = 
			&p_root_table->GetEntry(i);
	}

	const char*p_name = entry->GetSource();

	FbxProperty property =
		p_obj->RootProperty.FindHierarchical(p_name);

	int file_texture_count = 
		property.GetSrcObjectCount<FbxFileTexture>();

	std::string file_tex_file_name;

	for (int i = 0; i < file_texture_count; i++) {
		FbxFileTexture*p_file_tex =
			property.GetSrcObject<FbxFileTexture>(i);

		file_tex_file_name =
			p_file_tex->GetFileName();

		// ファイルパス分割
		std::vector<std::string>string_list;

		// カレントの種類を調べそれぞれで文字列分割
		if(strstr(file_tex_file_name.c_str(),"\\") !=NULL){

			string_list = 
				Utility::SplitStr('\\', file_tex_file_name);
		}
		else if(strstr(file_tex_file_name.c_str(), "/") != NULL) {

			string_list = 
				Utility::SplitStr('/', file_tex_file_name);
		}
		

		// テクスチャ登録名を保存
		std::string str_path =
			m_current_path + string_list.back();

		// テクスチャ読み込み
		TextureManager::GetInstance()->
			Load(str_path.c_str(), string_list.back().c_str());

		// テクスチャ登録名を保存
		p_material_info->texture_name = string_list.back();
	}

}


bool Fbx::LoadColor(
	std::vector<FbxMeshData>&p_vertex_data_list,
	FbxMesh*p_mesh
) {

	// 現在のメッシュデータを受け取り
	FbxMeshData*p_mesh_data = &p_vertex_data_list.back();

	// 属性のカラー数を受け取り
	int color_element_count = 
		p_mesh->GetElementVertexColorCount();

	// カラー情報がないならばなし
	if (color_element_count == 0) {
		return false;
	}

	// 頂点カラー受け取り
	FbxGeometryElementVertexColor*p_color = 
		p_mesh->GetElementVertexColor(0);

	// マッピングモード
	FbxLayerElement::EMappingMode map_mode = 
		p_color->GetMappingMode();

	// リファレンスモード
	FbxLayerElement::EReferenceMode ref_mode =
		p_color->GetReferenceMode();
	
	if (map_mode == FbxLayerElement::eByPolygonVertex) {

		if (ref_mode == FbxLayerElement::eIndexToDirect) {

			// バーテックスバッファ受け取り
			IDirect3DVertexBuffer9*p_vertex_buffer =
				p_mesh_data->vertex_buffer;

			AnimationCustomVertex*p_vertices;

			// バッファをロックして書き込む
			p_vertex_buffer->Lock(
				0,
				p_mesh_data->vertex_num * sizeof(AnimationCustomVertex),
				(void**)&p_vertices,
				0
			);

			FbxLayerElementArrayTemplate<FbxColor>&colors =
				p_color->GetDirectArray();

			FbxLayerElementArrayTemplate<int>&indeces =
				p_color->GetIndexArray();

			for (int i = 0; i < indeces.GetCount(); i++) {

				// fbxカラー
				FbxColor&color = colors.GetAt(indeces.GetAt(i));

				// カラー情報受け取り
				DWORD a = (DWORD)(color.mAlpha * 255.0);
				DWORD r = (DWORD)(color.mRed   * 255.0);
				DWORD g = (DWORD)(color.mGreen * 255.0);
				DWORD b = (DWORD)(color.mBlue  * 255.0);

				p_vertices[i].color =
					(a << 24) + (r << 16) + (g << 8) + (b);

				//p_vertices[i].color = 53;
			}
			p_vertex_buffer->Unlock();
		}
	}

	return true;
}



void Fbx::SelectAnimation(
	int select_anim_num
) {

	// 受け取るアニメーションを切り替え
	FbxAnimStack* pStack = m_fbx_mod.mp_fbx_scene->
		GetSrcObject<FbxAnimStack>(select_anim_num);

	m_fbx_mod.mp_fbx_scene->SetCurrentAnimationStack(pStack);
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
	int vertex_num = p_fbx_mesh_data->vertex_num;

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

	// クラスタ数保存
	m_cluster_num = bone_num;

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
			// 既存のボーンを代入
			p_bone = &m_bone_data_list[bone_num];
		}
		else {

			// ボーンを増やす
			bone_num = m_bone_num;
			p_bone = &m_bone_data_list[bone_num];
			m_bone_num++;

			// ボーン名を本体にコピー
			strcpy_s(p_bone->name, p_name);

			// オフセット行列作成
			FbxAMatrix bone_matrix;

			// ボーン初期行列受け取り
			p_cluster->GetTransformLinkMatrix(bone_matrix);

			// 反転
			bone_matrix.mData[0][1] *= -1;
			bone_matrix.mData[0][2] *= -1;
			bone_matrix.mData[1][0] *= -1;
			bone_matrix.mData[2][0] *= -1;
			bone_matrix.mData[3][0] *= -1;

			// 移動、回転、拡縮
			FbxAMatrix geo_mat = GetGeometry(p_mesh->GetNode());
			trans *= geo_mat;

			// ボーンオフセット行列作成(逆行列 * 変換行列)
			FbxAMatrix offset = bone_matrix.Inverse() * trans;
			FbxDouble * offset_mat = (FbxDouble*)offset;


			// リスト追加
			m_mat_trans_list.emplace_back();
			
			for (int matrix = 0; matrix < 16; matrix++){

				// オフセット行列保存
				p_bone->offset.m[matrix / 4][matrix % 4] = 
					(float)offset_mat[matrix];

				// ワールド行列
				m_mat_trans_list.back().m[matrix / 4][matrix % 4] =
					(float)offset_mat[matrix];
			}

			// キーフレーム読み込み
			//LoadKeyFrame("default", bone_num, p_cluster->GetLink());
		}


		// クラスターがないならボーンもウェイトもない
		if (p_cluster == nullptr) {
			return;
		}

		// 影響する頂点の数
		int weight_num = p_cluster->GetControlPointIndicesCount();

		// 頂点のインデックス
		int * p_weight_index = p_cluster->GetControlPointIndices();

		// 重み
		double * weight_list = p_cluster->GetControlPointWeights();

		// ポリゴンインデックス数受け取り
		int *index = p_mesh->GetPolygonVertices();


		// それぞれの重さの分だけ回す
		for (int i = 0; i < weight_num; i++) {

			// 重みのインデックス
			int weight_index2 = p_weight_index[i];

			// 重みがある頂点を探す
			for (int vertex_index = 0;
				vertex_index < vertex_num;
				vertex_index++){

				// 影響するインデックスを捜索
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
					if (p_mesh_vertics[vertex_index].
						weight[weight_count]
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
				p_mesh_vertics[vertex_index].
					bone_index[weight_count]
					= bone_num;

				p_mesh_vertics[vertex_index].
					weight[weight_count]
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

			// 全ての重さを加算
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



void Fbx::LoadLocalMatrix(
	FbxMesh*p_mesh
) {

	// メッシュからノードを取得
	FbxNode*p_node = p_mesh->GetNode();

	D3DXVECTOR3 vec3;
	FbxDouble3 transform[3];

	// ワールド座標変換行列取得
	transform[0] = p_node->LclTranslation.Get();
	transform[1] = p_node->LclRotation.Get();
	transform[2] = p_node->LclScaling.Get();

	m_world_trans_list.emplace_back();

	// 移動,回転,拡縮データを入れる
	for (int v = 0; v < 3; v++) {

		vec3.x = (float)transform[v][0];
		vec3.y = (float)transform[v][1];
		vec3.z = (float)transform[v][2];

		// リストに追加
		m_world_trans_list.back().emplace_back(vec3);
	}
}




void Fbx::AttitudeSkinning() {

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

	// フレーム受け取り
	float frame = m_frame;

	// 配列用変数
	int f = (int)frame;

	// 姿勢行列
	D3DXMATRIX atti_mat;

	// 行列初期化
	D3DXMatrixIdentity(&atti_mat);
	
	// 頂点変形
	for (UINT mesh_index = 0;
		mesh_index < m_mesh_num;
		mesh_index++) {

		// メッシュを取得
		FbxMesh * p_mesh = 
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(mesh_index);


		// 読み込みアニメーションフレーム姿勢
		LoadAnimFrameAttitudeMatrix(
			&atti_mat,
			p_mesh,
			m_frame
		);

		// メッシュデータ受け取り
		FbxMeshData*p_mesh_data = &m_mesh_data_list[mesh_index];

		// 頂点データ受け取り
		AnimationCustomVertex*p_src_vertics =
			m_p_vertics[mesh_index];

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)(p_mesh_data->vertex_num *
				sizeof(AnimationCustomVertex));

		// 頂点バッファを定義
		AnimationCustomVertex * p_vertics;

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mesh_index].vertex_buffer->Lock(
			0, size, (void**)&p_vertics, 0
		);

		D3DXMATRIX frame_mat[4];

		// 頂点 * 逆行列 * 座標変換行列 * 初期行列(元に戻す行列)
		for (int bn = 0; bn < 4; bn++) {
			frame_mat[bn] = 
				m_bone_data_list[bn].offset * 
				atti_mat;
		}

		// 頂点全てアニメーションを行う
		for (UINT v = 0; v < p_mesh_data->vertex_num; v++) {

			// 頂点 * ボーン行列
			// b = v番目の頂点の影響ボーン[n]
			if (p_vertics[v].weight[0] <= 0.f) {
				continue;
			}

			p_vertics[v].vertex.x = 0.f;
			p_vertics[v].vertex.y = 0.f;
			p_vertics[v].vertex.z = 0.f;

			float weight = 0.f;

			// 影響個数文ループ
			for (int n = 0; n < 4; n++) {

				weight = p_vertics[v].weight[n];

				// ボーンインデックス
				int b = (int)p_vertics[v].bone_index[n];

				// 現在の頂点情報を位置として受け取る
				float x = p_src_vertics[v].vertex.x;
				float y = p_src_vertics[v].vertex.y;
				float z = p_src_vertics[v].vertex.z;
				
				// 座標を影響力分移動
				// 頂点x * 1行1列番目の行列...
				p_vertics[v].vertex.x
					+=
					(x * frame_mat[b]._11
						+ y * frame_mat[b]._21
						+ z * frame_mat[b]._31
						+ 1 * frame_mat[b]._41)
					* weight;
				
				p_vertics[v].vertex.y
					+=
					(x * frame_mat[b]._12
						+ y * frame_mat[b]._22
						+ z * frame_mat[b]._32
						+ 1 * frame_mat[b]._42)
					* weight;
				
				p_vertics[v].vertex.z
					+= (x * frame_mat[b]._13
						+ y * frame_mat[b]._23
						+ z * frame_mat[b]._33
						+ 1 * frame_mat[b]._43)
					* weight;

			}
		}

		// アンロック、描画設定ok
		p_mesh_data->vertex_buffer->Unlock();
	}
}


void Fbx::SetAnimation(int anim_num) {

	// アニメーションフレーム数を取得する

	FbxArray<FbxString*>names;

	// 利用するアニメーションを選び設定を行う
	m_fbx_mod.mp_fbx_scene->FillAnimStackNameArray(names);

	// テイク数を取得
	int take_num = names.GetCount();

	// モーション数取得
	int motion_num = m_fbx_mod.mp_importer->GetAnimStackCount();

	// モーションがないなら返す
	if (take_num == 0 && motion_num == 0) {
		return;
	}

	// モーションが存在する時
	FbxTakeInfo*p_take = m_fbx_mod.mp_fbx_scene->
		GetTakeInfo(names[0]->Buffer());

	FbxAnimStack*stack = 
		m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxAnimStack>(0);

	// アニメーション(1コマ)が実行されていく時間情報を保持する
	FbxLongLong start = 
		p_take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = 
		p_take->mLocalTimeSpan.GetStop().Get();

	// フレーム数になる
	FbxLongLong fps60 
		= FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	//m_period.SetTime(0,0,0,1,0,m_fbx_mod.mp_fbx_scene->GetGlobalSettings().GetTimeMode());

	// 1フレーム時間(period)で割ればフレーム数になる
	m_start_frame 
		= (int)(start / fps60);

	m_stop_frame
		= (int)((stop - start) / fps60);

	m_frame = (float)m_start_frame;
}



void Fbx::LoadAnimFrameAttitudeMatrix(
	D3DXMATRIX*p_out_mat,
	FbxMesh*p_mesh,
	float frame
) {
	
	FbxNode*p_node = p_mesh->GetNode();

	// スキン取得
	FbxSkin*skin = static_cast<FbxSkin*>
		(p_mesh->GetDeformer(0, FbxDeformer::eSkin));

	// ミリ秒
	FbxTime m_period;

	FbxMatrix bone_mat;

	m_period.SetSecondDouble(frame);

	if (skin != nullptr) {

		int cluster_num = skin->GetClusterCount();

		// クラスター受け取り
		FbxCluster*p_cluster = skin->GetCluster(cluster_num - 1);

		// ボーン姿勢行列取得
		bone_mat =
			p_cluster->GetLink()->EvaluateGlobalTransform(m_period);
	}

	FbxMatrix fbx_mat;

	// 姿勢行列取得
	fbx_mat = p_node->EvaluateGlobalTransform(m_period);


	// 姿勢行列取得
	{

		// タイムモード取得
		FbxGlobalSettings&global_time_s = 
			m_fbx_mod.mp_fbx_scene->GetGlobalSettings();

		// タイムモード取得
		FbxTime::EMode time_mode = global_time_s.GetTimeMode();

	}

	// 姿勢行列 * ボーンフレーム時姿勢
	FbxMatrix total_mat = fbx_mat.Inverse() * bone_mat;

	// コンバート
	FbxMatConvertD3DMat(
		p_out_mat,
		total_mat
	);
}


void Fbx::FbxMatConvertD3DMat(
	D3DXMATRIX*p_out_mat,
	FbxMatrix &fbx_mat
) {

	if (p_out_mat == nullptr) {
		return;
	}

	// 行列について調べる
	fbx_mat.mData[0][1] *= -1;// _12
	fbx_mat.mData[0][2] *= -1;// _13
	fbx_mat.mData[1][0] *= -1;// _21
	fbx_mat.mData[2][0] *= -1;// _31
	fbx_mat.mData[3][0] *= -1;// _41

	// doubleに変換
	FbxDouble * mat = (FbxDouble*)fbx_mat;

	// オフセット行列保存
	for (int matrix = 0; matrix < 16; matrix++) {

		p_out_mat->m[matrix / 4][matrix % 4] =
			(float)mat[matrix];
	}
}


void Fbx::LoadMotion(
	FbxScene*p_scene
) {

	if (p_scene == nullptr) {
		return;
	}

	// アニメーション名一覧を格納する配列の宣言
	FbxArray<FbxString*>names;

	// シーンからアニメーション一覧を取得＆格納
	p_scene->FillAnimStackNameArray(names);

	// モーション数取得
	//m_motion_num = m_fbx_mod.mp_importer->GetAnimStackCount();

	if (names != NULL) {

		// アニメ番号0番を元にFbxTakeInfoを取得
		FbxTakeInfo*p_take = p_scene->
			GetTakeInfo(names[0]->Buffer());

		// アニメーション(1コマ)が実行されていく時間情報を保持する
		FbxLongLong start = p_take->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = p_take->mLocalTimeSpan.GetStop().Get();

		FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		m_start_frame = (int)(start / fps60);

		// モーションのフレーム値を代入
		m_motion["default"].frame_num =
			(int)((stop - start) / fps60);
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

	// メッシュを取得
	FbxMesh * p_mesh = 
		m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(0);

	// 0番目のポリゴンの頂点数を取得

	int count = p_mesh->GetPolygonSize(0);

	// ポリゴンが4以上ならば
	//if (count >= 4) {

		fbxsdk::FbxGeometryConverter converter(m_fbx_mod.mp_manager);
		// マテリアルと1メッシュ毎に分ける
		converter.SplitMeshesPerMaterial(m_fbx_mod.mp_fbx_scene, true);
		// 全てを三角形にする
		converter.Triangulate(m_fbx_mod.mp_fbx_scene, true);

		// 追加
		converter.RemoveBadPolygonsFromMeshes(
			m_fbx_mod.mp_fbx_scene
		);
	//}
}


bool Fbx::IndexBufferCreate(
	int total_face_num,
	std::vector<INT>indices,
	LPDIRECT3DINDEXBUFFER9 * buffer) {

	// 頂点32bit
	int* index_buffer;

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


int Fbx::FindBone(const char*p_name) {

	for (INT bone_index = 0; bone_index < m_bone_num; bone_index++) {

		// 文字列が一致しているならば
		if (strcmp(m_bone_data_list[bone_index].name, p_name)) {
			return (int)bone_index;
		}
	}

	return -1;
}


D3DXMATRIX Fbx::WorldTransform(
	D3DXVECTOR3 &pos,
	D3DXVECTOR3 &rot,
	D3DXVECTOR3 &scale
) {

	D3DXMATRIX matrix_world;
	D3DXMATRIX matrix_trans;
	D3DXMATRIX matrix_scale;

	D3DXMATRIX total_rot;
	D3DXMATRIX rot_x;
	D3DXMATRIX rot_y;
	D3DXMATRIX rot_z;

	// 移動
	D3DXMatrixTranslation(
		&matrix_trans,
		pos.x,
		pos.y,
		pos.z
	);

	// 拡縮
	D3DXMatrixScaling(
		&matrix_scale,
		scale.x,
		scale.y,
		scale.z
	);

	D3DXMatrixRotationX(&rot_x, rot.x);
	D3DXMatrixRotationY(&rot_y, rot.y);
	D3DXMatrixRotationZ(&rot_z, rot.z);

	total_rot =
		rot_z *
		rot_x *
		rot_y;
	
	// 行列合成 回転 * 拡縮 * 移動
	matrix_world =
		matrix_scale *
		total_rot *
		matrix_trans;

	return matrix_trans;
}


void Fbx::LoadCurrentPath(const std::string &path_name) {

	// ファイルパス分割
	std::vector<std::string>string_list;
	string_list = Utility::SplitStr('/', path_name);

	for (unsigned int i = 0; i < string_list.size() - 1; i++) {
		m_current_path += string_list[i];
	}
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

		// t秒の姿勢行列を取得(フレーム時姿勢)(ボーンフレーム行列)
		FbxMatrix m =
			p_bone_node->EvaluateGlobalTransform(fbx_time);

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


void Fbx::Release() {

	m_fbx_mod.mp_manager->Destroy();

	m_fbx_mod.mp_fbx_scene->Destroy();

	// インポータはファイルを開いてシーンクラスとの橋渡し
	// をしてくれるだけなので、ここで破棄してもいい
	// インポータの明示的な破棄
	m_fbx_mod.mp_importer->Destroy();
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


FbxAMatrix Fbx::GetGeometry(FbxNode* pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}


void Fbx::LoadModelInfo(
	FbxMeshData*p_fbx_mesh_data,
	FbxMesh*p_mesh
) {

	// スキン取得
	FbxSkin*skin = static_cast<FbxSkin*>
		(p_mesh->GetDeformer(0, FbxDeformer::eSkin));
	
	// スキンの数
	int deformer_count = 
		p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	if (skin == nullptr) {
		return;
	}

	// スキンの数
	int cluster_num = skin->GetClusterCount();

	if (cluster_num == 0) {
		return;
	}

	// クラスターの数だけ回す
	for (int bone = 0; bone < cluster_num; bone++) {

		// クラスター受け取り
		FbxCluster*p_cluster = skin->GetCluster(bone);

		// 影響する頂点の数
		int point_num = p_cluster->
			GetControlPointIndicesCount();

		// 頂点のインデックス
		int * point_array = p_cluster->
			GetControlPointIndices();

		// 重み
		double * weight_array = p_cluster->
			GetControlPointWeights();

		FbxAMatrix trans_mat;
		FbxAMatrix init_glo_mat;
		FbxAMatrix geo_mat;

		// ボーン初期姿勢
		p_cluster->GetTransformLinkMatrix(init_glo_mat);

		// 変換行列
		p_cluster->GetTransformMatrix(trans_mat);

		geo_mat = GetGeometry(p_cluster->GetLink());

		// 変換 * 移動、回転、拡縮
		trans_mat *= geo_mat;

		// ボーン姿勢逆行列 * 変換行列
		FbxMatrix total_mat = init_glo_mat.Inverse() * trans_mat;

		// 代入
		m_glo_bone_mat_list.emplace_back(total_mat);
	}
}