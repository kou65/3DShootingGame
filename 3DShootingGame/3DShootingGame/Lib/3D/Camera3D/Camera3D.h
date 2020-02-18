
#ifndef CAMERA_3D
#define CAMERA_3D

#include"../../Graphics/Graphics.h"
#include"CameraData.h"


class Camera3D {
public:

	// カメラのモード
	enum CameraMode {
		FPS,
		TPS
	};

public:

	// コンストラクタ
	Camera3D(
		CameraMode camera_mode,
		CameraData data
	);

	// 更新
	void Update();

	// カメラをセット
	void SetCamera(const CameraData&camera_data);
	
	// 位置をセット
	void SetPos(const D3DXVECTOR3 &pos);

	// 軸をセット
	void SetAxis(const D3DXVECTOR3&axis);

	// 位置を返す
	D3DXVECTOR3 GetPos();

	// 位置を加算
	void AddPos(const D3DXVECTOR3&pos);

	// 回転値を加算
	void AddRotation(const D3DXVECTOR3&rotation);

	// 移動値を加算
	void AddMove(const D3DXVECTOR3&move);

	// 軸座標加算
	void AddAxis(const D3DXVECTOR3&axis);

	// 注視点
	void SetLookAtPoint(const D3DXVECTOR3&point);

	// 注視点加算
	void AddLookAtPoint(const D3DXVECTOR3&point);

	// デバッグ描画
	void TransformDraw(float pos_x = 100.f,float pos_y = 100.f);

private:

	// Vec3の初期化
	void D3DXVec3Init(D3DXVECTOR3&init_vec3);

	// 1人称カメラ変換
	void FPSTransform();

	// 3人称カメラ変換
	void TPSTransform();

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

	CameraData m_data;
	// カメラモード
	CameraMode m_mode;
};

#endif 