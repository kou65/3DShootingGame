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
#include"../Lib/Shader/ShaderFunc/DepthShadowShader/DepthShadowEffectFile.h"
#include"../Lib/Shader/ShaderFunc/ZTexture/ZTexture.h"
#include"../Lib/3D/FBX/FbxFile/Fbx.h"
#include"../Lib/Shader/ShaderFunc/Declation/VertexDeclation.h"
#include"../Lib/Shader/ShaderFunc/BlurFilter/BlurFilter.h"
#include"../Debugger/SoftShadowDebug/SoftShadowDebug.h"
#include"../Debugger/FbxDebug/FbxDebug.h"
#include<string>




// シングルトンデバッグモード
class Debugger{
public:


	/**
	* @brief 初期化
	*/
	void Init();


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
	* @brief ライトデバッグ描画
	*/
	void LightDebugDraw();


	/**
	* @brief シャドウ描画2
	*/
	void ObjShadowDraw();


	/**
	* @brief LightShadow描画
	*/
	void DrawLightShadow();


	/**
	* @brief ライト初期化
	*/
	void UpdateLight();


	/**
	* @brief 影初期化
	*/
	void UpdateShadow();


	/**
	* @brief 初期シェーダー
	*/
	void InitXFileShader();


	/**
	* @brief 固定スプライト生成
	*/
	void CreateSprite();


	/**
	* @brief シャドウ描画
	*/
	void XFileShadowDraw();


	/**
	* @brief Zテクスチャ書き込み
	*/
	void XFileZTextureWrite();


	/**
	* @brief Zテクスチャ描画
	*/
	void ZTextureDraw(
		IDirect3DTexture9*p_tex,
		Vec2 pos = Vec2(0.f,0.f),
		Vec2 scale = Vec2(1000.f,1000.f)
	);

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
	IDirect3DDevice9*mp_device;

	//! 標準ライト
	DefaultLight *light;

	//! ブラーフィルター
	BlurFilter m_blur;

	//! ソフトシャドウのデバッグクラス
	SoftShadowDebug m_ss_debug;

	//! fbxのデバッグ
	FbxDebug m_fbx_debug;

	//! カメラ
	Camera3D *mp_camera;

	//! FPS
	FPS *mp_fps;

	//! 総合マテリアル数
	int m_total_material_num;

	//! 固定スプライト
	ID3DXSprite* cpSprite;

	//! ワールドXFile位置
	Vec3 m_cube_pos;
	Vec3 m_plate_pos;

	//! 影関連
	DWORD dwMatNum, dwMatNum_Plate;
	ID3DXMesh *cpMeshCube, *cpMeshPlate;
	DepthShadowShader *mp_shadow;
};

#endif