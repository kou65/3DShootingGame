#include"FBX.h"
#include"../../../Window/Window.h"
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Graphics/Graphics.h"
#include"../../VertexBuffer/VertexBuffer.h"
#include"../../../D3DFont/D3DFont.h"




Fbx::Fbx() : Model()
{

	m_is_shader = true;

	m_is_skinning = true;

	m_count = 0;

	m_frame = 0.f;

	m_current_motion_name = "default";

	// グラフィックス受け取り
	m_p_graphics = Graphics::GetInstance();

	// fbxマネージャオブジェクトの生成
	m_fbx_module.mp_manager = FbxManager::Create();

	// fbxシーンの作成
	m_fbx_module.mp_fbx_scene = FbxScene::Create(m_fbx_module.mp_manager, "");

	// nullチェック
	{
		if (m_fbx_module.mp_manager == NULL) {
			Window::TextMessageBox("FBXManagerの生成に失敗しました");
		}

		if (m_fbx_module.mp_fbx_scene == NULL) {
			Window::TextMessageBox("FBXSceneの作成に失敗しました");
		}
	}
}


void Fbx::Update() {

	if (m_is_shader == false) {

		if (m_is_skinning == true) {
			CPUSkinning();
		}
	}
	
}


void Fbx::Draw(TextureData* td) {


	// ライトをオフにする
	Graphics::GetInstance()->GetDevice()->
		SetRenderState(D3DRS_LIGHTING, FALSE);

	// デバイス
	IDirect3DDevice9* p_device =
		Graphics::GetInstance()->GetDevice();

	// メッシュインデックスを回す
	for (UINT mi = 0; mi < m_mesh_data_list.size(); mi++)
	{

		MaterialInfo* mate_info =
			&m_mesh_data_list[mi].material_info;

		// テクスチャ
		{
			// 外部からテクスチャを読み取れた場合
			if (td != nullptr) {

				p_device->SetTexture(
					0,
					td->p_texture_buffer
				);
			}

			// テクスチャーの設定
			else if (TextureManager::GetInstance()->
				FindTexture(mate_info->texture_name) == true) {

				if (m_is_shader == false) {
					p_device->SetTexture(
						0,
						TextureManager::GetInstance()->GetTextureData
						(mate_info->texture_name)
					);
				}
				else {

					m_effect.SetTexture(
						TextureManager::GetInstance()->
						GetTextureData(mate_info->texture_name)
					);
				}
			}
		}

		// メッシュデータ
		FbxMeshData* p_mesh_data =
			&m_mesh_data_list[mi];

		if (p_mesh_data->p_index_buffer == nullptr) {
			return;
		}
		if (p_mesh_data->p_vertex_buffer == nullptr) {
			return;
		}

		// ワールド座標変換
		D3DXMATRIX world_mat = GetFbxWorldD3DMatrix(mi);
		
		// パラメータセット
		Set3DParameter(
			m_p_graphics,
			p_mesh_data->p_index_buffer,
			p_mesh_data->p_vertex_buffer,
			sizeof(SkinCustomVertex),
			mate_info->material,
			FVF_SKIN
		);

		// シェーダーの描画
		if (m_is_shader == true) {

			EffectDraw(
				p_mesh_data->vertex_num,
				p_mesh_data->polygon_num,
				world_mat,
				// ボーン行列
				m_motion
				[m_current_motion_name][mi].
				d3d_animation_mat[m_count].bone_list,
				// ボーン数
				m_motion
				[m_current_motion_name][mi].animation_matrix[m_count].size()
			);
		}
		else
		{
			NormalDraw(
				p_mesh_data->vertex_num,
				p_mesh_data->polygon_num,
				world_mat
			);
		}

	}

}


void Fbx::NormalDraw(
	const int&vertex_num,
	const int&polygon_num,
	D3DXMATRIX&world_mat
) {

	m_p_graphics->GetInstance()->GetDevice()
		->SetTransform(D3DTS_WORLD, &world_mat);

	DrawPrimitive(vertex_num, polygon_num);
}


