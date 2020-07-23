#include"EffectFactory.h"
#include"../../Manager/ObjectManager/ObjectManager.h"



void EffectFactory::CreatePolygonEffect(
	const Vec3&pos
) {

	// エフェクト作成
	ObjectManager::GetInstance()->EntryUniqueObj(
		std::make_unique<PolygonEffect>(pos)
	);
}
