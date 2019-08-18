#pragma once
#include<timeapi.h>
#include<Windows.h>

#pragma comment(lib,"winmm.lib")

/**
* @file FPS.h
* @brief フレーム処理
*/

// MEMO
// システムの時間(ミリ秒単位)を取得する
// 戻り値　システムの時間
// timeGetTime()

// MEMO
// タイマーの精度を変更する
// 引数 タイマーの新しい精度(ms)
// timeBeginPeriod()

// MEMO
// timeBeginPeriodで設定した内容を解除する
// 引数 timeBeginPeriodで設定した引数
// timeEndPeriod()


class FPS {
public:

	FPS();

	FPS(DWORD frame);

	void DebugDraw();

	void FrameWait();

private:

	int m_min_fream_time;

	// フレームカウント
	int m_frame_count;

	// 現在
	int m_current_time;

	// 過去
	int m_prev_time;
};