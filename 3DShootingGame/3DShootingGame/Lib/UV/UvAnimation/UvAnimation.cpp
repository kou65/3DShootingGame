#include"UvAnimation.h"
#include<vector>



// コンストラクタ
UvAnimation::UvAnimation(
	const int &div_width,
	const int &div_height,
	const int &select_graph_num,
	const bool &is_animation,
	const float &animation_frame,
	const Mode &use_type,
	const TurnType&turn_type
) {

	// 分割数
	param.division_width = div_width;
	param.division_height = div_height;

	// 分割総数
	m_total_div_num = param.division_width * param.division_height;

	// 矩形情報受け取り
	UvRect *rect = &m_uv_rect;

	// テクスチャ座標の初期化(頂点は全体に設定)
	rect->top_left_pos.x = 0.f;
	rect->top_left_pos.y = 0.f;
	rect->top_right_pos.x = MAX_UV_SIZE;
	rect->top_right_pos.y = 0.f;
	rect->bottom_left_pos.x = 0.f;
	rect->bottom_left_pos.y = MAX_UV_SIZE;
	rect->bottom_right_pos.x = MAX_UV_SIZE;
	rect->bottom_right_pos.y = MAX_UV_SIZE;

	// アニメーション関連初期化
	param.animation_frame = animation_frame;
	param.current_animation_count = 0;
	param.current_graph_num = select_graph_num;
	param.is_animation = is_animation;
	param.use_type = use_type;
	param.turn_type = turn_type;
	m_is_last_animation = false;
}


void UvAnimation::Update() {


	// アニメーションを行うなら
	if (param.is_animation == true) {

		// 右から画像を分割する
		switch (param.turn_type) {

		case TurnType::RIGHT:

			ToRightDivGraph(param.current_graph_num);
			break;

		case TurnType::LEFT:

			// 左から画像を分割する
			ToLeftDivGraph(param.current_graph_num);
			break;

		default:
			break;
		}
	}
	else {

	}
}


void UvAnimation::ToRightDivGraph(int div_num) {

	// 画像サイズを超えたら最大値を入れる
	div_num = m_total_div_num < div_num ? m_total_div_num : div_num;
	div_num = 0 > div_num ? m_total_div_num : div_num;

	// 先の頂点を含む
	const float NEXT_GO_GRAPH = 1.f;

	// 切り取る幅を変数に代入
	int div_x = 0;
	int div_y = 0;

	// 最初の0だけはあまり算出来ないので応急措置
	if (div_num == 0) {
		div_x = div_num;
		div_y = div_num;
	}
	else {
		// y座標は変換しないと正しい値が取れない
		div_x = ((div_num) % param.division_width);
		div_y = ((div_num) / param.division_width);// 割った数分,y座標が進む
	}
	// 切り出すテクスチャ座標を(最大サイズ/横の分割数=一つの画像の大きさ)*全体の位置で割り出す
	// 小さな画像を掛け算でずらしていく
	
	UvRect *rect = &m_uv_rect;

	// 左上
	rect->top_left_pos.x =    (MAX_UV_SIZE / param.division_width) * div_x;
	rect->top_left_pos.y =    (MAX_UV_SIZE / param.division_height)* div_y;
	// 右上
	rect->top_right_pos.x =   (MAX_UV_SIZE / param.division_width) * (div_x + NEXT_GO_GRAPH);
	rect->top_right_pos.y =   (MAX_UV_SIZE / param.division_height)* div_y;
	// 左下
	rect->bottom_left_pos.x =  (MAX_UV_SIZE / param.division_width) * div_x;
	rect->bottom_left_pos.y =  (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GO_GRAPH);
	// 右下
	rect->bottom_right_pos.x = (MAX_UV_SIZE / param.division_width) * (div_x + NEXT_GO_GRAPH);
	rect->bottom_right_pos.y = (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GO_GRAPH);
}


void UvAnimation::ToLeftDivGraph(int div_num) {

	// 画像サイズを超えたら最大値を入れる
	div_num = m_total_div_num < div_num ? m_total_div_num : div_num;
	div_num = 0 > div_num ? m_total_div_num : div_num;

	// 先の頂点を含む
	const float NEXT_GRAPH = 1.f;

	// 切り取る幅を変数に代入
	int div_x = 0;
	int div_y = 0;

	// 最初の0だけはあまり算出来ないので応急措置
	if (div_num == 0) {
		div_x = div_num;
		div_y = div_num;
	}
	else {
		// y座標は変換しないと正しい値が取れない
		div_x = ((div_num) % param.division_width);
		div_y = ((div_num) / param.division_width);// 割った数分,y座標が進む
	}
	// 切り出すテクスチャ座標を(最大サイズ/横の分割数=一つの画像の大きさ)*全体の位置で割り出す
	// 小さな画像を掛け算でずらしていく

	// 最大サイズを引く

	UvRect *rect = &m_uv_rect;

	// 左上
	rect->top_left_pos.x =  (MAX_UV_SIZE - (MAX_UV_SIZE / param.division_width)) - (MAX_UV_SIZE / param.division_width) * div_x;
	rect->top_left_pos.y = (MAX_UV_SIZE / param.division_height)* div_y;
	// 右上
	rect->top_right_pos.x = MAX_UV_SIZE - (MAX_UV_SIZE / param.division_width) * div_x;
	rect->top_right_pos.y = (MAX_UV_SIZE / param.division_height)* div_y;
	// 左下
	rect->bottom_left_pos.x = (MAX_UV_SIZE - (MAX_UV_SIZE / param.division_width)) - (MAX_UV_SIZE / param.division_width) * div_x;
	rect->bottom_left_pos.y = (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GRAPH);
	// 右下
	rect->bottom_right_pos.x = MAX_UV_SIZE - (MAX_UV_SIZE / param.division_width) * div_x;
	rect->bottom_right_pos.y = (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GRAPH);
}


