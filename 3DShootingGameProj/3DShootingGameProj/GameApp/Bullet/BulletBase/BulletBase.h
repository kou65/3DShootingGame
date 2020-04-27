#pragma once
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../../Lib/Vec3/Vec3.h"
#include<math.h>



/**
* @brief �e�̊��N���X
*/
class BulletBase : public CollisionObject3DBase{
protected:


	/**
	* @brief �e�̋�������
	*/
	void Limit();

	
	/**
	* @brief �e�̋�������
	* @param[in] pos ���̈ʒu����
	*/
	void Limit(Vec3&pos);


	/**
	* @brief ����
	*/
	void Direction();

protected:

	//! �e�̕���
	Vec3 m_dir;

	//! �e�̑��x
	Vec3 m_speed;

	//! �e�̗L���͈�
	Vec3 m_limit_distance;

	// �����ʒu
	Vec3 m_length_pos;
};