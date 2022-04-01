//**************************************************
//
// Hackathon ( player.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h" 
#include "fade.h"
#include "input.h"
#include "select.h"
#include "mode.h"
#include "rectangle.h"
#include "light.h"
#include "color.h"
#include "sound.h"
#include "texture.h"
#include "utility.h"
#include "game.h"
#include "effect.h"
#include "player.h"
#include "answer.h"
#include "time.h"
#include "bg.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_TIME = 5;			// タイムの最大値
const int	MAX_LIGHT = 16;			// ライトの最大数
const float	LIGHT_SIZE = 50.0f;		// ライトのサイズ
const float	PUSH_WIDTH = 120.0f;	// 押せの幅
const float	PUSH_HEIGHT = 80.0f;	// 押せの高さ

typedef enum
{
	LIGHT_COLOR_RED = 0,	// 赤
	LIGHT_COLOR_GREEN,		// 緑
	LIGHT_COLOR_BLUE,		// 青
	LIGHT_COLOR_YELLOW,		// 黄色
	LIGHT_COLOR_MAX,
}LIGHT_COLOR;
}// namespaceはここまで

 //==================================================
 // スタティック変数
 //==================================================
namespace
{
COLOR	s_aColor[LIGHT_COLOR_MAX];		// ライトの色
int		s_nPlayer;						// ライトの現在の数
int		s_nIdxPush;						// 押せの矩形のインデックス
int		s_nIdxSelect;					// メニューの配列のインデックス
int		s_nIdxFrame;					// 枠の配列のインデックス
int		s_nIdxColor[MAX_LIGHT];			// 色の番号
TEXTURE	s_aTexture[LIGHT_COLOR_MAX];	// テクスチャ
}// namespaceはここまで

 //==================================================
 // スタティック関数プロトタイプ宣言
 //==================================================
namespace
{
void ResetDrawPlayer(void);
}// namespaceはここまで

 //--------------------------------------------------
 // 初期化
 //--------------------------------------------------
