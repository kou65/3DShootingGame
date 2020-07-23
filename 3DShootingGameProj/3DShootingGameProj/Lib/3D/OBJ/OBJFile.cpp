#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include"../../Texture/TextureManager/TextureManager.h"
#include"../../Utility/Convert/Convert.h"
#include"OBJFile.h"




Obj::Obj() : Model(){

	// グラフィックス取得
	mp_graphics = Graphics::GetInstance();

	// 初期パスタイプ
	m_pass_type = PassType::PHONE_REFLECTION;

	// 各シェーダー初期化
	m_normal_shader.Init();

	// ライト影シェーダー初期化
	m_light_shadow.Init(VertexDecl::Type::OBJ);

	// シャドウの初期化
	m_shadow.Init();

	// ライトの初期化
	m_light_shader.Init();

	// ztexを渡す
	m_shadow.SetShandowMap(
		ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		)->GetZTexture()
	);

	// ztexを渡す
	m_light_shadow.SetShadowMap(
		ZTextureManager::GetInstance()->GetZTexturePtr(
			FuncZTexture::Const::Z_TEX_1024
		)->GetZTexture()
	);

	RenderTargetData data;
	data.width = 1024;
	data.height = 1024;

	m_rt.CreateSurface(data);
}


void Obj::Update(
	const ObjParameter&param,
	StandardTSShader*shader
) {

	// 合成行列
	D3DXMATRIX total_mat = GetTransformMatrix(param);

	// ワールド行列セット
	shader->SetWorldMatrix(total_mat);

	// 更新
	shader->Update();
}


void Obj::Draw(
	const DrawStatus&state,
	ObjParameter&param,
	const UINT &pass
) {

	// 各書き込み
	switch (state) {

	case DrawStatus::NORMAL:

		// 通常描画
		DrawShader(
			param,
			&m_normal_shader
		);

		break;

	case DrawStatus::LIGHT:

		// ライト描画
		DrawLightObj(
			param,
			pass
		);
		break;

	case DrawStatus::SHADOW:

		// 影描画
		DrawShadowObj(
			param,
			pass
		);
		break;

	case DrawStatus::LIGHT_SHADOW:

		// ライトと影描画
		LightShadowDraw(
			param,
			m_light_data,
			m_shadow_data,
			pass
		);
		break;

	}
}


void Obj::InitGrapicData() {

	// 行列
	D3DXMATRIX mat_camera_view;
	D3DXMATRIX mat_camera_proj;
	D3DXMATRIX mat_light_view;
	D3DXMATRIX mat_light_proj;
	D3DXMATRIX mat_current_view;
	D3DXMATRIX mat_current_proj;

	// カメラ射影
	D3DXMatrixPerspectiveFovLH(
		// カメラ射影行列
		&mat_camera_proj,
		// 画角
		D3DXToRadian(45),
		// アスペクト比
		640.0f / 480.0f,
		// 視推台の最も近い距離()
		10.0f,
		// 視推台の最も遠い距離
		1000.0f
	);

	float light_scale = 1.5f;

	// デフォルト
	D3DXVECTOR3 defa(
		light_scale * 100,
		light_scale * 100,
		light_scale * 100);

	// 視点
	D3DXVECTOR3 look(
		0.f,
		-20.f,
		0.f
	);

	// ライトビュー
	D3DXMatrixLookAtLH(
		// ライトビュー行列
		&mat_light_view,
		// カメラの位置座標
		&defa,
		// 注視点座標ポインタ
		&look,
		// アップベクトル
		&D3DXVECTOR3(0, 1, 0)
	);

	// グラフィックデータ
	LightData light_data;
	ShadowData shadow_data;

	// ビュー行列
	Graphics::GetInstance()->GetDevice()
		->GetTransform(D3DTS_VIEW, &mat_current_view);

	// プロジェクション行列
	Graphics::GetInstance()->GetDevice()
		->GetTransform(D3DTS_PROJECTION, &mat_current_proj);

	// データ代入
	shadow_data.camera_view_mat = mat_current_view;
	shadow_data.camera_proj_mat = mat_current_proj;

	// データ取得
	ZTextureData data
		= ZTextureManager::GetInstance()->GetZTexData();

	// z値からのカメラ情報を入れる
	shadow_data.light_view_mat = data.mat_camera_view;
	shadow_data.light_proj_mat = data.mat_camera_proj;

	// ライトデータ代入
	Obj::GetInstance()->SetLightData(
		light_data
	);
	// 影データ代入
	Obj::GetInstance()->SetShadowData(
		shadow_data
	);
}


void Obj::UpdateCameraDataInShadow() {

	// 行列
	D3DXMATRIX mat_current_view;
	D3DXMATRIX mat_current_proj;

	// シャドウデータ
	ShadowData shadow_data;

	// ビュー行列
	Graphics::GetInstance()->GetDevice()
		->GetTransform(D3DTS_VIEW, &mat_current_view);

	// プロジェクション行列
	Graphics::GetInstance()->GetDevice()
		->GetTransform(D3DTS_PROJECTION, &mat_current_proj);

	// データ代入
	shadow_data.camera_view_mat = mat_current_view;
	shadow_data.camera_proj_mat = mat_current_proj;

	// データ取得
	ZTextureData data
		= ZTextureManager::GetInstance()->GetZTexData();

	// z値からのカメラ情報を入れる
	shadow_data.light_view_mat = data.mat_camera_view;
	shadow_data.light_proj_mat = data.mat_camera_proj;

	m_shadow_data = shadow_data;
}


