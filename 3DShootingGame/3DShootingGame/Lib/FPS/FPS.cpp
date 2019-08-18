#include"../D3DFont/D3DFont.h"
#include"FPS.h"// 上と入れ替えることで相対参照を起こしていた



FPS::FPS() : 
m_frame_count(0),
m_current_time(0)
{

	// タイマー精度変更
	timeBeginPeriod(1);
	
	// 1フレームの時間
	m_min_fream_time = 1000 / 60;

	// 今の時間を取得
	m_prev_time = timeGetTime();

}


FPS::FPS(DWORD frame) : 
m_frame_count(0),
m_current_time(0)
{
	// タイマー精度変更
	timeBeginPeriod(1);

	// フレームレートが0以下なら
	if (frame <= 0) {
		// 1フレームの時間
		m_min_fream_time = 1000 / 60;
	}
	else {
		// 1フレームの時間
		m_min_fream_time = 1000 / frame;
	}

	// 今の時間を取得
	m_prev_time = timeGetTime();
}


void FPS::DebugDraw() {

	// 前回の計測時間
	static int prev_time = timeGetTime();

	// 今回の計測時間
	m_current_time = timeGetTime();

	// 1ループごとに表示する
	m_frame_count++;

	// 1秒(1000ms)を超えたら表示
	if (m_current_time - prev_time >= 1000) {
		D3DFont::Draw(300.f, 300.f, 256, "fream_count = %d",m_frame_count);
		m_frame_count = 0;
	}
}


void FPS::FrameWait() {

	// 現在のフレーム受け取り
	FPS::m_current_time = timeGetTime();

	// 経過時間 = 今の時間 - 前フレームの時間 < フレームレート
	// フレームレートの方が大きいなら待つ
	while(m_current_time - m_prev_time < m_min_fream_time) {

		// 1ms待機
		Sleep(1);

		// 現在の時間取得
		m_current_time = timeGetTime();
	}
	
	// 前の時間を現在にする
	m_prev_time = m_current_time;
	
}