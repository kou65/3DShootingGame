#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Graphics/Graphics.h"
#include"../../VertexBuffer/VertexBuffer.h"
#include"../../../D3DFont/D3DFont.h"
#include<d3dcompiler.h>


Fbx::Fbx() :
m_current_frame(0.0),
m_mesh_num(0)
{

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

	// 本チャン
	//AnimationSkinning();
	Skinning();

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

		// ワールド行列変換
		{

			FbxMatrix local_fmat;
			D3DXMATRIX l_mat;

			D3DXMatrixIdentity(&l_mat);

			// メッシュを取得
			FbxMesh* mesh =
				m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(meshIdx);

			// メッシュノード
			FbxNode* mesh_node = mesh->GetNode();

			// <移動、回転、拡大のための行列を作成>
			// ジョイントのローカル SRT 受け取り
			fbxsdk::FbxDouble3 local_translate = mesh_node->LclTranslation.Get();
			fbxsdk::FbxDouble3 local_rotate = mesh_node->LclRotation.Get();
			fbxsdk::FbxDouble3 local_scale = mesh_node->LclScaling.Get();
			local_fmat = FbxAMatrix(
				local_translate, local_rotate, local_scale);
			
			FbxMatConvertD3DMat(&l_mat, local_fmat);

				// ワールド座標変換
			mp_graphics->GetInstance()->GetLpDirect3DDevice9()
				->SetTransform(D3DTS_WORLD, &l_mat);
		}


		// マテリアルの設定
		pDevice->SetMaterial(
			&mate_info->material
		);

		FbxMeshData* p_mesh_data = 
			&m_mesh_data_list[meshIdx];
		
		// 頂点バッファの設定
		pDevice->SetStreamSource(
			0,
			p_mesh_data->p_vertex_buffer,
			0,
			// 1頂点当たりの大きさ
			sizeof(AnimationCustomVertex)
		);

		// 頂点フォーマットの指定
		pDevice->SetFVF(FVF_FBX);

		// インデックス数
		if (p_mesh_data->index_num)
		{
			// インデックスバッファの設定
			pDevice->SetIndices(
				p_mesh_data->p_index_buffer
			);

			// 描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				p_mesh_data->vertex_num,
				0,
				p_mesh_data->polygon_num
			);
		}
	}

	// アニメーションデバッグ
	D3DFont::Draw(500.f, 20.f, 200,"anim_num=>%f", m_current_frame);

}


bool Fbx::Load(
	const std::string&fbx_file_path
	) {


	// パス指定
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

	// メッシュ分のバッファを確保
	UINT mesh_num = (UINT)
		m_fbx_mod.mp_fbx_scene->GetSrcObjectCount<FbxMesh>();

	// マテリアル分のバッファを確保
	UINT materialNum = mesh_num;

	// ポリゴン変換
	FbxPolygon3Convert();


	SelectAnimation(0);
	SetAnimation();

	LoadDefaultMotion(0);

	// メッシュ数を追加
	m_mesh_num = mesh_num;

	// メッシュ数分回す
	for (UINT i = 0; i < mesh_num;i++) {

		// メッシュを取得
		FbxMesh * p_mesh = 
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(i);

		// 頂点データ生成
		m_mesh_data_list.emplace_back();

		m_mesh_data_list.back().mesh_node_name =
			p_mesh->GetNode()->GetName();

		// インデックス読み込み
		LoadIndeces(
			m_mesh_data_list,
			p_mesh
		);

		// 頂点情報読み込み
		LoadVertexInfo(
			p_mesh,
			m_mesh_data_list
		);

		// uv情報読み込み
		LoadUvInfo(
			m_mesh_data_list,
			p_mesh
		);
		
		// 法線情報読み込み
		LoadNormal(
			m_mesh_data_list,
			p_mesh
		);
		
		// カラー情報読み込み
		LoadColor(
			m_mesh_data_list,
			p_mesh
		);
		
		// マテリアル情報
		LoadMaterial(
			m_mesh_data_list,
			p_mesh
		);

		// ボーンモデルの読み込み
		LoadModelInfo(
			m_mesh_data_list,
			p_mesh
		);

		AnimationCustomVertex*p_vertics = nullptr;

		// サイズ設定
		UINT size =
			(UINT)(m_mesh_data_list.back().vertex_num *
				sizeof(AnimationCustomVertex));

		// バッファをロックしてデータを書き込む
		m_mesh_data_list[i].p_vertex_buffer->Lock(
		0,size,(void**)&p_vertics,0);

		// 頂点追加
		m_p_vertics.emplace_back(new AnimationCustomVertex[size]);

		// メモリコピー
		memcpy(m_p_vertics[i], p_vertics, size);

		if (p_vertics == nullptr) {
			return;
		}

		m_mesh_data_list[i].p_vertex_buffer->Unlock();
	}

	// モーションセット
	SetMotion("default");
}


