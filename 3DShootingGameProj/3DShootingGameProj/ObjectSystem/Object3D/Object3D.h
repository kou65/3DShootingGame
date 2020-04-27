
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include"../ObjectBase/ObjectBase.h"
#include"../../Lib/Vec3/Vec3.h"


/**
* @enum Object3DTag
* @brieft タグ
*/
enum class Object3DTag {
	NONE,
	PLAYER,
	ENEMY,
	BLOCK,
	BULLET_ENEMY,
	SHOTGUN_ENEMY,
	BULLET,
	FILED,
};



/**
* @brief 3Dオブジェクトクラス
*/
class Object3D : public ObjectBase{
public:

	/**
	* @brief コンストラクタ
	*/
	Object3D();

	/**
	* @brief 位置を取得
	* @return Vec3
	*/
	Vec3 GetPos();

	/**
	* @brief オブジェクトのタグを取得
	* @return Object3DTag
	*/
	Object3DTag GetTag();

protected:

	/**
	* @brief オブジェクトのタグをセット
	* @param[in] tag セットするタグ名
	*/
	void SetTag(const Object3DTag&tag);

protected:

	//! 位置
	Vec3 m_pos;

	//! 回転値
	Vec3 m_rotate;

	//! 拡縮
	Vec3 m_scale;

	//! タグ
	Object3DTag m_object_tag;
};

#endif