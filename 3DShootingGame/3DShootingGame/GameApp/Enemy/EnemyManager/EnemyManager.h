#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../ObjectSystem/FileObjectDataManager/FileObjectDataManager.h"
#include<vector>



class EnemyManager {
public:

	EnemyManager(
		FileObjectDataManager*p_manager,
		ObjectFactory*p_factory,
		CharacterBase*p_player) {

		for (int i = 0; i < p_manager->GetMaxPosListNum("res2"); i++) {

			// リソース位置追加
			m_pos_list.emplace_back(p_manager->OutPosList("res2",i));
		}

		// 生成されているかどうかを代入
		m_p_enemy_list.resize(m_pos_list.size());

		m_p_factory = p_factory;

	}

	void SerchCreateDeleteTaile() {

		for (auto&pos : m_pos_list) {

			// キャラクター情報を渡す
			m_p_factory->CreateHEnemy(pos,m_p_chara);
		}
	}

private:

	// 敵配列
	std::vector<EnemyBase*>m_p_enemy_list;

	// プレイヤー
	CharacterBase*m_p_chara;

	// 取得した位置リスト
	std::vector<Vec3>m_pos_list;

	// 工場
	ObjectFactory*m_p_factory;

};