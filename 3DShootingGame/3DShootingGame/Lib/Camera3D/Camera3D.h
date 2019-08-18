#pragma once
#include"../D3D9/D3D9.h"



class Camera3D {
public:

	// カメラのモード
	enum CameraMode {
		FPS,
		TPS
	};

public:

	Camera3D(CameraMode camera_mode);

	void Update();

	void SetPos(const D3DXVECTOR3 &pos);

	void AddPos(const D3DXVECTOR3&pos);

	void AddRotation(const D3DXVECTOR3&rotation);

	void AddMove(const D3DXVECTOR3&move);

	void SetLookAtPoint(const D3DXVECTOR3&point);

	void AddLookAtPoint(const D3DXVECTOR3&point);

	void TransformDraw();

private:

	// Vec3の初期化
	void D3DXVec3Init(D3DXVECTOR3&init_vec3);

	// 1人称カメラ変換
	void FPSTransform();

	// 3人称カメラ変換
	void TPSTransform(
		// 軸の位置
		const D3DXVECTOR3 &axis_pos,
		// 軸からの距離の位置にカメラを置く
		const D3DXVECTOR3 &axis_distance
	);

	// 注視点行列変換
	D3DXMATRIX CalcLookAtPointMatrix(
		D3DXMATRIX* pout,
		D3DXVECTOR3* pPos,
		D3DXVECTOR3* pLook,
		D3DXVECTOR3* pUp
	);

	// Y軸回転による方向ベクトルを返す
	D3DXVECTOR3 GetYAxisDirectionVector();

	// TODO まだ使えません
	void TPSQuaternionTransform();

	void ProjectionTransform();

private:

	// 位置
	D3DXVECTOR3 m_pos;

	// 移動値
	D3DXVECTOR3 m_move;

	// カメラの回転値
	D3DXVECTOR3 m_rotation;

	// 注視点
	D3DXVECTOR3 m_look_at_point;

	// 上方向
	D3DXVECTOR3 m_up_direction;

	// カメラモード
	CameraMode m_camera_mode;
};