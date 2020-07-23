#pragma once
#include"../BulletBase/BulletBase.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../BulletData/BulletData.h"



/**
* @brief �G�̒e
*/
class EnemyBullet : public BulletBase{
public:


	/**
	* @brief �R���X�g���N�^
	* @param[in] pos �ʒu
	* @param[in] speed ���x
	* @param[in] limit_distance �e�̐�������
	* @param[in] dir ����
	*/
	EnemyBullet(
		const ObjParameter&param,
		const BulletData&data
	);


	/**
	* @brief �X�V ���z�֐�
	*/
	void Update()override;


	/**
	* @brief �`�� ���z�֐�
	*/
	void Draw()override;


private:


	/**
	* @brief �Փˌ�̏���
	* @param[in] type ����̏Փˎ��
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief ����Ԃ�
	*/
	Sphere GetSphere()override;


	/**
	* @brief �����ɒe���΂�
	*/
	void AddDirToPos();

private:

	//! �G�̈ʒu
	Vec3 m_enemy_pos;

	//! �I�u�W�F�N�g�p�����[�^
	ObjParameter m_obj_param;
};