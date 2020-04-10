#pragma once
#include"../Shape3DBase/ShapeBase.h"
#include"../../Collision/Collision.h"



struct Cube : public Shape3DBase {

	Cube() {};

	Rect3D m_rect_3d;
};