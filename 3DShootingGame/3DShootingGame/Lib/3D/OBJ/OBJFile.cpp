#include"ObjFile.h"
#include<string>
#include<sstream>
#include<vector>
#include"../../Texture/TextureManager/TextureManager.h"


ObjFile::ObjFile() {

	m_p_graphics = Graphics::GetInstance();
	texture_file_name = "";
}


void ObjFile::Draw(int material_num) {


	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
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
		sizeof(Object3DCustomVertex)
	);

	// テクスチャが存在しているなら
	//if (TextureManager::GetInstance()->Find3DTexture(texture_file_name) == false) {
	//
	//	// テクスチャ管理者からテクスチャ受け取り
	//	TextureData3D texture_data = TextureManager::GetInstance()
	//		->GetTextureData3D(texture_file_name);
	//
	//	// テクスチャセット
	//	m_p_graphics->GetLpDirect3DDevice9()->SetTexture(
	//		0,
	//		texture_data.texture_list[material_num]
	//	);
	//}

	// インデックス番号をデバイスに設定する
	m_p_graphics->GetLpDirect3DDevice9()->SetIndices(
		m_p_index_buffer
	);

	// インデックスをオフセットする
	const int OFFSET_INDEX = 0;

	// 描画に使用する最小のインデックス番号
	const int MIN_INDEX = 0;

	// 描画を開始する頂点インデックスまでのオフセット値を指定
	// 途中から描画したい場合に有効
	const int START_INDEX = 0;

	// 三角ポリゴン頂点数
	const int VERTEX_NUM = 3;

	// START_INDEXを先頭として描画するポリゴンの数を指定する
	const int PRIMITIVE_COUNT = m_face_num / VERTEX_NUM;

	// どの情報を伝えるか
	m_p_graphics->GetLpDirect3DDevice9()->SetFVF(FVF_CUSTOM);

	// インデックス描画
	m_p_graphics->GetLpDirect3DDevice9()->DrawIndexedPrimitive(
		// 頂点のつなぎ方
		D3DPT_TRIANGLELIST,
		// 頂点インデックスの一番最初までのオフセット値を指定
		OFFSET_INDEX,
		// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
		MIN_INDEX,
		// 上引数の最小以降の頂点数を指定
		m_face_num - MIN_INDEX,
		// 描画を開始する頂点インデックスまでのオフセット値を指定
		START_INDEX,
		// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
		PRIMITIVE_COUNT
	);
}


bool ObjFile::Load(std::string file_name) {


	// 頂点読み込みバッファ
	std::vector<D3DXVECTOR3>vertex_list;

	// 法線読み込みバッファ
	std::vector<D3DXVECTOR3>normal_list;

	// テクスチャ座標バッファ
	std::vector<D3DXVECTOR3>uv_list;

	// 面情報バッファ[SubSetNumber][FacePolygon]
	std::vector<FacePolygon>face_list;

	// マテリアルデータバッファ
	std::vector<MaterialData>material_data_list;

	
	// メイン読み込み
	if (VertexInfoLoad(
		file_name,
		vertex_list,
		normal_list,
		uv_list,
		face_list
		) == false) {
		return false;
	}

	// 頂点数
	m_vertex_num = TOTAL_VERTEX_TYPE;

	// 面数
	m_face_num = (int)face_list.size();

	// バーテックスバッファ生成
	VertexBufferCreate(
		m_vertex_num * m_face_num,
		vertex_list,
		normal_list,
		uv_list
		);

	// インデックスバッファ生成
	IndexBufferCreateFaceBase(m_face_num,face_list);

	return true;
}


bool ObjFile::MaterialLoad(std::string file_name) {


	return true;
}


