#include"FbxMotion.h"
#include"../../../Utility/Convert/Convert.h"
#include"../../../Utility/Math/Math.h"
#include"../FbxFunction/FbxFunction.h"
#include"../../Model/Model.h"
#include"../../../Texture/TextureManager/TextureManager.h"



FbxMotion::FbxMotion(){

	m_is_transition = false;
	m_anim_count = 0;
	m_stop_count = 0;
	m_frame = 0.f;

	// エフェクトの初期化
	m_vb_effect.Init();

	// 最初のアニメーションカウントを1に設定
	m_init_count = INIT_ANIMATION_COUNT;
}



void FbxMotion::SetTexture(
const std::string&texture_name
) {
	TextureManager::GetInstance()->
		GetTextureData(texture_name);
}


bool FbxMotion::IsMotionEnd() {

	// アニメーションが最大まできたら最初に戻す
	if (m_anim_count > m_stop_count - 2) {
		return true;
	}

	return false;
}


void FbxMotion::ShaderMotionDraw(
	const std::string&mesh_name,
	const int&vertex_num,
	const int&polygon_num,
	const int&mesh_idx,
	const D3DXMATRIX&world_mat
) {

	// モーション名取り出し
	std::string motion_name = 
		m_current_motion_name_list[mesh_name];

	// モーションリストを追加
	std::vector<D3DXMATRIX> mat =
		m_motion
		[motion_name][mesh_idx].
		d3d_animation_mat[m_anim_count].anim_mat_list;

	// ボーン数
	UINT bone_size = 
	m_motion
		[motion_name][mesh_idx].
		animation_matrix[m_anim_count].size();

	// ボーン行列セット
	m_vb_effect.SetWorldMatrix(world_mat);

	// ボーンセット
	m_vb_effect.SetBoneMatrix(
		mat.data()
	);

	m_vb_effect.SetBoneCount(bone_size);

	m_vb_effect.SetMaxIndex(
		bone_size
	);

	// 更新
	m_vb_effect.Update();

	UINT pass_num = 0;

	m_vb_effect.Begin(pass_num, 0);
	m_vb_effect.BeginPass(0);

	// 描画
	Graphics::GetInstance()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		vertex_num,
		0,
		polygon_num
	);

	m_vb_effect.EndPass();
	m_vb_effect.End();
}



void FbxMotion::SetMotion(
	const std::string &bone_name,
	const std::string &motion_name
)
{

	// モーション名代入
	m_current_motion_name_list[bone_name]
		= motion_name;

	// フレーム
	m_frame = 0.0f;

	if (m_motion.count(motion_name) == 0) {
		return;
	}

	// アニメーション最大数を入れる
	m_stop_count = 
		m_motion[motion_name].back().d3d_animation_mat.size();
}



void FbxMotion::ChangeMotion(
	const std::string &bone_name,
	const std::string& motion_name
) {

	// 次のモーションにする
	m_next_motion_name = motion_name;

	// トランジションを行う
	m_is_transition = true;

	// フレーム
	m_frame = 0.0f;

	// モーション数が見つからない
	if (m_motion.count(motion_name) == 0) {
		return;
	}

	// アニメーション最大数を入れる
	m_stop_count =
		m_motion[motion_name].back().d3d_animation_mat.size();
}


void FbxMotion::InitTransition(
	const std::string&mesh_name
) {

	// トランジション終了
	m_is_transition = false;

	// モーション名代入
	m_current_motion_name_list[mesh_name]
		= m_next_motion_name;

	// アニメーション最大数を入れる
	m_stop_count = 
		m_motion[m_next_motion_name].
		back().d3d_animation_mat.size();

	// 予約モーション名をなしにする
	m_next_motion_name = "";

	// トランジションフレーム初期化
	m_transition_frame = 0.f;

}


void FbxMotion::UpdateTransition(
	const std::string&mesh_name,
	const int&frame
) {


	// トランジション中なら
	if (m_is_transition == true) {

		// アニメーション更新
		m_transition_frame += TRANSITION_FRAME;

		// 最終アニメーションになったらトランジション終了
		if (m_transition_frame >= 1.f) {

			InitTransition(mesh_name);

			// アニメーションカウント数も初期化する
			m_anim_count = INIT_ANIMATION_COUNT;

		}
		
	}
}


void FbxMotion::Animate(
	const std::string&mesh_name,
	const float &frame
)
{

	// トランジション更新
	UpdateTransition(
		mesh_name
	);

	// アニメーション
	if (m_frame > frame) {
		
		// トランジションしていないなら
		if(m_is_transition == false){

		// トランジション中でないなら通常アニメーションをするので
		// アニメーションカウント加算
			m_anim_count++;
		}

		m_frame = 0;
	}
	else {

		// フレーム値加算
		m_frame++;
	}

	// アニメーションが最大まできたら最初に戻す
	if (m_anim_count > m_stop_count - 1) {
		m_anim_count = m_init_count;
	}

}


