#include"AnimationCustomVertex.h"



AnimationCustomVertex::AnimationCustomVertex() {


	vertex.x = vertex.y = vertex.z = 0.f;
	normal.x = normal.y = normal.z = 0.f;
	uv.x = uv.y = 0.f;
	color = 0xfffffff;

	for (int i = 0; i < 4; i++) {

		bone_index[i] = -1;
		weight[i] = 0.f;
	}

	// ボーンインデックス初期化
	bone_index[0] =
		bone_index[1] =
		bone_index[2] =
		bone_index[3] = -1;

	weight[0] =
		weight[1] =
		weight[2] =
		weight[3] =
		0.f;
}