#pragma once
#include"../Window/Window.h"



struct Vec2 :public D3DXVECTOR2{

	Vec2(
		const float &x,
		const float &y
	) {
		this->x = x;
		this->y = y;
	}

	Vec2() {
		Initialization();
	};

	// 拡張機能
	void operator =(D3DXVECTOR2 vec2) {
		this->x = vec2.x;
		this->y = vec2.y;
	}

	D3DXVECTOR2 operator +(const D3DXVECTOR2 &vec2) {
		D3DXVECTOR2 d_vec2;
		d_vec2.x = this->x + vec2.x;
		d_vec2.y = this->y + vec2.y;
		return vec2;
	}
	
	D3DXVECTOR2 operator -(const D3DXVECTOR2 &vec2) {
		D3DXVECTOR2 d_vec2;
		d_vec2.x = this->x - vec2.x;
		d_vec2.y = this->y - vec2.y;
		return vec2;
	}
	
	D3DXVECTOR2 operator *(const D3DXVECTOR2 &vec2) {
		D3DXVECTOR2 d_vec2;
		d_vec2.x = this->x * vec2.x;
		d_vec2.y = this->y * vec2.y;
		return vec2;
	}
	
	D3DXVECTOR2 operator /(const D3DXVECTOR2 &vec2) {
		D3DXVECTOR2 d_vec2;
		d_vec2.x = this->x / vec2.x;
		d_vec2.y = this->y / vec2.y;
	
		return vec2;
	}

	void operator +=(const D3DXVECTOR2 &vec2) {
		this->x += vec2.x;
		this->y += vec2.y;
	}

	void operator *=(const D3DXVECTOR2 &vec2) {
		this->x = vec2.x;
		this->y = vec2.y;
	}

	void operator -=(const D3DXVECTOR2 &vec2) {
		this->x = vec2.x;
		this->y = vec2.y;
	}

	void operator /=(const D3DXVECTOR2 &vec2) {
		this->x = vec2.x;
		this->y = vec2.y;
	}

	void Initialization() {
		x = 0.f;
		y = 0.f;
	}
};