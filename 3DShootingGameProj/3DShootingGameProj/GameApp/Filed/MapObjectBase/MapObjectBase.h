
#ifndef MAP_OBJECT_BASE_H
#define MAP_OBJECT_BASE_H

#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../ObjectSystem/Object3DInterface/Object3DInterface.h"
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../Filed/MapData/MapData.h"



/**
* @brief ���}�b�v�I�u�W�F�N�g�N���X
*/
class MapObjectBase : public CollisionObject3DBase{
public:

	/**
	* @brief �R���X�g���N�^
	* @param[in] inter Interface�N���X
	*/
	MapObjectBase(Object3DInterface*inter) : CollisionObject3DBase(){
		m_p_interface_obj = inter;
	}


	/**
	* @brief �f�t�H���g�R���X�g���N�^
	*/
	MapObjectBase() {}


	/**
	* @brief �O������폜����֐�
	*/
	void Destory() {
		Exit();
	}


	/**
	* @brief ������~
	*/
	void NotActive() {
		m_is_active = false;
	}


	/**
	* @brief �}�b�v�^�O�O���֏o��
	*/
	MapTag GetMapTag() {
		return m_map_tag;
	}

protected:

	//! �C���^�[�t�F�[�X�N���X
	Object3DInterface * m_p_interface_obj;

	//! �}�b�v�i���o�[
	MapTag m_map_tag;
};
#endif