void FbxMotion::CPUSkinning(
	const std::string &mesh_name,
	std::vector<FbxMeshData>& mesh_data_list
) {

	// サイズが存在するか
	if (mesh_data_list.size() == 0){
		return;
	}

	// 頂点変形
	for (UINT mi = 0;
		mi < mesh_data_list.size();
		mi++)
	{

		// メッシュデータ受け取り(モーションデータと合っていない)
		FbxMeshData mesh_data
			= mesh_data_list[mi];

		// 元頂点がない場合
		if (mesh_data.mp_vertics == nullptr){
			return;
		}

		// 頂点バッファを定義
		SkinCustomVertex* vertices;

		// 頂点バッファのサイズ作成
		UINT size = 
			(UINT)((mesh_data.vertex_num) * 
				sizeof(SkinCustomVertex));

		// バッファをロックして書き込み可能にする
		mesh_data_list[mi].p_vertex_buffer->Lock(
			0,
			size,
			(void**)&vertices,
			0
		);

		if (m_is_transition == false) {
			// 最新アニメーション
			WeightSkinning(
				mesh_name,
				mesh_data,
				vertices,
				mesh_data.mp_vertics,
				mi
			);
		}
		// トランジション中
		else {

			// モーション変化
			TransitionMotion(
				mesh_name,
				mesh_data,
				vertices,
				mesh_data.mp_vertics,
				mi,
				m_next_motion_name,
				1
			);
		}

		// 頂点更新終了
		mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}



void FbxMotion::WeightSkinning(
	const std::string &mesh_name,
	FbxMeshData& mesh_data,
	SkinCustomVertex* vertices,
	SkinCustomVertex*mp_vertics,
	const int &cur_bone_index
) {

	// ボーン数受け取り
	int cluster_count = mesh_data.bone_num;

	// モーション名取得
	std::string motion_name = 
		m_current_motion_name_list[mesh_name];

	// モーションが入っているかどうか
	if (m_motion.count(motion_name) == 0) {
		return;
	}

	// モーションデータ(ここでエラー)
	MotionData *md = 
		&m_motion[motion_name][cur_bone_index];

	// 属性データ
	ElementData data =
		m_elem_type_list[mesh_name][cur_bone_index];

		// アニメーションリストサイズ
		UINT anim_list_size =
			md->d3d_animation_mat[m_anim_count].
			anim_mat_list.size();

		// 総頂点
		UINT vertex_num = 
		mesh_data.vertex_num;

		// 全ての頂点を調べる
		for (UINT v = 0;
			v < vertex_num;
			v++) {

			// 頂点
			D3DXVECTOR4 vec4 = 
				mp_vertics[v].vertex;

			// 一旦初期化
			vertices[v].vertex.x = 0.f;
			vertices[v].vertex.y = 0.f;
			vertices[v].vertex.z = 0.f;

			float x = 0.f;
			float y = 0.f;
			float z = 0.f;

			// d3d用
			D3DXVECTOR4 out;

			// ボーン数
			for (int b = 0; b < 4; b++) {

				// 中身がないなら
				if (vertices[v].weight[b] <= 0.f) {
					// そのあとの配列にもないと思うので抜ける
					break;
				}

				// 重み
				int w_idx = vertices[v].bone_index[b];
				float weight = vertices[v].weight[b];

				// インデックスに対応していないなら終了エラー
				if ((int)(anim_list_size) < w_idx) {
					break;
				}

				// 行列計算
				Utility::Math::MatrixMultipleVec4(
					out,
					vec4,
					md->d3d_animation_mat[m_anim_count].
					anim_mat_list[w_idx]
				);// vector

				// 頂点取り出し
				x = (float)out.x;
				y = (float)out.y;
				z = (float)out.z;

				// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
				vertices[v].vertex.x += x * weight;
				vertices[v].vertex.y += y * weight;
				vertices[v].vertex.z += z * weight;

			}
	}
}



void FbxMotion::TransitionMotion(
	const std::string &mesh_name,
	FbxMeshData& mesh_data,
	SkinCustomVertex* vertices,
	SkinCustomVertex*mp_vertics,
	const int &cur_bone_index,
	const std::string&next_motion_name,
	const int&start_time
) {

	// ボーン数受け取り
	int cluster_count = mesh_data.bone_num;

	// モーション名取得
	std::string motion_name =
		m_current_motion_name_list[mesh_name];

	// モーションが入っているかどうか
	if (m_motion.count(motion_name) == 0) {
		return;
	}

	// モーションデータ(ここでエラー)
	MotionData *md =
		&m_motion[motion_name][cur_bone_index];

	// 次のモーションデータ
	MotionData*next_md =
	&m_motion[next_motion_name][cur_bone_index];

	// 属性データ
	ElementData data =
		m_elem_type_list[mesh_name][cur_bone_index];

	// アニメーションリストサイズ
	UINT anim_list_size =
		md->d3d_animation_mat[m_anim_count].
		anim_mat_list.size();

	// 総頂点
	UINT vertex_num =
		mesh_data.vertex_num;

	// 全ての頂点を調べる
	for (UINT v = 0;
		v < vertex_num;
		v++) {

		// 頂点
		D3DXVECTOR4 vec4 =
			mp_vertics[v].vertex;

		// 一旦初期化
		vertices[v].vertex.x = 0.f;
		vertices[v].vertex.y = 0.f;
		vertices[v].vertex.z = 0.f;

		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		// d3d用
		D3DXVECTOR4 out;

		// 行列
		D3DXMATRIX mat;

		// ボーン数
		for (int b = 0; b < 4; b++) {

			// 中身がないなら
			if (vertices[v].weight[b] <= 0.f) {
				// そのあとの配列にもないと思うので抜ける
				break;
			}

			// 重み
			int w_idx = vertices[v].bone_index[b];
			float weight = vertices[v].weight[b];

			// インデックスに対応してないなら終了エラー
			if ((int)(anim_list_size) < w_idx) {
				break;
			}

			// メッシュとボーンは同じモーション数が違う
			// 現在のモーションと入れ替えるモーション行列補間
			FbxFunction::MatrixInterporate3(
				mat,
				// 前
				md->d3d_animation_mat[m_anim_count].anim_mat_list[w_idx],
				// 後
				next_md->d3d_animation_mat[start_time].
				anim_mat_list[w_idx],
				m_transition_frame
			);


			// 行列計算
			Utility::Math::MatrixMultipleVec4(
				out,
				vec4,
				mat
			);// vector

			// 頂点取り出し
			x = (float)out.x;
			y = (float)out.y;
			z = (float)out.z;

			// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
			vertices[v].vertex.x += x * weight;
			vertices[v].vertex.y += y * weight;
			vertices[v].vertex.z += z * weight;

		}
	}
}


bool FbxMotion::SelectAnimation(
	FbxScene*p_scene,
	FbxImporter*importer,
	int select_anim_num
) {
	
	if (importer == nullptr) {
		return false;
	}

	int anim_num = importer->
		GetAnimStackCount();

	// 総アニメーション数よりも大きいなら
	if (anim_num <= select_anim_num) {
		return false;
	}

	// 受け取るアニメーションを切り替え
	FbxAnimStack* pStack = p_scene->
		GetSrcObject<FbxAnimStack>(select_anim_num % anim_num);

	p_scene->SetCurrentAnimationStack(pStack);

	return true;
}


int FbxMotion::GetMotionNum(
	FbxImporter*importer
) {

	// 整数取得
	int animaStackCount = importer->GetAnimStackCount();
	return animaStackCount;
}


bool FbxMotion::SetAnimation(
	FbxScene*p_scene,
	FbxTime&start,
	FbxTime&stop,
	FbxTime&frame_time,
	const int&anim_num
) {


	// アニメーションファイル名を受け取る
	FbxArray<FbxString*> AnimStackNameArray;
	p_scene->
		FillAnimStackNameArray(AnimStackNameArray);

	// アニメーション情報が入ったスタックを受け取る
	FbxAnimStack* AnimationStack =
		p_scene->
		FindMember<FbxAnimStack>(AnimStackNameArray[anim_num]->Buffer());

	// アニメーションをセット
	p_scene->
		SetCurrentAnimationStack(AnimationStack);

	// アニメーション時間をセット
	FbxTakeInfo* take_info = p_scene->
		GetTakeInfo(*(AnimStackNameArray[anim_num]));

	// 最初と最後のフレームを取得
	start = take_info->mLocalTimeSpan.GetStart();
	stop = take_info->mLocalTimeSpan.GetStop();

	// 正規化したフレーム取得(現在は使っていない)
	{
		FbxTime t = take_info->mImportOffset;

		//60fpsでの開始時間を設定
		FbxLongLong start_f = (t.Get() + start.Get())
			/ FbxTime::GetOneFrameValue(FbxTime::eFrames60);

		//60fpsでの終了時間を設定
		FbxLongLong stop_f = (t.Get() + stop.Get()) /
			FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	}

	// アニメーションタイムをセット
	frame_time.SetTime(0, 0, 0, 1, 0,
		p_scene->GetGlobalSettings().GetTimeMode());

	return true;
}


bool FbxMotion::LoadMotion(
	FbxScene*p_scene,
	FbxImporter*p_importer,
	const std::string&bone_name,
	const std::string& motion_name,
	const int &select_motion_num
) {

	// アニメーション選択
	if (SelectAnimation(
		p_scene,
		p_importer,
		select_motion_num
	) == false) {
		return false;
	}

	// タイマー
	FbxTime frame_time, start, stop;

	// アニメーション情報受け取り
	if (SetAnimation(
		p_scene,
		start,
		stop,
		frame_time,
		select_motion_num
	) == false) {
		return false;
	}

	// ボーンデータ
	std::vector<BoneData> b_data = 
		m_bone_mat_list[bone_name];

	// メッシュ名
	int mesh_num = 
		p_scene->GetSrcObjectCount<FbxMesh>();

	// ボーンのリストサイズを代入
	UINT bone_lise_size =
	b_data.size();

	// メッシュ分回す
	for (UINT mi = 0;
		mi < bone_lise_size;
		mi++) {

		// スキン数取得
		int skin_num = b_data[mi].skin_num;

		// メッシュを取得
		FbxMesh * p_mesh =
			(p_scene)->GetSrcObject<FbxMesh>(mi);

		if (p_mesh == nullptr) {
			return false;
		}

		// スキンの数を取得
		int defor_count = 
			p_mesh->GetDeformerCount(FbxDeformer::eSkin);

		// スキン分回す
		for (int s = 0; s < defor_count; s++) {

			// ボーン数取得
			int bone_num = 
			b_data[mi].bone_num;

			// ボーンが入っていないのにアニメーションスタックが存在する場合があるので
			// その場合は戻す
			if (bone_num <= 0) {
				continue;
			}

		// アニメーション読み込み
		AnimationData a_data = LoadAnimation(
			p_scene,
			mi,
			skin_num,
			bone_num,
			start,
			stop,
			frame_time
		);

		// モーション受け取り
		MotionData m_data
			= CompositeMotionBoneMatrix(
				motion_name,
				a_data,
				b_data[mi].fbx_mat_list
			);

			// モーション情報の追加
			m_motion[motion_name].emplace_back(m_data);
		}
	}

	return true;
}


bool FbxMotion::LoadSkeletonMotion(
	FbxScene*p_scene,
	FbxImporter*p_importer,
	const std::string&bone_name,
	const std::string&motion_name,
	const int &select_motion_num
) {


	// アニメーション選択
	if(SelectAnimation(
		p_scene,
		p_importer,
		select_motion_num
	) == false) {
		return false;
	}

	// タイマー
	FbxTime frame_time, start, stop;

	// アニメーション情報受け取り
	if (SetAnimation(
		p_scene,
		start,
		stop,
		frame_time,
		select_motion_num
	) == false) {
		return false;
	}

	// ボーンデータ取得
	std::vector<BoneData>b_data = 
		m_bone_mat_list[bone_name];

	// 属性データ取得
	std::vector<ElementData>e_data =
	m_elem_type_list[bone_name];

	// ボーンのリストサイズを代入
	UINT bone_list_size = 
		b_data.size();

	UINT elem_list_size =
		e_data.size();

	// アニメーションデータ
	AnimationData a_data;

	// モーションデータ
	MotionData mo_data;

	int b_index = 0;
	int m_index = 0;

	// メッシュ分のバッファを確保
	UINT mesh_num_max = (UINT)
		(p_scene)->GetSrcObjectCount<FbxMesh>();

	// ボーンリストの数を元に作成
	for (UINT ei = 0;
		ei < elem_list_size;
		ei++) {

		// 上から順に属性ごとに行列を作る
		switch (e_data[ei].type){

		case ElementType::MESH:

			// 全てのメッシュ数分回す
			for (uint mi = 0; mi < mesh_num_max; mi++) {

				// メッシュアニメデータ読み込み
				if (LoadMeshAnimationData(
					p_scene,
					mi,
					// 現在の属性のメッシュノードを入れる
					e_data[ei % mesh_num_max].mesh_node_name,
					start,
					stop,
					frame_time,
					a_data // ここにアニメーションデータが入ってくる
				) == true) {

					break;
				}
			}

			// モーション変換データ取得
			mo_data = GetMotionTransformData(
				a_data
			);

			// ここで全ての頂点の重みを1.0にする

			break;

		case ElementType::BONE_MESH:

			// スキン数取得
			int skin_num = b_data[b_index].skin_num;

			// ボーン数取得
			int bone_num =
				b_data[b_index].bone_num;

			// ボーンが入っていないのにアニメーションスタックが存在する場合があるので
			// その場合は戻す
			if (bone_num <= 0) {
				continue;
			}

			// スケルトンアニメーション行列取得
			a_data = GetSkeletonAnimMat(
				p_scene,
				bone_name,
				b_index,
				start,
				stop,
				frame_time
			);

			// モーション受け取り
			mo_data
				= CompositeMotionBoneMatrix(
					motion_name,
					a_data,
					b_data[b_index].fbx_mat_list
				);

			// 次のボーン情報に行く
			b_index++;
			break;
		}

		// モーション情報の追加
		m_motion[motion_name].emplace_back(mo_data);
	}

	return true;
}



AnimationData FbxMotion::GetSkeletonAnimMat(
	FbxScene*p_scene,
	const std::string &mesh_name,
	const int &mi,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count
) {

	std::vector<BoneData> b_data =
		m_bone_mat_list[mesh_name];

	// 受け取る用のスケルトン
	FbxSkeleton*p_skel = nullptr;

	// タイムカウント初期化
	FbxTime time_count = start;

	// アニメーションデータ
	AnimationData data;

	// スケルトン数を返す
	UINT skeleton_num = (UINT)
		(p_scene)->GetSrcObjectCount<FbxSkeleton>();

	// モーション数がストップするまで
	while (time_count < stop) {

		// メッシュリスト追加
		data.mat.emplace_back();

		// ボーンリストサイズ
		UINT bone_list_size = 
			b_data[mi].bone_list.size();

		// 現在のスケルトン数
		int current_sleleton_count = 0;

		// ボーン数(内部配列が存在する)
		for (UINT bl = 0;
			bl < bone_list_size;
			bl++) {

			// スケルトンモーションが一致しているか
			if (FbxFunction::Skeleton::MatchSkeletonMotion(
				p_scene,
				b_data[mi].bone_list[bl].skeleton_name,
				&p_skel,
				skeleton_num,
				current_sleleton_count
			) == true) {

				// ここまでにはスケルトンが入っている

				// スケルアニメーション行列取得
				FbxAMatrix anim_mat =
					FbxFunction::Skeleton::
					GetSkeletonAnimationMatrix(
						p_skel,
						p_scene,
						time_count
					);

				// データに追加
				data.mat.back().emplace_back(anim_mat);

			}
			// 読み込めていなかったら
			else {

				// 後ろを探す
				// なのでもう一度最初から
				bl--;
				// 現在のスケルトンカウントリセット
				current_sleleton_count = 0;
			}
		}

		// 時間を追加
		time_count += frame_count;

	}

	return data;
}


MotionData FbxMotion::GetMotionTransformData(
	AnimationData&anim_data
) {

	// ここが間違っている場合がある

	MotionData mo_data;

	// 変換してモーションデータに代入

	// アニメーションリストサイズ分回す
	for (UINT i = 0; i < anim_data.mat.size(); i++) {

		// 配列
		mo_data.animation_matrix.emplace_back();
		mo_data.d3d_animation_mat.emplace_back();

		// アニメーションサイズ
		UINT anim_data_list_size = 
			anim_data.mat[i].size();

		D3DXMATRIX conv_mat;

		// クラスターサイズ分回す
		for (UINT c = 0; c < anim_data_list_size; c++) {

			// 行列
			FbxMatrix fbx_mat = 
				anim_data.mat[i][c];

			// アニメーション行列追加
			mo_data.animation_matrix.
				back().
				emplace_back(fbx_mat);

			// d3d行列に変換する
			FbxMatConvertD3DMat(&conv_mat, fbx_mat);

			// d3d用に代入
			mo_data.d3d_animation_mat.back().
				anim_mat_list.emplace_back(conv_mat);
		}
	}

	return mo_data;
}


bool FbxMotion::LoadMeshAnimationData(
	FbxScene*p_scene,
	const int&mesh_num,
	const std::string&mesh_node_name,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count,
	AnimationData&out_anim_data
) {


	if (p_scene == nullptr) {
		return false;
	}

	// メッシュ取得
	FbxMesh*p_mesh =
		p_scene->GetSrcObject<FbxMesh>(mesh_num);

	if (p_mesh == nullptr) {
		return false;
	}

	// ノード取得
	FbxNode*p_node =
		p_mesh->GetNode();

	// ルートノード取得
	FbxNode*p_root_node =
		p_scene->GetRootNode();

	// 現在のメッシュノード
	std::string cur_mi_node_name =
		p_node->GetName();

	// 同じメッシュノードか調べる
	if (Utility::Convert::
		IsStrCmp(mesh_node_name.c_str(), cur_mi_node_name.c_str())
		== false) {
		return false;
	}

	// アニメーションデータ
	AnimationData anim_data;

	// モーション作成
	MotionData motion;

	// スタートフレーム代入
	FbxTime time_count = start;

	// アニメーションデータ追加
	anim_data.mat.emplace_back();

	// モーション数がストップするまで読み込み続ける
	while (time_count < stop) {

		// 1姿勢行列を各頂点に掛け合わせる

		// fbx行列追加
		FbxMatrix out_mat =
			p_root_node->
			EvaluateGlobalTransform(time_count).Inverse() *
			p_node->
			EvaluateGlobalTransform(time_count);

		// 一つだけ追加
		anim_data.mat.back().emplace_back(out_mat);

		// 時間を追加
		time_count += frame_count;

		// アニメーション配列追加
		anim_data.mat.emplace_back();
	}

	// アニメーションデータ更新
	out_anim_data = anim_data;

	return true;
}


MotionData FbxMotion::CompositeMotionBoneMatrix(
	const std::string&motion_name,
	AnimationData&data,
	std::vector<FbxMatrix>&bone_mat_list
) {

	// モーション作成
	MotionData motion;

	// アニメーションスタック
	for (UINT a = 0; a < data.mat.size(); a++) {

		// 配列
		motion.animation_matrix.emplace_back();
		motion.d3d_animation_mat.emplace_back();

		// アニメーションサイズ
		UINT anim_data_list_size = 
		data.mat[a].size();

		// ボーンサイズよりアニメーションデータが多い場合、減らす
		if (anim_data_list_size > bone_mat_list.size()) {
			anim_data_list_size = bone_mat_list.size();
		}

		// ボーンサイズ
		for (UINT b = 0; b < anim_data_list_size; b++) {

			// ボーン行列変換(アニメーション行列 * 
			// オフセット変換済みボーン行列)
			FbxMatrix total_mat = 
				data.mat[a][b] * 
				bone_mat_list[b];

			D3DXMATRIX conv_mat;

			// アニメーション行列追加
			motion.animation_matrix.
				back().
				emplace_back(total_mat);

			// 変換して本体の行列に変換する
			FbxMatConvertD3DMat(&conv_mat, total_mat);

			// d3d用に代入
			motion.d3d_animation_mat.back().
				anim_mat_list.emplace_back(conv_mat);

			//// 最大ボーン数を過ぎたら入れない
			//if (b > MAX_BONE_MATRIX_NUM - 1) {
			//	break;
			//}
		}

	}

	return motion;

}


MotionData FbxMotion::CompositeMotionBoneMatrix(
	const std::string&motion_name,
	AnimationData&data,
	std::vector<D3DXMATRIX>&bone_mat_list
) {

	// モーション作成
	MotionData motion;

	// アニメーションサイズ
	for (UINT a = 0; a < data.mat.size(); a++) {

		// 配列
		motion.animation_matrix.emplace_back();
		motion.d3d_animation_mat.emplace_back();

		// ボーンサイズ
		for (UINT b = 0; b < data.mat[a].size(); b++) {


			D3DXMATRIX anim_mat;

			// 変換して本体の行列に変換する
			FbxMatConvertD3DMat(&anim_mat, data.mat[a][b]);

			// ボーン行列変換(アニメーション行列 * 
			// オフセット変換済みボーン行列)
			D3DXMATRIX total_mat = 
				anim_mat * 
				bone_mat_list[b];

			// d3d用に代入
			motion.d3d_animation_mat.back().
				anim_mat_list.emplace_back(total_mat);

		}

	}

	return motion;

}


void FbxMotion::LoadAnimation(
	FbxScene*p_scene,
	const std::string& motion_name,
	std::vector<FbxMatrix>& bone_mat_list,
	const int&mesh_num,
	const int &skin_num,
	const int& bone_num,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count
) {

	// モーション作成
	MotionData motion;

	// アニメーションデータ
	AnimationData data;

	// スタートフレーム代入
	FbxTime time_count = start;

	int time = 0;

	// モーション数がストップするまで
	while (time_count < stop) {

		// フレーム用配列作成
		motion.animation_matrix.emplace_back();
		motion.d3d_animation_mat.emplace_back();

		// デフォーマ数分回す
		for (int s = 0; s < skin_num; s++) {

			// 総ボーン数分回す
			for (int b = 0;
				b < bone_num;
				b++) {

				// 合成行列を計算
				// 時間に合ったアニメーション行列取得
				FbxMatrix anim_mat = GetAnimationMatrix(
					*p_scene,
					time_count,
					mesh_num,
					s,
					b
				);

				// ボーン行列変換(アニメーション行列 * 
				// オフセット変換済みボーン行列)
				FbxMatrix total_mat =
					anim_mat * 
					bone_mat_list[b];

				D3DXMATRIX conv_mat;

				// アニメーション行列追加
				motion.animation_matrix.back().
					emplace_back(total_mat);

				// 変換して本体の行列に変換する
				FbxMatConvertD3DMat(&conv_mat, total_mat);

				// d3d用に代入
				motion.d3d_animation_mat.back().
					anim_mat_list.emplace_back(conv_mat);

				// 最大ボーン数を過ぎたら入れない
				if (b > MAX_BONE_MATRIX_NUM - 1) {
					break;
				}

				//// d3d用に追加
				//motion.d3d_animation_mat.back().bone_list[b] = 
				//	conv_mat;
			}

			// 時間を追加
			time_count += frame_count;
		}
	}

	m_motion[motion_name].emplace_back(motion);

	// モーションデータを返す

	// カウントリセット
	time_count = start;
}


AnimationData FbxMotion::LoadAnimation(
	FbxScene*p_scene,
	const int&mesh_num,
	const int &skin_num,
	const int& bone_num,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count
) {

	// アニメーションデータ
	AnimationData data;

	FbxTime time_count = start;

	// モーション数がストップするまで
	while (time_count < stop) {

		// フレーム用配列作成
		data.mat.emplace_back();

		// デフォーマ数分回す
		for (int s = 0; s < skin_num; s++) {

			// 総ボーン数分回す
			for (int b = 0;
				b < bone_num;
				b++) {

				// 合成行列を計算
				// アニメーション行列取得
				data.mat.back().emplace_back(
					GetAnimationMatrix(
					*p_scene,
					time_count,
					mesh_num,
					s,
					b
				)
				);
			}

			// 時間を追加
			time_count += frame_count;
		}
	}

	// モーションデータを返す
	return data;
}


FbxMatrix FbxMotion::GetAnimationMatrix(
	const FbxScene&fbx_scene,
	const FbxTime& count,
	const int&mesh_num,
	const int &deformer_num,
	const int &cluster_num
) {

	// 現在のメッシュを取得
	FbxMesh * p_mesh =
		fbx_scene.
		GetSrcObject<FbxMesh>(mesh_num);

	// ノード取得開始
	FbxNode* pNode = fbx_scene.GetRootNode();

	// <移動、回転、拡大のための行列を作成>
	FbxMatrix global_position = pNode->
		EvaluateGlobalTransform(count);

	// スキン取得
	FbxSkin* skin_deformer = (FbxSkin*)
		p_mesh->GetDeformer(deformer_num, FbxDeformer::eSkin);

	// スキンが受け取れていない場合
	if (skin_deformer == nullptr){
		FbxMatrix mat;
		return mat;
	}

	// <クラスタ(ボーン)の取り出し>
	FbxCluster* cluster = skin_deformer->
		GetCluster(cluster_num);

	// スケルトン名確認
	std::string skeleton_name = 
		cluster->GetLink()->GetName();

	// スケルトンアニメーションの取得
	// (アニメーション時間を指定して、アニメーション行列を取得する)
	// GetLinkの先は対象のスケルトンノード
	FbxMatrix cluster_global_current_position = 
		cluster->GetLink()->
		EvaluateGlobalTransform(count);

	// ルートの逆行列(初期地点) * 時間姿勢行列
	FbxMatrix cluster_relative_current_position_inverse = 
		global_position.Inverse() *
		cluster_global_current_position;

	return cluster_relative_current_position_inverse;

}


FbxAMatrix FbxMotion::GetGeometry(FbxNode* p_mesh_node)
{
	FbxVector4 t0 = p_mesh_node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = p_mesh_node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = p_mesh_node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(t0, r0, s0);
}


void FbxMotion::LoadWeight(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	// メッシュデータ
	FbxMeshData* data = &p_mesh_data_list.back();

	// 全ての頂点サイズ(数)を出力
	int size = (UINT)(data->vertex_num * sizeof(SkinCustomVertex));

	// 設定用のバーテックス
	SkinCustomVertex* p_vertics;

	// バッファをロックしてデータを書き込む
	data->p_vertex_buffer->Lock(
		0,
		size,
		(void**)&p_vertics,
		0
	);

	// デバッグ用
	std::vector<std::vector<int>>debug_index_list;
	std::vector<std::vector<float>>debug_weight_list;

	debug_index_list.resize(data->vertex_num);
	debug_weight_list.resize(data->vertex_num);

	// メッシュだけならば
	if (defor_count == 0) {

		int vertex_num =
		p_mesh->GetControlPointsCount();

		// 頂点全てに最大の重みを代入
		for (int i = 0;
			i < vertex_num;
			i++) {

			// アニメーションの最初の行列しか使わない
			p_vertics[i].bone_index[0] = 0;

			// 1.fの重み付け
			p_vertics[i].weight[0] =
				(float)1.f;
		}
	}

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		FbxSkin* skinDeformer = (FbxSkin*)
			p_mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skinDeformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = skinDeformer->
			GetClusterCount();

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skinDeformer->
				GetCluster(ci);

			for (int i = 0; i <
				cluster->GetControlPointIndicesCount();
				i++) {

				double weight = cluster->
					GetControlPointWeights()[i];

				int index = cluster->
					GetControlPointIndices()[i];

				// 重みを回す
				for (int w = 0; w < 4; w++) {

					const int CLEAR = 0;

					// 入っているなら
					if (p_vertics[index].weight[w] > 0.f)// 前はindexで調べていた
					{
						// 空きがないので次に行く
						continue;
					}

					debug_index_list[index].push_back(ci);

					// 現在のボーン数を代入
					p_vertics[index].bone_index[w] = ci;

					debug_weight_list[index].
						push_back((float)weight);

					// 重み代入
					p_vertics[index].weight[w] =
						(float)weight;

					// 代入できたので終了
					break;
				}
			}
		}
	}

	data->p_vertex_buffer->Unlock();
}


