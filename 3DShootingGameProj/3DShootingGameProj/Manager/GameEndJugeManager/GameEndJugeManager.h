#pragma once
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../../Scene/SceneBase/SceneBase.h"
#include"../ManagerBase/ManagerBase.h"




/**
* @brief ゲーム終了判定管理者
*/
class GameEndJugeManager{
public:


	/**
	* @brief コンストラクタ
	* @param[in] p_data オブジェクトデータ
	*/
	GameEndJugeManager(
		std::shared_ptr<ObjectData>p_data
	);


	/**
	* @brief 更新
	* @param[in] scene_type シーンタイプ
	*/
	void Update(SceneType&scene_type);


private:

	//! オブジェクトデータ
	std::weak_ptr<ObjectData>m_p_data;
};