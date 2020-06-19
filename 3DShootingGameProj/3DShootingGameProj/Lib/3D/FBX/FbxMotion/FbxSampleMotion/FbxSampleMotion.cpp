#include"FbxSampleMotion.h"





//	ボーン行列の補間
static void MatrixInterporate(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}


void FbxSampleMotion::Skinning(
const int &total_mesh,
std::vector<FbxMeshData>&mesh_data_list,
std::vector<SkinCustomVertex*>&skin_vertex
){

	// 空
	if (m_motion_list.empty()) {
		return;
	}

	// モーション
	Motion* pMotion = &m_motion_list[m_play_motion];


	if (pMotion == NULL) {
		return;
	}

	// フレーム
	if (pMotion->numFrame < 0) {
		return;
	}

	// 現在のフレーム
	float Frame = (float)m_frame;

	//	配列用変数
	int f = (int)Frame;

	//	行列準備
	D3DXMATRIX KeyMatrix[256];

	for (INT b = 0; b < m_total_bone; b++)
	{
		if (pMotion->Key[b].empty()) continue;
		//	行列補間
		D3DXMATRIX m;
		MatrixInterporate(m,
			pMotion->Key[b][f], pMotion->Key[b][f + 1],
			Frame - (int)Frame);
		m_bone_list[b].transform = m;
		//	キーフレーム
		KeyMatrix[b] = m_bone_list[b].offset * m;
	}

	// 頂点変形
	for (INT meshIdx = 0; meshIdx < total_mesh; meshIdx++)
	{
		FbxMeshData pMeshData = mesh_data_list[meshIdx];
		SkinCustomVertex* pSrcVertices = skin_vertex[meshIdx];

		// サイズ
		UINT size = (UINT)(pMeshData.vertex_num *
			sizeof(SkinCustomVertex));

		//MeshVertex* pVertices;
		SkinCustomVertex * pVertices;
		// バッファをロックしてデータを書き込む
		pMeshData.p_vertex_buffer->Lock(
			0, size, (void**)&pVertices, 0);
		for (UINT v = 0; v < pMeshData.vertex_num; v++)
		{
			//	頂点 * ボーン行列
			// b = v番目の頂点の影響ボーン[n]
			if (pVertices[v].weight[0] <= 0) continue;

			pVertices[v].vertex.x = 0;
			pVertices[v].vertex.y = 0;
			pVertices[v].vertex.z = 0;

			//	影響個数分ループ
			for (int n = 0; n < 4; n++)
			{
				if (pVertices[v].weight[n] <= 0) break;

				int b = (int)pVertices[v].bone_index[n];

				float x = pSrcVertices[v].vertex.x;
				float y = pSrcVertices[v].vertex.y;
				float z = pSrcVertices[v].vertex.z;
				//	座標を影響力分移動
				pVertices[v].vertex.x
					+=
					(x*KeyMatrix[b]._11
						+ y * KeyMatrix[b]._21
						+ z * KeyMatrix[b]._31
						+ 1 * KeyMatrix[b]._41)
					*pVertices[v].weight[n];
				pVertices[v].vertex.y +=
					(x*KeyMatrix[b]._12
						+ y * KeyMatrix[b]._22
						+ z * KeyMatrix[b]._32
						+ 1 * KeyMatrix[b]._42)
					*pVertices[v].weight[n];
				pVertices[v].vertex.z += (x*KeyMatrix[b]._13
					+ y * KeyMatrix[b]._23
					+ z * KeyMatrix[b]._33
					+ 1 * KeyMatrix[b]._43)
					*pVertices[v].weight[n];

				float nx = pSrcVertices[v].normal.x;
				float ny = pSrcVertices[v].normal.y;
				float nz = pSrcVertices[v].normal.z;
				//	法線を影響力分変換
				pVertices[v].normal.x += (nx*KeyMatrix[b]._11 + ny * KeyMatrix[b]._21 + nz * KeyMatrix[b]._31)*pVertices[v].weight[n];
				pVertices[v].normal.y += (nx*KeyMatrix[b]._12 + ny * KeyMatrix[b]._22 + nz * KeyMatrix[b]._32)*pVertices[v].weight[n];
				pVertices[v].normal.z += (nx*KeyMatrix[b]._13 + ny * KeyMatrix[b]._23 + nz * KeyMatrix[b]._33)*pVertices[v].weight[n];
			}
		}
		pMeshData.p_vertex_buffer->Unlock();
	}
}