bool FbxMotion::LoadBone(
	FbxScene*p_scene,
	FbxImporter*p_importer,
	const std::string&mesh_name,
	FbxMesh* p_mesh,
	const int&mesh_num,
	std::vector<std::string>&out_skeleton_name_list
) {

	if (p_mesh == nullptr) {
		return false;
	}

	p_mesh->GetNode()->EvaluateLocalTranslation();

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// メッシュのノード名取得
	std::string node_name =
	mesh_node->GetName();

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	ElementData e_data;

	// 最初にメッシュを代入
	e_data.type = ElementType::MESH;
	e_data.mesh_node_name = node_name;

	/* スキンがないなら終了 */
	if (defor_count <= 0) {

		// メッシュだけを追加
		m_elem_type_list[mesh_name].push_back(e_data);

		return false;
	}

	// メッシュでのgeo受け取り
	FbxAMatrix geometry_offset = GetGeometry(
		p_scene->GetRootNode()
	);

	// ボーンデータ
	BoneData b_data;

	// スキン数を保存
	b_data.skin_num = defor_count;

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		// スキン
		FbxSkin* skin_deformer = (FbxSkin*)
			p_mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skin_deformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = 
			skin_deformer->GetClusterCount();

		// ボーン数代入
		b_data.bone_num = cluster_count;

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skin_deformer->
				GetCluster(ci);

			// スケルトン名を外部に出す
			out_skeleton_name_list.emplace_back(
				cluster->GetLink()->GetName()
			);

			// 行列
			FbxAMatrix reference_global_init_position;
			FbxAMatrix cluster_global_init_position;
			FbxMatrix cluster_relative_init_position;

			// 頂点を変換
			cluster->GetTransformMatrix(reference_global_init_position);

			// リファレンスグローバル初期化位置
			reference_global_init_position *= geometry_offset;

			// 初期姿勢行列取得
			cluster->GetTransformLinkMatrix(cluster_global_init_position);

			// クラスター位置
			cluster_relative_init_position = 
				cluster_global_init_position.Inverse() *
				reference_global_init_position;

			// ボーン
			Bone bone;

			// クラスターのリンクの名前を取ってくる
			bone.skeleton_name = cluster->GetLink()->GetName();

			// 変換
			FbxMatConvertD3DMat(&bone.mat, 
				cluster_relative_init_position);

			// ボーンデータに代入
			b_data.bone_list.emplace_back(bone);

			// fbx行列ボーンデータに代入
			b_data.fbx_mat_list.emplace_back(
				cluster_relative_init_position);

		}
	}


	// ボーン付きメッシュ
	e_data.type = ElementType::BONE_MESH;

	// 追加
	m_elem_type_list[mesh_name].push_back(e_data);

	// ボーンリストに追加
	m_bone_mat_list[mesh_name].emplace_back(b_data);

	return true;
}



