#pragma once
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../../Lib/Vec3/Vec3.h"
#include<math.h>



/**
* @brief ’e‚ÌŠî’êƒNƒ‰ƒX
*/
class BulletBase : public CollisionObject3DBase{
protected:


	/**
	* @brief ’e‚Ì‹——£§ŒÀ
	*/
	void Limit();

	
	/**
	* @brief ’e‚Ì‹——£§ŒÀ
	* @param[in] pos ‚»‚ÌˆÊ’u‚©‚ç
	*/
	void Limit(Vec3&pos);


	/**
	* @brief •ûŒü
	*/
	void Direction();

protected:

	//! ’e‚Ì•ûŒü
	Vec3 m_dir;

	//! ’e‚Ì‘¬“x
	Vec3 m_speed;

	//! ’e‚Ì—LŒø”ÍˆÍ
	Vec3 m_limit_distance;

	// §ŒÀˆÊ’u
	Vec3 m_length_pos;
};