void Fbx::LoadSkeleton() {

	// スケルトン分のバッファを確保
	UINT skeleton_num = (UINT)
		m_fbx_mod.mp_fbx_scene->GetSrcObjectCount<FbxSkeleton>();

	for (UINT i = 0; i < skeleton_num; i++) {

		// スケルトンを取得
		FbxSkeleton * p_skel = 
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxSkeleton>(i);

		// スケルトン名
		p_skel->GetNode()->GetName();
	}
}


void Fbx::SetMotion(std::string name)
{
	m_motion_name = name;
	m_current_frame = 0.0f;
}


void Fbx::Animate(float sec,float reset_frame)
{
	//	モーション時間の更新
	m_current_frame += sec / 60.0f;

	if (m_current_frame >= (m_stop_frame + ((int)reset_frame))) {
		m_current_frame =
			m_start_frame + m_current_frame - m_stop_frame;
	}

}


//	ボーン行列の補間
static void MatrixInterporate(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}


void Fbx::LoadIndeces(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ないなら戻す
	if (p_mesh_data_list.size() <= 0) {
		return;
	}

	// メッシュ取得
	FbxMeshData* p_mesh_data = &p_mesh_data_list.back();

	if (p_mesh_data == nullptr) {
		return;
	}

	// 本体移植用
	std::vector<int>indeces;

	// ポリゴン数を取得
	int polygon_count = p_mesh->GetPolygonCount();

	// 修正インデックス
	int fix_index_num = 0;

	// サイズ
	UINT size = (UINT)((polygon_count * 3) * sizeof(int));

	// tmp用
	std::vector<int>tmp_indeces;

	
	for (int i = 0; i < polygon_count; i++) {

		int index_size = p_mesh->GetPolygonSize(i);
		int polygon_num = 1;

		for (int j = 0; j < index_size; j++) {

			int index = p_mesh->GetPolygonVertex(i, j);

			tmp_indeces.push_back(index);
		}

		// 4ポリなら
		if (index_size >= 4) {

			// 修正インデックス数
			fix_index_num++;

			// ポリゴンを追加
			polygon_num++;

			// ポリゴン変換
			tmp_indeces = Polygon4ToPolygon3Convert(tmp_indeces);
		}

		int index_num = 0;

		for (int j = 0; j < polygon_num * 3; j++) {

			indeces.push_back(tmp_indeces[j]);
		}

		tmp_indeces.clear();
	}

	p_mesh_data->polygon_num = (UINT)polygon_count;
	p_mesh_data->index_num = (UINT)(polygon_count * 3);


	// インデックスバッファ作成
	{
		Graphics::GetInstance()->GetLpDirect3DDevice9()->
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
				&p_mesh_data->p_index_buffer,
				// 現在使用されていないので基本NULL
				NULL
			);

		// nullチェック
		if (p_mesh_data->p_index_buffer == nullptr) {
			return;
		}
	}

	// バッファ書き込み
	int* p_indeces;

	// バッファをロックしてデータを書き込む
	p_mesh_data->p_index_buffer->Lock(
		0,
		size,
		(void**)&p_indeces,
		0);

	//p_indeces = p_mesh->GetPolygonVertices();


	//for (UINT j = 0; j < indeces.size(); j++) {
	//
	//	p_indeces[j] = tmp_indeces[j];
	//}


	for (int i = 0; i < polygon_count; i++) {

		for (int j = 0; j < p_mesh->GetPolygonSize(i); j++) {
			int index = p_mesh->GetPolygonVertex(i, j);

			p_indeces[i * 3 + j] = index;
		}
	}
	p_mesh_data->p_index_buffer->Unlock();

}