void Obj::DrawFhoneLight(
	const ObjParameter&param
) {

	// ライトカメラセット
	m_shadow.SetLightViewMatrix(m_shadow_data.light_view_mat);
	m_shadow.SetLightProjMatrix(m_shadow_data.light_proj_mat);

	// カメラ情報セット
	m_shadow.SetViewMatrix(m_shadow_data.camera_view_mat);
	m_shadow.SetProjMatrix(m_shadow_data.camera_proj_mat);

	// ライトデータ挿入
	m_shadow.SetLightData(m_light_data);

	// 影に変更
	m_pass_type = PassType::DEPTH_SHADOW;

	// 更新
	Update(param, &m_shadow);

	// シャドウ更新
	m_shadow.Update();

	// ライト描画
	ShaderParameterDraw(param, &m_shadow,2);

	// 影描画
	ShaderParameterDraw(param, &m_shadow, 0);
}


void Obj::SetShadowData(
	ShadowData&data
) {
	m_shadow_data = data;
}


void Obj::SetLightData(
	LightData&data
) {
	m_light_data = data;
}


void Obj::DrawObjByNormalShader(
	const ObjParameter &param
	){

	m_pass_type = PassType::NORMAL;

	// 行列を返す
	D3DXMATRIX total_mat = GetTransformMatrix(param);

	// ワールド行列セット
	m_normal_shader.SetWorldMatrix(&total_mat);
	m_normal_shader.SetColor(param.color);

	// 更新
	m_normal_shader.Update();

	ShaderParameterDraw(param, &m_normal_shader);
}


void Obj::DrawShadowObj(
	const ObjParameter &param,
	UINT pass
) {

	// ライトカメラセット
	m_shadow.SetLightViewMatrix(m_shadow_data.light_view_mat);
	m_shadow.SetLightProjMatrix(m_shadow_data.light_proj_mat);

	// カメラ情報セット
	m_shadow.SetViewMatrix(m_shadow_data.camera_view_mat);
	m_shadow.SetProjMatrix(m_shadow_data.camera_proj_mat);

	// ライトデータ挿入
	m_shadow.SetLightData(m_light_data);
	
	// 影に変更
	m_pass_type = PassType::DEPTH_SHADOW;

	// 更新
	Update(param, &m_shadow);

	// 影描画
	ShaderParameterDraw(param,&m_shadow,pass);
}


void Obj::DrawSoftShadow(
	const ObjParameter&param,
	const UINT &pass,
	LPDIRECT3DTEXTURE9 shadow_map
) {

	// ライトカメラセット
	m_soft_shadow.SetLightView(m_shadow_data.light_view_mat);
	m_soft_shadow.SetLightProj(m_shadow_data.light_proj_mat);

	// カメラ情報セット
	m_soft_shadow.SetViewMatrix(m_shadow_data.camera_view_mat);
	m_soft_shadow.SetProjMatrix(m_shadow_data.camera_proj_mat);

	// ライトデータ挿入
	m_soft_shadow.SetLightData(m_light_data);

	// 影に変更
	m_pass_type = PassType::NONE;

	// マップサイズ
	m_soft_shadow.SetMapSize(1024.f);

	// 更新
	Update(param, &m_soft_shadow);

	MultRenderTarget mrt;

	// レンダリング開始
	mrt.SetBackBuffer();
	mrt.SetRender(m_rt);

	// 影描画
	ShaderParameterDraw(param,&m_soft_shadow,pass);
	
	LPDIRECT3DTEXTURE9 tex = 
		m_rt.GetTexture();

	// シャドウマップをセット
	m_soft_shadow.SetShadowMap(tex);
	
	// 更新
	m_soft_shadow.Update();
	
	// 描画
	//m_soft_shadow.SetRenderIndex(1);

	// 描画
	mrt.SetRender(m_rt);
	ShaderParameterDraw(param,&m_soft_shadow,1);

	// 描画
	//ShaderParameterDraw(param, &m_soft_shadow, 2);

	//ShaderParameterDraw(param, &m_soft_shadow, 3);

	// バックバッファを戻す
	mrt.GetBackBuffer();

	m_soft_shadow.DrawTex(m_rt.GetTexture());
}


void Obj::DrawLightObj(
	const ObjParameter &param,
	UINT pass
) {

	// パス
	m_pass_type = PassType::PHONE_SHADER;

	// カラー情報セット
	m_light_shader.SetColor(param.color);

	// ライトセット
	m_light_shader.SetLightData(
		m_light_data
	);

	// 通常更新
	Update(param, &m_light_shader);

	// シェーダーパラメータ描画
	ShaderParameterDraw(param, &m_light_shader,pass);
}



void Obj::LightShadowDraw(
	const ObjParameter&param,
	const LightData&light_data,
	const ShadowData&shadow_data,
	UINT pass
) {

	// 影データ挿入
	{
	
		// カメラ情報セット
		m_light_shadow.SetViewMatrix(shadow_data.camera_view_mat);
		m_light_shadow.SetProjMatrix(shadow_data.camera_proj_mat);

		// ライトデータ挿入
		m_light_shadow.SetLightData(light_data);

		// 影データ挿入
		m_light_shadow.SetShadowData(shadow_data);

		// 影に変更
		m_pass_type = PassType::DEPTH_SHADOW;

		// 更新
		Update(param, &m_light_shadow);

		// シャドウ描画
		ShaderParameterDraw(param, &m_light_shadow,pass);
	}
}


