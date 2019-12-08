#pragma once
#include"../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DObject.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/3D/Camera3D/Camera3D.h"
#include"../Lib/3D/D3DDefaultMesh/D3DDefaultMesh.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/3D/Light/Light.h"
#include"../Lib/3D/IndexBuffer/IndexBuffer.h"
#include"../Lib/3D/VertexBuffer/VertexBuffer.h"
#include"../Lib/3D/OBJ/OBJFile.h"
#include<string>



// シングルトンデバッグモード
class Debugger{
public:

	bool IsEnd();

	void Update();

	void Draw();

	void Release();

	// シングルトン設定
	static Debugger&GetInstance();

private:

	Debugger();

	void CameraMove();

	void CameraRotation();

	bool IsStopUpdate();

private:

	Light *light;

	Camera3D *camera_3d;

	D3DDefaultMesh d3d_mesh;

	FPS *fps;

	IndexBuffer *index_buffer;

	bool m_is_program_stop;

	int m_total_material_num;

	// データ
	Sprite2DData s2d;
};