#pragma once
#include"../MapObjectBase/MapObjectBase.h"



/**
* @brief 位置などを予約しておく仮想空白ブロッククラス
*/
class ClearBlock : public MapObjectBase {
public:

	ClearBlock() : MapObjectBase() {

		// 無しにしておく
		m_object_tag = Object3DTag::NONE;
		// オブジェクトとしては使わない
		m_is_active = false;
	};

	ClearBlock(const MapTag&tag,MapData&data)
		: MapObjectBase() {

		m_pos = data.pos;
		m_map_tag = tag;
		m_is_active = false;
	}

	void SetObj3DTag(const MapTag&tag){
		m_map_tag = tag;
	}
};