#pragma once
#include<stdio.h>
#include<iostream>
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../Object3DCustomVertex/Object3DCustomVertex.h"


// HACK リファクタリング対象(複数のstructなど)


// 1面ポリゴン数
enum Polygon {
	POLYGON3,
	POLYGON4,
};


// Drawサブセット用
struct ObjectSubset {
	unsigned int material_index;
	unsigned int face_start;
	unsigned int face_count;
};


// 面ポリゴン情報
struct FacePolygon {
	int vertex_num;
	int uv_num;
	int normal_num;
};


// マテリアルのデータ
struct MaterialData {
	float ambient;
	float diffuse;
	float specular;
	std::string texture_name;
};


// メッシュ情報
struct MeshInfo {
	std::vector<std::vector<FacePolygon>>face_list;
};


class ObjFile {
public:

	// 面情報の種類
	enum FaceInfoType {
		VERTEX,
		UV,
		NORMAL,
		TOTAL_FACE_INFO
	};

public:

	ObjFile();

	// 読み込み
	bool Load(std::string file_name);

	// マテリアルファイル読み込み
	bool MaterialLoad(std::string file_name);

	// 表示
	void Draw(int material_num);

private:

	void VertexFscanfLoad(
		FILE*p_file,
		D3DXVECTOR3 &vec3);


	// 頂点情報読み込み
	bool VertexInfoLoad(
		std::string file_name,
		std::vector<D3DXVECTOR3>&out_vertex_list,
		std::vector<D3DXVECTOR3>&out_normal_list,
		std::vector<D3DXVECTOR3>&out_uv_list,
		std::vector<FacePolygon>&out_face_list
	);


	// 面情報からカスタムバーテックス作成
	void FaceInfoLoad(
		FILE*p_file,
		std::vector<FacePolygon>&face_list
		);


	// 面情報代入
	std::vector<FacePolygon> InsertFaceList(
		std::vector<std::vector<std::string>>face_info_str
	);


	// 文字列分割
	std::vector<std::string> SplitStr(
		char cut_base_str,
		const std::string &string
	);


	// 整数値変換
	bool Vector3ConversionByString(
		int &axis,
		const std::string &string
	);


	// 頂点バッファ生成
	void VertexBufferCreate(
		int vertex_num,
		std::vector<D3DXVECTOR3>vertex_list,
		std::vector<D3DXVECTOR3>normal_list,
		std::vector<D3DXVECTOR3>uv_list
	);


	// 面情報から頂点情報埋め込み
	bool IndexBufferCreateFaceBase(
		int face_num,
		std::vector<FacePolygon>face_list
		);


	// 4面を3面にする
	std::vector<FacePolygon> Face4IsFace3Change(
		std::vector<FacePolygon>face_polygon_list
	);

private:

	// 頂点数
	int m_vertex_num;

	// 面数
	int m_face_num;

	// マテリアル番号
	int material_num;

	// マテリアル頂点総インデックス(インデックス数を保存)
	std::vector<int>total_material_num;

	// マテリアルファイル名
	std::string mtl_file_name;

	// テクスチャファイル名
	std::string texture_file_name;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer;

	// インデックスバッファ
	IDirect3DIndexBuffer9 * m_p_index_buffer;

	// DirectGraphics
	Graphics *m_p_graphics;
};
