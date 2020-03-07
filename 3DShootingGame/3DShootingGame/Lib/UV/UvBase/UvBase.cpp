#include"UvBase.h"



UvRect UvBase::GetUvRect() {
	return m_uv_rect;
}


void UvBase::SetMaxMinUvParam(
	const Vec2&max,
	const Vec2&min
) {

	// 右上なので横最大
	m_uv_rect.top_right_pos.x = max.x;
	m_uv_rect.top_right_pos.y = min.y;

	// 左上なのでどちらも最小
	m_uv_rect.top_left_pos.x = min.x;
	m_uv_rect.top_left_pos.y = min.y;

	// 右下なのでどちらも最大
	m_uv_rect.bottom_right_pos.x = max.x;
	m_uv_rect.bottom_right_pos.y = max.y;

	// 左下なので下だけ最大
	m_uv_rect.bottom_left_pos.x = min.x;
	m_uv_rect.bottom_left_pos.y = max.y;
}