#include"Camera3D.h"
#include"../../DirectInput/DirectInput.h"
#include"../../D3DFont/D3DFont.h"




Camera3D::Camera3D(
	CameraMode camera_mode,
	CameraData data
) {

	// 位置の初期化
	D3DXVec3Init(m_data.pos);

	// データを受け渡す
	m_data = data;

	// 上方向の定義
	m_data.up_direction.x = 0.f;
	m_data.up_direction.y = 1.f;
	m_data.up_direction.z = 0.f;

	// カメラモードの代入
	m_mode = camera_mode;
}


void Camera3D::D3DXVec3Init(D3DXVECTOR3&init_vec3) {

	// 初期化
	init_vec3.x = 0.f;
	init_vec3.y = 0.f;
	init_vec3.z = 0.f;
}


void Camera3D::Update() {

	switch (m_mode) {

	case FPS:
		FPSTransform();

		break;

	case TPS:
		TPSTransform(
			D3DXVECTOR3(m_data.pos.x, m_data.pos.y, m_data.pos.z),
			D3DXVECTOR3(m_data.axis.x, m_data.axis.y, m_data.axis.z)
		);

		break;
	}

	// プロジェクション座標変換
	ProjectionTransform();
}


void Camera3D::TransformDraw(float pos_x,float pos_y) {

	// カメラ位置描画
	D3DFont::Draw(pos_x, 100.f + pos_y, 256, "postion_x => %f", m_data.pos.x);
	D3DFont::Draw(pos_x, 120.f + pos_y, 256, "postion_y => %f", m_data.pos.y);
	D3DFont::Draw(pos_x, 140.f + pos_y, 256, "postion_z => %f", m_data.pos.z);

	D3DFont::Draw(pos_x, 180.f + pos_y, 256, "rotation.x => %f", m_data.rota.x);


}


/*
正規化済みの回転行列と移動行列を加算、移動 
回転行列 * 合成済み移動値
で移動できる

厳密には回転ラジアンと掛け合わせることで
その方向に移動する
正規化するのは1にして、回転値だけ出すため
内積や外積もこれらに当てはまる

回転方向を移動に加算

進む方向への回転方向を移動に加算して移動
*/

void Camera3D::FPSTransform() {

	// カメラ情報を格納する行列
	D3DXMATRIX matrix_view;
	D3DXMATRIX matrix_trans;
	D3DXMATRIX matrix_pos;
	D3DXMATRIX matrix_rotation;

	// 各種回転行列
	D3DXMATRIX matrix_rotation_x;
	D3DXMATRIX matrix_rotation_y;

	// 初期化
	D3DXMatrixIdentity(&matrix_view);

	// 正規化し方向だけもらう。(0,0,1)でもいい
	D3DXVec3Normalize(&m_data.move_num, &m_data.move_num);

	// X軸回転
	D3DXMatrixRotationX(
		&matrix_rotation_x,
		D3DXToRadian(m_data.rota.y)
	);

	// Y軸回転
	D3DXMatrixRotationY(
		&matrix_rotation_y,
		D3DXToRadian(m_data.rota.x));


	// 位置へ移動値加算(4*4の行列をベクトルに掛け算)

	// デバッグモードなら
	if (m_data.is_debug == true) {
		D3DXVec3TransformNormal(
			&m_data.move_num,
			&m_data.move_num,
			&matrix_rotation_x);
	}

	// 移動値
	D3DXVec3TransformNormal(
		&m_data.move_num,
		&m_data.move_num,
		&matrix_rotation_y);

	// 回転方向を加算した移動加算
	m_data.pos += m_data.move_num;

	// 初期値
	D3DXMatrixTranslation(
		&matrix_pos,
		m_data.pos.x,
		m_data.pos.y,
		m_data.pos.z
	);

	// 移動 * 回転 * 初期位置で合成
	matrix_pos = matrix_rotation_x * matrix_rotation_y * matrix_pos;
	

	// 逆行列変換
	D3DXMatrixInverse(&matrix_view, NULL, &matrix_pos);

	// ビュー座標変換
	Graphics::GetInstance()->GetDevice()->SetTransform(
		D3DTS_VIEW,
		&matrix_view);

	// 移動を初期化
	D3DXVec3Init(m_data.move_num);
}


