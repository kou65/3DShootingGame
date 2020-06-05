
#ifndef CAMERA_3D
#define CAMERA_3D

#include"../../Graphics/Graphics.h"
#include"CameraData.h"



/**
* @brief 3Dで使うカメラクラス
*/
class Camera3D {
public:

	
	/**
	* @enum CameraMode カメラの取り方モード
	*/
	enum CameraMode {
		FPS,
		TPS
	};

public:


	/**
	* @brief コンストラクタ
	* @param[in] camera_mode カメラの設定モード
	* @param[in] data カメラデータ
	*/
	Camera3D(
		CameraMode camera_mode,
		CameraData data
	);


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief カメラデータセット
	* @param[in] camera_data カメラデータ構造体
	*/
	void SetCamera(const CameraData&camera_data);
	

	/**
	* @brief 位置をセット
	* @param[in] pos 位置
	*/
	void SetPos(const D3DXVECTOR3 &pos);


	/**
	* @brief TPSの時の回転軸をセットする
	* @param[in] axis 回転軸
	*/
	void SetAxis(const D3DXVECTOR3&axis);


	/**
	* @brief 位置を返す
	*/
	D3DXVECTOR3 GetPos();


	/**
	* @brief 注視点情報を返す
	*/
	D3DXVECTOR3 GetEyePos();


	/**
	* @brief 変換済み位置を返す
	*/
	D3DXVECTOR3 GetTransInversPos();


	/**
	* @brief fps変換済み逆行列を返す
	*/
	D3DXMATRIX GetFpsTransInversMatrix();

	
	/**
	* @brief 位置を加算
	* @brief pos 位置
	*/
	void AddPos(const D3DXVECTOR3&pos);


	/**
	* @brief 回転値を加算
	* @param[in] rotation 回転値
	*/
	void AddRotation(const D3DXVECTOR3&rotation);


	/**
	* @brief 移動値を加算
	* @param[in] move 移動値
	*/
	void AddMove(const D3DXVECTOR3&move);


	/**
	* @brief 軸座標加算
	* @param[in] axis 軸
	*/
	void AddAxis(const D3DXVECTOR3&axis);


	/**
	* @brief 注視点セット
	* @param[in] point 注視点位置
	*/
	void SetLookAtPoint(const D3DXVECTOR3&point);

	
	/**
	* @brief 注視点加算
	* @param[in] point 注視点加算
	*/
	void AddLookAtPoint(const D3DXVECTOR3&point);


	/**
	* @brief デバッグ描画
	* @param[in] pos_x デバッグ表示位置x
	* @param[in] pos_y デバッグ表示位置y
	*/
	void TransformDraw(float pos_x = 100.f,float pos_y = 100.f);

private:


	/**
	* @brief Vec3の初期化
	* @param[in] init_vec3 vec3構造体
	*/
	void D3DXVec3Init(D3DXVECTOR3&init_vec3);


	/**
	* @brief 1人称カメラ変換
	*/
	void FPSTransform();


	/**
	* @brief 3人称カメラ変換
	*/
	void TPSTransform();


	/**
	* @brief 注視点行列変換
	* @param[in] pout 外部算出
	* @param[in] p_posカメラ位置 
	* @param[in] p_look 注視点位置
	* @param[in] p_up アップベクトル
	*/
	D3DXMATRIX CalcLookAtPointMatrix(
		D3DXMATRIX* pout,
		D3DXVECTOR3* p_pos,
		D3DXVECTOR3* p_look,
		D3DXVECTOR3* p_up
	);


	/**
	* @brief Y軸回転による方向ベクトルを返す
	*/
	D3DXVECTOR3 GetYAxisDirectionVector();

	// TODO まだ使えません
	void TPSQuaternionTransform();

	// TODO まだ使えません
	void ProjectionTransform();

private:

	//! カメラデータ
	CameraData m_data;

	//! カメラモード
	CameraMode m_mode;

	//! FPS変換済み逆行列変換済み行列
	D3DXMATRIX m_fps_trans_inverse_mat;
};

#endif 