bool ObjFile::VertexInfoLoad(
	std::string file_name,
	std::vector<D3DXVECTOR3>&out_vertex_list,
	std::vector<D3DXVECTOR3>&out_normal_list,
	std::vector<D3DXVECTOR3>&out_uv_list,
	std::vector<FacePolygon>&out_face_list
	) {

	// ファイルストリーム
	FILE *p_file;

	HRESULT result;

	// ファイルオープン
	result = fopen_s(&p_file, file_name.c_str(), "r");

	if (result != D3D_OK) {
		Window::TextMessageBox("OBJファイルオープンに失敗しました");
		return false;
	}

	// 頂点情報読み込み用
	D3DXVECTOR3 vec3;

	const int BUFFER = 32;

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

		// マテリアルファイルを読み込む
		if (front_str == "mtllib") {

			// mtlファイル読み込み
			fscanf_s(p_file, "%s", front_str, BUFFER);

			// マテリアルファイル名読み込み
			mtl_file_name = front_str;
		}

		// 頂点関係なら
		if (front_str[0] == 'v') {

			// x軸,y軸,z軸を読み込み
			VertexFscanfLoad(p_file, vec3);

			// 頂点なら
			if (front_str[1] == '\0') {

				out_vertex_list.emplace_back(vec3);
			}
			// 法線なら
			else if (front_str[1] == 'n') {

				out_normal_list.emplace_back(vec3);
			}
			// テクスチャ座標なら
			else if (front_str[1] == 't') {
				
				out_uv_list.emplace_back(vec3);
			}
		}

		// マテリアル情報なら要素加算
		else if (front_str == "usemtl") {
			material_num++;
			// 面情報数受け取り
		}
		// 面情報なら
		else if (front_str[0] == 'f') {

			// 面情報読み込み
			FaceInfoLoad(p_file,out_face_list);
		}

		// 初期化
		vec3.x = 0.f;
		vec3.y = 0.f;
		vec3.z = 0.f;
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


void ObjFile::FaceInfoLoad(
	FILE*p_file,
	std::vector<FacePolygon>&out_face_list
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
	space_split_str = SplitStr(' ', load_str);

	int face_num = 0;

	for (auto &str : space_split_str) {

		// 面情報文字列
		face_info_str.emplace_back();

		// 面情報文字列
		face_info_str[face_num].emplace_back();

		// /で文字列分割
		face_info_str[face_num] = SplitStr('/', str);

		// 面数加算
		face_num++;
	}

	// 仮の入れ物
	std::vector<FacePolygon>prov_face;

	// 面情報を代入
	prov_face = InsertFaceList(face_info_str);

	// 頂点が4つあるなら
	if (face_num >= 4) {

		// 変換させる
		prov_face = Face4IsFace3Change(prov_face);
	}

	// 面情報代入
	for (auto face : prov_face) {

		// [マテリアル][面情報]という感じに入れる
		out_face_list.emplace_back(face);
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

	// 仮の入れ物
	std::vector<FacePolygon>prov_face;

	// 面情報文字列
	for (unsigned int i = 0;
		i < face_info_str.size();
		i++) {

		// 面情報要素追加
		prov_face.emplace_back();

		// 軸分回す
		for (int j = 0; j < TOTAL_FACE_INFO; j++) {

			// 頂点情報に変換する
			Vector3ConversionByString(vertex_info[j], face_info_str[i][j]);

			// 面情報へ代入
			switch (j) {

			case VERTEX:
				prov_face[i].vertex_num = vertex_info[j];
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


std::vector<std::string> ObjFile::SplitStr(
	char cut_base_str,
	const std::string &string
) {

	std::string string_buffer;
	const char*string_ = string.c_str();

	std::vector<std::string> out_string_list;


	// 文字列が存在しないなら
	if (string.c_str() == NULL) {
		return out_string_list;
	}

	for (unsigned int i = 0; i < strlen(string.c_str()); i++) {

		// 文字列加算
		string_buffer += string_[i];

		// 区切りを指定
		if (cut_base_str == string_[i]) {

			std::string str(string_buffer);

			// 文字列配列に追加
			out_string_list.push_back(str);

			string_buffer.clear();
		}
	}


	// 最後の文字列算出
	{

		std::string str(string_buffer);

		out_string_list.push_back(str);
	}

	return out_string_list;
}


bool ObjFile::Vector3ConversionByString(
	int &axis,
	const std::string &string
) {

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


std::vector<FacePolygon> ObjFile::Face4IsFace3Change(
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
	std::vector<D3DXVECTOR3>uv_list
) {

	// 頂点バッファ作成
	m_p_graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		sizeof(Object3DCustomVertex) * vertex_num,
		// リソースの使用法
		0,
		// 柔軟な頂点フォーマットの型を指定する
		D3DFMT_INDEX16,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&m_p_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	Object3DCustomVertex *custom_vertex_list;

	// ロック
	m_p_vertex_buffer->Lock(0, 0, (void**)&custom_vertex_list, 0);

	// ここで頂点情報を代入させる

	int count = 0;

	// 頂点分回す
	for (auto vertex : vertex_list) {

		// 頂点
		custom_vertex_list[count].vertex = vertex;
		count++;
	}

	count = 0;

	// UV分回す
	for (auto uv : uv_list) {

		// テクスチャ座標
		custom_vertex_list[count].uv = uv;
		count++;
	}

	count = 0;

	// 法線分回す
	for(auto normal : normal_list){

		// 法線
		custom_vertex_list[count].normal = normal;
		count++;
	}

	// アンロック
	m_p_vertex_buffer->Unlock();
}


// 面情報から頂点情報埋め込み
bool ObjFile::IndexBufferCreateFaceBase(
	int face_num,
	std::vector<FacePolygon>face_list
) {

	// インデックスバッファ作成
	m_p_graphics->GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		face_num * sizeof(WORD) * TOTAL_FACE_INFO,
		// 頂点バッファをどのように使用するか
		0,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX16,
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

	// 頂点
	WORD * v;

	if (FAILED(m_p_index_buffer->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&v,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		D3DLOCK_DISCARD
	)
	)
		) {
		return false;
	}

	const int OFFSET = 1;

	// とりあえず頂点インデックスをセット
	{

		// 面数
		for (int j = 0; j < face_num; j++) {

			// 面情報から埋め込み
			v[j] = face_list[j].vertex_num - OFFSET;
		}
		
	}
	// nullチェック
	if (v == nullptr) {
		return false;
	}

	// アンロック
	m_p_index_buffer->Unlock();

	return true;
}

