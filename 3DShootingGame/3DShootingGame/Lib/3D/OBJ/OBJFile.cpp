#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<vector>
#include"../../Texture/TextureManager/TextureManager.h"
#include"OBJFile.h"



ObjFile::ObjFile() {

	m_p_graphics = Graphics::GetInstance();
}


void ObjFile::DrawSubSet(
	const std::string &register_name,
	int material_num,
	float pos_x,
	float pos_y,
	float pos_z
	){

	// キーが登録されていないなら描画しない
	if (m_obj_file_data.count(register_name) == 0) {
		return;
	}
	// 生成されていないなら描画しない
	if (m_obj_file_data[register_name] == nullptr) {
		return;
	}

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData *p_obj_file_data = m_obj_file_data[register_name];

	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 移動
	D3DXMatrixTranslation(&mat, pos_x, pos_y, pos_z);

	m_p_graphics->GetInstance()->GetLpDirect3DDevice9()
		->SetTransform(D3DTS_WORLD, &mat);

	if (p_obj_file_data->m_p_vertex_buffer == nullptr) {
		return;
	}

	// ストリームをセット
	m_p_graphics->GetLpDirect3DDevice9()->SetStreamSource(
		0,
		p_obj_file_data->m_p_vertex_buffer,
		0,
		sizeof(MeshCustomVertex)
	);
	
	// マテリアル名
	std::string material_name = p_obj_file_data->m_usemtl_name_list[material_num];

	// テクスチャが存在しているなら
	if (TextureManager::GetInstance()->Find2DTexture(
		p_obj_file_data->m_material_data_list[material_name].texture_name
	) == true) {
	
		// テクスチャ管理者からテクスチャ受け取り
		TextureData2D texture_data = TextureManager::GetInstance()
			->GetTextureData2D(
				p_obj_file_data->m_material_data_list[material_name].texture_name
			);
	
		// テクスチャセット
		m_p_graphics->GetLpDirect3DDevice9()->SetTexture(
			0,
			texture_data
		);
	}

	// マテリアルをセット
	m_p_graphics->GetLpDirect3DDevice9()->SetMaterial(
		&p_obj_file_data->m_material_data_list[material_name].material_color
	);

	// インデックス番号をデバイスに設定する
	m_p_graphics->GetLpDirect3DDevice9()->SetIndices(
		p_obj_file_data->m_p_index_buffer
	);

	// インデックスをオフセットする
	int BASE_VERTEX_INDEX = 0;

	// 描画に使用する最小のインデックス番号
	int MIN_INDEX = p_obj_file_data->
		m_object_sub_set_list[material_num].face_start;

	// 描画を開始する頂点インデックスまでのオフセット値を指定
	// 途中から描画したい場合に有効
	int START_INDEX = p_obj_file_data->
		m_object_sub_set_list[material_num].face_start;

	// 三角ポリゴン頂点数
	int NUM_VERTEX = p_obj_file_data->
		m_object_sub_set_list[material_num].face_count * 3;

	// START_INDEXを先頭として描画するポリゴンの数を指定する
	unsigned int PRIMITIVE_COUNT = p_obj_file_data->
		m_object_sub_set_list[material_num].face_count;

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

	// セットテクスチャリセット
	m_p_graphics->GetLpDirect3DDevice9()->SetTexture(
		0,
		NULL
	);
}


bool ObjFile::Load(
	const std::string &obj_file_path,
	const std::string &register_name,
	const std::string &texture_file_path,
	int &out_total_material_num) {

	std::vector<std::string>split_file_name;
	split_file_name = Utility::SplitStr('/',obj_file_path);

	// マテリアルデータバッファ
	std::vector<MaterialData>material_data_list;

	// マテリアル名
	std::string material_name;

	// 頂点バッファ
	std::vector<MeshCustomVertex>mesh_vertex_list;

	// 面情報バッファ
	std::vector<INT>indices;

	// オブジェクトファイルデータを生成
	m_obj_file_data[register_name] = new ObjFileData;

	// メイン読み込み
	if (MeshLoad(
		obj_file_path,
		register_name,
		texture_file_path,
		material_name,
		out_total_material_num,
		indices,
		mesh_vertex_list) == false) {
		return false;
	}

	std::string file_path_material_name;

	for (unsigned int i = 0; i < split_file_name.size() - 1; i++) {
		 file_path_material_name += split_file_name[i];
	}

	// ファイルパス付のマテリアル名にする
	file_path_material_name += material_name;

	// マテリアル読み込み
	MaterialFileLoad(
		file_path_material_name,
		register_name,
		texture_file_path);

	
	// バーテックスバッファ生成
	VertexBufferCreate(
		(int)mesh_vertex_list.size(),
		register_name,
		mesh_vertex_list
	);

	// インデックスバッファ生成
	IndexBufferCreateFaceBase(
		(int)indices.size(),
		register_name,
		indices
		);

	return true;
}



