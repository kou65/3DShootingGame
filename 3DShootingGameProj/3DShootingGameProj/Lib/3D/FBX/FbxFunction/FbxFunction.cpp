#include"FbxFunction.h"
#include"../../../Utility/Convert/Convert.h"



namespace FbxFunction {


	bool CreateLoadFbxModule(
		LP_FBX_MANAGER&p_manager,
		LP_FBX_SCENE&p_scene,
		LP_FBX_IMPORTER&p_importer,
		const std::string&file_name,
		bool is_non_blocking
	) {

		if (CreateFbxManager(p_manager) == false) {
			return false;
		}
		if (CreateFbxScene(p_manager, p_scene) == false) {
			return false;
		}
		if (CreateFbxImporter(
			p_importer,
			p_manager,
			file_name.c_str()
		) == false) {
			return false;
		}
		if (LoadFbxSceneToImporter(
			p_importer,
			p_scene,
			is_non_blocking
		) == false) {
			return false;
		}

		return true;
	}


	bool Destory(
		LP_FBX_SCENE&p_scene,
		LP_FBX_IMPORTER&p_importer,
		LP_FBX_MANAGER&p_manager
	) {


		p_scene->Destroy();
		p_importer->Destroy();
		// 管理者を後に削除する
		p_manager->Destroy();
		

		return true;
	}


	bool CreateFbxManager(LP_FBX_MANAGER &p_manager) {


		// fbxマネージャオブジェクトの生成
		p_manager = FbxManager::Create();

		if (p_manager == nullptr) {
			Window::TextMessageBox("FbxManagerの生成に失敗しました");
			return false;
		}

		return true;
	}


	bool CreateFbxScene(
		LP_FBX_MANAGER &p_manager,
		LP_FBX_SCENE &p_scene
	) {

		if (p_manager == nullptr) {
			return false;
		}

		p_scene = FbxScene::Create(p_manager, "");
		if (p_scene == nullptr) {
			Window::TextMessageBox("FBXSceneの生成に失敗しました");
			return false;
		}
		return true;
	}


	bool CreateFbxImporter(
		LP_FBX_IMPORTER&p_out_impoter,
		LP_FBX_MANAGER&p_manager,
		const std::string&file_name
	) {

		if (p_manager == nullptr) {
			return false;
		}

		p_out_impoter =
			FbxImporter::Create(
				p_manager,
				""
			);

		if ((p_out_impoter)->Initialize(file_name.c_str()) == false) {
			return false;
		}

		if (p_out_impoter == nullptr) {
			Window::TextMessageBox("FbxImporterの生成に失敗しました");
			return false;
		}

		return true;
	}


	bool LoadFbxSceneToImporter(
		LP_FBX_IMPORTER &p_importer,
		LP_FBX_SCENE&p_scene,
		bool is_non_blocking
	) {

		if (p_scene == nullptr) {
			return false;
		}

		// 同期でインポート
		if ((p_importer)->Import(p_scene, is_non_blocking) == false) {
			return false;
		}

		return true;
	}


	//	ボーン行列の補間
	void MatrixInterporate(
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



	
	void MatrixInterporate2(
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
	
		//m._31 *= 0.1f;
		//m._32 *= 0.1f;
		//m._33 *= 0.1f;
	
	
		//out = m;
	}

	void MatrixInterporate3(D3DXMATRIX& out, D3DXMATRIX& A, D3DXMATRIX B, float rate)
	{
		out = A * (1.0f - rate) + B * rate;
	}
	

	namespace Node {

		void RecursiveNode(
			FbxNode*p_node,
			const FbxScene&p_scene
		)
		{

			// メッシュ分のバッファを確保
			UINT skeleton_count = (UINT)
				(p_scene).GetSrcObjectCount<FbxSkeleton>();

			for (int i = 0; i < p_node->GetNodeAttributeCount(); i++) {

				FbxNodeAttribute *attr = p_node->GetNodeAttributeByIndex(i);
				FbxNodeAttribute::EType type = attr->GetAttributeType();

				switch (type) {

					// スケルトンならば
				case FbxNodeAttribute::EType::eSkeleton:

					// スケルトン情報取得
					FbxSkeleton *ske = p_node->GetSkeleton();

					break;
				}
			}

			for (int i = 0; i < p_node->GetChildCount(); i++) {

				// 子ノード取得
				FbxNode*p_child_node = p_node->GetChild(i);

				RecursiveNode(p_node, p_scene);
			}
		}

		void SerchNode(
			const FbxScene&fbx_scene
		) {


			FbxNode*p_node = fbx_scene.GetRootNode();

			// ノードの再帰関数
			RecursiveNode(p_node, fbx_scene);
		}

	}

	namespace Skeleton {


		FbxAMatrix GetSkeletonAnimationMatrix(
			FbxSkeleton*s,
			FbxScene*p_scene,
			const FbxTime&time
		) {
			
			FbxAMatrix mat =
				s->GetNode()->EvaluateGlobalTransform(time);

			FbxAMatrix inv_mat =
				p_scene->GetRootNode()
				->EvaluateGlobalTransform(time).Inverse();

			FbxAMatrix comb_mat = inv_mat * mat;

			return comb_mat;
		}


		bool MatchSkelNameAndMeshSkelName(
			const std::string&mesh_name,
			const std::string&mesh_skeleton_name,
			FbxSkeleton*s
		) {

			// 現在のノードのスケルトン名
			std::string skeleton_name =
				s->GetNode()->GetName();

			// 現在のメッシュの中にあるボーンのクラスターのスケルトン名
			std::string bone_skeleton_name =
				mesh_skeleton_name;

			// スケルトンの名とメッシュのスケルトン名が一致しているか
			if (Utility::Convert::IsStrCmp(
				skeleton_name.c_str(),
				bone_skeleton_name.c_str()
			) == true) {
				return true;
			}
			return false;
		}



		bool MatchSkeletonMotion(
			FbxScene*p_scene,
			const std::string&skel_name, // メッシュのスケルトン名
			FbxSkeleton**p_out_skel, 
			const int&skeleton_num_max,
			int&current_skeleton_count
		) {

			// メッシュ数分回す
			for (int i = current_skeleton_count;
				i < skeleton_num_max;
				i++) {

				/* この順番に読み込んでいく */

				// メッシュを取得
				FbxSkeleton * p_skel = 
					(p_scene)->GetSrcObject<FbxSkeleton>(i);

				// スケルトンノード名
				std::string skeleton_node_name = 
					p_skel->GetNode()->GetName();

				// 文字列が一致しているかどうか
				if (MatchSkelNameAndMeshSkelName(
					skeleton_node_name,
					skel_name,
					p_skel
				) == true) {

					// スケルトン取得
					*p_out_skel =
						p_skel;

					// 現在のスケルトンカウントを更新
					current_skeleton_count = i;

					return true;
				}

			}

			return false;
		}
	}


}