void Obj::WriteZTexture(
	const ObjParameter&param,
	const std::string &register_name
) {

	// zテクスチャ
	ZTexture*p_tex
		= ZTextureManager::GetInstance()->
		GetZTexturePtr(register_name);

	// zテクスチャを書き込む
	WriteZTexture(
		param,
		p_tex
	);
}


void Obj::WriteZTexture(
	const ObjParameter&param,
	ZTexture*p_tex
) {

	// 取得
	ZTextureData data = 
		ZTextureManager::GetInstance()->GetZTexData();

	// カメラ情報セット
	p_tex->SetViewMatrix(data.mat_camera_view);
	p_tex->SetProjMatrix(data.mat_camera_proj);

	// パス
	m_pass_type = PassType::ZTEXTURE;

	// 更新
	Update(param, p_tex);

	// zテクスチャ描画
	DrawShader(param, p_tex);
}


void Obj::WriteZTexByParam(
	const std::string &z_tex_name
) {

	// zテクスチャ
	ZTexture*p_tex
		= ZTextureManager::GetInstance()->
		GetZTexturePtr(z_tex_name);

	UINT i;

	// 描画開始
	p_tex->Begin(i, 0);

	// 書き込み
	for (auto&itr : m_shadow_param_list){
		
		// Zテクスチャ書き込み
		WriteZTexture(itr.second, p_tex);
	}

	p_tex->End();
}


void Obj::EntryObjParam(
	const std::string&param_name,
	const ObjParameter&param
){

	// パラメータを追加
	m_shadow_param_list[param_name] = param;
}


void Obj::DrawShadowParamList()
{

	// zテクスチャ書き込み
	WriteZTexByParam(FuncZTexture::Const::Z_TEX_1024);

	// 本編書き込み
	for (auto&itr1 : m_shadow_param_list) {

		// 描画
		Draw(itr1.second.draw_status, itr1.second);
	}
}


void Obj::ResetParamList() {

	m_shadow_param_list.clear();
}


void Obj::ShaderParameterDraw(
	const ObjParameter &param,
	ShaderBase*shader,
	UINT pass
) {

	// パス
	UINT total_pass;

	// 描画開始
	shader->Begin(total_pass, 0);

	// パス描画
	DrawShader(
		param,
		shader,
		pass
	);

	// 描画終了
	shader->End();
}


void Obj::DrawSubSet(
	const ObjParameter&param,
	const DrawStatus&state,
	int sub_set_number
) {

	ShaderBase*p_shader = nullptr;

	// 選んだ描画状態のシェーダーを入れる
	switch (state) {

	case DrawStatus::NORMAL:
		p_shader = &m_normal_shader;
		break;

	case DrawStatus::LIGHT:
		p_shader = &m_light_shader;
		break;

	case DrawStatus::SHADOW:
		p_shader = &m_shadow;
		break;
	}

	UINT total_pass;

	// 描画開始
	p_shader->Begin(total_pass,0);

	DrawShader(
		param,
		p_shader
	);

	// 描画終了
	p_shader->End();
}


void Obj::DrawSubSet(
	const ObjParameter&param,
	ShaderBase*p_shader,
	int sub_set_number
) {


	UINT total_pass;

	p_shader->Begin(total_pass, 0);

	DrawShader(
		param,
		p_shader
	);

	p_shader->End();
}


void Obj::DrawShader(
	const ObjParameter &param,
	ShaderBase*shader,
	UINT pass
) {

	// キーが登録されていないなら描画しない
	if (m_obj_file_data.count(param.register_obj_file_name) == 0) {
		return;
	}
	// 生成されていないなら描画しない
	if (m_obj_file_data[param.register_obj_file_name] == nullptr) {
		return;
	}

	// テクスチャが存在するかどうか
	bool is_texture = false;

	// オブジェクトファイルへアクセス
	ObjFileData* p_data =
		m_obj_file_data[param.register_obj_file_name];


	// マテリアル数分回す
	for (UINT i = 0; i < p_data->material_num; i++) {

		// 使用マテリアル名取得
		std::string usemtl_name = 
			p_data->m_usemtl_name_list[i];

		// マテリアル情報取得
		MaterialInfo mtl = 
			p_data->
			m_material_data_list[usemtl_name];

		// テクスチャが存在しているか確認
		is_texture = CheckTexture(mtl.texture_name);
		if (param.texture_name.size() != 0) {
			is_texture = true;
		}

		// パス数
		UINT select_pass = pass;

		// パス指定がなければ
		if (pass == -1) {

			// 現在のパス数取得
			select_pass = GetUsePass(m_pass_type);

			// テクスチャ用に変更
			if (is_texture == true){
				select_pass++;
			}
		}

		// シェーダーをパスごとに描画
		DrawBeginPassShader(
			shader,
			p_data,
			param,
			select_pass,
			i
		);
	}
}


