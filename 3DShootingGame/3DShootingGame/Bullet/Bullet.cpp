#include"Bullet.h"




Bullet::Bullet(
	const float &pos_x,
	const float &pos_y,
	const float &pos_z,
	const float &radius,
	const float &speed
) {

	// 位置
	m_pos.x = pos_x;
	m_pos.y = pos_y;
	m_pos.z = pos_z;

	// 角度
	m_radius = radius;

	// 移動速度
	m_speed = speed;
}


void Bullet::Update() {

}


void Bullet::Draw() {

}


void Bullet::Move() {

	// 角度の対して移動速度を掛けて進ませる
	m_pos.x = cosf(m_radius) * m_speed;
	m_pos.z = sinf(m_radius) * m_speed;


}


void Bullet::Exit() {

	if (m_pos.x >= LIMIT_M ||
		m_pos.y >= LIMIT_M ||
		m_pos.z >= LIMIT_M) {
		m_is_active = false;
	}

}