#include"ColorShader.h"




ColorShader::ColorShader() : StandardTSShader(){

}


void ColorShader::InitColorShader(
	const std::string&color_handle_name,
	const std::string&texture_handle_name
) {

	// ワールドハンドル取得
	m_h_color = mp_effect->
		GetParameterByName(
			NULL, color_handle_name.c_str());

	// ワールドハンドル取得
	m_h_texture = mp_effect->
		GetParameterByName(
			NULL, texture_handle_name.c_str());
}


void ColorShader::SetColor(const D3DXVECTOR4&color) {
	m_color = color;
}


void ColorShader::SetTexture(IDirect3DTexture9*p_tex) {
	mp_texture = p_tex;
}