void Obj::DrawBeginPassShader(
	ShaderBase*p_shader,
	ObjFileData*p_data,
	const ObjParameter&param,
	const UINT&current_pass,
	int sub_num
) {

	// テクスチャが存在するかどうか
	bool is_texture = false;

	// 使用マテリアル名取得
	std::string usemtl_name =
		p_data->m_usemtl_name_list[sub_num];

	// マテリアル情報取得
	MaterialInfo mtl =
		p_data->
		m_material_data_list[usemtl_name];

	// 代入用
	std::string texture_name = param.texture_name;

	// ポインタがあるなら
	if (param.p_tex != nullptr) {

		// シェーダーに直接読み込ませる
		LoadTextureShader(param.p_tex);
	}
	// ポインタがないなら
	else {

		// テクスチャの読み込み
		is_texture = LoadTexture(mtl.texture_name, texture_name);
	}

	// 3D描画に必要なパラメータをセット
	Model::Set3DParameter(
		Graphics::GetInstance(),
		p_data->mp_index_buffer,
		p_data->mp_vertex_buffer,
		sizeof(MeshCustomVertex),
		// マテリアル
		mtl.material,
		FVF_CUSTOM
	);

	// 情報受け取り
	UINT face_start = p_data->
		m_object_sub_set_list[sub_num].face_start;
	// 面数
	UINT face_count = p_data->
		m_object_sub_set_list[sub_num].face_count;

	// パスの開始
	p_shader->BeginPass(current_pass);

	// インデックス描画
	mp_graphics->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		// 頂点インデックスオフセット値
		0,
		// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
		face_start,
		// 上引数の最小以降の頂点数を指定
		face_count * 3,
		// 描画を開始する頂点インデックスまでのオフセット値を指定
		face_start,
		// 上のSTART_INDEX引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
		face_count
	);

	p_shader->EndPass();
}


void Obj::LoadTextureShader(
	LPDIRECT3DTEXTURE9 p_tex
) {

	m_light_shader.SetTexture(p_tex);
	m_normal_shader.SetTexture(p_tex);
	m_shadow.SetTexture(p_tex);
	m_light_shadow.SetTexture(p_tex);
	m_blur.SetTexture(p_tex);
	m_soft_shadow.SetModelTex(p_tex);
}


UINT Obj::GetUsePass(const PassType&type) {

	switch (type)
	{
	case PassType::NORMAL:
		return 0;

	case PassType::DEPTH_SHADOW:
		return 0;

	case PassType::DIRECTIONAL:
		return 0;

	case PassType::ZTEXTURE:
		return 0;

	case PassType::DIFFUSE_REFLECTION:
		return 2;

	case PassType::SPECULAR_REFLECTION:
		return 4;

	case PassType::POINT_LIGHT:
		return 6;

	case PassType::PHONE_REFLECTION:
		return 8;

	case PassType::PHONE_SHADER:
		return 9;

	case PassType::BULR_FILTER:
		return 0;

	default:
		break;
	}

	return 0;
}


void Obj::DrawNormal(
	const ObjParameter&param
) {


	// キーが登録されていないなら描画しない
	if (m_obj_file_data.count(param.register_obj_file_name) == 0) {
		return;
	}
	// 生成されていないなら描画しない
	if (m_obj_file_data[param.register_obj_file_name] == nullptr) {
		return;
	}

	D3DXMATRIX total_mat = GetTransformMatrix(param);

	// マテリアル数分回す
	for (unsigned int i = 0;
		i < m_obj_file_data[param.register_obj_file_name]->material_num;
		i++) {

		// 特定のオブジェクトファイルデータを受け取る
		ObjFileData *p_obj_file_data =
			m_obj_file_data[param.register_obj_file_name];

		if (p_obj_file_data->mp_vertex_buffer == nullptr) {
			return;
		}

		// ストリームをセット
		mp_graphics->GetDevice()->SetStreamSource(
			0,
			p_obj_file_data->mp_vertex_buffer,
			0,
			sizeof(MeshCustomVertex)
		);

		// マテリアル名
		std::string material_name =
			p_obj_file_data->m_usemtl_name_list[i];

		// テクスチャが存在しているなら
		if (TextureManager::GetInstance()->FindTexture(
			p_obj_file_data->m_material_data_list[material_name].texture_name
		) == true) {

			// テクスチャ管理者からテクスチャ受け取り
			TextureData texture_data = TextureManager::GetInstance()
				->GetTextureData(
					p_obj_file_data->
					m_material_data_list[material_name].texture_name
				);

			// テクスチャセット
			mp_graphics->GetDevice()->
				SetTexture(0,texture_data);
		}

		// マテリアルをセット
		mp_graphics->GetDevice()->SetMaterial(
			&p_obj_file_data->m_material_data_list[material_name]
			.material
		);

		// インデックス番号をデバイスに設定する
		mp_graphics->GetDevice()->SetIndices(
			p_obj_file_data->mp_index_buffer
		);

		// どの情報を伝えるか
		mp_graphics->GetDevice()->SetFVF(FVF_CUSTOM);

		mp_graphics->GetInstance()->GetDevice()
			->SetTransform(D3DTS_WORLD, &total_mat);

		// インデックス描画
		mp_graphics->GetDevice()->DrawIndexedPrimitive(
			// 頂点のつなぎ方
			D3DPT_TRIANGLELIST,
			// 頂点インデックスの一番最初までのオフセット値を指定
			0,
			// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
			p_obj_file_data->
			m_object_sub_set_list[i].face_start,
			// 上引数の最小以降の頂点数を指定
			p_obj_file_data->
			m_object_sub_set_list[i].face_count * 3,
			// 描画を開始する頂点インデックスまでのオフセット値を指定
			p_obj_file_data->
			m_object_sub_set_list[i].face_start,
			// 上のSTART_INDEX引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
			p_obj_file_data->
			m_object_sub_set_list[i].face_count
		);

		// セットテクスチャリセット
		mp_graphics->GetDevice()->SetTexture(
			0,
			NULL
		);

	}
}