bool ObjFile::MeshLoad(
	const std::string &file_path,
	const std::string &registr_name,
	const std::string &texture_file_path,
	std::string&out_material_name,
	int &out_total_material_num,
	std::vector<INT>&indices,
	std::vector<MeshCustomVertex>&mesh_list
	) {

	// 頂点読み込みバッファ
	std::vector<D3DXVECTOR3>vertex_list;

	// 法線読み込みバッファ
	std::vector<D3DXVECTOR3>normal_list;

	// テクスチャ座標バッファ
	std::vector<D3DXVECTOR2>uv_list;

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
				vertex_list,
				uv_list,
				normal_list
			);
		}
		
		// 面情報なら
		else if (front_str[0] == 'f' && front_str[1] == '\0') {

			// 面情報読み込み
			FaceInfoLoad(
				p_file,
				registr_name,
				indices,
				mesh_list,
				vertex_list,
				uv_list,
				normal_list
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
				registr_name,
				out_total_material_num,
				indices,
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
	const std::string &register_name,
	std::vector<INT>&indices,
	std::vector<MeshCustomVertex>&mesh_list,
	const std::vector<D3DXVECTOR3>&vertex_list,
	const std::vector<D3DXVECTOR2>&uv_list,
	const std::vector<D3DXVECTOR3>&normal_list
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
	std::vector<INT>prov_face;

	// 面情報を代入
	InsertFaceList(
		face_info_str,
		mesh_list,
		indices,
		vertex_list,
		uv_list,
		normal_list,
		m_obj_file_data[register_name]->m_object_sub_set_list
	);

}


void ObjFile::InsertFaceList(
	const std::vector<std::vector<std::string>>&face_info_str,
	std::vector<MeshCustomVertex>&custom_vertex,
	std::vector<INT>&indices,
	const std::vector<D3DXVECTOR3>&vertex_list,
	const std::vector<D3DXVECTOR2>&uv_list,
	const std::vector<D3DXVECTOR3>&normal_list,
	std::vector<ObjectSubset>&out_draw_sub_set
) {

	// 読み取れない場合
	const int NONE = -1;
	
	// 頂点番号用(-1は情報を受け取れなかった初期状態)
	int vertex_info[3] = 
	{
		NONE,NONE,NONE,
	};

	int subject_vertex = 0;
	std::vector<INT>prov_face;

	// 面情報文字列
	for (unsigned int i = 0;
		i < face_info_str.size();
		i++) {

	
		MeshCustomVertex mesh_vertex;

		// 法線がない場合
		if (face_info_str[1].size() == 2) {
			subject_vertex = 1;
		}

		// 軸分回す
		for (int j = 0; j < TOTAL_FACE_INFO - subject_vertex; j++) {

			// 頂点情報に変換する
			Vector3ConversionByString(vertex_info[j],face_info_str[i][j]);

			int id = vertex_info[j] - 1;

			// 面情報へ代入
			switch (j) {

			case VERTEX:
				mesh_vertex.position = vertex_list[id];
				break;

			case UV:
				mesh_vertex.uv = uv_list[id];
				break;

			case NORMAL:
				mesh_vertex.normal = normal_list[id];
				break;
			}
		}

		custom_vertex.push_back(mesh_vertex);
		
		prov_face.push_back((int)custom_vertex.size() - 1);
		
	}

	// 頂点数が4以上なら
	if (face_info_str.size() >= 4) {

		// 面を切り分ける
		prov_face = Vertex4CutToVertex3Face(prov_face);

		for (unsigned int i = 0; i < prov_face.size(); i++) {

			// 現在のサブセットリストの面数を加算
			out_draw_sub_set.back().face_count++;

			// インデックスに追加
			indices.push_back(prov_face[i]);

			if (i == 2 || i == 5) {
				// ポリゴンの作成の頂点順番を反転する
				int size = (int)indices.size();
				unsigned int temp = indices[(int)size - 1];
				indices[size - 1] = indices[size - 3];
				indices[size - 3] = temp;
			}
		}
	}
	// 3頂点なら
	else {

		for (unsigned int i = 0; i < prov_face.size(); i++) {

			// 現在のサブセットリストの面数を加算
			out_draw_sub_set.back().face_count++;

			indices.push_back(prov_face[i]);
		}

		// ポリゴンの作成の頂点順番を反転する
		int size = (int)indices.size();
		int temp = indices[size - 1];
		indices[size - 1] = indices[size - 3];
		indices[size - 3] = temp;
	}
}


bool ObjFile::MaterialFileLoad(
	const std::string &mtl_file_name,
	const std::string &register_name,
	const std::string &texture_file_path
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

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	std::ifstream ifs(mtl_file_name);

	// ファイル読み込みに失敗した場合
	if (ifs.fail() == true) {
		return false;
	}

	// ファイル読み込み
	while (ifs.getline(load_str, BUFFER - 1))
	{

		// 文字列分割
		str_list = Utility::SplitStr(' ', load_str);

		// 新しいマテリアル
		if (strcmp(str_list[0].c_str(),"newmtl ") == 0) {

			// テクスチャ名受け取り
			p_obj_file_data->
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
			p_obj_file_data->m_material_data_list[texture_str].
				material_color.Emissive = color;
			p_obj_file_data->m_material_data_list[texture_str].
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
			p_obj_file_data->m_material_data_list[texture_str].
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
			p_obj_file_data->m_material_data_list[texture_str].
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
			p_obj_file_data->m_material_data_list[texture_str].
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
			p_obj_file_data->m_material_data_list[texture_str].texture_name =
				 texture_str;

			// テクスチャ読み込み
			TextureManager::GetInstance()->Load2D(
				texture_name.c_str(),
				p_obj_file_data->m_material_data_list[texture_str].texture_name.c_str());
		}
	}

	return true;
}


void ObjFile::UseMaterialInfoLoad(
	FILE*p_file,
	const std::string &register_name,
	int &out_total_material_num,
	std::vector<INT>&indices,
	char*front_str,
	int load_buffer
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	// マテリアル
	{
		// マテリアル数加算
		out_total_material_num++;

		// ファイル読み込み
		fscanf_s(p_file, "%s", front_str, load_buffer);

		// 面情報数受け取り
		p_obj_file_data->m_usemtl_name_list.push_back(front_str);
	}

	// DrawSubSet
	{
		// 要素追加
		p_obj_file_data->m_object_sub_set_list.emplace_back();

		// マテリアル数加算
		p_obj_file_data->m_object_sub_set_list.back().material_index =
			out_total_material_num;

		// 最初から加算していく
		p_obj_file_data->m_object_sub_set_list.back().face_count =
			0;

		// 最初の面を入れる
		p_obj_file_data->m_object_sub_set_list.back().face_start =
			(int)indices.size();
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


std::vector<INT> ObjFile::Vertex4CutToVertex3Face(
	const std::vector<INT>&vertex4_polygon_list
) {

	// 4面の場合は3面にする

	// 面情報受け取り用
	std::vector<INT>get_face_list;

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
	const int &vertex_num,
	const std::string &register_name,
	const std::vector<MeshCustomVertex>&mesh_vertex_list
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

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
		&p_obj_file_data->m_p_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	MeshCustomVertex *custom_vertex_list;

	// ロック
	p_obj_file_data->m_p_vertex_buffer->Lock(
		0,
		vertex_num * sizeof(MeshCustomVertex),
		(void**)&custom_vertex_list,
		0
	);

	// ここで頂点情報を代入させる

	int count = 0;

	const int OFFSET = 1;

	for (unsigned int i = 0; i < mesh_vertex_list.size(); i++) {
		custom_vertex_list[i] = mesh_vertex_list[i];
	}

	// アンロック
	p_obj_file_data->m_p_vertex_buffer->Unlock();
}


// 面情報から頂点情報埋め込み
bool ObjFile::IndexBufferCreateFaceBase(
	const int &face_num,
	const std::string &register_name,
	const std::vector<INT>&indices
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	// unsigned shortサイズ(2バイト)
	const int TWO_BYTE = 32768 * 2;

	// 面数が2バイトを超えているなら32フォーマットサイズにする
	if (face_num >= TWO_BYTE) {

		// 頂点32bit
		int * index_buffer;

		// 32bitサイズのインデックスバッファを作成
		m_p_graphics->CreateIndexBuffer32BitSize(
			&p_obj_file_data->m_p_index_buffer,
			(face_num * sizeof(int))
		);

		// ロック
		index_buffer = m_p_graphics->LockIndexBuffer32BitSize(
			&p_obj_file_data->m_p_index_buffer
		);


		// nullチェック
		if (index_buffer == nullptr) {
			return false;
		}

		// 面数
		for (unsigned int i = 0; i < indices.size(); i++) {
			index_buffer[i] = indices[i];
		}
	}
	// 16bitサイズなら
	else {

		// 頂点16bit
		WORD * index_buffer;

		// 32bitサイズのインデックスバッファを作成
		m_p_graphics->CreateIndexBuffer16BitSize(
			&p_obj_file_data->m_p_index_buffer,
			(face_num * sizeof(WORD))
		);

		// ロック
		index_buffer = m_p_graphics->LockIndexBuffer16BitSize(
			&p_obj_file_data->m_p_index_buffer
		);


		// nullチェック
		if (index_buffer == nullptr) {
			return false;
		}

		// 面数
		for (unsigned int i = 0; i < indices.size(); i++) {
			index_buffer[i] = indices[i];
		}

	}

	// nullチェック
	if (p_obj_file_data->m_p_index_buffer == nullptr) {
		return false;
	}

	// アンロック
	m_p_graphics->UnlockIndexBuffer(
		&p_obj_file_data->m_p_index_buffer
	);

	return true;
}
