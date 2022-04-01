//**************************************************
//
// Hackathon ( ui.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "ui.h"
#include "rectangle.h"
#include "fan.h"
#include "texture.h"
#include "mode.h"
#include "color.h"
#include "sound.h"
#include "gauge.h"
#include "light.h"
#include "utility.h"
#include "effect.h"

//==================================================
// 定義
//==================================================
namespace
{
const float	MAX_ALPHA_GRAY = 0.6f;		// 灰色のゲージのα値の最大値
const float	MAX_ALPHA_YELLOW = 1.0f;	// 黄色のゲージのα値の最大値
const float	MIN_ALPHA_GRAY = 0.3f;		// 灰色のゲージのα値の最小値
const float	MIN_ALPHA_YELLOW = 0.3f;	// 黄色のゲージのα値の最小値
const D3DXVECTOR3 MOON_POS(140.5f, 90.5f, 0.0f);	// 月の座標
const int MOONEFFECT_TIME = 45;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int		s_nTimeMoon;			// 月のエフェクトのタイム
int		s_nTimeGauge;			// ゲージのタイム
int		s_nIdxGaugeGray;		// 灰色のゲージの配列のインデックス
int		s_nIdxGaugeYellow;		// 黄色のゲージの配列のインデックス
float	s_fAlphaGaugeGray;		// 灰色のゲージのα値
float	s_fAlphaGaugeYellow;	// 黄色のゲージのα値
}// namespaceはここまで

//--------------------------------------------------
// ゲージの設定
//--------------------------------------------------
void SetGaugeUI(void)
{
	s_nTimeGauge = 0;
	s_nTimeMoon = 0;
	s_fAlphaGaugeGray = MIN_ALPHA_GRAY;
	s_fAlphaGaugeYellow = MIN_ALPHA_YELLOW;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5f, 0.0f);

	D3DXCOLOR col = GetColor(COLOR_GRAY);
	col.a = s_fAlphaGaugeGray;

	// ゲージの設定
	s_nIdxGaugeGray = SetGauge(pos, col, SCREEN_WIDTH, SCREEN_HEIGHT, GAUGE_LEFT);

	if ((GetMode() == MODE_TITLE) || (GetMode() == MODE_TUTORIAL))
	{// タイトル、チュートリアル
		return;
	}

	col = GetColor(COLOR_YELLOW);
	col.a = s_fAlphaGaugeYellow;

	float fWidth = 0.0f;

	if (GetMode() != MODE_GAME)
	{// ゲームじゃない
		fWidth = (GetLight() - 1) * (SCREEN_WIDTH / 16.0f);
	}

	// ゲージの設定
	s_nIdxGaugeYellow = SetGauge(pos, col, fWidth, SCREEN_HEIGHT, GAUGE_LEFT);
}

//--------------------------------------------------
// ゲージの更新
//--------------------------------------------------
void UpdateGaugeUI(void)
{
	s_nTimeGauge++;

	{// α値の変更
		float fCurve = CosCurve(s_nTimeGauge, 0.01f);
		s_fAlphaGaugeGray = Curve(fCurve, MAX_ALPHA_GRAY, MIN_ALPHA_GRAY);
		s_fAlphaGaugeYellow = Curve(fCurve, MAX_ALPHA_YELLOW, MIN_ALPHA_YELLOW);
	}

	D3DXCOLOR col = GetColor(COLOR_GRAY);
	col.a = s_fAlphaGaugeGray;

	// ゲージの色の設定(灰色)
	SetColorGauge(s_nIdxGaugeGray, col);

	if ((GetMode() == MODE_TITLE) || (GetMode() == MODE_TUTORIAL))
	{// タイトル、チュートリアル
		return;
	}

	col = GetColor(COLOR_YELLOW);
	col.a = s_fAlphaGaugeYellow;

	// ゲージの色の設定(黄色)
	SetColorGauge(s_nIdxGaugeYellow, col);
}

//--------------------------------------------------
// ゲージの変更
//--------------------------------------------------
void ChangeGaugeUI(void)
{
	// ゲージのサイズの設定
	SetSizeGauge(s_nIdxGaugeYellow, GetLight() * (SCREEN_WIDTH / 16.0f), SCREEN_HEIGHT);
}

//--------------------------------------------------
// 月のエフェクトの更新
//--------------------------------------------------
void UpdateEffectMoonUI(void)
{
	if (s_nTimeMoon % MOONEFFECT_TIME == 0)
	{
		// 色の設定
		D3DXCOLOR col = GetColor(COLOR_WHITE);
		col.b = 0.1f;

		// エフェクトの設定
		SetEffect(MOON_POS, EFFECT_TYPE_003, col);
	}

	s_nTimeMoon++;
}