bool Obj::CheckTexture(const std::string &tex) {

	return TextureManager::GetInstance()->FindTexture(tex);
}


bool Obj::LoadTexture(
	std::string&mtl_texture_name,
	std::string&objparam_file_name
) {

	bool is_texture = false;

	// テクスチャが存在しているなら
	if (CheckTexture(
		mtl_texture_name
	) == true) {

		// テクスチャ管理者からテクスチャ受け取り
		TextureData texture_data = TextureManager::GetInstance()
			->GetTextureData(mtl_texture_name);

		// シェーダーテクスチャセット
		LoadTextureShader(texture_data);

		// テクスチャが存在する
		is_texture = true;
	}
	// テクスチャがない場合
	else {

		// 外部からテクスチャがあるなら
		if (objparam_file_name.size() != 0) {

			// テクスチャ管理者からテクスチャ受け取り
			TextureData texture_data = TextureManager::GetInstance()
				->GetTextureData(
					objparam_file_name
				);

			// シェーダーテクスチャセット
			LoadTextureShader(texture_data);

			// テクスチャが存在する
			is_texture = true;
		}
	}

	return is_texture;
}


D3DXMATRIX Obj::GetTransformMatrix(const ObjParameter&param) {
	

	// ワールド座標初期化
	D3DXMATRIX total_mat, rota_mat, scale_mat, move_mat;
	D3DXMatrixIdentity(&total_mat);

	D3DXMATRIX rota_x, rota_y, rota_z;

	// 回転
	D3DXMatrixRotationZ(&rota_z, D3DXToRadian(param.rota.z));
	D3DXMatrixRotationX(&rota_x, D3DXToRadian(param.rota.x));
	D3DXMatrixRotationY(&rota_y, D3DXToRadian(param.rota.y));

	// 回転行列作成
	rota_mat = (rota_z * rota_y * rota_x);

	// 拡縮
	D3DXMatrixScaling(
		&scale_mat, param.scale.x, param.scale.y, param.scale.z);

	// 移動
	D3DXMatrixTranslation(
		&move_mat, param.pos.x, param.pos.y, param.pos.z);

	total_mat = (scale_mat * rota_mat * move_mat);

	return total_mat;
}


bool Obj::Load(
	const std::string &obj_file_path,
	const std::string &register_name,
	const std::string &texture_file_path) {

	std::vector<std::string>split_file_name;

	// サイズがないならobjファイルがある階層ディレクトリを取ってくる
	if (texture_file_path.size() <= 0) {
		split_file_name = 
			Utility::Convert::SplitStr('/', obj_file_path);
	}
	else {
		split_file_name.emplace_back(texture_file_path);
		split_file_name.emplace_back();
	}

	// マテリアルデータバッファ
	std::vector<MaterialInfo>material_data_list;

	// マテリアル名
	std::string material_name;

	// 頂点バッファ
	std::vector<MeshCustomVertex>mesh_vertex_list;

	// 面情報バッファ
	std::vector<INT>indices;

	// 総マテリアル数
	int material_num = 0;


	// メイン読み込み
	if (MeshLoad(
		obj_file_path,
		register_name,
		material_name,
		material_num,
		indices,
		mesh_vertex_list) == false) {
		return false;
	}

	std::string file_path_material_name;
	std::string file_path_texture;

	// 最後のカレントdは外す
	for (unsigned int i = 0; i < split_file_name.size() - 1; i++) {
		 file_path_material_name += split_file_name[i];
	}

	// テクスチャファイルパス作成
	file_path_texture = file_path_material_name;

	// ファイルパス付のマテリアル名にする
	file_path_material_name += material_name;

	// マテリアル読み込み
	MaterialFileLoad(
		file_path_material_name,
		register_name,
		file_path_texture
	);

	
	// バーテックスバッファ生成
	VertexBufferCreate(
		(int)mesh_vertex_list.size(),
		register_name,
		mesh_vertex_list
	);

	// インデックスバッファ生成
	IndexBufferCreateFaceBase(
		(int)indices.size(),
		register_name,
		indices
		);

	// 総マテリアル数保存
	m_obj_file_data[register_name]->material_num = material_num;

	return true;
}