D3DXMATRIX Camera3D::CalcLookAtPointMatrix(
	D3DXMATRIX* pout,
	D3DXVECTOR3* pPos,
	D3DXVECTOR3* pLook,
	D3DXVECTOR3* pUp
) {

	D3DXVECTOR3 X, Y, Z, D;
	D = *pLook - *pPos;
	D3DXVec3Normalize(&D, &D);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, pUp), &D);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Z, D3DXVec3Cross(&Z, &X, &Y));

	pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
	pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
	pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
	pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

	return *pout;
}


void Camera3D::TPSTransform(
	const D3DXVECTOR3 &axis_pos,
	const D3DXVECTOR3 &axis_distance
) {

	// ビュー行列
	D3DXMATRIX matrix_view;

	// 軸移動行列
	D3DXMATRIX mat_axis_trans;

	// Y軸回転行列
	D3DXMATRIX mat_rot_y;

	// X軸回転行列
	D3DXMATRIX mat_rot_x;

	// 合計行列
	D3DXMATRIX mat_total;

	// 行列の初期化
	D3DXMatrixIdentity(&mat_axis_trans);
	D3DXMatrixIdentity(&mat_total);

	// 注視点の初期化
	D3DXVec3Init(m_data.look_at_point);

	// 軸の位置
	//D3DXVECTOR3 axis_pos(-100.f,5.f,0.f);

	// 軸移動行列作成
	D3DXMatrixTranslation(
		&mat_axis_trans,
		axis_pos.x,
		axis_pos.y,
		axis_pos.z
	);

	// カメラのX軸回転
	D3DXMatrixRotationX(
		&mat_rot_x,
		D3DXToRadian(m_data.rota.y)
	);

	// カメラのY軸回転
	D3DXMatrixRotationY(
		&mat_rot_y,
		D3DXToRadian(m_data.rota.x)
	);

	D3DXMATRIX mat_trans;

	// カメラの移動
	{
	
		// 正規化
		D3DXVec3Normalize(&m_data.move_num, &m_data.move_num);

		// 頂点化
		D3DXVec3TransformCoord(
			&m_data.move_num,
			&m_data.move_num,
			&mat_rot_y
		);

		// 位置加算
		m_data.pos += m_data.move_num;

		// カメラ位置行列作成
		D3DXMatrixTranslation(
			&mat_trans,
			m_data.pos.x,
			m_data.pos.y,
			m_data.pos.z
		);
	}

	// カメラの初期位置と回転を合成
	mat_total = mat_rot_x * mat_rot_y * mat_axis_trans;

	// 移動をオフセット
	mat_total *= mat_trans;

	// 軸からのカメラの位置
	D3DXVECTOR3 new_pos = axis_distance;

	// 回転後の移動
	D3DXVec3TransformCoord(
		&new_pos,
		&new_pos,
		&mat_total
	);

	// 注視点を軸地点から位置を加算する
	m_data.look_at_point = axis_pos + m_data.pos;


	D3DXVec3Init(m_data.move_num);

	// ビュー座標変換
	{
		// ビュー行列作成
		D3DXMatrixLookAtLH(
			// ビュー行列
			&matrix_view,
			// カメラ位置
			&new_pos,
			// 注視点(どこを見ているか)
			&m_data.look_at_point,
			// 上方向の定義
			&m_data.up_direction
		);

		// ビュー座標変換
		Graphics::GetInstance()->GetDevice()->SetTransform(
			D3DTS_VIEW,
			&matrix_view);
	}

}


D3DXVECTOR3 Camera3D::GetYAxisDirectionVector() {

	D3DXVECTOR3 direction_y;

	// 正規化して、方向を受け取る
	D3DXVec3Normalize(&direction_y, &m_data.rota);

	return direction_y;
}


