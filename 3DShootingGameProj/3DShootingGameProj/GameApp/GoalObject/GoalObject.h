
#ifndef GOAL_OBJECT_H
#define GOAL_OBJECT_H
#include"../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"



/**
* @brief ゴールを示すobjectクラス
*/
class GoalObject : public CollisionObject3DBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	*/
	GoalObject(const Vec3&pos);


	/**
	* @brief 外部削除関数
	*/
	void Destory();


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;

};

#endif 