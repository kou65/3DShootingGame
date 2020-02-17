#ifndef DEBUGGER_H
#define DEBUGGER_H


#include"../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/3D/Camera3D/Camera3D.h"
#include"../Lib/3D/D3DDefaultMesh/D3DDefaultMesh.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/3D/Light/Light.h"
#include"../Lib/3D/IndexBuffer/IndexBuffer.h"
#include"../Lib/3D/VertexBuffer/VertexBuffer.h"
#include"../Lib/3D/OBJ/OBJFile.h"
#include"../Lib/EffectFileShader/DepthShadowEffectFile.h"
#include"../Lib/EffectFileShader/ZTexture.h"
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

	void InitShader();

	void ShadowDraw();

	void ZTextureDraw();

private:

	Debugger();

	void CameraMove();

	void CameraRotation();

	bool IsStopUpdate();

private:

	IDirect3DDevice9*m_p_device;

	Light *light;

	Camera3D *camera_3d;

	FPS *fps;

	IndexBuffer *index_buffer;

	int m_total_material_num;

	Sprite2DParameter s2d;


	DWORD dwMatNum, dwMatNum_Plate;
	ID3DXMesh *cpMeshCube, *cpMeshPlate;
	DepthShadowEffectFile m_d_effect;
	ZTexture m_z_tex_effect;
	IDirect3DTexture9*m_p_shadow_tex;
};

#endif