void FbxMotion::FbxMatConvertD3DMat(
	D3DXMATRIX*p_out_mat,
	FbxMatrix &fbx_mat
) {

	if (p_out_mat == nullptr) {
		return;
	}

	// doubleに変換
	FbxDouble * mat = (FbxDouble*)fbx_mat;

	// オフセット行列保存
	for (int matrix = 0; matrix < 16; matrix++) {

		p_out_mat->m[matrix / 4][matrix % 4] = 
			(float)mat[matrix];
	}
}



void FbxMotion::FbxMatLConvert(
	FbxMatrix& out_fbx_mat
) {

	// Z軸を反転させる
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}


void FbxMotion::FbxMatLConvert(
	FbxAMatrix& out_fbx_mat
) {
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}


void FbxMotion::GetAnimInfo(
	FbxImporter*importer,
	int index
) {

	int anim_stack_c = GetMotionNum(importer);

	FbxTakeInfo*p_info = 
		importer->GetTakeInfo(index % anim_stack_c);

	FbxLongLong start = p_info->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = p_info->mLocalTimeSpan.GetStop().Get();

	FbxLongLong oneFrameValue = 
		FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	// 合計フレーム数
	FbxLongLong frame =
		stop - start / oneFrameValue;
	// EMode pTimeMode = eDefaultMode
}



