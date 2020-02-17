
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include"../Object/Object.h"
#include"../../Lib/Vec3/Vec3.h"



class Object3D : public Object{
public:

	Vec3 GetPos();

protected:

	Vec3 m_pos;
};

#endif