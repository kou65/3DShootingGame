
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec3/Vec3.h"

enum class Object3DTag {
	NONE,
	PLAYER,
	ENEMY,
	CUBE,
	BULLET_ENEMY,
	BULLET,
	FILED,
};


class Object3D : public ObjectBase{
public:

	Object3D();

	Vec3 GetPos();
	Object3DTag GetTag();

protected:

	void SetTag(const Object3DTag&tag);

protected:
	Vec3 m_pos;
	Vec3 m_rotate;
	Vec3 m_scale;

	// タグ
	Object3DTag m_object_tag;
};

#endif