bool Obj::MeshLoad(
	const std::string &file_path,
	const std::string &registr_name,
	std::string&out_material_name,
	int &out_total_material_num,
	std::vector<INT>&indices,
	std::vector<MeshCustomVertex>&mesh_list
	) {

	// 頂点読み込みバッファ
	std::vector<D3DXVECTOR3>vertex_list;

	// 法線読み込みバッファ
	std::vector<D3DXVECTOR3>normal_list;

	// テクスチャ座標バッファ
	std::vector<D3DXVECTOR2>uv_list;

	// ファイルストリーム
	FILE *p_file;

	HRESULT result;

	std::string obj_file_path = file_path;

	// ファイルオープン
	result = fopen_s(&p_file, obj_file_path.c_str(), "r");

	if (result != D3D_OK) {
		Window::TextMessageBox("OBJファイルオープンに失敗しました");
		return false;
	}

	// オブジェクトファイルデータを生成
	m_obj_file_data[registr_name] = new ObjFileData;

	// とりあえず256で
	const int BUFFER = 256;

	char front_str[BUFFER];

	// 分割用の文字列配列
	std::vector<std::string>split_str;

	while (feof(p_file) == 0) {

		// 初期化
		front_str[0] = '\0';

		// 先頭の情報を取得する
		fscanf_s(p_file, "%s ", (char*)front_str, BUFFER);

		// 先頭文字列がコメントならはじく
		if (front_str[0] == '#') {
			continue;
		}
		
		// 頂点関係なら
		else if (front_str[0] == 'v') {

			// 頂点関係読み込み
			VertexInfoLoad(
				p_file,
				front_str,
				vertex_list,
				uv_list,
				normal_list
			);
		}
		
		// 面情報なら
		else if (front_str[0] == 'f' && front_str[1] == '\0') {

			// 面情報読み込み
			FaceInfoLoad(
				p_file,
				registr_name,
				indices,
				mesh_list,
				vertex_list,
				uv_list,
				normal_list
			);
		}
		// マテリアルファイル名読み込み
		else if (strcmp(front_str,"mtllib") == 0) {
		
			fscanf_s(p_file, "%s",&front_str,BUFFER);
			out_material_name = front_str;
		}
		// 使用するマテリアル情報なら要素加算
		else if (strcmp(front_str, "usemtl") == 0) {

			// 使用マテリアル読み込み
			UseMaterialInfoLoad(
				p_file,
				registr_name,
				out_total_material_num,
				indices,
				front_str,
				BUFFER
			);
		}
	}

	// ファイルを閉じる
	fclose(p_file);

	return true;
}


void Obj::VertexFscanfLoad(
	FILE*p_file,
	D3DXVECTOR3 &vec3) {

	// x軸,y軸,z軸を取得
	fscanf_s(
		p_file, "%f %f %f",
		&vec3.x,
		&vec3.y,
		&vec3.z);
}


void Obj::VertexInfoLoad(
	FILE *p_file,
	const char*front_str,
	std::vector<D3DXVECTOR3>&out_vertex_list,
	std::vector<D3DXVECTOR2>&out_uv_list,
	std::vector<D3DXVECTOR3>&out_normal_list
) {

	D3DXVECTOR3 vec3;

	// x軸,y軸,z軸を読み込み
	VertexFscanfLoad(p_file, vec3);

	// 頂点なら
	if (front_str[1] == '\0') {

		// *-1.fで左手系に変換できる
		vec3.x *= -1.f;

		out_vertex_list.push_back(vec3);
	}
	// 法線なら
	else if (front_str[1] == 'n') {

		// 反転
		vec3.x *= -1.f;

		out_normal_list.push_back(vec3);
	}
	// テクスチャ座標なら
	else if (front_str[1] == 't') {

		// uvのみy軸で反転
		vec3.y *= -1.f;

		// vec3をvec2にする
		D3DXVECTOR2 vec2(vec3.x,vec3.y);

		out_uv_list.push_back(vec2);
	}
}


void Obj::FaceInfoLoad(
	FILE*p_file,
	const std::string &register_name,
	std::vector<INT>&indices,
	std::vector<MeshCustomVertex>&mesh_list,
	const std::vector<D3DXVECTOR3>&vertex_list,
	const std::vector<D3DXVECTOR2>&uv_list,
	const std::vector<D3DXVECTOR3>&normal_list
) {

	// とりあえず256 * 8バイト
	const int LOAD_BUFFER = 256;

	// 読み込み文字列バッファ
	char load_str[LOAD_BUFFER] = {};

	// 空白分割文字列バッファ
	std::vector<std::string>space_split_str;

	// 文字列面情報バッファ
	std::vector<std::vector<std::string>>face_info_str;

	// ファイル読み込み
	fgets(
		load_str,
		LOAD_BUFFER,
		p_file
	);

	// 要素追加
	space_split_str.emplace_back();

	// 空白で文字列分割
	space_split_str = Utility::Convert::SplitStr(' ', load_str);

	int face_num = 0;

	for (auto &str : space_split_str) {

		// 面情報文字列
		face_info_str.emplace_back();

		// 面情報文字列
		face_info_str[face_num].emplace_back();

		// /で文字列分割
		face_info_str[face_num] = 
			Utility::Convert::SplitStr('/', str);

		// 面数加算
		face_num++;
	}

	// 仮の入れ物
	std::vector<INT>prov_face;

	// 面情報を代入
	InsertFaceList(
		face_info_str,
		mesh_list,
		indices,
		vertex_list,
		uv_list,
		normal_list,
		m_obj_file_data[register_name]->m_object_sub_set_list
	);

}