void Fbx::EffectDraw(
	const int&vertex_num,
	const int&polygon_num,
	const D3DXMATRIX&world_mat,
	D3DXMATRIX*bone_mat_list,
	const int&max_bone_index
) {


	// ボーン行列セット
	m_effect.SetWorldMatrix(world_mat);

	// ボーンセット
	m_effect.SetBoneMatrix(
		bone_mat_list
	);

	m_effect.SetBoneCount(max_bone_index);

	m_effect.SetMaxIndex(
		max_bone_index
	);

	// 更新
	m_effect.Update();

	UINT pass_num = 0;

	m_effect.ShaderBegin(pass_num, 0);
	m_effect.BeginPass(0);

	DrawPrimitive(vertex_num, polygon_num);

	m_effect.EndPass();
	m_effect.ShaderEnd();
}


D3DXMATRIX Fbx::GetFbxWorldD3DMatrix(int mesh_index) {

	// ワールド行列変換
	{

		FbxMatrix local_fmat;
		D3DXMATRIX l_mat;

		D3DXMatrixIdentity(&l_mat);

		// メッシュを取得
		FbxMesh* mesh =
			m_fbx_module.mp_fbx_scene->GetSrcObject<FbxMesh>(mesh_index);

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

		return l_mat;
	}
}


bool Fbx::Load(
	const std::string&fbx_file_path
	) {


	// パス指定
	LoadCurrentPath(fbx_file_path);

	// fbxインポータの作成
	m_fbx_module.m_p_importer = FbxImporter::Create
	(m_fbx_module.mp_manager, "");

	// nullチェック
	if (m_fbx_module.m_p_importer == NULL) {
		Window::TextMessageBox("FBXImporterの生成に失敗しました");
	}

	// インポータの初期化
	if (!m_fbx_module.m_p_importer->Initialize(fbx_file_path.c_str())) {

		Window::TextMessageBox("FBXimporter初期化失敗");
		return false;
	}

	// ファイルからシーンを読み込む
	if (!m_fbx_module.m_p_importer->Import(m_fbx_module.mp_fbx_scene)) {

		Window::TextMessageBox("シーン読み込みに失敗");
		return false;
	}

	

	// シェーダーの初期化
	if (m_is_shader == true) {
		m_effect.SetShaderName("VertexBlend.hlsl");
		m_effect.SetTechnique("tech1");

		m_effect.Init();
	}

	/* ここに処理を書いていく */
	LoadMesh();

	// 最初のモーション読み込み
	LoadMotion("default", 0);

	// モーションセット
	SetMotion("default");

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
		m_fbx_module.mp_fbx_scene->GetSrcObjectCount<FbxMesh>();

	// マテリアル分のバッファを確保
	UINT materialNum = mesh_num;

	// ポリゴン変換
	FbxPolygon3Convert();

	// メッシュ数分回す
	for (UINT i = 0; i < mesh_num;i++) {

		// メッシュを取得
		FbxMesh * p_mesh = 
			m_fbx_module.mp_fbx_scene->GetSrcObject<FbxMesh>(i);

		// 頂点データ生成
		m_mesh_data_list.emplace_back();


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

		// ウェイトを頂点に読み込ませる
		LoadWeightVertexPoint(
			m_mesh_data_list,
			p_mesh
		);

		SkinCustomVertex*p_vertics = nullptr;

		// サイズ設定
		UINT size =
			(UINT)(m_mesh_data_list.back().vertex_num *
				sizeof(SkinCustomVertex));

		// バッファをロックしてデータを書き込む
		m_mesh_data_list[i].p_vertex_buffer->Lock(
		0,size,(void**)&p_vertics,0);

		// 頂点追加
		m_p_vertics.emplace_back(new SkinCustomVertex[size]);

		// メモリコピー
		memcpy(m_p_vertics[i], p_vertics, size);

		if (p_vertics == nullptr) {
			return;
		}

		m_mesh_data_list[i].p_vertex_buffer->Unlock();
	}

}


void Fbx::SetMotion(std::string name)
{
	m_current_motion_name = name;
	m_frame = 0.0f;

	// アニメーション最大数を入れる
	m_stop_count = 
		m_motion[name].back().d3d_animation_mat.size();
}


void Fbx::Animate(const float &frame)
{

	if (m_frame > frame) {
		m_count++;
		m_frame = 0;
	}

	if (m_count > m_stop_count - 1) {
		m_count = 0;
	}

	// フレーム値加算
	m_frame++;
}