void Camera3D::TPSQuaternionTransform() {

	// 移動と回転の行列
	D3DXMATRIX matrix_rotation;
	D3DXMATRIX matrix_trans;

	// ビュー行列
	D3DXMATRIX matrix_view;

	// 軸の姿勢を整える
	{
		D3DXMatrixIdentity(&matrix_view);

		memcpy(matrix_view.m[0], &D3DXVECTOR3(1, 0, 0), sizeof(D3DXVECTOR3));
		memcpy(matrix_view.m[1], &D3DXVECTOR3(0, 1, 0), sizeof(D3DXVECTOR3));
		memcpy(matrix_view.m[2], &D3DXVECTOR3(0, 0, 1), sizeof(D3DXVECTOR3));
	}

	// 移動正規化
	D3DXVec3Normalize(&m_data.move_num, &m_data.move_num);

	// 平行移動値加算
	m_data.pos += m_data.move_num;

	// 移動
	D3DXMatrixTranslation(
		&matrix_trans,
		m_data.pos.x,
		m_data.pos.y,
		m_data.pos.z
	);

	// 注視点
	D3DXVECTOR3 direction(-10.f, 0.f, 0.f);

	D3DXVECTOR3 DL;

	// 回転面を求める
	{
		D3DXVec3TransformCoord(
			&DL,
			&direction,
			&matrix_view);
	}

	D3DXVECTOR3 rot_axis;
	const int Z_AXIS = 2;

	// 回転軸を求める
	{
		D3DXVECTOR3 *camera_z_axis = (D3DXVECTOR3*)matrix_view.m[Z_AXIS];
		D3DXVec3Cross(&rot_axis, &DL, camera_z_axis);
	}

	// ここで移動と回転を付ける
	D3DXMATRIX matrix_trans_rot;
	D3DXVECTOR3 camera_pos;

	// カメラ位置更新
	{
		D3DXQUATERNION trans_q;

		// クオータニオン回転
		D3DXQuaternionRotationAxis(
			&trans_q,
			&rot_axis,
			D3DXToRadian(m_data.rota.x)
		);

		// クオータニオンを行列に変換
		D3DXMatrixRotationQuaternion(&matrix_trans_rot, &trans_q);

		// 頂点変換
		D3DXVec3TransformCoord(
			&camera_pos,
			&camera_pos,
			&matrix_trans_rot
		);
	}

	// 空の方向
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	// 軸定義
	D3DXVECTOR3 X, Y, Z;

	// ビュー行列に変換
	matrix_view = matrix_trans_rot;

	// 回転行列作成
	{
		// Z軸方向(注視点 - 自分の位置)
		Z = direction - camera_pos;
		// 正規化
		D3DXVec3Normalize(&Z, &Z);

		// X軸算出
		D3DXVec3Cross(&X, &up, &Z);
		// 正規化
		D3DXVec3Normalize(&X, &X);

		// Y軸算出
		D3DXVec3Cross(&Y, &Z, &X);
		// 正規化
		D3DXVec3Normalize(&Y, &Y);
	}

	// ビュー行列の任意の軸に変換
	memcpy(matrix_view.m[0], &X, sizeof(D3DXVECTOR3));
	memcpy(matrix_view.m[1], &Y, sizeof(D3DXVECTOR3));
	memcpy(matrix_view.m[2], &Z, sizeof(D3DXVECTOR3));


	// ビュー座標変換
	{

		D3DXVECTOR3 *camera_z_axis = (D3DXVECTOR3*)matrix_view.m[Z_AXIS];
		camera_pos += m_data.pos.z * (*camera_z_axis);

		// 逆行列
		D3DXMatrixInverse(&matrix_view, NULL, &matrix_view);

		// ビュー座標変換
		Graphics::GetInstance()->GetDevice()->SetTransform(
			D3DTS_VIEW,
			&matrix_view);
	}

	D3DXVec3Init(m_data.move_num);
}


void Camera3D::ProjectionTransform() {

	// プロジェクション座標変換
	{
		// プロジェクション情報を格納するための行列
		D3DXMATRIX matrix_proj;

		D3DXMatrixPerspectiveFovLH(
			// 行列変数
			&matrix_proj,
			// 視野角
			D3DXToRadian(45),
			// アスペクト比
			640.f / 480.f,
			// 前方クリッピング領域
			0.1f,
			// 後方クリッピング領域
			2000.f
		);

		// プロジェクション座標変換
		Graphics::GetInstance()->GetDevice()->SetTransform(
			D3DTS_PROJECTION,
			&matrix_proj
		);
	}
}


void Camera3D::SetCamera(const CameraData&camera) {
	m_data = camera;
}


void Camera3D::SetPos(const D3DXVECTOR3 &pos) {
	m_data.pos = pos;
}


void Camera3D::AddPos(const D3DXVECTOR3&pos) {
	m_data.pos += pos;
}


void Camera3D::AddAxis(const D3DXVECTOR3&axis) {
	m_data.axis += axis;
}


void Camera3D::SetLookAtPoint(const D3DXVECTOR3&point) {
	m_data.look_at_point = point;
}


void Camera3D::AddLookAtPoint(const D3DXVECTOR3&point) {
	m_data.look_at_point = point;
}


void Camera3D::AddRotation(const D3DXVECTOR3&rotation) {
	m_data.rota += rotation;
}


void Camera3D::AddMove(const D3DXVECTOR3&move) {
	m_data.move_num += move;
}


D3DXVECTOR3 Camera3D::GetPos() {
	return m_data.pos;
}
