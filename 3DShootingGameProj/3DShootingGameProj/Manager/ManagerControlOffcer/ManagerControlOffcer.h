#pragma once
#include"../ManagerBase/ManagerBase.h"
#include<vector>
#include<memory>




/**
* @brief 管理者をコントロールする統括者
*/
class ManagerControlOffcer {
public:

	/**
	* @brief アクセス権
	*/
	static ManagerControlOffcer*GetInstance() {
		static ManagerControlOffcer mng;
		return &mng;
	}

public:


	/**
	* @brief コンストラクタ
	*/
	ManagerControlOffcer();
	

	/**
	* @brief 共有ポインタ用追加
	*/
	void EntryShared(std::shared_ptr<ManagerBase>p_mng);


	/**
	* @brief ユニークポインタ追加
	*/
	void EntryUnique(std::unique_ptr<ManagerBase>p_mng);


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 全て解放
	*/
	void AllRelease();

private:

	//! 共有用
	std::vector<std::shared_ptr<ManagerBase>>mp_shared_mng_list;

	//! ユニーク用
	std::vector<std::unique_ptr<ManagerBase>>mp_unique_mng_list;
};