void FbxSampleMotion::Play(std::string name)
{
	m_play_motion = name;
	m_frame = 0;
}


void FbxSampleMotion::Animate(float sec)
{
	//	モーション時間の更新
	m_frame += (int)(sec * 60.0f);

	//	ループチェック
	if (m_frame >= (int)(m_motion_list[m_play_motion].numFrame - 1))
	{
		// ループ
		m_frame = 0;
	}
}


int FbxSampleMotion::FindBone(const char* pName)
{
	for (INT boneIdx = 0; boneIdx < m_total_bone; boneIdx++)
	{
		if (strcmp(m_bone_list[boneIdx].name, pName) == 0)
		{
			return (int)boneIdx;
		}
	}
	return -1;
}


void FbxSampleMotion::LoadBone(
	UINT matIdx,
	FbxMesh* pMesh, 
	std::vector<FbxMeshData>& p_mesh_data_list
)
{
	FbxManager* pManager = FbxManager::Create();
	FbxScene* pScene = FbxScene::Create(pManager, "");

	FbxImporter* pImpoter = FbxImporter::Create(pManager, "");
	pImpoter->Initialize("Resource/3DModel/taiki/taiki.fbx");
	pImpoter->Import(pScene);

	FbxArray<FbxString*> names;
	pScene->FillAnimStackNameArray(names);


	int StartFrame = 0;
	if (names != NULL) {
		//	モーションが存在するとき
		FbxTakeInfo* take = pScene->GetTakeInfo(names[0]->Buffer());
		FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
		FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		StartFrame = (int)(start / fps60);

		m_motion_list["default"].numFrame =
			(int)((stop - start) / fps60);
	}
	m_start_frame = StartFrame;


	//MeshData* pMeshData = GetMesh(matIdx);
	FbxMeshData mesh_data = p_mesh_data_list[matIdx];
	// スキン情報の有無
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		return;
	}

	// メッシュデータから頂点数取り出し
	int vertexNum = p_mesh_data_list[matIdx].vertex_num;

	// サイズ算出
	UINT size = (UINT)(vertexNum * sizeof(SkinCustomVertex));

	SkinCustomVertex* pVertices;

	// バッファをロックしてデータを書き込む
	mesh_data.p_vertex_buffer->Lock(0, size, (void**)&pVertices, 0);

	FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);

	// ボーン数
	int nBone = pSkin->GetClusterCount();
	//	全ボーン情報取得
	for (int bone = 0; bone < nBone; bone++)
	{
		//	ボーン情報取得
		FbxCluster* pCluster = pSkin->GetCluster(bone);
		FbxAMatrix trans;
		pCluster->GetTransformMatrix(trans);
		trans.mData[0][1] *= -1;
		trans.mData[0][2] *= -1;
		trans.mData[1][0] *= -1;
		trans.mData[2][0] *= -1;
		trans.mData[3][0] *= -1;

		//	ボーン名取得
		const char* pName = pCluster->GetLink()->GetName();

		BoneData* pBone;
		//	ボーン検索
		int bone_no = FindBone(pName);
		if (bone_no != -1)
		{
			// 同じボーンがあったらコピー
			pBone = &m_bone_list[bone_no];
		}
		else
		{

			pBone = &m_bone_list[m_total_bone];
			m_total_bone++;

			strcpy_s(pBone->name, pName);
			//	オフセット行列作成
			FbxAMatrix LinkMatrix;

			// クラスターからリンク行列算出
			pCluster->GetTransformLinkMatrix(LinkMatrix);
			
			// 変換
			FbxMatLConvert(LinkMatrix);

			// オフセット行列作成
			FbxAMatrix Offset = LinkMatrix.Inverse() * trans;
			FbxDouble* OffsetM = (FbxDouble*)Offset;

			//	オフセット行列保存
			for (int i = 0; i < 16; i++)
			{
				pBone->offset.m[i / 4][i % 4] = (float)OffsetM[i];
			}
			//	キーフレーム読み込み
			LoadKeyFrames("default",m_total_bone,pCluster->GetLink());
		}

		//	ウェイト読み込み
		int weightCount = pCluster->GetControlPointIndicesCount();
		int* pWeightIndex = pCluster->GetControlPointIndices();
		double* wgt = pCluster->GetControlPointWeights();

		int* index = pMesh->GetPolygonVertices();


		for (int i = 0; i < weightCount; i++)
		{
			int wgtIdx2 = pWeightIndex[i];
			//	全ポリゴンからwgtIdx2番目の頂点検索
			for (int vtxIdx = 0; vtxIdx < vertexNum; vtxIdx++)
			{
				if (index[vtxIdx] != wgtIdx2) continue;

				//	頂点にウェイト保存
				int weightCount;
				for (weightCount = 0; weightCount < 4; weightCount++)
				{
					if (pVertices[vtxIdx].weight[weightCount]
						<= 0.0f)
					{
						break;
					}
				}
				if (4 <= weightCount) continue;

				pVertices[vtxIdx].bone_index[weightCount]
					= bone_no;
				pVertices[vtxIdx].weight[weightCount]
					= (float)wgt[i];
			}
		}
	}
	//	ウェイト正規化
	// ５本以上にまたっがてる場合のため
	for (int vtxIdx = 0; vtxIdx < vertexNum; vtxIdx++)
	{
		float n = 0;
		int weightCount;
		//	頂点のウェイトの合計値
		for (weightCount = 0; weightCount < 4; weightCount++)
		{
			if (pVertices[vtxIdx].weight[weightCount] <= 0.0f)
			{
				break;
			}
			n += pVertices[vtxIdx].weight[weightCount];
		}
		//	正規化
		for (weightCount = 0; weightCount < 4; weightCount++)
		{
			pVertices[vtxIdx].weight[weightCount] /= n;
		}
	}

	mesh_data.p_vertex_buffer->Unlock();
}


