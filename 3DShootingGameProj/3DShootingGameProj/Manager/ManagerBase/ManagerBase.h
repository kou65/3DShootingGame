#pragma once



/**
* @brief 基底管理者
*/
class ManagerBase {
public:


	/**
	* @brief 仮想デストラクタ
	*/
	virtual ~ManagerBase() {};


	/**
	* @brief 更新
	*/
	virtual void Update() = 0;


protected:

	//! 活動しているかどうか
	bool m_is_active;
};