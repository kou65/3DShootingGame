#pragma once
#include"../Lib/3D/OBJ/OBJFile.h"
#include"../Object3D/Object3D.h"



class Filed : public Object3D{
public:

	void Update()override;
	void Draw()override;

private:

	D3DXVECTOR3 m_pos;
};