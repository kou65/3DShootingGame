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

	// タイルの種類
	enum class TaileType {
		TAILE,
		LEFT_WALL,
		RIGHT_WALL
	};

	// 前方の総合タイル数
	const static int TOTAL_FRONT_OBJ = 1000;

	// 横の総合タイル数
	const static int TOTAL_TAILE_OBJ = 5;

	// 生成範囲前方
	const static int CREATE_RANGE_FRONT = 20;

	// 生成範囲後方
	const static int CREATE_RANGE_BACK = 2;


	// 横のオブジェクト種類
	const TaileType WIDHT_FILED_OBJ[TOTAL_TAILE_OBJ] = {
		TaileType::LEFT_WALL,
		TaileType::TAILE,
		TaileType::TAILE,
		TaileType::TAILE,
		TaileType::RIGHT_WALL
	};

private:

	// 広い床描画
	void DrawWideTaile();

	// 右の壁生成
	void CreateRightWall();

	// 左の壁生成
	void CreateLeftWall();

	// バトル床を描画
	void Init();

	// 毎ターンタイルを生成するか調べて生成する
	void SerchCreateDeleteTaile();


private:

	// ファクトリー
	ObjectFactory *m_p_factory;

	// キャラクターを中心に描画
	CharacterBase*m_p_chara;

	// マップオブジェクトの参照()
	//std::vector<std::vector<std::weak_ptr<MapObjectBase>>>m_p_map_obj_list;
	std::vector<std::vector<MapObjectBase*>>m_p_map_obj_list;

	// 生成されたかどうかの状態
	std::vector<std::vector<bool>>m_is_create_list;
};