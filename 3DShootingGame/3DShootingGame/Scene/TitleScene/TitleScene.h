#pragma once
#include"../SceneBase/SceneBase.h"



class TitleScene : public SceneBase{
public:

	void Update(SceneType&scene_type)override;
	void Draw()override;
	void ChangeGameScene(SceneType&scene_type);

private:


};