void Fbx::LoadVertexInfo(
	FbxMesh*p_mesh,
	std::vector<FbxMeshData>&mesh_data_list
	) {


	//int vertex_num = p_mesh->GetPolygonVertexCount();
	int vertex_num = p_mesh->GetControlPointsCount();

	// 全ての頂点サイズ(数)を出力
	UINT size = (UINT)(vertex_num * sizeof(AnimationCustomVertex));

	// メッシュデータ受け取り
	FbxMeshData *p_mesh_data = &mesh_data_list.back();
	p_mesh_data->vertex_num = vertex_num;
	
	// バーテックスバッファ生成
	Graphics::GetInstance()->GetLpDirect3DDevice9()->
		CreateVertexBuffer(
			size,
			0,
			0,
			D3DPOOL_MANAGED,
			&p_mesh_data->p_vertex_buffer,
			NULL
		);
	
	AnimationCustomVertex* pVertices;

	// バッファをロックしてデータを書き込む
	p_mesh_data->p_vertex_buffer->Lock(
		0,
		size,
		(void**)&pVertices,
		0
	);

	// ポリゴン頂点インデックス配列受け取り
	int* p_index_vertex = p_mesh->GetPolygonVertices();

	FbxVector4* pos =
	p_mesh->GetControlPoints();
	
	int v = 0;

	// int index= p_mesh->GetPolygonVertexCount();

	// 頂点数分回す
	for (v; v < p_mesh->GetControlPointsCount(); v++)
	{
		// インデックス受け取り
		//int index = p_index_vertex[v];

		// もう一つの受け取り方
		//FbxVector4 pos = p_mesh->GetControlPointAt(index);

		// 位置
		pVertices[v].vertex.x = (float)pos[v][0];
		pVertices[v].vertex.y = (float)pos[v][1];
		pVertices[v].vertex.z = (float)pos[v][2];

		// 法線
		pVertices[v].normal.x = 0.0f;
		pVertices[v].normal.y = 1.0f;
		pVertices[v].normal.z = 0.0f;
		
		// テクセル
		pVertices[v].uv.x = 0.0f;
		pVertices[v].uv.y = 0.0f;
		
		// 頂点カラー
		pVertices[v].color = 0xfffffff;
	}

	// アンロック
	p_mesh_data->p_vertex_buffer->Unlock();
}


void Fbx::LoadUvInfo(
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
	p_mesh_data->p_vertex_buffer->Lock(
		0,
		size,
		(void**)&p_vertices,
		0
	);

	int uv_size = uv_sets.Size();

	// バッファ書き込み
	int* p_indeces;

	// バッファをロックしてデータを書き込む
	p_mesh_data->p_index_buffer->Lock(
		0,
		size,
		(void**)&p_indeces,
		0);
	
	for (int i = 0;
		i <  uv_size;
		i++) {
	
		// uv座標受け取り
		FbxVector2& uv_set = uv_sets[i];
	
		// uvはインデックスに沿った物でないといけない
		int index = p_indeces[i];


		p_vertices[index].uv.x = (float)uv_set[0];
		// 右手系なので左手系にする
		p_vertices[index].uv.y = (float)(1.f - uv_set[1]);
	}

	p_mesh_data->p_index_buffer->Unlock();
	// アンロック
	p_mesh_data->p_vertex_buffer->Unlock();
}