void InitPlayer(void)
{
	s_aColor[LIGHT_COLOR_RED] = COLOR_RED;
	s_aColor[LIGHT_COLOR_GREEN] = COLOR_GREEN;
	s_aColor[LIGHT_COLOR_BLUE] = COLOR_BLUE;
	s_aColor[LIGHT_COLOR_YELLOW] = COLOR_YELLOW;
	s_aTexture[LIGHT_COLOR_RED] = TEXTURE_Red;
	s_aTexture[LIGHT_COLOR_GREEN] = TEXTURE_Green;
	s_aTexture[LIGHT_COLOR_BLUE] = TEXTURE_Blue;
	s_aTexture[LIGHT_COLOR_YELLOW] = TEXTURE_Yellow;

	s_nPlayer = 0;

	{// 押せ
		// 矩形の設定
		s_nIdxPush = SetRectangle(TEXTURE_Push);

		D3DXVECTOR3 size = D3DXVECTOR3(PUSH_WIDTH, PUSH_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(PUSH_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxPush, pos, size);

		// 矩形の色の設定
		SetColorRectangle(s_nIdxPush, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}

	{// 枠
		SelectArgument select;
		select.nNumUse = MAX_LIGHT;
		select.fLeft = PUSH_WIDTH;
		select.fRight = SCREEN_WIDTH;
		select.fTop = SCREEN_HEIGHT * 0.5f;
		select.fBottom = SCREEN_HEIGHT * 0.5f;
		select.fWidth = LIGHT_SIZE;
		select.fHeight = LIGHT_SIZE;
		select.bSort = false;

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			select.texture[i] = TEXTURE_Hackathon_Ring;
		}

		// セレクトの設定
		s_nIdxFrame = SetSelect(select);
	}

	{// メニュー
		SelectArgument select;
		select.nNumUse = MAX_LIGHT;
		select.fLeft = PUSH_WIDTH;
		select.fRight = SCREEN_WIDTH;
		select.fTop = SCREEN_HEIGHT * 0.5f;
		select.fBottom = SCREEN_HEIGHT * 0.5f;
		select.fWidth = LIGHT_SIZE;
		select.fHeight = LIGHT_SIZE;
		select.bSort = false;

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			select.texture[i] = TEXTURE_effect_000;
		}

		// セレクトの設定
		s_nIdxSelect = SetSelect(select);
	}

	// 描画のリセット
	ResetDrawPlayer();

	// タイムの設定
	SetTime(MAX_TIME);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPlayer(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxPush);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePlayer(void)
{
	if (GetFade() != FADE_NONE)
	{
		return;
	}

	switch (GetGameState())
	{
	case GAMESTATE_RESET:	// リセット状態
		// 描画のリセット
		ResetDrawPlayer();

		// ゲーム状態の設定
		SetGameState(GAMESTATE_SAMPLE);
		break;

	case GAMESTATE_PLAYER:	// プレイヤー状態

		if (GetAnswer())
		{// 最大値
			return;
		}

		if (GetLightKeyTrigger(LIGHT_KEY_RED))
		{// 赤
			PlaySound(SOUND_LABEL_SE_SELECT);
			s_nIdxColor[s_nPlayer] = LIGHT_COLOR_RED;

			// セレクトのテクスチャの設定
			ChangeTextuteSelect(s_nIdxSelect, s_nPlayer, s_aTexture[s_nIdxColor[s_nPlayer]]);

			// セレクトの描画するかどうか
			SetDrawSelect(s_nIdxSelect, s_nPlayer, true);

			// ライトの描画設定
			SetDrawLight(s_nPlayer);

			// 答え合わせ
			SetAnswer(s_nPlayer);

			// タイムの設定
			SetTime(MAX_TIME);

			// 矩形の色の設定
			SetColorRectangle(GetIdxBG(0), GetColor(COLOR_RED));

			s_nPlayer++;

			// 枠の設定
			SetFramePlayer(s_nPlayer);

			// エフェクトの設定
			SetEffect(GetPosSelect(s_nIdxSelect, s_nPlayer - 1), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[s_nPlayer - 1]]));
		}
		else if (GetLightKeyTrigger(LIGHT_KEY_GREEN))
		{// 緑
			PlaySound(SOUND_LABEL_SE_SELECT);
			s_nIdxColor[s_nPlayer] = LIGHT_COLOR_GREEN;

			// セレクトのテクスチャの設定
			ChangeTextuteSelect(s_nIdxSelect, s_nPlayer, s_aTexture[s_nIdxColor[s_nPlayer]]);

			// セレクトの描画するかどうか
			SetDrawSelect(s_nIdxSelect, s_nPlayer, true);

			// ライトの描画設定
			SetDrawLight(s_nPlayer);

			// 答え合わせ
			SetAnswer(s_nPlayer);

			// タイムの設定
			SetTime(MAX_TIME);

			// 矩形の色の設定
			SetColorRectangle(GetIdxBG(0), GetColor(COLOR_GREEN));

			s_nPlayer++;

			// 枠の設定
			SetFramePlayer(s_nPlayer);

			// エフェクトの設定
			SetEffect(GetPosSelect(s_nIdxSelect, s_nPlayer - 1), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[s_nPlayer - 1]]));
		}
		else if (GetLightKeyTrigger(LIGHT_KEY_BLUE))
		{// 青
			PlaySound(SOUND_LABEL_SE_SELECT);
			s_nIdxColor[s_nPlayer] = LIGHT_COLOR_BLUE;

			// セレクトのテクスチャの設定
			ChangeTextuteSelect(s_nIdxSelect, s_nPlayer, s_aTexture[s_nIdxColor[s_nPlayer]]);

			// セレクトの描画するかどうか
			SetDrawSelect(s_nIdxSelect, s_nPlayer, true);

			// ライトの描画設定
			SetDrawLight(s_nPlayer);

			// 答え合わせ
			SetAnswer(s_nPlayer);

			// タイムの設定
			SetTime(MAX_TIME);

			// 矩形の色の設定
			SetColorRectangle(GetIdxBG(0), GetColor(COLOR_BLUE));

			s_nPlayer++;

			// 枠の設定
			SetFramePlayer(s_nPlayer);

			// エフェクトの設定
			SetEffect(GetPosSelect(s_nIdxSelect, s_nPlayer - 1), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[s_nPlayer - 1]]));
		}
		else if (GetLightKeyTrigger(LIGHT_KEY_YELLOW))
		{// 黄色
			PlaySound(SOUND_LABEL_SE_SELECT);
			s_nIdxColor[s_nPlayer] = LIGHT_COLOR_YELLOW;

			// セレクトのテクスチャの設定
			ChangeTextuteSelect(s_nIdxSelect, s_nPlayer, s_aTexture[s_nIdxColor[s_nPlayer]]);

			// セレクトの描画するかどうか
			SetDrawSelect(s_nIdxSelect, s_nPlayer, true);

			// ライトの描画設定
			SetDrawLight(s_nPlayer);
		
			// 答え合わせ
			SetAnswer(s_nPlayer);

			// タイムの設定
			SetTime(MAX_TIME);

			// 矩形の色の設定
			SetColorRectangle(GetIdxBG(0), GetColor(COLOR_YELLOW));

			s_nPlayer++;

			// 枠の設定
			SetFramePlayer(s_nPlayer);

			// エフェクトの設定
			SetEffect(GetPosSelect(s_nIdxSelect, s_nPlayer - 1), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[s_nPlayer - 1]]));
		}
		break;

	case GAMESTATE_SAMPLE:	// 見本状態
	case GAMESTATE_NONE:	// 何もしていない状態
	case GAMESTATE_START:	// 開始状態
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPlayer(void)
{
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetPlayer(void)
{
	return s_nPlayer;
}

//--------------------------------------------------
// 色の取得
//--------------------------------------------------
int GetColorPlayer(int nNowLight)
{
	assert(nNowLight >= 0 && nNowLight <= MAX_LIGHT);

	return s_nIdxColor[nNowLight];
}

//--------------------------------------------------
// 枠の設定
//--------------------------------------------------
void SetFramePlayer(int nNowLight)
{
	assert(nNowLight >= 0 && nNowLight <= MAX_LIGHT);

	if (GetAnswer())
	{// 最大値
		return;
	}

	// セレクトの描画するかどうか
	SetDrawSelect(s_nIdxFrame, nNowLight, true);

	// タイムの位置の設定
	SetPosTime(GetPosSelect(s_nIdxSelect, nNowLight));
}

//--------------------------------------------------
// 押せの色の設定
//--------------------------------------------------
void SetColorPushPlayer(D3DXCOLOR col)
{
	// 矩形の色の設定
	SetColorRectangle(s_nIdxPush, col);
}

namespace
{
//--------------------------------------------------
// 描画のリセット
//--------------------------------------------------
void ResetDrawPlayer(void)
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxSelect, i, false);
	}

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxFrame, i, false);
	}

	s_nPlayer = 0;
}
}// namespaceはここまで