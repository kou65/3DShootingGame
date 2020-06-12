#include"LightShader.h"



Light::Light() {

}


void Light::UpdateLight() {

	// データが送れていない

	// 全体の環境光
	mp_effect->SetVector(
		m_h_ambient,
		&m_light_data.ambient
	);

	// マテリアル
	mp_effect->SetVector(
		m_h_mt_ambient,
		&m_light_data.material.ambient
	);
	mp_effect->SetVector(
		m_h_mt_diffuse,
		&m_light_data.material.diffuse
	);
	mp_effect->SetVector(
		m_h_mt_specular,
		&m_light_data.material.specular
	);

	// 点光源(アンビエントは使わない)
	mp_effect->SetVector(
		m_h_pl_diffuse,
		&m_light_data.point_light.diffuse
	);
	mp_effect->SetVector(
		m_h_pl_specular,
		&m_light_data.point_light.specular
	);
	mp_effect->SetVector(
		m_h_pl_pos,
		&m_light_data.point_light.pos
	);
	// 減衰
	mp_effect->SetVector(
		m_h_attenuate,
		&m_light_data.point_light.attenuation
	);

	// カメラ
	mp_effect->SetVector(
		m_h_eye_pos,
		&m_light_data.eye_pos
	);
	mp_effect->SetVector(
		m_h_eye_dir,
		&m_light_data.eye_direction
	);

	// その他ライト関係
	mp_effect->SetVector(
		m_h_light_color,
		&m_light_data.light_color
	);

	mp_effect->SetVector(
		m_h_light_dir,
		&m_light_data.direction
	);

}


void Light::InitMateHandle(
	const std::string&mt_ambient_name,
	const std::string&mt_specular_name,
	const std::string&mt_diffuse_name,
	const std::string&ambient_name
) {

	// ハンドルセット
	m_h_mt_ambient = mp_effect->
		GetParameterByName(NULL,mt_ambient_name.c_str());

	m_h_mt_specular = mp_effect->
		GetParameterByName(NULL, mt_specular_name.c_str());

	m_h_mt_diffuse = mp_effect->
		GetParameterByName(NULL, mt_diffuse_name.c_str());

	m_h_ambient = mp_effect->
		GetParameterByName(NULL, ambient_name.c_str());
}


void Light::InitCameraHandle(
	const std::string&mt_eye_pos,
	const std::string&mt_eye_dir
) {

	// 視点位置
	m_h_eye_pos = mp_effect->
		GetParameterByName(NULL, mt_eye_pos.c_str());

	// 視点
	m_h_eye_dir = mp_effect->
		GetParameterByName(NULL, mt_eye_dir.c_str());
}


void Light::InitLightDirHandle(
	const std::string&mt_light_dir,
	const std::string&mt_light_color
) {

	m_h_light_dir =
		mp_effect->GetParameterByName(NULL, mt_light_dir.c_str());

	m_h_light_color =
		mp_effect->GetParameterByName(NULL, mt_light_color.c_str());
}


void Light::InitPointLightHandle(
	const std::string&pos,
	const std::string&mt_diffuse,
	const std::string&mt_specular,
	const std::string&attenuate_name
) {

	m_h_pl_pos = 
		mp_effect->GetParameterByName(NULL, pos.c_str());

	m_h_pl_diffuse =
		mp_effect->GetParameterByName(NULL, mt_diffuse.c_str());

	m_h_pl_specular =
		mp_effect->GetParameterByName(NULL, mt_specular.c_str());

	m_h_attenuate =
		mp_effect->GetParameterByName(NULL, attenuate_name.c_str());
}


void Light::SetLightData(const LightData&light_data) {

	m_light_data = light_data;
}