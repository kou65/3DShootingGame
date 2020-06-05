
#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include"../../Lib/Graphics/Graphics.h"
#include"../../GameApp/GameConstant/GameConstant.h"



/**
* @brief 基底オブジェクトクラス
*/
class ObjectBase {
public:

	/**
	* @brief 仮想デストラクタ
	*/
	virtual ~ObjectBase(){};


	/**
	* @brief コンストラクタ
	*/
	ObjectBase();


	/**
	* @brief 更新用 純粋仮想関数
	*/
	virtual void Update() = 0;


	/**
	* @brief 描画 純粋仮想関数
	*/
	virtual void Draw() = 0;


	/**
	* @brief zテクスチャを書き込む用の仮想関数
	*/
	virtual void DrawZTexture() {};


	/**
	* @brief 活動しているかを返す
	* @return bool
	*/
	bool IsActive();

	
	/**
	* @brief 描画できるかを返す
	* @return bool
	*/
	bool CanDraw();


protected:

	/**
	* @brief オブジェクト削除関数,基底先で呼び出す
	*/
	void Exit();

protected:

	//! 描画できるかどうか
	bool m_can_draw;

	//! 活動しているかどうか
	bool m_is_active;
};

#endif