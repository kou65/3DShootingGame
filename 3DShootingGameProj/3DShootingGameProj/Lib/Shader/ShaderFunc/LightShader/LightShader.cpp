#include"LightShader.h"



LightShader::LightShader() {

}


void LightShader::Update() {

}


void LightShader::Init(
	const VertexDecl::Type&type
) {

	CreateEffectFile(
		"Lib/Shader/EffectFile/DepthShadow.fx",
		"DepthBufferShadowTec",
		type
	);
}


void LightShader::InitMateHandle(
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


void LightShader::InitCameraHandle(
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


void LightShader::InitLightDirHandle(
	const std::string&mt_light_dir,
	const std::string&mt_light_color
) {

	m_h_light_dir =
		mp_effect->GetParameterByName(NULL, mt_light_dir.c_str());

	m_h_light_color =
		mp_effect->GetParameterByName(NULL, mt_light_color.c_str());
}


void LightShader::InitPointLightHandle(
	const std::string&mt_ambient,
	const std::string&mt_diffuse,
	const std::string&mt_specular
) {

	m_h_pl_ambient = 
		mp_effect->GetParameterByName(NULL, mt_ambient.c_str());

	m_h_pl_diffuse =
		mp_effect->GetParameterByName(NULL, mt_diffuse.c_str());

	m_h_pl_specular =
		mp_effect->GetParameterByName(NULL, mt_specular.c_str());
}


void LightShader::InitAttenuate(
	const std::string&attenuate_name
) {
	
	m_h_attenuate =
		mp_effect->GetParameterByName(NULL, attenuate_name.c_str());
}
