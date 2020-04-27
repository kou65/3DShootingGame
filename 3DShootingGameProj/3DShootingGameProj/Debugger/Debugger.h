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


	/**
	* @brief breakを行う
	* @return bool
	*/
	bool IsEnd();


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw();


	/**
	* @brief 解放
	*/
	void Release();


	/**
	* @brief インスタンスを返す
	*/
	static Debugger&GetInstance();

private:


	/**
	* @brief 初期シェーダー
	*/
	void InitShader();


	/**
	* @brief シャドウ描画
	*/
	void ShadowDraw();


	/**
	* @brief Zテクスチャ描画
	*/
	void ZTextureDraw();


	/**
	* @brief ライトデバッグ描画
	*/
	void LightDebugDraw();


	/**
	* @brief シャドウ描画2
	*/
	void ShadowDraw2();

private:


	/**
	* @brief コンストラクタ
	*/
	Debugger();


	/**
	* @brief カメラの移動
	*/
	void CameraMove();


	/**
	* @brief カメラの回転
	*/
	void CameraRotation();


private:

	//! デバイス
	IDirect3DDevice9*m_p_device;

	//! 標準ライト
	DefaultLight *light;

	//! カメラ
	Camera3D *m_p_camera;

	//! FPS
	FPS *m_p_fps;

	//! 総合マテリアル数
	int m_total_material_num;

	//! ライトデータ
	LightData m_light_data;

	//! 影関連
	DWORD dwMatNum, dwMatNum_Plate;
	ID3DXMesh *cpMeshCube, *cpMeshPlate;
	DepthShadowShader m_shadow_effect;
	ZTexture m_z_tex_effect;
	IDirect3DTexture9*m_p_shadow_tex;
};

#endif