void Fbx::LoadNormal(
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

	IDirect3DVertexBuffer9* p_vertex_buffer = p_mesh_data->p_vertex_buffer;

	AnimationCustomVertex * p_vertices;

	int normal_size = normals.Size();

	// バッファをロックしてデータを書き込む
	p_vertex_buffer->Lock(0, size, (void**)&p_vertices, 0);

	for (UINT index = 0; index < p_mesh_data->vertex_num; index++) {

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

	// マテリアルがないならエントリにあるかも
	if (material_num == 0) {

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

			// テクスチャ読み込み
			LoadTexture(
				p_mesh,
				&p_vertex_data_list.back().material_info);
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

		// ファイルパス分割
		std::vector<std::string>string_list;
		string_list = Utility::SplitStr('/', p_file_name);

		strcpy_s(path, m_current_path.c_str());
		strcat_s(path, string_list.back().c_str());
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

	if (material_count <= 0) {
		return;
	}

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

	if (p_imple == nullptr) {
		return;
	}

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

	const char*p_name 
		= entry->GetSource();

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
				p_mesh_data->p_vertex_buffer;

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
				FbxColor color = colors.GetAt(indeces.GetAt(i));

				// カラー情報受け取り
				DWORD a = (DWORD)(color.mAlpha * 255.0);
				DWORD r = (DWORD)(color.mRed   * 255.0);
				DWORD g = (DWORD)(color.mGreen * 255.0);
				DWORD b = (DWORD)(color.mBlue  * 255.0);

				p_vertices[i].color =
					(a << 24) + (r << 16) + (g << 8) + (b);

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



void Fbx::LoadDefaultMotion(int anim_num) {

	// アニメーションフレーム数を取得する

	FbxArray<FbxString*>names;

	// 利用するアニメーションを選び設定を行う
	m_fbx_mod.mp_fbx_scene->FillAnimStackNameArray(names);

	// テイク数を取得
	int take_num = names.GetCount();

	// 上限を超えていたら戻す
	if (anim_num >= take_num) {
		anim_num = take_num;
	}

	// モーション数取得
	int motion_num = m_fbx_mod.mp_importer->GetAnimStackCount();

	// モーションがないなら返す
	if (take_num == 0 && motion_num == 0) {
		return;
	}

	// モーションが存在する時
	FbxTakeInfo*p_take = m_fbx_mod.mp_importer->
		GetTakeInfo(anim_num);
		//GetTakeInfo(names[0]->Buffer());

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

	// モーションのフレーム値を代入
	m_motion["default"].frame_num =
		(int)((stop - start) / fps60);

	m_current_frame = (float)m_start_frame;
}



void Fbx::LoadAnimFrameAttitudeMatrix(
	D3DXMATRIX*p_out_mat,
	FbxMesh*p_current_mesh,
	FbxCluster*p_current_cluster,
	float frame
) {
	
	FbxNode*p_node = p_current_mesh->GetNode();

	// ミリ秒
	FbxTime m_period;

	FbxMatrix bone_mat;

	m_period.SetSecondDouble(frame);

	// ボーン姿勢行列取得
	bone_mat =
		p_current_cluster->
		GetLink()->EvaluateGlobalTransform(m_period);

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
	FbxMatrix total_mat = bone_mat;
	// fbx_mat * bone_mat;

	// 左手系変換
	FbxMatLConvert(total_mat);

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

	// doubleに変換
	FbxDouble * mat = (FbxDouble*)fbx_mat;

	// オフセット行列保存
	for (int matrix = 0; matrix < 16; matrix++) {

		p_out_mat->m[matrix / 4][matrix % 4] =
			(float)mat[matrix];
	}
}


Fbx::NodeType Fbx::SerchNodeType(FbxNode*p_node) {

	FbxNodeAttribute * p_attrib = p_node->GetNodeAttribute();

	if (p_attrib == NULL) {
		return NodeType::NONE;
	}

	// タイプを使う
	FbxNodeAttribute::EType type = (FbxNodeAttribute::EType)p_attrib->GetAttributeType();
	
	// メッシュ
	if (type == FbxNodeAttribute::eMesh) {
		return  NodeType::MESH;
	}

	return  NodeType::NONE;
}


void Fbx::SerchNodeAttributes(FbxNode*p_parent_node) {

	int child_count = p_parent_node->GetChildCount();

	for (int i = 0; i < child_count; i++) {

		// 子ノードへ探査
		SerchNodeAttributes(
			p_parent_node->GetChild(i)
		);
	}
}


void Fbx::FbxPolygon3Convert() {

	fbxsdk::FbxGeometryConverter converter(m_fbx_mod.mp_manager);
	// マテリアルと1メッシュ毎に分ける
	converter.SplitMeshesPerMaterial(m_fbx_mod.mp_fbx_scene, true);
	// 全てを三角形にする
	converter.Triangulate(m_fbx_mod.mp_fbx_scene, true);
	
	// 追加
	converter.RemoveBadPolygonsFromMeshes(
		m_fbx_mod.mp_fbx_scene
	);

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
	//p_motion->key_list[bone].resize(p_motion->frame_num);

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

		// 左手系に変換
		FbxMatLConvert(m);

		FbxDouble * mat = (FbxDouble*)m;

		for (int i = 0; i < 16; i++) {
			//p_motion->key_list[bone][f].m[i / 4][i % 4]
			//	= (float)mat[i];
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

	char* full_path;
	full_path = _fullpath(fullPath, path.c_str(), _MAX_PATH);

	// cp932 → UTF8
	char* path_utf8;
	FbxAnsiToUTF8(fullPath, path_utf8);

	// char* → std::string
	std::string coverted_path(path_utf8);
	// FBX SDK内部で確保されたメモリは専用の関数で解放
	FbxFree(path_utf8);

	return coverted_path;
}


FbxAMatrix Fbx::GetGeometry(FbxNode* p_mesh_node)
{
	FbxVector4 t0 = p_mesh_node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = p_mesh_node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = p_mesh_node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(t0, r0, s0);
}


void Fbx::LoadWeight(
	FbxCluster*p_cluster,
	FbxMesh*p_mesh,
	std::vector<WeightData>&weight_data_list
) {

	// 影響する頂点の数
	int point_num = p_cluster->
		GetControlPointIndicesCount();

	//m_weight_num_list.back().push_back(point_num);

	// 影響する頂点のインデックス
	int * point_array = p_cluster->
		GetControlPointIndices();

	// 重み
	double * weight_array = p_cluster->
		GetControlPointWeights();

	WeightData wd;

	for (int i = 0; i < point_num; i++) {

		// 重みを代入
		wd.weight = (float)weight_array[i];

		// インデックス代入
		wd.weight_index = point_array[i];

		// リストに追加
		weight_data_list.push_back(wd);
	}
}


void Fbx::LoadMotion(
	std::string name,
	const char* pFilename
)
{
	FbxManager* pManager = FbxManager::Create();
	FbxScene* pScene = FbxScene::Create(pManager, "");

	//	ファイルからシーンに読み込み
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	pImporter->Initialize(pFilename);
	pImporter->Import(pScene);
	pImporter->Destroy();

	//	モーション情報取得
	FbxArray<FbxString*> names;
	pScene->FillAnimStackNameArray(names);

	FbxTakeInfo* take = pScene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	m_start_frame = (int)(start / fps60);
	m_motion[name].frame_num = (int)((stop - start) / fps60);
	//	ルートノード取得
	FbxNode* root = pScene->GetRootNode();

	//	全ボーン読み込み
	//for (int b = 0; b < (int)m_bone_num; b++)
	//{
	//	//	ボーンノード検索
	//	FbxNode* pBone = root->FindChild(m_bone_data_list[b].name);
	//	if (pBone == NULL) continue;
	//
	//	//	キーフレーム読み込み
	//	LoadKeyFrame(name, b, pBone);
	//}
	//	解放
	pScene->Destroy();
	pManager->Destroy();
}



void Fbx::FbxMatLConvert(
	FbxMatrix&out_fbx_mat
) {

	// Z軸を反転させる
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}

void Fbx::FbxMatLConvert(
	FbxAMatrix&out_fbx_mat
) {
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}


void Fbx::AnimationSkinning() {


	std::vector<std::string>skin_name_list;
	//FbxTime timeCount;
	//
	//timeCount.SetSecondDouble(m_current_frame);

	FbxNode* pNode = m_fbx_mod.mp_fbx_scene->GetRootNode();

	// <移動、回転、拡大のための行列を作成>
	FbxMatrix globalPosition = pNode->
		EvaluateGlobalTransform(timeCount);

	int total_bone_num = 0;
	int total_cluster = 0;

	// 頂点変形
	for (UINT mi = 0;
		mi < m_mesh_num;
		mi++) {

		// メッシュを取得
		FbxMesh* mesh =
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

		// メッシュノード
		FbxNode* mesh_node = mesh->GetNode();

		// メッシュでのgeo受け取り
		FbxVector4 t0 = mesh_node->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 r0 = mesh_node->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 s0 = mesh_node->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);

		// メッシュデータ受け取り
		FbxMeshData* p_mesh_data = &m_mesh_data_list[mi];

		// 頂点データ受け取り
		AnimationCustomVertex* p_src_vertics =
			m_p_vertics[mi];

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)(mesh->GetControlPointsCount() *
				sizeof(AnimationCustomVertex));

		// 頂点バッファを定義
		AnimationCustomVertex* vertices;

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mi].p_vertex_buffer->Lock(
			0, size, (void**)&vertices, 0
		);

		FbxMatrix init_mat;

		init_mat.mData[0][0] = 0.f;
		init_mat.mData[1][1] = 0.f;
		init_mat.mData[2][2] = 0.f;
		init_mat.mData[3][3] = 0.f;

		// 他の頂点のアニメーション用
		FbxMatrix fanim_mat = init_mat;

		// <各頂点に掛けるための最終的な行列の配列>
		std::vector<FbxMatrix>cluster_deformation(
			(int)mesh->GetControlPointsCount(),
			init_mat
		);

		// スキンの数を取得
		int defor_count = mesh->GetDeformerCount(FbxDeformer::eSkin);

		int weight_num = 0;

		std::vector<FbxMatrix>mat_list;
		std::vector<FbxMatrix>anim_mat_list;
		std::vector<std::vector<double>>weight_list;
		std::vector<std::vector<int>>weight_index_list;


		timeCount += FrameTime;
		if (timeCount > stop) {
			timeCount = start;
		}

		// スキン分回す
		for (int skin_num = 0; skin_num < defor_count; skin_num++) {


			FbxSkin* skinDeformer = (FbxSkin*)
				mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

			if (skinDeformer == nullptr) {
				continue;
			}

			// クラスター数
			int cluster_count = skinDeformer->
				GetClusterCount();

			total_cluster += cluster_count;

			// <各クラスタから各頂点に影響を与えるための行列作成>
			for (int clusterIndex = 0;
				clusterIndex < cluster_count;
				clusterIndex++) {

				// <クラスタ(ボーン)の取り出し>
				FbxCluster* cluster = skinDeformer->
					GetCluster(clusterIndex);

				// ボーン(eskelton)ノード取得
				FbxNode* link_node = cluster->GetLink();

				// スケルトンノードの名前を代入
				skin_name_list.push_back(link_node->GetName());

				// 行列
				FbxMatrix vertexTransformMatrix;
				FbxAMatrix referenceGlobalInitPosition;
				FbxAMatrix clusterGlobalInitPosition;
				FbxMatrix clusterGlobalCurrentPosition;
				FbxMatrix clusterRelativeInitPosition;
				FbxMatrix clusterRelativeCurrentPositionInverse;

				cluster->GetTransformMatrix(referenceGlobalInitPosition);
				referenceGlobalInitPosition *= geometryOffset;
				cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

				clusterGlobalCurrentPosition =
					cluster->GetLink()->
					EvaluateGlobalTransform(timeCount);

				clusterRelativeInitPosition =
					clusterGlobalInitPosition.Inverse() *
					referenceGlobalInitPosition;

				clusterRelativeCurrentPositionInverse =
					globalPosition.Inverse() *
					clusterGlobalCurrentPosition;

				vertexTransformMatrix =
					clusterRelativeCurrentPositionInverse
					* clusterRelativeInitPosition;

				// ボーン加算
				total_bone_num++;

				int control_point_indices_count = cluster->
					GetControlPointIndicesCount();

				weight_list.emplace_back();
				weight_index_list.emplace_back();


				// マトリックス追加
				mat_list.emplace_back(vertexTransformMatrix);
				

				// <上で作った行列に各頂点毎の影響度(重み)
				// を掛けてそれぞれに加算>
				for (int cnt = 0;
					cnt < control_point_indices_count;
					cnt++) {

					int index = cluster->
						GetControlPointIndices()[cnt];

					double weight = cluster->
						GetControlPointWeights()[cnt];

					// マトリックス変換
					FbxMatrix influence =
						vertexTransformMatrix * weight;

					// 影響するインデックス行列
					cluster_deformation[index] += influence;

					// 重み
					weight_list[clusterIndex].push_back(weight);

					// 重みインデックス
					weight_index_list[clusterIndex].push_back(index);
				}
			}
		}

		FbxSkin* skinDeformer = (FbxSkin*)
			mesh->GetDeformer(0, FbxDeformer::eSkin);

		for (int cnt = 0;
			cnt < skinDeformer->GetClusterCount();
			cnt++) {

			for (UINT w = 0; w < weight_list[cnt].size(); w++) {

				// 影響するインデックス受け取り
				int w_idx = weight_index_list[cnt][w];

				vertices[w_idx].vertex.x = 0;
				vertices[w_idx].vertex.y = 0;
				vertices[w_idx].vertex.z = 0;
			}
		}

		for (int cnt = 0;
			cnt < skinDeformer->GetClusterCount();
			cnt++) {


			for (UINT w = 0; w < weight_list[cnt].size(); w++) {

				// 影響するインデックス受け取り
				int w_idx = weight_index_list[cnt][w];

				// 位置の取得
				FbxVector4 pos = mesh->GetControlPointAt(
					w_idx
				);

				// new_mat
				FbxMatrix new_mat = 
					mat_list[cnt];

				FbxVector4 out_vertex = 
					new_mat.MultNormalize
					(pos);

				float x = (float)out_vertex[0];
				float y = (float)out_vertex[1];
				float z = (float)out_vertex[2];

				float weight = (float)weight_list[cnt][w];

				vertices[w_idx].vertex.x += x * weight;
				vertices[w_idx].vertex.y += y * weight;
				vertices[w_idx].vertex.z += z * weight;
			}
		}


		//// <最終的な頂点座標を計算しVERTEXに変換>
		//int count = mesh->GetControlPointsCount();
		//
		//for (int i = 0; i < count; i++) {
		//
		//	FbxVector4 pos = mesh->GetControlPointAt(
		//		i
		//	);
		//
		//	FbxVector4 out_vertex =
		//		cluster_deformation[i].MultNormalize
		//		(pos);
		//
		//	float x = (float)out_vertex[0];
		//	float y = (float)out_vertex[1];
		//	float z = (float)out_vertex[2];
		//
		//	vertices[i].vertex.x = x;
		//	vertices[i].vertex.y = y;
		//	vertices[i].vertex.z = z;
		//}

		m_mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}


void Fbx::LoadModelInfo(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// メッシュでのgeo受け取り
	FbxAMatrix geometryOffset = GetGeometry(m_fbx_mod.mp_fbx_scene->GetRootNode());

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);


	FbxMeshData * data= &p_mesh_data_list.back();

	int total_weight = 0;

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		FbxSkin* skinDeformer = (FbxSkin*)
			p_mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skinDeformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = skinDeformer->
			GetClusterCount();
	

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skinDeformer->
				GetCluster(ci);

			// 行列
			FbxAMatrix referenceGlobalInitPosition;
			FbxAMatrix clusterGlobalInitPosition;
			FbxMatrix clusterRelativeInitPosition;

			cluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= geometryOffset;
			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

			// クラスター位置
			clusterRelativeInitPosition =
				clusterGlobalInitPosition.Inverse() *
				referenceGlobalInitPosition;

			// ボーンリストに追加
			p_mesh_data_list.back().bone_list.
				emplace_back(clusterRelativeInitPosition);


			// クラスタの追加
			data->m_weight_index_list.emplace_back();
			data->m_weight_list.emplace_back();

			for (int i = 0; i < 
				cluster->GetControlPointIndicesCount();
				i++) {

				double weight = cluster->
					GetControlPointWeights()[i];

				// 重み
				data->
				m_weight_list.back().push_back(weight);

				int index = cluster->
					GetControlPointIndices()[i];

				// 重みインデックス
				data->
				m_weight_index_list.back().push_back(index);

				total_weight++;
			}
		}
	}

	total_weight = total_weight;
}


