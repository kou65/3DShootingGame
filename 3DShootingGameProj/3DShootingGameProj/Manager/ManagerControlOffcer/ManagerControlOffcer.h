#pragma once
#include"../ManagerBase/ManagerBase.h"
#include<vector>
#include<memory>




/**
* @brief 管理者をコントロールする統括者
*/
class ManagerControlOffcer {
public:

	// アクセス権
	static ManagerControlOffcer*GetInstance() {
		static ManagerControlOffcer mng;
		return &mng;
	}

public:

	ManagerControlOffcer();
	
	void EntryShared(std::shared_ptr<ManagerBase>p_mng);
	void EntryUnique(std::unique_ptr<ManagerBase>p_mng);

	void Update();

	// 全て解放
	void AllRelease();

private:

	//! 共有用
	std::vector<std::shared_ptr<ManagerBase>>mp_shared_mng_list;

	//! ユニーク用
	std::vector<std::unique_ptr<ManagerBase>>mp_unique_mng_list;
};