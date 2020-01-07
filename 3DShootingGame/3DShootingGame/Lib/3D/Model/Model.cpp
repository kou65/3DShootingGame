#include"Model.h"
#include"../AnimationCustomVertex/AnimationCustomVertex.h"




bool Model::IndexBufferCreate(
	int total_face_num,
	std::vector<INT>indices,
	LPDIRECT3DINDEXBUFFER9 * buffer) {

	// 頂点32bit
	int* index_buffer;

	Graphics*p_graphics = Graphics::GetInstance();

	// 32bitサイズのインデックスバッファを作成
	p_graphics->CreateIndexBuffer32BitSize(
		buffer,
		(total_face_num * sizeof(int))
	);

	// ロック
	index_buffer = p_graphics->LockIndexBuffer32BitSize(
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


bool Model::VertexBufferCreate(
	int total_vertex_num,
	LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
) {


	Graphics*p_graphics = Graphics::GetInstance();

	// 頂点バッファ作成
	p_graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
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



D3DXMATRIX Model::WorldTransform(
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

// x
// y
// z
// 1

// D3DMATRIX
// _11, _12, _13, _14
// _21, _22, _23, _24
// _31, _32, _33, _34
// _41, _42, _43, _44

// x = (x * m._11) + (y * m._21) + (z * m._31) + (1 * m._41)
// y = (x * m._12) + (y * m._22) + (z * m._32) + (1 * m._42)
// z = (x * m._13) + (y * m._23) + (z * m._33) + (1 * m._43)

void MatrixMultipleVec3(
	D3DXVECTOR3&out_vec3,
	const D3DXVECTOR3&base_vec3,
	const D3DXMATRIX&mat
) {
	// 頂点x * 1行1列番目の行列...
	out_vec3.x
		=
		(
			base_vec3.x * mat._11
			+ base_vec3.y * mat._21
			+ base_vec3.z * mat._31
			+ 1 * mat._41);

	out_vec3.y
		=
		(
			base_vec3.x * mat._12
			+ base_vec3.y * mat._22
			+ base_vec3.z * mat._32
			+ 1 * mat._42);

	out_vec3.z
		= (base_vec3.x * mat._13
			+ base_vec3.y * mat._23
			+ base_vec3.z * mat._33
			+ 1 * mat._43);

}


// ポリゴン4を3にする
std::vector<INT> Model::Polygon4ToPolygon3Convert(
	const std::vector<INT>& vertex4_polygon_list) {


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

	// 3ポリを2個返す
	return get_face_list;

}