//	ボーン行列の補間
static void MatrixInterporate(
	D3DXMATRIX& out,
	D3DXMATRIX& A,
	D3DXMATRIX B,
	float rate
)
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
	//p_mesh_data->index_num = (UINT)(polygon_count * 3);


	// インデックスバッファ作成
	{
		Graphics::GetInstance()->GetDevice()->
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


	// こっちが本体
	for (int i = 0; i < polygon_count; i++) {

		for (int j = 0; j < p_mesh->GetPolygonSize(i); j++) {
			int index = p_mesh->GetPolygonVertex(i, j);

			p_indeces[i * 3 + j] = index;
		}
	}
	p_mesh_data->p_index_buffer->Unlock();

}


void Fbx::InitVertexInfo(
	FbxMesh*p_mesh,
	FbxMeshData*p_mesh_data,
	const UINT &size
) {


	SkinCustomVertex* p_vertices;

	// バッファをロックしてデータを書き込む
	p_mesh_data->p_vertex_buffer->Lock(
		0,
		size,
		(void**)&p_vertices,
		0
	);

	FbxVector4* pos = 
		p_mesh->GetControlPoints();

	// 頂点数分回す
	for (int v = 0; v < p_mesh->GetControlPointsCount(); v++)
	{
		// インデックス受け取り
		//int index = p_index_vertex[v];

		// もう一つの受け取り方
		//FbxVector4 pos = p_mesh->GetControlPointAt(index);

		// 位置
		p_vertices[v].vertex.x = (float)pos[v][0];
		p_vertices[v].vertex.y = (float)pos[v][1];
		p_vertices[v].vertex.z = (float)pos[v][2];
		p_vertices[v].vertex.w = 1.f;

		// 初期値
		D3DXCOLOR col;
		col.a = 255.f;
		col.b = 255.f;
		col.g = 255.f;
		col.r = 255.f;

		// ここでカラー変更
		p_vertices[v].diffuse = col;

		// 法線
		p_vertices[v].normal.x = 0.0f;
		p_vertices[v].normal.y = 1.0f;
		p_vertices[v].normal.z = 0.0f;

		for (int i = 0; i < 4; i++) {
			p_vertices[v].weight[i] = 0.f;
			p_vertices[v].bone_index[i] = 0;
		}

		// w値を1.fにする
		//p_vertices[v].normal.w = 1.0f;

		// デバッグ用
		p_vertices[v].bone_index[2] = 3;
		//p_vertices[v].weight[3] = 100.f;
	}

	// アンロック
	p_mesh_data->p_vertex_buffer->Unlock();
}


void Fbx::LoadVertexInfo(
	FbxMesh*p_mesh,
	std::vector<FbxMeshData>&mesh_data_list
	) {


	//int vertex_num = p_mesh->GetPolygonVertexCount();
	int vertex_num = p_mesh->GetControlPointsCount();

	// 全ての頂点サイズ(数)を出力
	UINT size = (UINT)(vertex_num * sizeof(SkinCustomVertex));

	// メッシュデータ受け取り
	FbxMeshData *p_mesh_data = &mesh_data_list.back();
	p_mesh_data->vertex_num = vertex_num;
	
	// バーテックスバッファ生成
	Graphics::GetInstance()->GetDevice()->
		CreateVertexBuffer(
			size,
			0,
			0,
			D3DPOOL_MANAGED,
			&p_mesh_data->p_vertex_buffer,
			NULL
		);

	// 頂点データの初期化
	InitVertexInfo(
		p_mesh,
		p_mesh_data,
		size
	);	
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

	UINT size = p_mesh_data->vertex_num * sizeof(SkinCustomVertex);

	SkinCustomVertex*p_vertices;

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
	UINT size = p_mesh_data->vertex_num * sizeof(SkinCustomVertex);

	IDirect3DVertexBuffer9* p_vertex_buffer = p_mesh_data->p_vertex_buffer;

	SkinCustomVertex * p_vertices;

	int normal_size = normals.Size();

	// バッファをロックしてデータを書き込む
	p_vertex_buffer->Lock(0, size, (void**)&p_vertices, 0);

	for (UINT index = 0; index < p_mesh_data->vertex_num; index++) {

		FbxVector4&normal = normals[index];

		// それぞれxyz代入
		p_vertices[index].normal.x = (float)normal[0];
		p_vertices[index].normal.y = (float)normal[1];
		p_vertices[index].normal.z = (float)normal[2];
		//p_vertices[index].normal.w = (float)normal[3];
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

		strcpy_s(path, m_root_path.c_str());
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
			m_root_path + string_list.back();
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
			m_root_path + string_list.back();

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

			SkinCustomVertex*p_vertices;

			// バッファをロックして書き込む
			p_vertex_buffer->Lock(
				0,
				p_mesh_data->vertex_num * sizeof(SkinCustomVertex),
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

				//p_vertices[i].diffuse =
				//	(a << 24) + (r << 16) + (g << 8) + (b);
			}
			p_vertex_buffer->Unlock();
		}
	}

	return true;
}


