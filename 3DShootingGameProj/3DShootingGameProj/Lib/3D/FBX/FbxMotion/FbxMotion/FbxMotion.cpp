#include"FbxMotion.h"
#include"../../../../Utility/Utility.h"




FbxMotion::FbxMotion(
	FbxModuleModel*fbx_module
) {

	mp_module = fbx_module;
}



void FbxMotion::SetMotion(std::string name)
{
	m_current_motion_name = name;
	m_frame = 0.0f;

	// アニメーション最大数を入れる
	m_stop_count =
		m_motion[name].back().d3d_animation_mat.size();
}


void FbxMotion::Animate(const float &frame)
{

	if (m_frame > frame) {
		m_count++;
		m_frame = 0;
	}

	if (m_count > m_stop_count - 1) {
		m_count = 0;
	}

	// フレーム値加算
	m_frame++;

}


bool FbxMotion::SelectAnimation(
	int select_anim_num,
	FbxModuleModel&mod
) {
	
	int anim_num = mod.mp_importer->
		GetAnimStackCount();

	// 総アニメーション数よりも大きいなら
	if (anim_num < select_anim_num) {
		return false;
	}

	// 受け取るアニメーションを切り替え
	FbxAnimStack* pStack = mod.mp_fbx_scene->
		GetSrcObject<FbxAnimStack>(select_anim_num);

	mod.mp_fbx_scene->SetCurrentAnimationStack(pStack);

	return true;
}


void FbxMotion::SetAnimation(
	FbxModuleModel&mod,
	FbxTime&start,
	FbxTime&stop,
	FbxTime&frame_time,
	const int&anim_num
) {

	// アニメーションファイル名を受け取る
	FbxArray<FbxString*> AnimStackNameArray;
	mod.mp_fbx_scene->
		FillAnimStackNameArray(AnimStackNameArray);

	// アニメーション情報が入ったスタックを受け取る
	FbxAnimStack* AnimationStack =
		mod.mp_fbx_scene->
		FindMember<FbxAnimStack>(AnimStackNameArray[anim_num]->Buffer());

	// アニメーションをセット
	mod.mp_fbx_scene->
		SetCurrentAnimationStack(AnimationStack);

	// アニメーション時間をセット
	FbxTakeInfo* take_info = mod.mp_fbx_scene->
		GetTakeInfo(*(AnimStackNameArray[anim_num]));

	start = take_info->mLocalTimeSpan.GetStart();
	stop = take_info->mLocalTimeSpan.GetStop();

	frame_time.SetTime(0, 0, 0, 1, 0,
		mod.mp_fbx_scene->GetGlobalSettings().GetTimeMode());

}



void FbxMotion::LoadMotion(
	const std::string& name,
	const int &select_motion_num,
	std::vector<FbxMeshData>&m_mesh_data_list,
	FbxModuleModel&mod
) {

	// アニメーション選択
	if (SelectAnimation(select_motion_num,mod) == false) {
		return;
	}

	// タイマー
	FbxTime frame_time, start, stop;

	// アニメーション情報受け取り
	SetAnimation(
		mod,
		start,
		stop,
		frame_time,
		select_motion_num
	);

	for (UINT mi = 0;
		mi < m_mesh_data_list.size();
		mi++) {

		// メッシュを取得
		FbxMesh * p_mesh =
			mod.mp_fbx_scene->
			GetSrcObject<FbxMesh>(mi);

		for (int s = 0; s < p_mesh->GetDeformerCount(); s++) {


			// スキン受け取り
			FbxSkin *skin = (FbxSkin*)p_mesh->GetDeformer(s);

			// アニメーション読み込み
			LoadKeyFrame(
				p_mesh,
				name,
				m_mesh_data_list[mi].bone_list,
				skin->GetClusterCount(),
				start,
				stop,
				frame_time
			);
		}
	}
}


