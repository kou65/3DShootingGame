
#ifndef TAILE_H
#define TAILE_H

#include"../MapObjectBase/MapObjectBase.h"
#include"../../../Lib/TransformData/TransformData3D.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"



/**
* @brief タイルクラス
*/
class Taile : public MapObjectBase {
public:

	//! タイルテクスチャサイズ
	constexpr static float TAILE_SIZE_X = 4.f;
	constexpr static float TAILE_SIZE_Y = 4.f;
	constexpr static float TAILE_SIZE_Z = 4.f;

public:

	/**
	* @brief 方向
	*/
	enum class Direction {
		LEFT,
		RIGHT,
		FLOOR,
	};

public:


	/**
	* @brief コンストラクタ
	* @param[in] data objファイルパラメータ
	* @param[in] dir タイルの向き
	*/
	Taile(const ObjParameter&data,
		const Direction &dir = Direction::FLOOR);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


private:


	/**
	* @brief タイルの向きをセット
	* @param[in] dir 方向定数
	*/
	void SetDirection(const Direction&dir);


private:

	//! objファイルパラメータ
	ObjParameter m_data;
};

#endif