void Obj::InsertFaceList(
	const std::vector<std::vector<std::string>>&face_info_str,
	std::vector<MeshCustomVertex>&custom_vertex,
	std::vector<INT>&indices,
	const std::vector<D3DXVECTOR3>&vertex_list,
	const std::vector<D3DXVECTOR2>&uv_list,
	const std::vector<D3DXVECTOR3>&normal_list,
	std::vector<ObjectSubset>&out_draw_sub_set
) {

	// 読み取れない場合
	const int NONE = -1;
	
	// 頂点番号用(-1は情報を受け取れなかった初期状態)
	int vertex_info[3] = 
	{
		NONE,NONE,NONE,
	};

	int subject_vertex = 0;
	std::vector<INT>prov_face;

	// 面情報文字列
	for (unsigned int i = 0;
		i < face_info_str.size();
		i++) {

	
		MeshCustomVertex mesh_vertex;

		// 法線がない場合
		if (face_info_str[i].size() <= 2) {
			subject_vertex = 3 - face_info_str[i].size();
		}

		// 軸分回す
		for (int j = 0; j < 3 - subject_vertex; j++) {


			// 頂点情報に変換する
			Vector3ConversionByString(vertex_info[j],face_info_str[i][j]);

			int id = 0;

			if (vertex_info[j] != 0) {
				id = vertex_info[j] - 1;
			}

			// 面情報へ代入
			switch (j) {

			case VERTEX:
				mesh_vertex.position = vertex_list[id];
				break;

			case UV:

				if (uv_list.size() > 0) {
					mesh_vertex.uv = uv_list[id];
				}
				break;

			case NORMAL:

				if (normal_list.size() > 0) {
					mesh_vertex.normal = normal_list[id];
				}
				break;
			}
		}

		custom_vertex.push_back(mesh_vertex);
		
		prov_face.push_back((int)custom_vertex.size() - 1);
		
	}

	// 頂点数が4以上なら
	if (face_info_str.size() >= 4) {

		// 面を切り分ける
		prov_face = Vertex4CutToVertex3Face(prov_face);

		for (unsigned int i = 0; i < prov_face.size(); i++) {

			// 現在のサブセットリストの面数を加算
			out_draw_sub_set.back().face_count++;

			// インデックスに追加
			indices.push_back(prov_face[i]);

			if (i == 2 || i == 5) {
				// ポリゴンの作成の頂点順番を反転する
				int size = (int)indices.size();
				unsigned int temp = indices[(int)size - 1];
				indices[size - 1] = indices[size - 3];
				indices[size - 3] = temp;
			}
		}
	}
	// 3頂点なら
	else {

		for (unsigned int i = 0; i < prov_face.size(); i++) {

			// 現在のサブセットリストの面数を加算
			out_draw_sub_set.back().face_count++;

			indices.push_back(prov_face[i]);
		}

		// ポリゴンの作成の頂点順番を反転する
		int size = (int)indices.size();
		int temp = indices[size - 1];
		indices[size - 1] = indices[size - 3];
		indices[size - 3] = temp;
	}
}