bool Fbx::SelectAnimation(
	int select_anim_num
) {

	int anim_num = m_fbx_module.m_p_importer->
		GetAnimStackCount();

	// 総アニメーション数よりも大きいなら
	if (anim_num < select_anim_num){
		return false;
	}

	// 受け取るアニメーションを切り替え
	FbxAnimStack* pStack = m_fbx_module.mp_fbx_scene->
		GetSrcObject<FbxAnimStack>(select_anim_num);

	m_fbx_module.mp_fbx_scene->SetCurrentAnimationStack(pStack);

	return true;
}


void Fbx::SetAnimation(
	FbxTime&start,
	FbxTime&stop,
	FbxTime&frame_time,
	const int&anim_num
) {

	// アニメーションファイル名を受け取る
	FbxArray<FbxString*> AnimStackNameArray;
	m_fbx_module.mp_fbx_scene->
		FillAnimStackNameArray(AnimStackNameArray);

	// アニメーション情報が入ったスタックを受け取る
	FbxAnimStack* AnimationStack =
		m_fbx_module.mp_fbx_scene->
		FindMember<FbxAnimStack>(AnimStackNameArray[anim_num]->Buffer());

	// アニメーションをセット
	m_fbx_module.mp_fbx_scene->
		SetCurrentAnimationStack(AnimationStack);

	// アニメーション時間をセット
	FbxTakeInfo* take_info = m_fbx_module.mp_fbx_scene->
		GetTakeInfo(*(AnimStackNameArray[anim_num]));

	start = take_info->mLocalTimeSpan.GetStart();
	stop = take_info->mLocalTimeSpan.GetStop();

	frame_time.SetTime(0, 0, 0, 1, 0,
		m_fbx_module.mp_fbx_scene->GetGlobalSettings().GetTimeMode());

}



void Fbx::LoadMotion(
	const std::string& name,
	const int &select_motion_num
) {

	// アニメーション選択
	if (SelectAnimation(select_motion_num) == false) {
		return;
	}

	// タイマー
	FbxTime frame_time, start, stop;

	// アニメーション情報受け取り
	SetAnimation(start,stop,frame_time,select_motion_num);

	for (UINT mi = 0;
		mi < m_mesh_data_list.size();
		mi++) {

		// メッシュを取得
		FbxMesh * p_mesh =
			m_fbx_module.mp_fbx_scene->
			GetSrcObject<FbxMesh>(mi);

		for (int s = 0; s < p_mesh->GetDeformerCount(); s++) {

			// リストの中身がないなら
			if (m_mesh_data_list.size() == 0) {
				return;
			}

			// スキン受け取り
			FbxSkin *skin = (FbxSkin*)p_mesh->GetDeformer(s);

			// アニメーション読み込み
			LoadKeyFrame(
				p_mesh,
				name,
				m_mesh_data_list[mi].bone_list,
				skin->GetClusterCount(),
				start,
				stop,
				frame_time
			);
		}
	}
}


