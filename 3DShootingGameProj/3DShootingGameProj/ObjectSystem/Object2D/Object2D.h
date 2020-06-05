
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec2/Vec2.h"


/**
* @enum Object2DType 2Dオブジェクトの種類
*/
enum class Object2DType {
	UI
};


/**
* @brief オブジェクト2Dクラス
* @brief 2Dオブジェクトなら継承して使う
*/
class Object2D : public ObjectBase{
public:

	/**
	@ brief 位置取得
	*/
	Vec2 GetPos();

protected:

	//! 位置
	Vec2 m_pos;

	//! 回転
	Vec2 m_rotate;

	//! 拡縮
	Vec2 m_scale;
};

#endif