/* ここから下はお試し中なので今は使えない */

//
//void FbxMotion::KeySkinning(
//	std::vector<FbxMeshData>m_mesh_data_list,
//	const std::vector<SkinCustomVertex*>&p_vertics,
//	FbxScene*p_scene
//) {
//
//
//	// 頂点変形
//	for (
//		UINT mi = 0;
//		mi < m_mesh_data_list.size();
//		mi++) {
//
//		// メッシュデータ受け取り
//		FbxMeshData mesh_data = m_mesh_data_list[mi];
//
//		// メッシュを取得
//		FbxMesh* mesh =
//			p_scene->GetSrcObject<FbxMesh>(mi);
//
//		// 頂点バッファを定義
//		SkinCustomVertex* vertices;
//
//		// 頂点バッファのサイズ作成
//		UINT size =
//			(UINT)((mesh->GetControlPointsCount()) *
//				sizeof(SkinCustomVertex));
//
//		// バッファをロックして書き込み可能にする
//		m_mesh_data_list[mi].p_vertex_buffer->Lock(
//			0,
//			size,
//			(void**)&vertices,
//			0
//		);
//
//		// 最新アニメーション
//		KeyFrameSkinning(
//			mesh,
//			mesh_data,
//			vertices,
//			p_vertics,
//			mi
//		);
//
//		m_mesh_data_list[mi].p_vertex_buffer->Unlock();
//	}
//}
//
//

