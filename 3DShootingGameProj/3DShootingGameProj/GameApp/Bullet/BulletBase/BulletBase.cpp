#include"BulletBase.h"
#include"../../../Lib/Math/Math.h"



void BulletBase::Limit() {

	float l = D3DXVec3Length(&m_pos);

	if (m_pos.x >= m_limit_distance.x ||
		m_pos.y >= m_limit_distance.y ||
		m_pos.z >= m_limit_distance.z) {
		m_is_active = false;
	}
}


void BulletBase::Limit(Vec3&vec) {

	Vec3 d_l;

	d_l = Math::GetVec1ToVec2(vec, m_pos);

	// ‹——£Š„‚èo‚µ
	float l = Math::GetLength(d_l);

	if (l >= m_limit_distance.x ||
		l >= m_limit_distance.y ||
		l >= m_limit_distance.z) {
		m_is_active = false;
	}
}