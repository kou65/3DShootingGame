
#ifndef FILED_H
#define FILED_H

#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../GameApp/Filed/Taile/Taile.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../GameApp/CharacterBase/CharacterBase.h"
#include"../ManagerBase/ManagerBase.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include<memory>



/**
* @brief マップを形成するMapStructureManagerクラス
*/
class MapStructureManager: public ManagerBase{
public:

	//! 前方の総合タイル数
	const static int TOTAL_FRONT_OBJ = 1000;

public:


	/**
	* @brief コンストラクタ
	* @param[in] p_factory 工場ポインタ
	*/
	MapStructureManager(std::shared_ptr<ObjectFactory>p_factory);


	/**
	* @brief コンストラクタ
	* @param[in] p_factory 工場ポインタ
	* @param[in] p_chara 基底キャラポインタ
	*/
	MapStructureManager(
		std::shared_ptr<ObjectFactory>p_factory,
		std::shared_ptr<ObjectData>p_data
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;



	/**
	* @brief 軸を中心に生成するための軸オブジェクト代入 
	* @param[in] p_chara 基底キャラポインタ
	*/
	void CreateAxis(std::weak_ptr<CharacterBase>&p_chara);

private:


	/**
	* @enum TaileType タイルの種類識別定数
	*/
	enum class TaileType {
		TAILE,
		LEFT_WALL,
		RIGHT_WALL
	};

	//! 横の総合タイル数
	const static int TOTAL_TAILE_OBJ = 5;

	//! 生成範囲前方
	const static int CREATE_RANGE_FRONT = 20;

	//! 生成範囲後方
	const static int CREATE_RANGE_BACK = 2;


	//! 横のオブジェクト生成位置
	const TaileType WIDHT_FILED_OBJ[TOTAL_TAILE_OBJ] = {
		TaileType::LEFT_WALL,
		TaileType::TAILE,
		TaileType::TAILE,
		TaileType::TAILE,
		TaileType::RIGHT_WALL
	};

private:


	/**
	* @brief バトル床を描画
	*/
	void Init();


	/**
	* @brief 毎ターンタイルを生成するか調べて生成する
	*/
	void SerchCreateDeleteTaile();

private:

	//! ファクトリー
	std::weak_ptr<ObjectFactory>mp_factory;

	//! キャラクターを中心に描画
	std::weak_ptr<CharacterBase>mp_chara;

	//! マップオブジェクトの参照()
	std::vector<std::vector<std::weak_ptr<MapObjectBase>>>mp_map_obj_list;

	//! 生成されたかどうかの状態
	std::vector<std::vector<bool>>m_is_create_list;
};

#endif