void Fbx::LoadAnimationMatrix(
	FbxMesh* mesh,
	std::vector<FbxMatrix>& anim_mat_list
) {

	FbxNode* pNode = m_fbx_mod.mp_fbx_scene->GetRootNode();

	// <移動、回転、拡大のための行列を作成>
	FbxMatrix globalPosition = pNode->
		EvaluateGlobalTransform(timeCount);

	// メッシュノード
	FbxNode* mesh_node = mesh->GetNode();

	// スキンの数を取得
	int defor_count = mesh->GetDeformerCount(FbxDeformer::eSkin);

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		FbxSkin* skinDeformer = (FbxSkin*)
			mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skinDeformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = skinDeformer->
			GetClusterCount();

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skinDeformer->
				GetCluster(ci);

			// 行列
			FbxMatrix clusterGlobalCurrentPosition;
			FbxMatrix clusterRelativeCurrentPositionInverse;

			clusterGlobalCurrentPosition =
				cluster->GetLink()->
				EvaluateGlobalTransform(timeCount);

			clusterRelativeCurrentPositionInverse =
				globalPosition.Inverse() *
				clusterGlobalCurrentPosition;

			anim_mat_list.emplace_back(clusterRelativeCurrentPositionInverse);
		}
	}
}


void Fbx::Skinning() {


	std::vector<FbxMatrix>anim_list;


	timeCount += FrameTime;
	if (timeCount > stop) {
		timeCount = start;
	}


	// 頂点変形
	for (UINT mi = 0;
		mi < m_mesh_num;
		mi++) {

		// メッシュデータ受け取り
		FbxMeshData mesh_data = m_mesh_data_list[mi];

		// メッシュを取得
		FbxMesh* mesh = 
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

		anim_list.clear();

		// アニメーション行列
		LoadAnimationMatrix(mesh,anim_list);

		// 頂点バッファを定義
		AnimationCustomVertex* vertices;

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)((mesh->GetControlPointsCount()) * 
				sizeof(AnimationCustomVertex));

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mi].p_vertex_buffer->Lock(
			0, 
			size,
			(void**)&vertices,
			0
		);


		// スキン
		FbxSkin* skinDeformer = (FbxSkin*)
			mesh->GetDeformer(0, FbxDeformer::eSkin);


		for (int cnt = 0;
			cnt < skinDeformer->GetClusterCount();
			cnt++) {
		
			for (UINT w = 0; 
				w < mesh_data.m_weight_index_list[cnt].size();
				w++) {

				// 影響するインデックス受け取り
				int w_idx = mesh_data.m_weight_index_list[cnt][w];
		
				vertices[w_idx].vertex.x = 0;
				vertices[w_idx].vertex.y = 0;
				vertices[w_idx].vertex.z = 0;
			}
		}

		int count = skinDeformer->GetClusterCount();
		
		for (int cnt = 0;
			cnt < count;
			cnt++) {

			// ボーン行列変換
			FbxMatrix new_mat =
				anim_list[cnt] *
				m_mesh_data_list[mi].
				bone_list[cnt];


			for (UINT w = 0;
				w < mesh_data.m_weight_index_list[cnt].size();
				w++) {

				// 影響するインデックス受け取り
				int w_idx = mesh_data.m_weight_index_list[cnt][w];

				FbxVector4 pos = mesh->GetControlPointAt(
					w_idx
				);

				FbxVector4 out_vertex = 
					new_mat.MultNormalize
					(pos);

				float x = (float)out_vertex[0];
				float y = (float)out_vertex[1];
				float z = (float)out_vertex[2];

				float weight = 
					(float)mesh_data.m_weight_list[cnt][w];

				vertices[w_idx].vertex.x += x * weight;
				vertices[w_idx].vertex.y += y * weight;
				vertices[w_idx].vertex.z += z * weight;

			}
		}

		m_mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}


