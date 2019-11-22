#pragma once
#include"../../Graphics/Graphics.h"
#include"../UvBase/UvBase.h"
#include"../UvRect/UvRect.h"
#include"../AnimationParameter/AnimationParameter.h"



// 何をすれば何をできるかを書く



class UvAnimation : public UvBase{
public:

	// デフォルト
	UvAnimation() {};

	// 引数に横と縦の分割したい画像数を入れる
	UvAnimation(
		const int &div_w,
		const int &div_h,
		const int &select_graph_num,
		const bool &is_animation,
		const float &animation_frame,
		const Mode &use_type,
		const TurnType&turn_type = TurnType::RIGHT
		);


	// 更新
	void Update()override;

	// HACK　まだ完成していません
	void SelectTexture(int u, int v);

	// アニメーション方向を変える
	void ChangeRightAnimation();
	void ChangeLeftAnimation();

	// アニメーションを回復させる
	void RecoverAnimation();

	// アニメーションを変更
	void UpdateAnimation(AnimationParamter&param);

	// 拡縮させながらテクスチャ座標指定する
	/*
	scale_xは実際に拡縮させる大きさ
	while_scaleは全体の大きさ
	*/
	//void UVGauge(const float &scale_x, const float &scale_y,const float&whole_scale_x,const float&whole_scale_y);

private:

	// 最大の頂点バッファ
	const float MAX_UV_SIZE = 1.f;

private:

	// フレームを加算
	void AddAnimationCount();

	// 右から画像を分割する
	void ToRightDivGraph(int divsion_num);

	// 左から画像を分割する
	void ToLeftDivGraph(int divsion_num);

private:

	// 分割する合計の数
	int m_total_div_num;

	// 最後のアニメーションか
	bool m_is_last_animation;

	// アニメーションパラメータ
	AnimationParamter param;
};



/* UVクラス使い方講座

この関数は統合画像専用です

Draw系と併用して使用します

RightRotationDivGraph(int divsion_num)
...統合画像を右から切り取っていく関数

div_w...      統合画像の横の分割数
div_h...      統合画像の縦の分割数
divsion_num...分割後の全ての画像

使用例

①コンストラクタで統合画像を縦4横4で切り取り
UV(4,4);


②(3,0)番目の画像を切り取る
RightAdvanceDivGraph(3);


③切り取った統合画像をゲッターで受け取る

D3DXVECTOR2 * = & D3DXVECTOR2 GetUvUpLeftBuffer()

④最後にそれぞれの頂点に振り分ける

// VERTEX3Dの初期化
CUSTOM_VERTEX cv[] =
{
{ x1,y1,0.0f,1.0f, up_left->x,up_left->y },       // 左上
{ x2,y1,0.0f,1.0f ,up_right->x,up_right->y },     // 右上
{ x2,y2,0.0f,1.0f ,down_right->x,down_right->y }, // 右下
{ x1,y2,0.0f,1.0f ,down_left->x,down_left->y },   // 左下
};

*/

/*

仕様は

ListDivGraph(1)としたら
左から1番目の画像が読み込まれる

多少大まかになるが汎用的なUVクラスをつくるか
List,Fan,Stripなどに独自で対応したものをそれぞれに作るか(その場合分割過程のみ同じになるが
それ以外は派生したものになる)

統合画像は2乗で割れる

2乗ずつでないと正確な分割ができないが、一応奇数も作って置く
2,4,8,16
*/