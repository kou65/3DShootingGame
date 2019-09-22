#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<vector>
#include"../../Texture/TextureManager/TextureManager.h"
#include"OBJFile.h"



ObjFile::ObjFile() {

	m_p_graphics = Graphics::GetInstance();
	m_total_vertex_num = 0;
	m_total_face_num = 0;
}


void ObjFile::DrawSubSet(
	int material_num,
	float pos_x,
	float pos_y,
	float pos_z
	){


	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 移動
	D3DXMatrixTranslation(&mat, pos_x, pos_y, pos_z);

	m_p_graphics->GetInstance()->GetLpDirect3DDevice9()
		->SetTransform(D3DTS_WORLD, &mat);

	if (m_p_vertex_buffer == nullptr) {
		return;
	}

	// ストリームをセット
	m_p_graphics->GetLpDirect3DDevice9()->SetStreamSource(
		0,
		m_p_vertex_buffer,
		0,
		sizeof(MeshCustomVertex)
	);
	
	// マテリアル名
	std::string material_name = m_usemtl_name_list[material_num];

	// テクスチャが存在しているなら
	if (TextureManager::GetInstance()->Find2DTexture(m_material_data_list[material_name].texture_name) == true) {
	
		// テクスチャ管理者からテクスチャ受け取り
		TextureData2D texture_data = TextureManager::GetInstance()
			->GetTextureData2D(m_material_data_list[material_name].texture_name);
	
		// テクスチャセット
		m_p_graphics->GetLpDirect3DDevice9()->SetTexture(
			0,
			texture_data
		);
	}

	// マテリアルをセット
	m_p_graphics->GetLpDirect3DDevice9()->SetMaterial(
		&m_material_data_list[material_name].material_color
	);

	// インデックス番号をデバイスに設定する
	m_p_graphics->GetLpDirect3DDevice9()->SetIndices(
		m_p_index_buffer
	);

	// インデックスをオフセットする
	int BASE_VERTEX_INDEX = 0;

	// 描画に使用する最小のインデックス番号
	int MIN_INDEX = m_object_sub_set_list[material_num].face_start;

	// 描画を開始する頂点インデックスまでのオフセット値を指定
	// 途中から描画したい場合に有効
	int START_INDEX = m_object_sub_set_list[material_num].face_start;

	// 三角ポリゴン頂点数
	int NUM_VERTEX = m_object_sub_set_list[material_num].face_start * 3;

	// START_INDEXを先頭として描画するポリゴンの数を指定する
	unsigned int PRIMITIVE_COUNT = m_object_sub_set_list[material_num].face_count;

	// どの情報を伝えるか
	m_p_graphics->GetLpDirect3DDevice9()->SetFVF(FVF_CUSTOM);

	// インデックス描画
	m_p_graphics->GetLpDirect3DDevice9()->DrawIndexedPrimitive(
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

	// テクスチャリセット
	m_p_graphics->GetLpDirect3DDevice9()->SetTexture(
		0,
		NULL
	);
}


bool ObjFile::Load(
	std::string obj_file_path,
	std::string texture_file_path,
	int &out_total_material_num) {

	std::vector<std::string>split_file_name;
	split_file_name = Utility::SplitStr('/',obj_file_path);

	// 頂点読み込みバッファ
	std::vector<D3DXVECTOR3>vertex_list;

	// 法線読み込みバッファ
	std::vector<D3DXVECTOR3>normal_list;

	// テクスチャ座標バッファ
	std::vector<D3DXVECTOR2>uv_list;

	// 面情報バッファ[usemtl][FacePolygon]
	std::vector<std::vector<FacePolygon>>face_list;

	// マテリアルデータバッファ
	std::vector<MaterialData>material_data_list;

	// マテリアル名
	std::string material_name;

	// メイン読み込み
	if (MeshLoad(
		obj_file_path,
		texture_file_path,
		material_name,
		out_total_material_num,
		vertex_list,
		normal_list,
		uv_list,
		face_list
		) == false) {
		return false;
	}

	std::string file_path_material_name;

	for (unsigned int i = 0; i < split_file_name.size() - 1; i++) {
		 file_path_material_name += split_file_name[i];
	}

	// ファイルパス付のマテリアル名にする
	file_path_material_name += material_name;

	// マテリアル読み込み
	MaterialFileLoad(file_path_material_name,texture_file_path);

	// 頂点数
	int vertex_num = 0;

	// 面数
	for (int i = 0; i < face_list.size(); i++) {
		vertex_num += (int)face_list[i].size();
	}

	// バーテックスバッファ生成
	VertexBufferCreate(
		vertex_num,
		vertex_list,
		normal_list,
		uv_list,
		face_list
	);

	// インデックスバッファ生成
	IndexBufferCreateFaceBase(
		vertex_num * 2,
		face_list);

	return true;
}



bool ObjFile::MeshLoad(
	std::string file_path,
	std::string texture_file_path,
	std::string&out_material_name,
	int &out_total_material_num,
	std::vector<D3DXVECTOR3>&out_vertex_list,
	std::vector<D3DXVECTOR3>&out_normal_list,
	std::vector<D3DXVECTOR2>&out_uv_list,
	std::vector<std::vector<FacePolygon>>&out_face_list
	) {

	// ファイルストリーム
	FILE *p_file;

	HRESULT result;

	std::string obj_file_path = file_path;

	// ファイルオープン
	result = fopen_s(&p_file, obj_file_path.c_str(), "r");

	if (result != D3D_OK) {
		Window::TextMessageBox("OBJファイルオープンに失敗しました");
		return false;
	}

	// とりあえず256で
	const int BUFFER = 256;

	char front_str[BUFFER];

	// 分割用の文字列配列
	std::vector<std::string>split_str;

	while (feof(p_file) == 0) {

		// 初期化
		front_str[0] = '\0';

		// 先頭の情報を取得する
		fscanf_s(p_file, "%s ", (char*)front_str, BUFFER);

		// 先頭文字列がコメントならはじく
		if (front_str[0] == '#') {
			continue;
		}
		
		// 頂点関係なら
		else if (front_str[0] == 'v') {

			// 頂点関係読み込み
			VertexInfoLoad(
				p_file,
				front_str,
				out_vertex_list,
				out_uv_list,
				out_normal_list
			);
		}
		
		// 面情報なら
		else if (front_str[0] == 'f' && front_str[1] == '\0') {

			// 面情報読み込み
			FaceInfoLoad(
				p_file,
				out_face_list
			);
		}
		// マテリアルファイル名読み込み
		else if (strcmp(front_str,"mtllib") == 0) {
		
			fscanf_s(p_file, "%s",&front_str,BUFFER);
			out_material_name = front_str;
		}
		// 使用するマテリアル情報なら要素加算
		else if (strcmp(front_str, "usemtl") == 0) {

			// 使用マテリアル読み込み
			UseMaterialInfoLoad(
				p_file,
				out_total_material_num,
				out_face_list,
				front_str,
				BUFFER
			);
		}

		
	}

	// ファイルを閉じる
	fclose(p_file);

	return true;
}


void ObjFile::VertexFscanfLoad(
	FILE*p_file,
	D3DXVECTOR3 &vec3) {

	// x軸,y軸,z軸を取得
	fscanf_s(
		p_file, "%f %f %f",
		&vec3.x,
		&vec3.y,
		&vec3.z);
}


void ObjFile::VertexInfoLoad(
	FILE *p_file,
	const char*front_str,
	std::vector<D3DXVECTOR3>&out_vertex_list,
	std::vector<D3DXVECTOR2>&out_uv_list,
	std::vector<D3DXVECTOR3>&out_normal_list
) {

	D3DXVECTOR3 vec3;

	// x軸,y軸,z軸を読み込み
	VertexFscanfLoad(p_file, vec3);

	// 頂点なら
	if (front_str[1] == '\0') {

		// *-1.fで左手系に変換できる
		vec3.x *= -1.f;

		out_vertex_list.push_back(vec3);
	}
	// 法線なら
	else if (front_str[1] == 'n') {

		// 反転
		vec3.x *= -1.f;

		out_normal_list.push_back(vec3);
	}
	// テクスチャ座標なら
	else if (front_str[1] == 't') {

		// uvのみy軸で反転
		vec3.y *= -1.f;

		// vec3をvec2にする
		D3DXVECTOR2 vec2(vec3.x,vec3.y);

		out_uv_list.push_back(vec2);
	}
}



void ObjFile::FaceInfoLoad(
	FILE*p_file,
	std::vector<std::vector<FacePolygon>>&out_face_list
) {

	// とりあえず256 * 8バイト
	const int LOAD_BUFFER = 256;

	// 読み込み文字列バッファ
	char load_str[LOAD_BUFFER] = {};

	// 空白分割文字列バッファ
	std::vector<std::string>space_split_str;

	// 文字列面情報バッファ
	std::vector<std::vector<std::string>>face_info_str;

	// ファイル読み込み
	fgets(
		load_str,
		LOAD_BUFFER,
		p_file
	);

	// 要素追加
	space_split_str.emplace_back();

	// 空白で文字列分割
	space_split_str = Utility::SplitStr(' ', load_str);

	int face_num = 0;

	for (auto &str : space_split_str) {

		// 面情報文字列
		face_info_str.emplace_back();

		// 面情報文字列
		face_info_str[face_num].emplace_back();

		// /で文字列分割
		face_info_str[face_num] = Utility::SplitStr('/', str);

		// 面数加算
		face_num++;
	}

	// 仮の入れ物
	std::vector<FacePolygon>prov_face;

	// 面情報を代入
	prov_face = InsertFaceList(
		face_info_str
	);

	// 総面数加算
	m_total_face_num++;

	// 頂点が4つあるなら
	if (face_num >= 4) {

		// 総面数加算
		m_total_face_num++;
		
		// 変換させる
		prov_face = Face4CutToFace3(prov_face);
	}

	// 面情報代入
	for (auto face : prov_face){

		// 表示する面を加算
		m_object_sub_set_list.back().face_count++;

		// 総頂点数加算
		m_total_vertex_num++;

		// [マテリアル][面情報]という感じに入れる
		out_face_list.back().emplace_back(face);
	}
}


std::vector<FacePolygon> ObjFile::InsertFaceList(
	std::vector<std::vector<std::string>>face_info_str
) {

	// 読み取れない場合
	const int NONE = -1;
	
	// 頂点番号用(-1は情報を受け取れなかった初期状態)
	int vertex_info[3] = 
	{
		NONE,NONE,NONE,
	};

	int subject_vertex = 0;

	// 仮の入れ物
	std::vector<FacePolygon>prov_face;

	// 面情報文字列
	for (unsigned int i = 0;
		i < face_info_str.size();
		i++) {

		// 法線がない場合
		if (face_info_str[1].size() == 2) {
			subject_vertex = 1;
		}

		// 面情報要素追加
		prov_face.emplace_back();

		// 軸分回す
		for (int j = 0; j < TOTAL_FACE_INFO - subject_vertex; j++) {

			// 頂点情報に変換する
			Vector3ConversionByString(vertex_info[j],face_info_str[i][j]);

			// 面情報へ代入
			switch (j) {

			case VERTEX:
				prov_face[i].pos_num = vertex_info[j];
				break;

			case UV:
				prov_face[i].uv_num = vertex_info[j];
				break;

			case NORMAL:
				prov_face[i].normal_num = vertex_info[j];
				break;
			}
		}
	}

	return prov_face;
}



bool ObjFile::MaterialFileLoad(
	std::string mtl_file_name,
	std::string texture_file_path
) {

	const int BUFFER = 256;

	char load_str[BUFFER];

	// 文字列分割用
	std::vector<std::string>str_list;

	// テクスチャ文字列読み取り用
	std::string texture_str;

	const int COLOR_NUM = 3;

	// カラー情報を保存する
	float color_info[COLOR_NUM] = {};

	std::ifstream ifs(mtl_file_name);

	if (ifs.fail() == true) {
		return false;
	}

	// ファイル読み込み
	while (ifs.getline(load_str, BUFFER - 1))
	{

		str_list = Utility::SplitStr(' ', load_str);

		// 新しいマテリアル
		if (strcmp(str_list[0].c_str(), "newmtl ") == 0) {

			// テクスチャ名受け取り
			m_material_data_list[str_list[1].c_str()].texture_name
				= str_list[1].c_str();
			texture_str = str_list[1].c_str();

			// 使わない色を初期化
			D3DCOLORVALUE color = {
				100.f,
				100.f,
				100.f,
				1.0f
			};
			m_material_data_list[texture_str].
				material_color.Emissive = color;
			m_material_data_list[texture_str].
				material_color.Power = 0.f;
		}
		// アンビエントカラー
		else if (strcmp(str_list[0].c_str(), "Ka ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_material_data_list[texture_str].
				material_color.Ambient = color;
		}
		// ディフューズカラー
		else if (strcmp(str_list[0].c_str(), "Kd ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_material_data_list[texture_str].
				material_color.Diffuse = color;
		}
		// スペキュラーカラー
		else if (strcmp(str_list[0].c_str(), "Ks ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_material_data_list[texture_str].
				material_color.Specular = color;
		}
		// α値なら
		else if (strcmp(str_list[0].c_str(), "d ") == 0) {

			// α値設定
			//m_material_data_list[texture_str].
			//	material_color.Ambient.r = (float)atof(str_list[1].c_str());

			//m_material_data_list[texture_str].
			//	material_color.Diffuse.r = (float)atof(str_list[1].c_str());

			//m_material_data_list[texture_str].
			//	material_color.Specular.r = (float)atof(str_list[1].c_str());

		}
		// テクスチャなら
		else if (strcmp(str_list[0].c_str(), "map_Kd ") == 0) {

			// テクスチャ名がないなら
			if (str_list[1].empty() == true) {
				continue;
			}

			std::string texture_name = texture_file_path + str_list[1];

			// テクスチャ名代入
			m_material_data_list[texture_str].texture_name = 
				 texture_str;

			// テクスチャ読み込み
			TextureManager::GetInstance()->Load2D(
				texture_name.c_str(),
				m_material_data_list[texture_str].texture_name.c_str());
		}
	}

	return true;
}


void ObjFile::UseMaterialInfoLoad(
	FILE*p_file,
	int &out_total_material_num,
	std::vector<std::vector<FacePolygon>>&out_face_list,
	char*front_str,
	int load_buffer
) {


	// マテリアル
	{
		// マテリアル数加算
		out_total_material_num++;

		// ファイル読み込み
		fscanf_s(p_file, "%s", front_str, load_buffer);

		// 面情報数受け取り
		m_usemtl_name_list.push_back(front_str);
	}

	// DrawSubSet
	{
		// 要素追加
		m_object_sub_set_list.emplace_back();

		// マテリアル数加算
		m_object_sub_set_list.back().material_index =
			out_total_material_num;

		// 最初から加算していく
		m_object_sub_set_list.back().face_count =
			0;

		// 最初の面を入れる
		m_object_sub_set_list.back().face_start =
			m_total_vertex_num;
	}

	// 面配列を追加
	{
		out_face_list.emplace_back();
	}
}


bool ObjFile::Vector3ConversionByString(
	int &axis,
	const std::string &string
) {

	if (string.empty() == true) {
		//Window::TextMessageBox("文字列分割に失敗しました");
		return false;
	}

	// 数字がないなら
	if (string == "/") {
		return false;
	}
	else {
		// 小数点変換
		axis = atoi(string.c_str());
	}

	return true;
}


std::vector<FacePolygon> ObjFile::Face4CutToFace3(
	std::vector<FacePolygon>vertex4_polygon_list
) {

	// 4面の場合は3面にする

	// 面情報受け取り用
	std::vector<FacePolygon>get_face_list;

	// 2ポリゴン用意
	const int POLYGON2 = 2;

	// 代入する頂点(4つ分)
	const int ENTRY_VERTEX_NUM[6] = 
	{
		// 1面
		0,1,3,
		// 2面
		3,1,2 
	};

	// 頂点分回す
	for (int j = 0; j < (3 * 2); j++) {

		// 配列要素追加
		get_face_list.emplace_back();

		// 3頂点追加
		get_face_list[j] = vertex4_polygon_list[ENTRY_VERTEX_NUM[j]];
	}

	return get_face_list;
}


// 頂点バッファ生成
void ObjFile::VertexBufferCreate(
	int vertex_num,
	std::vector<D3DXVECTOR3>vertex_list,
	std::vector<D3DXVECTOR3>normal_list,
	std::vector<D3DXVECTOR2>uv_list,
	std::vector<std::vector<FacePolygon>>face_list
) {

	// 頂点バッファ作成
	m_p_graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		(sizeof(MeshCustomVertex) * vertex_num),
		// リソースの使用法
		0,
		// 柔軟な頂点フォーマットの型を指定する
		0,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&m_p_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	MeshCustomVertex *custom_vertex_list;

	// ロック
	m_p_vertex_buffer->Lock(
		0,
		vertex_num * sizeof(MeshCustomVertex),
		(void**)&custom_vertex_list,
		0
	);

	// ここで頂点情報を代入させる

	int count = 0;

	const int OFFSET = 1;

	// 頂点分回す
	for (int i = 0; i < face_list.size(); i++) {
	
		for (int j = 0; j < face_list[i].size(); j++) {
	
			// 面情報受け取り
			int pos_num = face_list[i][j].pos_num - 1;
	
			// 面情報に適した頂点情報受け取り
			custom_vertex_list[count].position =
				vertex_list[pos_num];
	
			count++;
		}
	}

	count = 0;

	// UV分回す
	for (int i = 0; i < face_list.size(); i++) {
	
		for (int j = 0; j < face_list[i].size(); j++) {
	
			// 面情報受け取り
			int uv_num = face_list[i][j].uv_num - OFFSET;
	
			// 面情報に適した頂点情報受け取り
			custom_vertex_list[count].uv = uv_list[uv_num];
	
			count++;
		}
	}

	count = 0;

	// 法線分回す
	for (int i = 0; i < face_list.size();i++) {
		for (int j = 0; j < face_list[i].size(); j++) {
	
			// 法線数
			int normal_num = face_list[i][j].normal_num;
	
			// 法線
			custom_vertex_list[i].normal =
				normal_list[normal_num - OFFSET];
		}
	}

	// アンロック
	m_p_vertex_buffer->Unlock();
}


// 面情報から頂点情報埋め込み
bool ObjFile::IndexBufferCreateFaceBase(
	int face_num,
	std::vector<std::vector<FacePolygon>>face_list
) {

	// インデックスバッファ作成
	m_p_graphics->GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		(face_num * sizeof(int)),
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX32,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		&m_p_index_buffer,
		// 現在使用されていないので基本NULL
		NULL
	);

	// nullチェック
	if (m_p_index_buffer == nullptr) {
		return false;
	}

	// 頂点(このサイズが適応する)
	int * index_vertex;

	if (FAILED(m_p_index_buffer->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&index_vertex,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		0
	)
	)
		) {
		return false;
	}

	int count = 0;

	std::vector<UINT>index_list;

	// とりあえず頂点インデックスをセット
	
	// 面数
	for (int i = 0; i < face_list.size(); i++) {

		for (int j = 0; j < face_list[i].size(); j++) {

			// インデックス追加
			index_list.push_back(count);

			count++;

			// ポリゴン生成を反転
			if (count % 3 == 0){
				
				UINT temp = 0;

				// 反転
				temp = index_list[(count - 3)];
				index_list[(count - 3)] = index_list[(count - 3) + 2];
				index_list[(count - 3) + 2] = temp;

				// 代入
				for (int k = 0; k < 3; k++) {
					index_vertex[(count - 3) + k] = 
					index_list[(index_list.size() - 3) + k];
				}
			}
		}
	}
	
	// nullチェック
	if (index_vertex == nullptr) {
		return false;
	}

	// アンロック
	m_p_index_buffer->Unlock();

	return true;
}
