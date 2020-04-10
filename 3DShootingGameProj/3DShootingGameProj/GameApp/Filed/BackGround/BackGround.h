#pragma once
#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../Lib/Vec3/Vec3.h"



class BackGround : public Object3D{
public:

	BackGround(const Vec3&pos);

	void Update()override;
	void Draw()override;

private:

	ObjParameter m_param;
};