void Fbx::SetAnimation() {

	// アニメーションファイル名を受け取る
	FbxArray<FbxString*> AnimStackNameArray;
	m_fbx_mod.mp_fbx_scene->
		FillAnimStackNameArray(AnimStackNameArray);

	// アニメーション情報が入ったスタックを受け取る
	FbxAnimStack* AnimationStack = 
		m_fbx_mod.mp_fbx_scene->
		FindMember<FbxAnimStack>(AnimStackNameArray[0]->Buffer());

	// アニメーションをセット
	m_fbx_mod.mp_fbx_scene->
		SetCurrentAnimationStack(AnimationStack);

	// アニメーション時間をセット
	FbxTakeInfo* take_info = m_fbx_mod.mp_fbx_scene->
		GetTakeInfo(*(AnimStackNameArray[0]));

	start = take_info->mLocalTimeSpan.GetStart();
	stop = take_info->mLocalTimeSpan.GetStop();

	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_fbx_mod.mp_fbx_scene->GetGlobalSettings().GetTimeMode());

	timeCount = start;
}


void Fbx::AttitudeSkinning() {

	// モーションがないならば
	if (m_motion.empty() == true) {
		return;
	}

	MotionData* p_motion = &m_motion[m_motion_name];

	if (p_motion == nullptr) {
		return;
	}
	if (p_motion->frame_num < 0) {
		return;
	}
	double frame = m_current_frame;

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
		FbxMesh* p_mesh =
			m_fbx_mod.mp_fbx_scene->GetSrcObject<FbxMesh>(mesh_index);


		// メッシュデータ受け取り
		FbxMeshData* p_mesh_data = &m_mesh_data_list[mesh_index];

		// 頂点データ受け取り
		AnimationCustomVertex* p_src_vertics =
			m_p_vertics[mesh_index];

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)(p_mesh_data->vertex_num *
				sizeof(AnimationCustomVertex));

		// 頂点バッファを定義
		AnimationCustomVertex* p_vertics;

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mesh_index].p_vertex_buffer->Lock(
			0, size, (void**)&p_vertics, 0
		);

		D3DXMATRIX frame_mat[4];

		// 頂点 * 逆行列 * 座標変換行列 * 初期行列(元に戻す行列)
		for (int bn = 0; bn < 4; bn++) {

			frame_mat[bn] =
				//m_bone_data_list[bn].offset *
				atti_mat;
		}

		// 頂点全てアニメーションを行う
		for (UINT v = 0; v < p_mesh_data->vertex_num; v++) {

			// 頂点 * ボーン行列
			// b = v番目の頂点の影響ボーン[n]
			// 影響しないので戻る
			if (p_vertics[v].weight[0] <= 0.f) {
				continue;
			}

			// 一旦初期値にする
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
		p_mesh_data->p_vertex_buffer->Unlock();
	}
}


