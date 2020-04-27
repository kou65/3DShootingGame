
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec2/Vec2.h"


//! 2Dオブジェクトタイプ
enum class Object2DType {
	UI
};


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