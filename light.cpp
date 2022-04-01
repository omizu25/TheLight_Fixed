//**************************************************
//
// Hackathon ( light.cpp )
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
#include "time.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_LIGHT = 16;			// ライトの最大数
const int	MAX_TIME = 90;			// タイムの最大値
const int	REPEAT_TIME = 30;		// タイムの繰り返し
const float	LIGHT_SIZE = 50.0f;		// ライトのサイズ
const float	LEARN_WIDTH = 120.0f;	// 覚えろの幅
const float	LEARN_HEIGHT = 80.0f;	// 覚えろの高さ

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
int		s_nNowLight;					// ライトの現在数
int		s_nMaxLight;					// ライトの最大数
int		s_nIdxLearn;					// 覚えろの矩形のインデックス
int		s_nIdxSelect;					// メニューの配列のインデックス
int		s_nIdxFrame;					// 枠の配列のインデックス
int		s_nTime;						// タイム
int		s_nIdxColor[MAX_LIGHT];			// 色の番号
bool	s_bMax;							// 増え切った
COLOR	s_aColor[LIGHT_COLOR_MAX];		// ライトの色
TEXTURE	s_aTexture[LIGHT_COLOR_MAX];	// テクスチャ
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void ResetDrawLight(void);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLight(void)
{
	s_aColor[LIGHT_COLOR_RED] = COLOR_RED;
	s_aColor[LIGHT_COLOR_GREEN] = COLOR_GREEN;
	s_aColor[LIGHT_COLOR_BLUE] = COLOR_BLUE;
	s_aColor[LIGHT_COLOR_YELLOW] = COLOR_YELLOW;
	s_aTexture[LIGHT_COLOR_RED] = TEXTURE_Red;
	s_aTexture[LIGHT_COLOR_GREEN] = TEXTURE_Green;
	s_aTexture[LIGHT_COLOR_BLUE] = TEXTURE_Blue;
	s_aTexture[LIGHT_COLOR_YELLOW] = TEXTURE_Yellow;

	s_nNowLight = 0;
	s_nMaxLight = 7;

	{// 覚えろ
		// 矩形の設定
		s_nIdxLearn = SetRectangle(TEXTURE_Learn);

		D3DXVECTOR3 size = D3DXVECTOR3(LEARN_WIDTH, LEARN_HEIGHT, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(LEARN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.35f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxLearn, pos, size);
	}

	{// 枠
		SelectArgument select;
		select.nNumUse = MAX_LIGHT;
		select.fLeft = LEARN_WIDTH;
		select.fRight = SCREEN_WIDTH;
		select.fTop = SCREEN_HEIGHT * 0.35f;
		select.fBottom = SCREEN_HEIGHT * 0.35f;
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

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxFrame, i, false);
	}

	{// メニュー
		SelectArgument select;
		select.nNumUse = MAX_LIGHT;
		select.fLeft = LEARN_WIDTH;
		select.fRight = SCREEN_WIDTH;
		select.fTop = SCREEN_HEIGHT * 0.35f;
		select.fBottom = SCREEN_HEIGHT * 0.35f;
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

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		s_nIdxColor[i] = IntRandam(LIGHT_COLOR_MAX, 0);

		// セレクトのテクスチャの設定
		ChangeTextuteSelect(s_nIdxSelect, i, s_aTexture[s_nIdxColor[i]]);
	}

	// 応急処置
	s_nIdxColor[0] = IntRandam(LIGHT_COLOR_MAX, 0);

	// セレクトのテクスチャの設定
	ChangeTextuteSelect(s_nIdxSelect, 0, s_aTexture[s_nIdxColor[0]]);

	// 描画のリセット
	ResetDrawLight();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLight(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxLearn);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateLight(void)
{
	switch (GetGameState())
	{
	case GAMESTATE_SAMPLE:	// 見本状態
		s_nTime++;

		if (s_nTime % REPEAT_TIME != 0)
		{
			return;
		}

		if (s_nNowLight < s_nMaxLight)
		{
			s_bMax = false;
			s_nNowLight++;
			
			// エフェクトの設定
			SetEffect(GetPosSelect(s_nIdxSelect, s_nNowLight - 1), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[s_nNowLight - 1]]));
			
			// 描画のリセット
			ResetDrawLight();
		}
		else
		{// 増え切った
			if (s_bMax)
			{
				if (s_nTime >= MAX_TIME)
				{
					for (int i = 0; i < MAX_LIGHT; i++)
					{
						// セレクトの描画するかどうか
						SetDrawSelect(s_nIdxSelect, i, false);
					}

					for (int i = 0; i < s_nNowLight; i++)
					{
						// セレクトの描画するかどうか
						SetDrawSelect(s_nIdxFrame, i, true);
					}

					// ゲーム状態の設定
					SetGameState(GAMESTATE_PLAYER);

					// 矩形の色の設定
					SetColorRectangle(s_nIdxLearn, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

					// 押せの色の設定
					SetColorPushPlayer(GetColor(COLOR_WHITE));
					
					// 枠の設定
					SetFramePlayer(0);
				}
			}
			else
			{
				s_nTime = 0;
				s_bMax = true;
			}
		}
		break;

	case GAMESTATE_RESET:	// リセット状態
		s_nNowLight = 0;
		s_nTime = 0;

		for (int i = 0; i < MAX_LIGHT; i++)
		{
			s_nIdxColor[i] = IntRandam(LIGHT_COLOR_MAX, 0);

			// セレクトのテクスチャの設定
			ChangeTextuteSelect(s_nIdxSelect, i, s_aTexture[s_nIdxColor[i]]);
		}

		// 描画のリセット
		ResetDrawLight();

		// 矩形の色の設定
		SetColorRectangle(s_nIdxLearn, GetColor(COLOR_WHITE));

		break;

	case GAMESTATE_PLAYER:	// プレイヤー状態
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
void DrawLight(void)
{
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetLight(void)
{
	return s_nMaxLight;
}

//--------------------------------------------------
// 色の取得
//--------------------------------------------------
int GetColorLight(int nNowLight)
{
	assert(nNowLight >= 0 && nNowLight < MAX_LIGHT);

	return s_nIdxColor[nNowLight];
}

//--------------------------------------------------
// 描画の設定
//--------------------------------------------------
void SetDrawLight(int nNowLight)
{
	// セレクトの描画するかどうか
	SetDrawSelect(s_nIdxSelect, nNowLight, true);

	// エフェクトの設定
	SetEffect(GetPosSelect(s_nIdxSelect, nNowLight), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[nNowLight]]));
}

//--------------------------------------------------
// 間違った
//--------------------------------------------------
void MistakeLight(void)
{
	for (int i = (GetPlayer() + 1); i < s_nMaxLight; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxSelect, i, true);

		// エフェクトの設定
		SetEffect(GetPosSelect(s_nIdxSelect, i), EFFECT_TYPE_000, GetColor(s_aColor[s_nIdxColor[i]]));
	}
}

namespace
{
//--------------------------------------------------
// 描画のリセット
//--------------------------------------------------
void ResetDrawLight(void)
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxSelect, i, false);
	}

	for (int i = 0; i < s_nNowLight; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxSelect, i, true);
	}

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// セレクトの描画するかどうか
		SetDrawSelect(s_nIdxFrame, i, false);
	}

	s_bMax = false;
}
}// namespaceはここまで