void FbxSampleMotion::LoadKeyFrames(std::string name, int bone, FbxNode* pBoneNode)
{
	//	メモリ確保
	Motion* pMotion = &m_motion_list[name];

	// キーのサイズをリサイズ
	pMotion->Key[bone].resize(pMotion->numFrame);

	// 時間(平均時間算出)
	double time = (double)m_start_frame * (1.0 / 60);

	// fbxタイム
	FbxTime T;

	for (UINT f = 0; f < pMotion->numFrame; f++)
	{

		// t秒の時間算出
		T.SetSecondDouble(time);

		//	T秒の姿勢行列をGet
		FbxMatrix m = pBoneNode->EvaluateGlobalTransform(T);
	
		// 逆変換
		FbxMatLConvert(
			m
		);

		FbxDouble* mat = (FbxDouble*)m;
		for (int i = 0; i < 16; i++)
		{
			pMotion->Key[bone][f].m[i / 4][i % 4] = (float)mat[i];
		}

		time += 1.0 / 60.0;
	}
}


void FbxSampleMotion::LoadMotion(
	std::string name,
	const char* pFilename
)
{
	FbxManager* pManager = FbxManager::Create();
	FbxScene* pScene = FbxScene::Create(pManager, "");

	//	ファイルからシーンに読み込み
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");

	// シーンを読み込むためにインポーター初期化
	pImporter->Initialize(pFilename);
	pImporter->Import(pScene);
	// 削除
	pImporter->Destroy();

	//	モーション情報取得
	FbxArray<FbxString*> names;
	pScene->FillAnimStackNameArray(names);

	// fbxのモーション時間取得
	FbxTakeInfo* take = pScene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	// スタートフレーム出力(fps60で割る)
	m_start_frame = (int)(start / fps60);

	// 最初から最後を引き、平均フレーム時間を出す
	m_motion_list[name].numFrame = (int)((stop - start) / fps60);

	//	ルートノード取得
	FbxNode* root = pScene->GetRootNode();

	//	全ボーン読み込み
	for (int b = 0; b < (int)m_total_bone; b++)
	{
		//	ボーンノード検索
		FbxNode* pBone = root->FindChild(m_bone_list[b].name);
		if (pBone == NULL) continue;

		//	キーフレーム読み込み
		LoadKeyFrames(name, b, pBone);
	}

	// 読み込んだので解放
	pScene->Destroy();
	pManager->Destroy();
}


void FbxSampleMotion::FbxMatConvertD3DMat(
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


void FbxSampleMotion::FbxMatLConvert(
	FbxAMatrix& out_fbx_mat
) {
	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}


void FbxSampleMotion::FbxMatLConvert(
	FbxMatrix&out_fbx_mat
) {

	out_fbx_mat.mData[0][1] *= -1;// _12
	out_fbx_mat.mData[0][2] *= -1;// _13
	out_fbx_mat.mData[1][0] *= -1;// _21
	out_fbx_mat.mData[2][0] *= -1;// _31
	out_fbx_mat.mData[3][0] *= -1;// _41
}