void Fbx::LoadKeyFrame(
	FbxMesh* mesh,
	const std::string& motion_name,
	std::vector<FbxMatrix>& bone_mat_list,
	const int& bone_num,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count
) {

	// スキン数
	int skin_count = mesh->GetDeformerCount();

	// モーション作成
	MotionData motion;

	FbxTime time_count = start;

	int time = 0;

	// モーション数がストップするまで
	while (time_count < stop) {

		// フレーム用配列作成
		motion.animation_matrix.emplace_back();
		motion.d3d_animation_mat.emplace_back();

		// デフォーマ数分回す
		for (int s = 0; s < skin_count; s++) {

			// 合成行列を計算
			for (int b = 0;
				b < bone_num;
				b++) {

				// アニメーション行列取得
				FbxMatrix anim_mat = GetAnimationMatrix(
					mesh,
					time_count,
					s,
					b
				);

				// ボーン行列変換(アニメーション行列 * ボーン行列)
				FbxMatrix total_mat =
					anim_mat *
					bone_mat_list[b];

				D3DXMATRIX conv_mat;

				// アニメーション行列追加
				motion.animation_matrix.
					back().
					emplace_back(total_mat);

				// 変換して本体の行列に変換する
				FbxMatConvertD3DMat(&conv_mat, total_mat);

				//// d3d用に代入
				//motion.d3d_animation_mat.
				//	back().
				//	emplace_back(conv_mat);


				// d3d用に追加
				motion.d3d_animation_mat.back().bone_list[b] =
					conv_mat;
			}

			// 時間を追加
			time_count += frame_count;
		}
	}

	m_motion[motion_name].emplace_back(motion);

	// カウントリセット
	time_count = start;
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


void Fbx::FbxPolygon3Convert() {

	fbxsdk::FbxGeometryConverter converter(m_fbx_module.mp_manager);
	// マテリアルと1メッシュ毎に分ける
	converter.SplitMeshesPerMaterial(m_fbx_module.mp_fbx_scene, true);
	// 全てを三角形にする
	converter.Triangulate(m_fbx_module.mp_fbx_scene, true);

	// 追加
	converter.RemoveBadPolygonsFromMeshes(
		m_fbx_module.mp_fbx_scene
	);

}


void Fbx::LoadCurrentPath(const std::string &path_name) {

	// ファイルパス分割
	std::vector<std::string>string_list;
	string_list = Utility::SplitStr('/', path_name);

	for (unsigned int i = 0; i < string_list.size() - 1; i++) {
		m_root_path += string_list[i];
	}
}



FbxMatrix Fbx::GetAnimationMatrix(
	FbxMesh* mesh,
	FbxTime& count,
	int deformer_num,
	int cluster_num
) {

	// ここでバグが起こっている

	FbxNode* pNode = m_fbx_module.mp_fbx_scene->GetRootNode();

	// <移動、回転、拡大のための行列を作成>
	FbxMatrix globalPosition = pNode->
		EvaluateGlobalTransform(count);

	// スキン取得
	FbxSkin* skin_deformer = (FbxSkin*)
		mesh->GetDeformer(deformer_num, FbxDeformer::eSkin);

	// スキンが受け取れていない場合
	if (skin_deformer == nullptr) {
		FbxMatrix mat;
		return mat;
	}

	// <クラスタ(ボーン)の取り出し>
	FbxCluster* cluster = skin_deformer->
		GetCluster(cluster_num);

	FbxMatrix clusterGlobalCurrentPosition =
		cluster->GetLink()->
		EvaluateGlobalTransform(count);

	FbxMatrix clusterRelativeCurrentPositionInverse =
		globalPosition.Inverse() *
		clusterGlobalCurrentPosition;

	return clusterRelativeCurrentPositionInverse;

}

void Fbx::Release() {

	m_fbx_module.mp_manager->Destroy();

	m_fbx_module.mp_fbx_scene->Destroy();

	// インポータはファイルを開いてシーンクラスとの橋渡し
	// をしてくれるだけなので、ここで破棄してもいい
	// インポータの明示的な破棄
	m_fbx_module.m_p_importer->Destroy();
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



void Fbx::LoadFileMotion(
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

	//m_start_frame = (int)(start / fps60);
	//m_motion[name].frame_num = (int)((stop - start) / fps60);
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



void Fbx::LoadWeightVertexPoint(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	FbxMeshData* data = &p_mesh_data_list.back();

	// 全ての頂点サイズ(数)を出力
	int size = (UINT)(data->vertex_num * sizeof(SkinCustomVertex));

	// 設定用のバーテックス
	SkinCustomVertex* p_vertics;

	// バッファをロックしてデータを書き込む
	data->p_vertex_buffer->Lock(
		0,
		size,
		(void**)&p_vertics,
		0
	);

	// デバッグ用
	std::vector<std::vector<int>>debug_index_list;
	std::vector<std::vector<float>>debug_weight_list;

	debug_index_list.resize(data->vertex_num);
	debug_weight_list.resize(data->vertex_num);

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

			for (int i = 0; i <
				cluster->GetControlPointIndicesCount();
				i++) {

				double weight = cluster->
					GetControlPointWeights()[i];

				int index = cluster->
					GetControlPointIndices()[i];

				// 重みを回す
				for (int w = 0; w < 4; w++) {
				
					const int CLEAR = 0;

					// 入っているなら
					if (p_vertics[index].weight[w] > 0.f)// 前はindexで調べていた
					{
						// 空きがないので次に行く
						continue;
					}
				
					debug_index_list[index].push_back(ci);

					// 現在のボーン数を代入
					p_vertics[index].bone_index[w] = ci;
				
					
						debug_weight_list[index].
							push_back((float)weight);

						// 重み代入
						p_vertics[index].weight[w] = 
							(float)weight;

					// 代入できたので終了
					break;
				}
			}
		}
	}

	data->p_vertex_buffer->Unlock();
}


void Fbx::CPUSkinning() {

	// 頂点変形
	for (UINT mi = 0;
		mi < m_mesh_data_list.size();
		mi++) {


		// メッシュデータ受け取り
		FbxMeshData mesh_data = m_mesh_data_list[mi];

		// メッシュを取得
		FbxMesh* mesh = 
			m_fbx_module.mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

		// 頂点バッファを定義
		SkinCustomVertex* vertices;

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)((mesh->GetControlPointsCount()) * 
				sizeof(SkinCustomVertex));

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mi].p_vertex_buffer->Lock(
			0, 
			size,
			(void**)&vertices,
			0
		);

		// 最新アニメーション
		WeightSkinning(
			mesh,
			mesh_data,
			vertices,
			mi
		);

		m_mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}

