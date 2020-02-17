
#ifndef OBJECT_H
#define OBJECT_H

#include"../../Lib/Graphics/Graphics.h"
#include"../../GameApp/GameConstant/GameConstant.h"



class Object {
public:

	~Object(){};

	Object() {
		m_is_active = true;
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsActive() {
		return m_is_active;
	}

protected:

	bool m_is_active;
};

#endif