//
//static void MatrixInterporate2(
//	D3DXMATRIX& out,
//	D3DXMATRIX& A,
//	D3DXMATRIX B,
//	float rate
//) {
//
//	// 最初から最後までの時間の長さ
//	float f = 1.f - 0.f;
//
//	// 最初から最後までの、変化量
//	D3DXMATRIX m = B;
//
//	// 時間の長さで割る
//	float f2 = (rate - 0.f) / f;
//
//	out = m * f2;
//
//	m._31 *= 0.1f;
//	m._32 *= 0.1f;
//	m._33 *= 0.1f;
//
//
//	out = m;
//}
//
//
//
//void FbxMotion::KeyFrameSkinning(
//	FbxMesh* mesh,
//	FbxMeshData& mesh_data,
//	SkinCustomVertex* vertices,
//	const std::vector<SkinCustomVertex*>&p_vertics,
//	const int &mi
//) {
//
//	// ボーン数受け取り
//	int cluster_count = mesh_data.bone_num;
//
//	MotionData *md =
//		&m_motion[m_current_motion_name][mi];
//
//	// 重み分回す
//	for (UINT v = 0;
//		v < mesh_data.vertex_num;
//		v++) {
//
//		D3DXVECTOR4 vec4 =
//			p_vertics[mi][v].vertex;
//
//		// 一旦初期化
//		vertices[v].vertex.x = 0.f;
//		vertices[v].vertex.y = 0.f;
//		vertices[v].vertex.z = 0.f;
//
//		for (int b = 0; b < 4; b++) {
//
//			// 中身がないなら
//			if (vertices[v].weight[b] <= 0.f) {
//				// そのあとの配列にもないと思うので抜ける
//				break;
//			}
//
//			int w_idx = vertices[v].bone_index[b];
//			float weight = vertices[v].weight[b];
//
//			D3DXMATRIX out;
//
//			float f = m_key_frame - (int)m_key_frame;
//
//			// 行列補間
//			MatrixInterporate(
//				out,
//				md->d3d_animation_mat.front().anim_mat_list[w_idx],
//				md->d3d_animation_mat.back().anim_mat_list[w_idx],
//				f
//			);
//
//			// 初期位置受け取り
//			FbxVector4 pos = mesh->GetControlPointAt(
//				v
//			);
//
//			// d3d用
//			D3DXVECTOR4 out_vec;
//
//			// 変換
//			Utility::Math::MatrixMultipleVec4(
//				out_vec,
//				vec4,
//				out
//			);
//
//			// 頂点取り出し
//			float x = (float)out_vec.x;
//			float y = (float)out_vec.y;
//			float z = (float)out_vec.z;
//
//			// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
//			vertices[v].vertex.x += x * weight;
//			vertices[v].vertex.y += y * weight;
//			vertices[v].vertex.z += z * weight;
//		}
//	}
//}
//



