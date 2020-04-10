#ifndef DEBUGGER_H
#define DEBUGGER_H


#include"../Lib/DirectInput/KeyBoard/KeyBoard.h"
#include"../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include"../Lib/Sound/Sound.h"
#include"../Lib/3D/Camera3D/Camera3D.h"
#include"../Lib/3D/D3DDefaultMesh/D3DDefaultMesh.h"
#include"../Lib/FPS/FPS.h"
#include"../Lib/3D/DefaultLight/DefaultLight.h"
#include"../Lib/3D/IndexBuffer/IndexBuffer.h"
#include"../Lib/3D/VertexBuffer/VertexBuffer.h"
#include"../Lib/3D/OBJ/OBJFile.h"
#include"../Lib/EffectFileShader/DepthShadowShader/DepthShadowEffectFile.h"
#include"../Lib/EffectFileShader/ZTexture/ZTexture.h"
#include"../Lib/3D/FBX/FbxFile/Fbx.h"
#include"../Lib/EffectFileShader/Declation/VertexDeclation.h"
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

	void LightDebugDraw();

	void ShadowDraw2();

private:

	Debugger();

	void CameraMove();

	void CameraRotation();

private:

	IDirect3DDevice9*m_p_device;

	DefaultLight *light;

	Camera3D *m_p_camera;

	FPS *m_p_fps;

	int m_total_material_num;

	LightData m_light_data;

	DWORD dwMatNum, dwMatNum_Plate;
	ID3DXMesh *cpMeshCube, *cpMeshPlate;
	DepthShadowShader m_shadow_effect;
	ZTexture m_z_tex_effect;
	IDirect3DTexture9*m_p_shadow_tex;
};

#endif