void UvAnimation::SelectTexture(int u, int v) {

	// 画像サイズを超えたら最大値を入れる
	
	// 先の頂点を含む
	const float NEXT_GO_GRAPH = 1.f;

	// 切り取る幅を変数に代入
	int div_x = 0;
	int div_y = 0;

	int div_num_u = u * param.division_width;
	int div_num_v = v * param.division_width;

	// 最初の0だけはあまり算出来ないので応急措置
	if (u == 0 || v == 0) {
		div_x = u;
		div_y = v;
	}
	else {
		// y座標は変換しないと正しい値が取れない
		div_x = ((div_num_u) % param.division_width);
		div_y = ((div_num_v) / param.division_width);// 割った数分,y座標が進む
	}
	// 切り出すテクスチャ座標を(最大サイズ/横の分割数=一つの画像の大きさ)*全体の位置で割り出す
	// 小さな画像を掛け算でずらしていく

	UvRect *rect = &m_uv_rect;

	// 左上
	rect->top_left_pos.x = (MAX_UV_SIZE / param.division_width) * div_x;
	rect->top_left_pos.y = (MAX_UV_SIZE / param.division_height)* div_y;
	// 右上
	rect->top_right_pos.x = (MAX_UV_SIZE / param.division_width) * (div_x + NEXT_GO_GRAPH);
	rect->top_right_pos.y = (MAX_UV_SIZE / param.division_height)* div_y;
	// 左下
	rect->bottom_left_pos.x = (MAX_UV_SIZE / param.division_width) * div_x;
	rect->bottom_left_pos.y = (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GO_GRAPH);
	// 右下
	rect->bottom_right_pos.x = (MAX_UV_SIZE / param.division_width) * (div_x + NEXT_GO_GRAPH);
	rect->bottom_right_pos.y = (MAX_UV_SIZE / param.division_height)* (div_y + NEXT_GO_GRAPH);
}



void UvAnimation::AddAnimationCount() {
	// カウントを加算
	param.current_animation_count++;
}


void UvAnimation::UpdateAnimation(AnimationParamter&param){

	// カウンタがフレームを超えたら画像を進ませる
	if (param.animation_frame <= param.current_animation_count) {

		// カウンタ初期化
		param.current_animation_count = 0;

		// 画像更新
		param.current_graph_num++;
	}

	// 最大まできてLOOPなら0に戻す
	if(m_total_div_num <= param.current_graph_num && 
		param.use_type == Mode::LOOP){

		// 画像をリセット
		param.current_graph_num = 0;

		// アニメーション最後
		m_is_last_animation = true;
	}
	// 一回だけならアニメーションをなしにする
	else if (m_total_div_num <= param.current_graph_num &&
		param.use_type == Mode::ONLY_ONCE) {

		// 画像をリセット
		param.is_animation = false;

		// アニメーション最後
		m_is_last_animation = true;
	}

	if (param.is_animation == true) {
		// アニメーションカウントを加算
		param.current_animation_count++;
	}
}


void UvAnimation::ChangeRightAnimation() {

	param.turn_type = TurnType::RIGHT;
}

void UvAnimation::ChangeLeftAnimation() {

	param.turn_type = TurnType::LEFT;
}

// 一回かぎりを回復させる
void UvAnimation::RecoverAnimation() {

	// 画像をリセット
	param.current_graph_num = 0;

	param.is_animation = true;
}


// 拡縮させながらテクスチャ座標指定する
//void UV::UVGauge(const float &scale_x, const float &scale_y, const float&whole_scale_x, const float&whole_scale_y) {
//
//	float uv_cut_x = whole_scale_x/scale_x;
//	float uv_cut_y = whole_scale_y/scale_y;
//
//}


// 特定の位置の分割画像を取り出す
//void UV::UAxisVAxisSelectionDivGraph(int u, int v) {
//
//}



// アノテーションコメント
// todo
// fixme
// hack
// 

/*
std::vector<D3DXVECTOR2>uv_buf_vec{ uv_left_up_buffer, uv_right_up_buffer,
uv_left_down_buffer,uv_right_down_buffer };

uv_buf_vec[0].x = (WHOLE_UV_BUFFER / width_div_num) * div_num;
uv_buf_vec[0].y = (WHOLE_UV_BUFFER / height_div_num) * div_num;
*/

/*

// 1/4=0.25*0=0  1/4=0.25*1=0.25 1/4=0.25*2=0.5
// vectorで回せそう
// yだけ前進させればいい,だがxも0に戻さないと画像の規定値をこえてしまう

// 全体-横の切り出す値
// 反対の位置を出す為の全体 - 余り算で後ろの残りの画像 = 取り出したい画像
// 最後に分割位置-1はuv座標は0から始まる為一つずらす

// 8   %    11=3
// 8   %    10=2
// 8   %    9=1
// 8   %    8=0
// 8   %	7=7
// 8   %	6=6
// 8   %	5=5
// 8   %	4=4
*/