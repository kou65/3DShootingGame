﻿#include"ObjectManager.h"
#include"../../Player/PlayerManager.h"
#include"../../Enemy/Enemy/EnemyManager.h"
#include"../../Player/Player.h"
#include"../../Map/Map/Map.h"
#include"../../GameUI/GameUI.h"
#include"../../Map/MapManager/MapManager.h"
#include"../../Rope/Rope.h"
#include"../Object/Object.h"
#include"../../CollisionObject/CollisionManager.h"
#include<algorithm>
#include<iostream>
#include"../../ScrollManager/ScrollManager.h"
#include"../../Lib/Input/KeyBord.h"
#include"../../GameInput/GameInput.h"



ObjectManager::ObjectManager() : 
	m_current_the_newest_id(0),
	m_is_pause(false)
{

	// 各コンストラクタに渡す為、一時的な参照に入れる
	m_p_player_manager = new PlayerManager(this);

	// オブジェクト生成
	{
		// ロープ生成
		m_p_rope = new Rope(m_p_player_manager);

		// スタミナGameUI生成
		m_p_ui = new GameUI(m_p_player_manager);

		// オブジェクト登録
		Entry(m_p_rope);
		Entry(m_p_ui);
	}

	// 管理者生成
	EnemyManager*p_enemy_manager = new EnemyManager(this,m_p_player_manager);
	MapManager * p_map_manager = new MapManager(p_enemy_manager, this);

	m_p_scroll_manager = new ScrollManager(m_p_player_manager, p_map_manager);


	// 管理者登録
	{
		// 敵管理登録
		m_p_manager_list.emplace_back(p_enemy_manager);

		// プレイヤー管理登録
		m_p_manager_list.emplace_back(m_p_player_manager);

		// マップ管理登録
		m_p_manager_list.emplace_back(p_map_manager);

		// スクロール管理者生成
		m_p_manager_list.emplace_back(m_p_scroll_manager);

		// 当たり判定管理生成
		m_p_manager_list.emplace_back(new CollisionManager(m_p_player_manager, p_enemy_manager, p_map_manager));

	}	
}


ObjectManager::~ObjectManager() {

	// オブジェクトリスト削除
	for (auto&obj : m_p_object_list) {
		delete &obj;
	}

	// 描画用オブジェクトリスト要素削除
	m_draw_object_list.clear();
}


void ObjectManager::Update() {

	// ポーズに移行するかどうか
	if (IsMoveToPause() == true) {
		return;
	}

	// オブジェクトの更新
	for (auto&itr : m_p_object_list) {

		// 更新
		itr.second->Update();
	}

	// 管理者の更新
	for (auto&manager : m_p_manager_list) {
		manager->Update();
	}


	// 描画用オブジェクトをソートする
	EntryAndSortDrawObject();
}


void ObjectManager::Draw() {


	// 描画用オブジェクト描画
	for (auto &obj : m_draw_object_list) {
		(*obj).Draw();
	}

	// ポーズ描画するかどうか
	PauseDraw();
}


void ObjectManager::EntryAndSortDrawObject(){

	// 前のリストを削除
	InitDrawObjectList();
	
	// 要素を全て入れる
	for (auto draw_object = m_p_object_list.begin(); draw_object != m_p_object_list.end();++draw_object) {

		m_draw_object_list.push_back(draw_object->second);
	}

	// 昇順ソートを行う
	std::sort(m_draw_object_list.begin(), m_draw_object_list.end(),
		[](const Object*obj1, const Object*obj2) {
		return obj1->GetSortNum() < obj2->GetSortNum();
	});
}


void ObjectManager::Entry(Object*obj) {
	// nullなら戻す
	if (obj == nullptr) {
		return;
	}

	// 生成専用id
	unsigned int create_id = 0;

	create_id = m_current_the_newest_id;

	m_current_the_newest_id++;

	// Objectの要素を追加
	m_p_object_list[create_id] = obj;

	// Objectの最新のidをセット
	m_p_object_list.at(create_id)->SetId(create_id);
}


void ObjectManager::InitDrawObjectList() {

	// 前のを削除
	std::vector<Object*>().swap(m_draw_object_list);
	m_draw_object_list.clear();
}


void ObjectManager::Exit(unsigned int id) {

	// Objectの配列番号の要素を削除
	m_p_object_list.erase(id);
}


void ObjectManager::Exit(Object*object) {
	
	// Objectの配列番号の要素を削除
	m_p_object_list.erase(object->GetId());
}


bool ObjectManager::IsClear()const{

	const float CLEAR_IINE = 200.f;

	// マップの背景とチップが最大で、かつ自機の位置が200.fよりも少ない(上)のとき
	if (m_p_scroll_manager->IsScrollMax() == true && m_p_player_manager->GetPosRelay(Player::STAR_1).y <= CLEAR_IINE || 
		m_p_scroll_manager->IsScrollMax() == true && m_p_player_manager->GetPosRelay(Player::STAR_2).y <= CLEAR_IINE) { 

		return true;
	}
		return false;
}


bool ObjectManager::IsGameOver()const {

	if (m_p_player_manager->IsActiveRelay(Player::STAR_1) == false &&
		m_p_player_manager->IsActiveRelay(Player::STAR_2) == false) {
		return true;
	}
	return false;
}


bool ObjectManager::IsMoveToPause() {

	Keybord& keybord = Keybord::getInterface();
	GameInput game_input;
	//game_input.Update();

	// キーが離されたら
	if (game_input.InputCommand(game_input.START_BUTTON) == true) {

		m_is_pause = !m_is_pause;
		return m_is_pause;
	}

	return m_is_pause;
}


void ObjectManager::PauseDraw() {

	const float OFSET_POS = 0.5f;
	const float SCALE = 1.f;

	if (m_is_pause == true) {

		// 描画
		Texture::Draw2D(
			"Resource/Texture/UI/pause.png",
			Window::WIDTH / 2,
			(Window::HEIGHT / 2) - 50,
			SCALE,
			SCALE,
			0.f,
			OFSET_POS,
			OFSET_POS
		);
	}
}