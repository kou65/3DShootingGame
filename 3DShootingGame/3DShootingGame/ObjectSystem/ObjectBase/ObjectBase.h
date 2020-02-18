
#ifndef OBJECT_H
#define OBJECT_H

#include"../../Lib/Graphics/Graphics.h"
#include"../../GameApp/GameConstant/GameConstant.h"



class ObjectBase {
public:

	~ObjectBase(){};

	ObjectBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsActive();

protected:

	bool m_is_active;
};

#endif