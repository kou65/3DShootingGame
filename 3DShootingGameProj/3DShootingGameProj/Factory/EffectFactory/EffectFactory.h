
#ifndef EFFECT_FACTORY_H
#define EFFECT_FACTORY_H
#include"../../GameApp/Effect/PolygonEffect/PolygonEffect.h"



/**
* @brief ObjectFactoryに包含するクラス
*/
class EffectFactory {
public:


	// エフェクト作成
	void CreatePolygonEffect(
		const Vec3&pos
	);

};

#endif