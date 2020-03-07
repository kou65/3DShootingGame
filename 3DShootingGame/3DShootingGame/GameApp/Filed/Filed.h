#pragma once
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"Taile/Taile.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../CharacterBase/CharacterBase.h"


class Filed : public Object3D{
public:

	Filed(ObjectFactory*m_p_factory);
	Filed(ObjectFactory*m_p_factory, CharacterBase*obj);

	void Update()override;
	void Draw()override;

	// 軸を中心に生成するための軸オブジェクト代入
	void CreateAxis(CharacterBase*p_chara);

private:

	// 前方の総合タイル数
	const int TOTAL_TAILE_FRONT = 1000;

	// 横の総合タイル数
	const int TOTAL_TAILE_WIDTH = 3;

	// 生成範囲前方
	const float CREATE_RANGE_F_Z = 200.f;

	// 生成範囲後方
	const float CREATE_RANGE_B_Z = -200.f;

	// 生成範囲右
	const float CREATE_RANGE_R_X = 200.f;

	// 生成範囲左
	const float CREATE_RANGE_L_Y = -200.f;

	// テクスチャサイズ
	const float TAILE_SIZE = 16.f;

private:

	// 広い床描画
	void DrawWideTaile();

	// 右の壁生成
	void CreateRightWall();

	// 左の壁生成
	void CreateLeftWall();

	// バトル床を描画
	void CreateTaile();

	// 部分的に描画する
	void DrawTaile();

private:

	// ファクトリー
	ObjectFactory *m_p_factory;

	// キャラクターを中心に描画
	CharacterBase*m_p_chara;

	// マップオブジェクトの参照
	std::vector<MapObjectBase*>m_p_map_obj_list;
};