void Fbx::WeightSkinning(
	FbxMesh* mesh,
	FbxMeshData& mesh_data,
	SkinCustomVertex* vertices,
	const int &mi
) {

	// ボーン数受け取り
	int cluster_count = mesh_data.bone_num;

	MotionData *md = &m_motion[m_current_motion_name][mi];

	// 重み分回す
	for (UINT w = 0;
		w < mesh_data.vertex_num;
		w++) {

		D3DXVECTOR4 vec4 =
			m_p_vertics[mi][w].vertex;

		// 一旦初期化
		vertices[w].vertex.x = 0.f;
		vertices[w].vertex.y = 0.f;
		vertices[w].vertex.z = 0.f;
		
		for (int b = 0; b < 4; b++){

			// 中身がないなら
			if (vertices[w].weight[b] <= 0.f) {
				// そのあとの配列にもないと思うので抜ける
				break;
			}

			int w_idx = vertices[w].bone_index[b];
			float weight = vertices[w].weight[b];

			// 初期位置受け取り
			FbxVector4 pos = mesh->GetControlPointAt(
				w
			);

			// 頂点受け取り
			FbxVector4 out_vertex = md->
				animation_matrix[m_count][w_idx].
				MultNormalize(pos);
			
			// 頂点取り出し
			float x = (float)out_vertex[0];
			float y = (float)out_vertex[1];
			float z = (float)out_vertex[2];

			//// d3d用
			//D3DXVECTOR4 out;
			//
			//Model::MatrixMultipleVec4(
			//	out,
			//	vec4,
			//	md->d3d_animation_mat[m_count].bone_list[w_idx]);


			//// 頂点取り出し
			//float x = (float)out.x;
			//float y = (float)out.y;
			//float z = (float)out.z;


			// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
			vertices[w].vertex.x += x * weight;
			vertices[w].vertex.y += y * weight;
			vertices[w].vertex.z += z * weight;
		}
	}
}


void Fbx::FbxMatLConvert(
	FbxMatrix& out_fbx_mat
) {

	// Z軸を反転させる
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}


