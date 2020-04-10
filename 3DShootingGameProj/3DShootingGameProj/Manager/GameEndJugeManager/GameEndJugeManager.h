#pragma once
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../../Scene/SceneBase/SceneBase.h"
#include"../ManagerBase/ManagerBase.h"


// ゲーム終了判定管理者


class GameEndJugeManager{
public:

	GameEndJugeManager(
		ObjectData*p_data
	);

	void Update(SceneType&scene_type);

private:

	ObjectData*m_p_data;
};