bool Obj::MaterialFileLoad(
	const std::string &mtl_file_name,
	const std::string &register_name,
	const std::string &file_path_texture
) {

	const int BUFFER = 256;

	char load_str[BUFFER];

	// 文字列分割用
	std::vector<std::string>str_list;

	// テクスチャ文字列読み取り用
	std::string texture_str;

	// カラーの最大値
	const int COLOR_NUM = 3;

	// カラー情報を保存する
	float color_info[COLOR_NUM] = {};

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	std::ifstream ifs(mtl_file_name);

	// ファイル読み込みに失敗した場合
	if (ifs.fail() == true) {
		return false;
	}

	// ファイル読み込み
	while (ifs.getline(load_str, BUFFER - 1))
	{

		// 文字列分割
		str_list = 
			Utility::Convert::SplitStr(' ', load_str);

		// 新しいマテリアル
		if (strcmp(str_list[0].c_str(),"newmtl ") == 0) {

			// テクスチャ名受け取り
			m_obj_file_data[register_name]->
				m_material_data_list[str_list[1].c_str()].texture_name
				= str_list[1].c_str();

			texture_str = str_list[1].c_str();

			// 使わない色を初期化
			D3DCOLORVALUE color = {
				0.f,
				0.f,
				0.f,
				1.0f
			};
			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Emissive = color;
		}
		// アンビエントカラー
		else if (strcmp(str_list[0].c_str(), "Ka ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Ambient = color;
		}
		// ディフューズカラー
		else if (strcmp(str_list[0].c_str(), "Kd ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Diffuse = color;
		}
		// スペキュラーカラー
		else if (strcmp(str_list[0].c_str(), "Ks ") == 0) {

			D3DCOLORVALUE color = {
				(float)atof(str_list[1].c_str()),
				(float)atof(str_list[2].c_str()),
				(float)atof(str_list[3].c_str()),
				1.0f
			};

			// マテリアルに代入
			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Specular = color;
		}
		// α値なら
		else if (strcmp(str_list[0].c_str(), "d ") == 0) {

			// α値設定
			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Ambient.a = (float)atof(str_list[1].c_str());

			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Diffuse.a = (float)atof(str_list[1].c_str());

			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Specular.a = (float)atof(str_list[1].c_str());

		}
		// パワー
		else if (strcmp(str_list[0].c_str(), "Ns ") == 0) {

			m_obj_file_data[register_name]->m_material_data_list[texture_str].
				material.Power = (float)atof(str_list[1].c_str());
		}
		// テクスチャなら
		else if (strcmp(str_list[0].c_str(), "map_Kd ") == 0) {

			// テクスチャ名がないなら
			if (str_list[1].empty() == true) {
				continue;
			}

			std::string texture_name = file_path_texture + str_list[1];

			// テクスチャ名代入
			p_obj_file_data->m_material_data_list[texture_str].texture_name =
				 texture_str;

			// テクスチャ読み込み
			TextureManager::GetInstance()->Load(
				texture_name.c_str(),
				p_obj_file_data->m_material_data_list[texture_str].texture_name.c_str());
		}
	}

	return true;
}


void Obj::UseMaterialInfoLoad(
	FILE*p_file,
	const std::string &register_name,
	int &out_total_material_num,
	std::vector<INT>&indices,
	char*front_str,
	int load_buffer
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	// マテリアル
	{
		// マテリアル数加算
		out_total_material_num++;

		// ファイル読み込み
		fscanf_s(p_file, "%s", front_str, load_buffer);

		// 面情報数受け取り
		p_obj_file_data->m_usemtl_name_list.push_back(front_str);
	}

	// DrawSubSet
	{
		// 要素追加
		p_obj_file_data->m_object_sub_set_list.emplace_back();

		// マテリアル数加算
		p_obj_file_data->m_object_sub_set_list.back().material_index =
			out_total_material_num;

		// 最初から加算していく
		p_obj_file_data->m_object_sub_set_list.back().face_count =
			0;

		// 最初の面を入れる
		p_obj_file_data->m_object_sub_set_list.back().face_start =
			(int)indices.size();
	}

}


bool Obj::Vector3ConversionByString(
	int &axis,
	const std::string &string
) {

	if (string.empty() == true) {
		//Window::TextMessageBox("文字列分割に失敗しました");
		return false;
	}

	// スラッシュなら
	if (string == "/") {
		return false;
	}
	else {
		// 小数点変換
		axis = atoi(string.c_str());
	}

	return true;
}


std::vector<INT> Obj::Vertex4CutToVertex3Face(
	const std::vector<INT>&vertex4_polygon_list
) {

	// 4面の場合は3面にする

	// 面情報受け取り用
	std::vector<INT>get_face_list;

	// 2ポリゴン用意
	const int POLYGON2 = 2;

	// 代入する頂点(4つ分)
	const int ENTRY_VERTEX_NUM[6] = 
	{
		// 1面
		0,1,3,
		// 2面
		3,1,2 
	};

	// 頂点分回す
	for (int j = 0; j < (3 * 2); j++) {

		// 配列要素追加
		get_face_list.emplace_back();

		// 3頂点追加
		get_face_list[j] = vertex4_polygon_list[ENTRY_VERTEX_NUM[j]];
	}

	return get_face_list;
}


// 頂点バッファ生成
void Obj::VertexBufferCreate(
	const int &vertex_num,
	const std::string &register_name,
	const std::vector<MeshCustomVertex>&mesh_vertex_list
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	// 頂点バッファ作成
	mp_graphics->GetDevice()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		(sizeof(MeshCustomVertex) * vertex_num),
		// リソースの使用法
		0,
		// 柔軟な頂点フォーマットの型を指定する
		0,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&p_obj_file_data->mp_vertex_buffer,
		// phandleは現在使用されていない
		NULL
	);

	// 頂点数分用意する
	MeshCustomVertex *custom_vertex_list;

	// ロック
	p_obj_file_data->mp_vertex_buffer->Lock(
		0,
		vertex_num * sizeof(MeshCustomVertex),
		(void**)&custom_vertex_list,
		0
	);

	// ここで頂点情報を代入させる

	int count = 0;

	const int OFFSET = 1;

	for (unsigned int i = 0; i < mesh_vertex_list.size(); i++) {
		custom_vertex_list[i] = mesh_vertex_list[i];
	}

	// アンロック
	p_obj_file_data->mp_vertex_buffer->Unlock();
}


// 面情報から頂点情報埋め込み
bool Obj::IndexBufferCreateFaceBase(
	const int &face_num,
	const std::string &register_name,
	const std::vector<INT>&indices
) {

	// 特定のオブジェクトファイルデータを受け取る
	ObjFileData*p_obj_file_data = m_obj_file_data[register_name];

	// unsigned shortサイズ(2バイト)
	const int TWO_BYTE = 32768 * 2;

	// 面数が2バイトを超えているなら32フォーマットサイズにする
	if (face_num >= TWO_BYTE) {

		// 頂点32bit
		int * index_buffer;

		// 32bitサイズのインデックスバッファを作成
		mp_graphics->CreateIndexBuffer32BitSize(
			&p_obj_file_data->mp_index_buffer,
			(face_num * sizeof(int))
		);

		// ロック
		index_buffer = mp_graphics->LockIndexBuffer32BitSize(
			&p_obj_file_data->mp_index_buffer
		);

		// nullチェック
		if (index_buffer == nullptr) {
			return false;
		}

		// 面数
		for (unsigned int i = 0; i < indices.size(); i++) {
			index_buffer[i] = indices[i];
		}
	}
	// 16bitサイズなら
	else {

		// 頂点16bit
		WORD * index_buffer;

		// 32bitサイズのインデックスバッファを作成
		mp_graphics->CreateIndexBuffer16BitSize(
			&p_obj_file_data->mp_index_buffer,
			(face_num * sizeof(WORD))
		);

		// ロック
		index_buffer = mp_graphics->LockIndexBuffer16BitSize(
			&p_obj_file_data->mp_index_buffer
		);

		// nullチェック
		if (index_buffer == nullptr) {
			return false;
		}

		// 面数
		for (unsigned int i = 0; i < indices.size(); i++) {
			index_buffer[i] = indices[i];
		}
	}

	// nullチェック
	if (p_obj_file_data->mp_index_buffer == nullptr) {
		return false;
	}

	// アンロック
	mp_graphics->UnlockIndexBuffer(
		&p_obj_file_data->mp_index_buffer
	);

	return true;
}
