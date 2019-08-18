#pragma once
#include"../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../Lib/Texture/DrawPolygon2D.h"
#include"../Lib/Sound/Sound.h"
#include<string>
#include"../Lib/Camera3D/Camera3D.h"
#include"../Lib/D3DDefaultMesh/D3DDefaultMesh.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/Light/Light.h"



// シングルトンデバッグモード
class DebugMode{
public:


	bool IsEnd();

	void Update();

	void Draw();

	void Release();

	// シングルトン設定
	static DebugMode& GetInstance();

private:

	DebugMode();

	void CameraMove();

	void CameraRotation();

	bool IsStopUpdate();

private:

	Light light;

	Camera3D *camera_3d;

	D3DDefaultMesh d3d_mesh;

	FPS fps;

	bool m_is_program_stop;
};