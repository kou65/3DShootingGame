#include"../D3DFont/D3DFont.h"
#include"FPS.h"// 上と入れ替えることで相対参照を起こしていた



FPS::FPS() : 
m_frame_count(0),
m_current_time(0)
{

	// タイマー精度変更
	timeBeginPeriod(1);
	
	// 1フレームの時間
	m_min_fream_time = (float)MS / FPS_60;

	// 今の時間を取得
	m_prev_time = timeGetTime();

	// 現在のfps値
	m_current_fps = 0;

	// デフォルトフォント
	dispaly_mode = FONT;
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
		m_min_fream_time = (float)MS / FPS_60;
	}
	else {
		// 1フレームの時間(1000)
		m_min_fream_time = (float)MS / frame;
	}

	// 今の時間を取得
	m_prev_time = timeGetTime();

	// 現在のfps値
	m_current_fps = 0;
}


void FPS::DebugDraw(
	const Vec2&draw_pos,
	const int&font_size
) {

	// 前回の計測時間
	static int prev_time = timeGetTime();

	// 今回の計測時間
	static int current_time = timeGetTime();

	// 1ループごとに表示する
	m_frame_count++;

	// 1秒(1000ms)を超えたら表示
	if (current_time - prev_time >= MS) {
		
		// 現在のfps値を受け取る
		m_current_fps = m_frame_count;

		// 後の時間に現在の時間を渡す
		prev_time = current_time;

		// 現在のフレーム値を0にする
		m_frame_count = 0;
	}
	else {
		// 現在の時間を取得
		current_time = timeGetTime();
	}

	// デバッグモードで表示
	D3DFont::Draw(draw_pos.x,draw_pos.y,font_size, "FPS = %d",m_current_fps);

	// コンソールで表示
	//OutputDebugString("");
}


void FPS::FrameWait() {


	// 現在のフレーム受け取り
	m_current_time = timeGetTime();

	// 経過時間 = 今の時間 - 前フレームの時間 < フレームレート
	float elapsed_time = 
		(float)((m_current_time - m_prev_time));

	// フレームレートの方が大きいなら待つ
	while(elapsed_time < m_min_fream_time){

		// タイマー精度変更
		timeBeginPeriod(1);

		// 1ms待機(1秒)
		Sleep(1);

		// 現在の時間取得して経過させる
		m_current_time = timeGetTime();

		// 毎回行う
		elapsed_time = 
			(float)((m_current_time - m_prev_time));
	}
	
	// 前の時間を現在にする
	m_prev_time = m_current_time;

	// sleepの精度を上げる
	timeEndPeriod(1);
}