void Fbx::FbxMatLConvert(
	FbxAMatrix& out_fbx_mat
) {
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}



void Fbx::LoadModelInfo(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// メッシュでのgeo受け取り
	FbxAMatrix geometryOffset = GetGeometry(m_fbx_module.mp_fbx_scene->GetRootNode());

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);


	FbxMeshData * data = &p_mesh_data_list.back();

	int total_weight = 0;

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		FbxSkin* skin_deformer = (FbxSkin*)
			p_mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skin_deformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = skin_deformer->
			GetClusterCount();

		// ボーン数代入
		p_mesh_data_list.back().bone_num = cluster_count;

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skin_deformer->
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

			D3DXMATRIX out_mat;

			// 変換
			FbxMatConvertD3DMat(&out_mat, clusterRelativeInitPosition);

			//// d3d用に追加
			//p_mesh_data_list.back().
			//	d3d_bone_mat_list.
			//	emplace_back(out_mat);
			//
			//// クラスタの追加
			//data->weight_index_list.emplace_back();
			//data->weight_list.emplace_back();

			for (int i = 0; i <
				cluster->GetControlPointIndicesCount();
				i++) {

				double weight = cluster->
					GetControlPointWeights()[i];

				//// 重み
				//data->
				//weight_list.back().push_back(weight);
				//
				//int index = cluster->
				//	GetControlPointIndices()[i];
				//
				//// 重みインデックス
				//data->
				//weight_index_list.back().push_back(index);

				total_weight++;

			}
		}
	}

	total_weight = total_weight;
}

// 前の
//std::vector<FbxMatrix>new_mat;
//new_mat.resize(cluster_count);
//
//// ボーン数分回す
//for (int cnt = 0;
//	cnt < cluster_count;
//	cnt++) {
//
//	// アニメーション行列
//	FbxMatrix anim_mat = GetAnimationMatrix(mesh,m_time_count, 0, cnt);
//
//	// ボーン行列変換(アニメーション行列 * ボーン行列)
//	new_mat[cnt] = 
//		anim_mat * m_mesh_data_list[mi].bone_list[cnt];
//}



/*
void Fbx::WeightSkinning1(
	FbxMesh*mesh,
	FbxMeshData&mesh_data,
	SkinCustomVertex*vertices,
	const int mi
) {

	// スキン
	FbxSkin* skinDeformer = (FbxSkin*)
		mesh->GetDeformer(0, FbxDeformer::eSkin);


	for (int cnt = 0;
		cnt < skinDeformer->GetClusterCount();
		cnt++) {


		for (UINT w = 0;
			w < mesh_data.weight_index_list[cnt].size();
			w++) {

			// 影響するインデックス受け取り
			int w_idx = mesh_data.weight_index_list[cnt][w];

			vertices[w_idx].vertex.x = 0;
			vertices[w_idx].vertex.y = 0;
			vertices[w_idx].vertex.z = 0;
		}
	}

	int cluster_count = skinDeformer->GetClusterCount();

	for (int cnt = 0;
		cnt < cluster_count;
		cnt++) {

		// アニメーション行列
		FbxMatrix anim_mat = GetAnimationMatrix(mesh, m_time_count,0, cnt);

		// ボーン行列変換(アニメーション行列 * ボーン行列)
		FbxMatrix new_mat =
			anim_mat * m_mesh_data_list[mi].bone_list[cnt];


		for (UINT w = 0;
			w < mesh_data.weight_index_list[cnt].size();
			w++) {

			// 影響するインデックス受け取り
			int w_idx = mesh_data.weight_index_list[cnt][w];

			FbxVector4 pos = mesh->GetControlPointAt(
				w_idx
			);

			FbxVector4 out_vertex =
				new_mat.MultNormalize
				(pos);

			float x = (float)out_vertex[0];
			float y = (float)out_vertex[1];
			float z = (float)out_vertex[2];

			// 重み取り出し
			float weight =
				(float)mesh_data.weight_list[cnt][w];

			// 重みを掛け合わせ
			vertices[w_idx].vertex.x += x * weight;
			vertices[w_idx].vertex.y += y * weight;
			vertices[w_idx].vertex.z += z * weight;

		}
	}

}*/



/*
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
}*/



// 旧スキニング
/*

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



*/