void FbxMotion::LoadKeyFrame(
	FbxMesh* mesh,
	const std::string& motion_name,
	std::vector<FbxMatrix>& bone_mat_list,
	const int& bone_num,
	const FbxTime&start,
	const FbxTime&stop,
	const FbxTime&frame_count
) {

	// スキン数
	int skin_count = mesh->GetDeformerCount();

	// モーション作成
	MotionData motion;

	FbxTime time_count = start;

	int time = 0;

	// モーション数がストップするまで
	while (time_count < stop) {

		// フレーム用配列作成
		motion.animation_matrix.emplace_back();
		motion.d3d_animation_mat.emplace_back();

		// デフォーマ数分回す
		for (int s = 0; s < skin_count; s++) {

			// 総ボーン数分回す
			for (int b = 0;
				b < bone_num;
				b++) {

				// 合成行列を計算
				// アニメーション行列取得
				FbxMatrix anim_mat = GetAnimationMatrix(
					*mp_module,
					mesh,
					time_count,
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
				motion.animation_matrix.
					back().
					emplace_back(total_mat);

				// 変換して本体の行列に変換する
				FbxMatConvertD3DMat(&conv_mat, total_mat);

				// d3d用に代入
				motion.d3d_animation_mat.
					back().bone_list[b] = conv_mat;

				// 最大ボーン数を過ぎたら入れない
				if (b > MAX_BONE_MATRIX_NUM - 1) {
					break;
				}

				// d3d用に追加
				motion.d3d_animation_mat.back().bone_list[b] =
					conv_mat;
			}

			// 時間を追加
			time_count += frame_count;
		}
	}

	m_motion[motion_name].emplace_back(motion);

	// カウントリセット
	time_count = start;
}



void FbxMotion::FbxPolygon3Convert(
const FbxModuleModel& mod
) {

	fbxsdk::FbxGeometryConverter converter(mod.mp_manager);
	// マテリアルと1メッシュ毎に分ける
	converter.SplitMeshesPerMaterial(mod.mp_fbx_scene, true);
	// 全てを三角形にする
	converter.Triangulate(mod.mp_fbx_scene, true);

	// 追加
	converter.RemoveBadPolygonsFromMeshes(
		mod.mp_fbx_scene
	);

}


FbxMatrix FbxMotion::GetAnimationMatrix(
	const FbxModuleModel&mod,
	FbxMesh* mesh,
	FbxTime& count,
	int deformer_num,
	int cluster_num
) {


	// 親からノードを取ってくる
	FbxNode* pNode = mod.mp_fbx_scene->GetRootNode();

	// <移動、回転、拡大のための行列を作成>
	FbxMatrix globalPosition = pNode->
		EvaluateGlobalTransform(count);

	// スキン取得
	FbxSkin* skin_deformer = (FbxSkin*)
		mesh->GetDeformer(deformer_num, FbxDeformer::eSkin);

	// スキンが受け取れていない場合
	if (skin_deformer == nullptr) {
		FbxMatrix mat;
		return mat;
	}

	// <クラスタ(ボーン)の取り出し>
	FbxCluster* cluster = skin_deformer->
		GetCluster(cluster_num);

	FbxMatrix clusterGlobalCurrentPosition =
		cluster->GetLink()->
		EvaluateGlobalTransform(count);

	FbxMatrix clusterRelativeCurrentPositionInverse =
		globalPosition.Inverse() *
		clusterGlobalCurrentPosition;

	return clusterRelativeCurrentPositionInverse;

}



FbxAMatrix FbxMotion::GetGeometry(FbxNode* p_mesh_node)
{
	FbxVector4 t0 = p_mesh_node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = p_mesh_node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = p_mesh_node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(t0, r0, s0);
}



void FbxMotion::LoadFileMotion(
	std::string name,
	const char* pFilename
)
{
	FbxManager* pManager = FbxManager::Create();
	FbxScene* pScene = FbxScene::Create(pManager, "");

	//	ファイルからシーンに読み込み
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	pImporter->Initialize(pFilename);
	pImporter->Import(pScene);
	pImporter->Destroy();

	//	モーション情報取得
	FbxArray<FbxString*> names;
	pScene->FillAnimStackNameArray(names);

	FbxTakeInfo* take = pScene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	//m_start_frame = (int)(start / fps60);
	//m_motion[name].frame_num = (int)((stop - start) / fps60);
	//	ルートノード取得
	FbxNode* root = pScene->GetRootNode();

	//	全ボーン読み込み
	//for (int b = 0; b < (int)m_bone_num; b++)
	//{
	//	//	ボーンノード検索
	//	FbxNode* pBone = root->FindChild(m_bone_data_list[b].name);
	//	if (pBone == NULL) continue;
	//
	//	//	キーフレーム読み込み
	//	LoadKeyFrame(name, b, pBone);
	//}
	//	解放
	pScene->Destroy();
	pManager->Destroy();
}



void FbxMotion::LoadWeightVertexPoint(
	std::vector<FbxMeshData>& p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

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


void FbxMotion::CPUSkinning(
	std::vector<FbxMeshData>& mesh_data_list,
	const std::vector<SkinCustomVertex*>&p_vertics
) {

	if (mp_module == nullptr) {
		return;
	}

	if (p_vertics.size() == 0) {
		return;
	}

	if (mesh_data_list.size() == 0) {
		return;
	}

	// 頂点変形
	for (UINT mi = 0;
		mi < mesh_data_list.size();
		mi++) {

		// メッシュデータ受け取り
		FbxMeshData mesh_data = mesh_data_list[mi];

		// メッシュを取得
		FbxMesh* mesh =
			mp_module->mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

		// 頂点バッファを定義
		SkinCustomVertex* vertices;

		// 頂点バッファのサイズ作成
		UINT size =
			(UINT)((mesh->GetControlPointsCount()) *
				sizeof(SkinCustomVertex));

		// バッファをロックして書き込み可能にする
		mesh_data_list[mi].p_vertex_buffer->Lock(
			0,
			size,
			(void**)&vertices,
			0
		);

		// 最新アニメーション
		WeightSkinning(
			mesh,
			mesh_data,
			vertices,
			p_vertics,
			mi
		);

		mesh_data_list[mi].p_vertex_buffer->Unlock();
	}
}


void FbxMotion::WeightSkinning(
	FbxMesh* mesh,
	FbxMeshData& mesh_data,
	SkinCustomVertex* vertices,
	const std::vector<SkinCustomVertex*>&mp_vertics,
	const int &mi
) {

	// ボーン数受け取り
	int cluster_count = mesh_data.bone_num;

	// モーションデータ
	MotionData *md = &m_motion[m_current_motion_name][mi];

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

			// 初期位置受け取り
			FbxVector4 pos = mesh->GetControlPointAt(
				v
			);

			// 頂点受け取り
			FbxVector4 out_vertex = md->
				animation_matrix[m_count][w_idx].
				MultNormalize(pos);

			// 頂点取り出し
			float x = (float)out_vertex[0];
			float y = (float)out_vertex[1];
			float z = (float)out_vertex[2];

			//// d3d用
			//D3DXVECTOR4 out;
			//
			//Model::MatrixMultipleVec4(
			//	out,
			//	vec4,
			//	md->d3d_animation_mat[m_count].bone_list[w_idx]);


			//// 頂点取り出し
			//float x = (float)out.x;
			//float y = (float)out.y;
			//float z = (float)out.z;


			// 重みを掛け合わせ(影響しない場合は0なので大丈夫)
			vertices[v].vertex.x += x * weight;
			vertices[v].vertex.y += y * weight;
			vertices[v].vertex.z += z * weight;
		}
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


void FbxMotion::LoadBone(
	const FbxModuleModel&mod,
	std::vector<FbxMeshData>*p_mesh_data_list,
	FbxMesh* p_mesh
) {

	// ボーン姿勢行列の計算と受け取り
	FbxNode* mesh_node = p_mesh->GetNode();

	// メッシュでのgeo受け取り
	FbxAMatrix geometryOffset = GetGeometry(mod.mp_fbx_scene->GetRootNode());

	// スキンの数を取得
	int defor_count = p_mesh->GetDeformerCount(FbxDeformer::eSkin);

	// fbxデータ
	FbxMeshData * data = &p_mesh_data_list->back();

	int total_weight = 0;

	// スキン分回す
	for (int skin_num = 0; skin_num < defor_count; skin_num++) {

		FbxSkin* skin_deformer = (FbxSkin*)
			p_mesh->GetDeformer(skin_num, FbxDeformer::eSkin);

		// スキンが受け取れていない場合
		if (skin_deformer == nullptr) {
			continue;
		}

		// クラスター数
		int cluster_count = skin_deformer->
			GetClusterCount();

		// ボーン数代入
		p_mesh_data_list->back().bone_num = cluster_count;

		// <各クラスタから各頂点に影響を与えるための行列作成>
		for (int ci = 0;
			ci < cluster_count;
			ci++) {

			// <クラスタ(ボーン)の取り出し>
			FbxCluster* cluster = skin_deformer->
				GetCluster(ci);

			// 行列
			FbxAMatrix referenceGlobalInitPosition;
			FbxAMatrix clusterGlobalInitPosition;
			FbxMatrix clusterRelativeInitPosition;

			// 頂点を変換
			cluster->GetTransformMatrix(referenceGlobalInitPosition);

			// リファレンスグローバル初期化位置
			referenceGlobalInitPosition *= geometryOffset;

			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

			// クラスター位置
			clusterRelativeInitPosition =
				clusterGlobalInitPosition.Inverse() *
				referenceGlobalInitPosition;

			// ボーンリストに追加
			p_mesh_data_list->back().bone_list.
				emplace_back(clusterRelativeInitPosition);

			D3DXMATRIX out_mat;

			// 変換
			FbxMatConvertD3DMat(&out_mat, clusterRelativeInitPosition);

			// d3d用に追加
			p_mesh_data_list->back().
				d3d_bone_mat_list.
				emplace_back(out_mat);

			// クラスタの追加
			data->weight_index_list.emplace_back();
			data->weight_list.emplace_back();

			for (int i = 0; i <
				cluster->GetControlPointIndicesCount();
				i++) {

				double weight = cluster->
					GetControlPointWeights()[i];

				// 重み
				data->
					weight_list.back().push_back(weight);

				int index = cluster->
					GetControlPointIndices()[i];

				// 重みインデックス
				data->
					weight_index_list.back().push_back(index);

				total_weight++;

			}
		}
	}

	total_weight = total_weight;
}



/* ここから下はお試し中なので今は使えない */


void FbxMotion::KeySkinning(
	std::vector<FbxMeshData>m_mesh_data_list,
	const std::vector<SkinCustomVertex*>&p_vertics
) {


	// 頂点変形
	for (
		UINT mi = 0;
		mi < m_mesh_data_list.size();
		mi++) {

		// メッシュデータ受け取り
		FbxMeshData mesh_data = m_mesh_data_list[mi];

		// メッシュを取得
		FbxMesh* mesh =
			mp_module->mp_fbx_scene->GetSrcObject<FbxMesh>(mi);

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
			p_vertics,
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
	const std::vector<SkinCustomVertex*>&p_vertics,
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
			p_vertics[mi][v].vertex;

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

			// 変換
			Utility::Math::MatrixMultipleVec4(
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


