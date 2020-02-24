
#ifndef OBJECT_H
#define OBJECT_H

#include"../../Lib/Graphics/Graphics.h"
#include"../../GameApp/GameConstant/GameConstant.h"


class ObjectBase {
public:

	// 仮想デストラクタ
	virtual ~ObjectBase(){};

	ObjectBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 活動しているか
	bool IsActive();

	// 描画できるか
	bool CanDraw();

protected:

	bool m_can_draw;
	bool m_is_active;
};

#endif