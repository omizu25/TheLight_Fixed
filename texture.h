//**************************************************
// 
// Hackathon ( texture.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// 列挙型
//==================================================
typedef enum
{
	TEXTURE_Cursor_Right = 0,	// カーソル
	TEXTURE_TITLE_LOGO,			// タイトルロゴ
	TEXTURE_TITLE_UI,			// 灯す
	TEXTURE_TITLE_GamePopup,	// チュートリアル
	TEXTURE_BackToTitle,		// ゲームに戻る
	TEXTURE_ReturnToGame,		// タイトルに戻る
	TEXTURE_Hackathon_Ring,		// ライトの枠
	TEXTURE_BG_0,				// 星
	TEXTURE_BG_1,				// 家
	TEXTURE_BG_2,				// 背景
	TEXTURE_BG_MOON,			// 月
	TEXTURE_end,				// やめる
	TEXTURE_play,				// 遊ぶ
	TEXTURE_Learn,				// 覚えろ
	TEXTURE_Push,				// 押せ
	TEXTURE_Red,				// 赤
	TEXTURE_Green,				// 緑
	TEXTURE_Blue,				// 青
	TEXTURE_Yellow,				// 黄色
	TEXTURE_effect_000,			// エフェクト
	TEXTURE_effect_001,			// エフェクト
	TEXTURE_effect_002,			// エフェクト
	TEXTURE_Number_Neon,		// ナンバー画像(ネオン)
	TEXTURE_Method,				// 操作説明
	TEXTURE_YourScore,			// 自己スコア
	TEXTURE_BestScore,			// ベストスコア
	TEXTURE_MAX,
	TEXTURE_NONE,				// 使用しない
}TEXTURE;

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadTexture(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTexture(void);

//--------------------------------------------------
// 取得
// 引数  : TEXTURE texture / 列挙型 種類
// 返値  : LPDIRECT3DTEXTURE9 / テクスチャ
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture);

#endif // !_TEXTURE_H_
