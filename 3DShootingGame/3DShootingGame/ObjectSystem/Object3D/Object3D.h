
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec3/Vec3.h"



class Object3D : public ObjectBase{
public:

	Vec3 GetPos();

protected:
	Vec3 m_pos;
};

#endif