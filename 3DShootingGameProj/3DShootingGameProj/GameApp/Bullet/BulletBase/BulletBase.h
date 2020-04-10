#pragma once
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../../Lib/Vec3/Vec3.h"
#include<math.h>


// ’e‚ÌŠî’êƒNƒ‰ƒX


class BulletBase : public CollisionObject3DBase{
public:


	// Šp“x‚Ì‘Î‚µ‚ÄˆÚ“®‘¬“x‚ğŠ|‚¯‚Äi‚Ü‚¹‚é
	//m_pos.z += cosf(m_radius) * m_speed;
	//m_pos.y += sinf(m_radius) * m_speed;


protected:

	void Limit();

	void Limit(Vec3&vec);

protected:

	// ’e‚Ì•ûŒü
	Vec3 m_dir;

	// ’e‚Ì‘¬“x
	Vec3 m_speed;

	// ’e‚Ì—LŒø”ÍˆÍ
	Vec3 m_limit_distance;
};