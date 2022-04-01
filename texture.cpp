//**************************************************
// 
// Hackathon ( texture.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定数
//==================================================
namespace
{
const char *s_FileName[] =
{// テクスチャのパス	
	"data/TEXTURE/Cursor_Right.png",				// カーソル
	"data/TEXTURE/Title.png",						// タイトルロゴ
	"data/TEXTURE/Hackathon_TitleUI.png",			// 灯す
	"data/TEXTURE/Hackathon_GamePopup.png",			// チュートリアル
	"data/TEXTURE/Hackathon_ReturnToGame.png",		// ゲームに戻る
	"data/TEXTURE/Hackathon_BackToTitle.png",		// タイトルに戻る
	"data/TEXTURE/Hackathon_Ring.png",				// ライトの枠
	"data/TEXTURE/Bg000.png",						// 星
	"data/TEXTURE/Bg001.png",						// 家
	"data/TEXTURE/Bg002.png",						// 背景
	"data/TEXTURE/BgMoon.png",						// 月
	"data/TEXTURE/end.png",							// やめる
	"data/TEXTURE/play.png",						// 遊ぶ
	"data/TEXTURE/Learn.png",						// 覚えろ
	"data/TEXTURE/Push.png",						// 押せ
	"data/TEXTURE/Hackathon_Direction_Red.png",		// 赤
	"data/TEXTURE/Hackathon_Direction_Green.png",	// 緑
	"data/TEXTURE/Hackathon_Direction_Blue.png",	// 青
	"data/TEXTURE/Hackathon_Direction_Yellow.png",	// 黄色
	"data/TEXTURE/effect/effect_000.png",			// エフェクト
	"data/TEXTURE/effect/effect_001.jpg",			// エフェクト
	"data/TEXTURE/effect/effect_003.jpg",			// エフェクト
	"data/TEXTURE/Number_Neon.png",					// ナンバー画像(ネオン)
	"data/TEXTURE/Method_of_operation_white.png",	// 操作説明
	"data/TEXTURE/Hackathon_YourScore.png",			// 自己スコア
	"data/TEXTURE/Hackathon_BestScore.png",			// ベストスコア
};
}// namespaceはここまで

static_assert(sizeof(s_FileName) / sizeof(s_FileName[0]) == TEXTURE_MAX, "aho");

//==================================================
// スタティック変数
//==================================================
namespace
{
LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];
}// namespaceはここまで

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadTexture(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTexture(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture)
{
	if (texture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return NULL;
	}

	assert(texture >= 0 && texture < TEXTURE_MAX);

	return s_pTexture[texture];
}
