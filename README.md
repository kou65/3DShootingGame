# 3DShootingGame
3DShootingGameMakeRepository



目次 :

 1 : コーディング規約
 2 : クラスフォーマット
 3 : コメントフォーマット



1 : コーディング規約{

文法規約 :

 変数 小文字スネークケース

 関数 先頭大文字アッパーキャメル

 定数 大文字スネークケース

 構造体　先頭大文字アッパーキャメル

 クラス　先頭大文字アッパーキャメル

 列挙型 先頭大文字アッパーキャメル

 列挙子 大文字スネークケース

 ポインタ変数　p_

 メンバ変数 m_

 メンバポインタ変数 mp_

 配列 _list

 メンバ関数 関数と同じ

 メンバ定数 定数と同じ

 静的メンバ変数 変数と同じ

 静的メンバ関数 関数と同じ 


例 :

 変数 player_hp;

 関数 void DrawStart();

 定数　PLAYER_HP;

 構造体 struct GameData{};

 クラス class Player{};

 列挙型 enum class ObjectType{
           PLAYER,// 列挙子
        };

 ポインタ変数 p_player;

 メンバ変数 m_player;

 メンバポインタ mp_player;

 配列 object_list;

 メンバ関数 DrawStart();

 メンバ定数 PLAYER_HP;

 静的メンバ変数 変数と同じ

 静的メンバ関数 関数と同じ


その他規約:

 virtualでオーバーライドした関数にoverrideと付ける

 アプリケーションの基底クラスはBaseを付ける

 管理者クラスは通常Managerと名付ける

 生成者クラスは通常factoryと名付ける

傾向:

 列挙体 typeやstateが主流(あくまで主流なのでその限りではない)

}



2 : クラス基本フォーマット{


 class Format{
 public:

 // メンバ定数

 public:

 // 仮想関数(オーバーライド)
 // メンバ関数

 protected:

 // メンバ定数

 protected:

 // メンバ関数

 private:

 // メンバ定数

 private:

 // メンバ関数

 priavte:

 // メンバ変数

 };

 注意事項 : 
 シングルトンの場合は変わる場合もある。

 ポリモーフィズム基底クラスなら仮想デストラクタを必ず記載、
 virtual ~ObjectBase(){};
 ポリモーフィズムを使用しないならば派生クラスを作っても仮想デストラクタは作らない

}


3 : コメントについて{

 コメントの種類 : doxygenを使用

 doxygenのホームページ(引用)↓
 URL : http://www.doxygen.jp/

 doxygenでc,c++書き方メモ(引用)↓
 URL : https://qiita.com/inabe49/items/23e615649e8539d857a8

}
