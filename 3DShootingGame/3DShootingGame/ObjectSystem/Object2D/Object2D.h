
#ifndef OBJECT2D_H
#define OBJECT2D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec2/Vec2.h"



class Object2D : public ObjectBase{
public:

	Vec2 GetPos();

protected:

	Vec2 m_pos;
};

#endif