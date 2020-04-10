#pragma once
#include<timeapi.h>
#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
#include"../Vec2/Vec2.h"


// GAMEWAORK LAB
// 参照↑
//#define	TRACE(_format_, ...)\
//		{\
//			char temp[MAX_PATH];\
//			sprintf(temp, _format_, __VA_ARGS__);\
//			OutputDebugString(temp);\
//		}

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

	enum DisplayMode {
		FONT,
		CONSOLE
	};

	// ミリ秒
	const int MS = 1000;

	// デフォルト
	const int FPS_60 = 60;
public:

	FPS();

	FPS(DWORD frame);

	// フォント
	void DebugDraw(
		const Vec2&draw_pos = Vec2(0.f,0.f),
		const int&font_size = 256,
		bool is_consol = false
	);

	void FrameWait();

private:

	float m_min_fream_time;

	// フレームカウント
	int m_frame_count;

	// 現在
	int m_current_time;

	// 過去
	int m_prev_time;

	// 現在のfps値
	int m_current_fps;

	// 表示モード
	DisplayMode dispaly_mode;
};