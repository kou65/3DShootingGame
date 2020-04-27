
#ifndef MODEL_H
#define MODEL_H
#include<vector>
#include"../../Graphics/Graphics.h"



/**
* @brief マテリアル情報構造体
*/
struct MaterialInfo {
	D3DMATERIAL9 material;
	std::string texture_name;
};


/**
* @brief 基底3Dモデルクラス
*/
class Model {
public:


	/**
	* @brief コンストラクタ
	*/
	Model();

protected:


	/**
	* @brief インデックスバッファ生成
	* @param[in] total_face_num 総面数
	* @param[in] indices インデックス形成をまとめたリスト
	* @param[in] buffer インデックスバッファポインタ
	* @return bool 生成できたかどうか
	*/
	bool IndexBufferCreate(
		const int &total_face_num,
		std::vector<INT>&indices,
		LPDIRECT3DINDEXBUFFER9 * buffer
	);


	/**
	* @brief 頂点バッファ生成
	* @param[in] total_vertex 総頂点数
	* @param[in] p_vertex_buffer 頂点ポインタ
	*/
	bool VertexBufferCreate(
		const int &total_vertex,
		LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
	);


	/**
	* @brief 3D描画する為のパラメータのセット
	* @param[in] graphics グラフィックスポインタ
	* @param[in] p_i_buf インデックスバッファ
	* @param[in] p_v_buf バーテックスバッファ
	* @param[in] stride 1頂点当たりのサイズ(sizeofなどで型を計測)
	* @param[in] material D3DMATERIAL9構造体
	* @param[in] fvf fvfデータ
	*/
	void Set3DParameter(
		Graphics*graphics,
		IDirect3DIndexBuffer9*p_i_buf,
		IDirect3DVertexBuffer9*p_v_buf,
		const UINT &stride,
		const D3DMATERIAL9&material,
		const DWORD&fvf
	);


	/**
	* @brief 通常描画
	* @param[in] vertex_num 頂点数
	* @param[in] polygon_num ポリゴン数
	*/
	void DrawPrimitive(
		const int &vertex_num,
		const int&polygon_num
	);


	/**
	* @brief ワールド座標変換
	* @param[in] pos 位置
	* @param[in] rot 回転値
	* @param[in] scale 拡縮
	* @return D3DXMATRIX 変換後の行列
	*/
	D3DXMATRIX WorldTransform(
		D3DXVECTOR3 &pos,
		D3DXVECTOR3 &rot,
		D3DXVECTOR3 &scale);


	/**
	* @brief 行列掛け算してvec3に変換
	* @param[out] out_vec3 変換後のvec3
	* @param[in] base_vec3 変換前のvec3
	* @param[in] mat 変換する行列
	*/
	void MatrixMultipleVec3(
		D3DXVECTOR3&out_vec3,
		const D3DXVECTOR3&base_vec3,
		const D3DXMATRIX&mat
	);


	/**
	* @brief 行列掛け算してvec4に変換
	* @param[out] out_vec4 変換後のvec4
	* @param[in] base_vec4 変換前のvec4
	* @param[in] mat 変換する行列
	*/
	void MatrixMultipleVec4(
		D3DXVECTOR4&out_vec3,
		const D3DXVECTOR4&base_vec3,
		const D3DXMATRIX&mat
	);


	/**
	* @brief ポリゴン4つを3つにする
	* @param[in] vertex4_polygon_list 分割する4つの頂点リスト
	*/
	std::vector<INT> Polygon4ToPolygon3Convert(
		const std::vector<INT>& vertex4_polygon_list
	);

protected:

	//! グラフィックス
	Graphics*m_p_graphics;
};
#endif