/*
void FbxMotion::KeySkinning() {


	// 頂点変形
	for (
		UINT mi = 0;
		mi < m_mesh_data_list.size();
		mi++) {

		// メッシュデータ受け取り
		FbxMeshData mesh_data = m_mesh_data_list[mi];

		// メッシュを取得
		FbxMesh* mesh =
			mod.mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

		// 頂点バッファを定義
		SkinCustomVertex* vertices;

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)((mesh->GetControlPointsCount()) *
				sizeof(SkinCustomVertex));

		// バッファをロックして書き込み可能にする
		m_mesh_data_list[mi].p_vertex_buffer->Lock(
			0,
			size,
			(void**)&vertices,
			0
		);

		// 最新アニメーション
		KeyFrameSkinning(
			mesh,
			mesh_data,
			vertices,
			mi
		);

		m_mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}


//	ボーン行列の補間
static void MatrixInterporate(
	D3DXMATRIX& out,
	D3DXMATRIX& A,
	D3DXMATRIX B,
	float rate
)
{
	D3DXMATRIX m_a = A * (1.0f - rate);
	D3DXMATRIX m_b = B * rate;

	out = m_a + m_b;
}

static void MatrixInterporate2(
	D3DXMATRIX& out,
	D3DXMATRIX& A,
	D3DXMATRIX B,
	float rate
) {

	// 最初から最後までの時間の長さ
	float f = 1.f - 0.f;

	// 最初から最後までの、変化量
	D3DXMATRIX m = B;

	// 時間の長さで割る
	float f2 = (rate - 0.f) / f;

	out = m * f2;

	m._31 *= 0.1f;
	m._32 *= 0.1f;
	m._33 *= 0.1f;


	out = m;
}


void FbxMotion::KeyFrameSkinning(
	FbxMesh* mesh,
	FbxMeshData& mesh_data,
	SkinCustomVertex* vertices,
	const int &mi
) {

	// ボーン数受け取り
	int cluster_count = mesh_data.bone_num;

	MotionData *md =
		&m_motion[m_current_motion_name][mi];

	// 重み分回す
	for (UINT v = 0;
		v < mesh_data.vertex_num;
		v++) {

		D3DXVECTOR4 vec4 =
			mp_vertics[mi][v].vertex;

		// 一旦初期化
		vertices[v].vertex.x = 0.f;
		vertices[v].vertex.y = 0.f;
		vertices[v].vertex.z = 0.f;

		for (int b = 0; b < 4; b++) {

			// 中身がないなら
			if (vertices[v].weight[b] <= 0.f) {
				// そのあとの配列にもないと思うので抜ける
				break;
			}

			int w_idx = vertices[v].bone_index[b];
			float weight = vertices[v].weight[b];

			D3DXMATRIX out;

			float f = m_key_frame - (int)m_key_frame;

			// 行列補間
			MatrixInterporate(
				out,
				md->d3d_animation_mat.front().bone_list[w_idx],
				md->d3d_animation_mat.back().bone_list[w_idx],
				f
			);

			// 初期位置受け取り
			FbxVector4 pos = mesh->GetControlPointAt(
				v
			);

			// d3d用
			D3DXVECTOR4 out_vec;

			Model::MatrixMultipleVec4(
				out_vec,
				vec4,
				out
			);

			// 頂点取り出し
			float x = (float)out_vec.x;
			float y = (float)out_vec.y;
			float z = (float)out_vec.z;

			// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
			vertices[v].vertex.x += x * weight;
			vertices[v].vertex.y += y * weight;
			vertices[v].vertex.z += z * weight;
		}
	}
}
*/

