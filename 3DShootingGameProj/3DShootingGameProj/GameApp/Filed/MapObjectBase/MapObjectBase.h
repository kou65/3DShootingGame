
#ifndef MAP_OBJECT_BASE_H
#define MAP_OBJECT_BASE_H

#include"../../../ObjectSystem/Object3D/Object3D.h"
#include"../../../ObjectSystem/Object3DInterface/Object3DInterface.h"
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../Filed/MapData/MapData.h"



/**
* @brief 基底マップオブジェクトクラス
*/
class MapObjectBase : public CollisionObject3DBase{
public:

	/**
	* @brief コンストラクタ
	* @param[in] inter Interfaceクラス
	*/
	MapObjectBase(Object3DInterface*inter) : CollisionObject3DBase(){
		m_p_interface_obj = inter;
	}


	/**
	* @brief デフォルトコンストラクタ
	*/
	MapObjectBase() {}


	/**
	* @brief 外部から削除する関数
	*/
	void Destory() {
		Exit();
	}


	/**
	* @brief 活動停止
	*/
	void NotActive() {
		m_is_active = false;
	}


	/**
	* @brief マップタグ外部へ出す
	*/
	MapTag GetMapTag() {
		return m_map_tag;
	}

protected:

	//! インターフェースクラス
	Object3DInterface * m_p_interface_obj;

	//! マップナンバー
	MapTag m_map_tag;
};
#endif