//**************************************************
//
// FPD制作 ( game.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "rectangle.h"
#include "fan.h"
#include "pause.h"
#include "menu.h"
#include "number.h"
#include "bg.h"
#include "cursor.h"
#include "gauge.h"
#include "effect.h"
#include "color.h"
#include "time.h"
#include "score.h"
#include "ranking.h"
#include "light.h"
#include "select.h"
#include "player.h"
#include "answer.h"
#include "tutorial.h"
#include "utility.h"
#include "ui.h"

#include <assert.h>

//==================================================
// スタティック変数
//==================================================
namespace
{
GAMESTATE	s_gameState = GAMESTATE_NONE;	// ゲームの状態
int			s_nCounterState;				// 状態管理カウンター
bool		s_bPause = false;				// ポーズ中かどうか [してる  : true してない  : false]
int			s_nIdxMethod;					// 操作説明の矩形のインデックス
}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	// ゲージの初期化
	InitGauge();

	// ゲージのUIの設定
	SetGaugeUI();

	// 背景の初期化
	InitBG();

	{// レーン
		D3DXCOLOR col = GetColor(COLOR_GRAY);
		col.a = 0.75f;

		// ゲージの設定
		SetGauge(D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.35f, 0.0f), col, SCREEN_WIDTH, 100.0f, GAUGE_LEFT);

		// ゲージの設定
		SetGauge(D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5f, 0.0f), col, SCREEN_WIDTH, 100.0f, GAUGE_LEFT);
	}

	// 数の初期化
	InitNumber();

	// タイムの初期化
	InitTime();

	// セレクトの初期化
	InitSelect();

	// ライトの初期化
	InitLight();

	// プレイヤーの初期化
	InitPlayer();

	// アンサーの初期化
	InitAnswer();

	// エフェクトの初期化
	InitEffect();

	// 月の背景の初期化
	InitMoonBG();

	{// 操作説明
		// 矩形の設定
		s_nIdxMethod = SetRectangle(TEXTURE_Method);

		D3DXVECTOR3 pos(SCREEN_WIDTH - 100.0f, 95.0f, 0.0f);
		D3DXVECTOR3 size(155.0f, 155.0f, 0.0f);

		SetPosRectangle(s_nIdxMethod, pos, size);
	}

	// メニューの初期化
	InitMenu();

	// カーソルの初期化
	InitCursor();

	// ポーズの初期化
	InitPause();

	// サウンド開始
	PlaySound(SOUND_LABEL_BGM_GAME);

	s_gameState = GAMESTATE_SAMPLE;	// サンプル状態に設定

	s_nCounterState = 0;	// カウンターの初期化

	s_bPause = false;	// ポーズ解除
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	// サウンドの停止
	StopSound();

	// 背景の終了
	UninitBG();

	// ゲージの終了
	UninitGauge();

	// 数の終了
	UninitNumber();

	// タイムの終了
	UninitTime();

	// ポーズの終了
	UninitPause();

	// セレクトの終了
	UninitSelect();

	// メニューの終了
	UninitMenu();

	// カーソルの終了
	UninitCursor();

	// エフェクトの終了
	UninitEffect();

	// ライトの終了
	UninitLight();

	// プレイヤーの終了
	UninitPlayer();

	// アンサーの終了
	UninitAnswer();

	// 使うのを止める
	StopUseRectangle(s_nIdxMethod);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START,0))
	{// Pキーが押された
		if (GetFade() != FADE_NONE)
		{// フェードしてる
			return;
		}

		if (GetAnswer())
		{// 最大値になった
			return;
		}

		if (s_gameState == GAMESTATE_PLAYER)
		{// 通常状態の時
			s_bPause = !s_bPause;

			if (s_bPause)
			{// ポーズしてる
				// ポーズの設定
				SetPause();
			}
			else
			{// ポーズしてない
				// ポーズのリセット
				ResetPause();
			}
		}
	}

	if (s_bPause)
	{// ポーズしている
		// ポーズの更新
		UpdatePause();

		// メニューの更新
		UpdateMenu();

		// カーソルの更新
		UpdateCursor();

		// ゲージのUIの更新
		UpdateGaugeUI();

		// 月のエフェクトの更新
		UpdateEffectMoonUI();

		// エフェクトの更新
		UpdateEffect();

		return;
	}

	// 背景の更新
	UpdateBG();

	// ライトの更新
	UpdateLight();

	// プレイヤーの更新
	UpdatePlayer();

	// アンサーの更新
	UpdateAnswer();

	// ゲージの更新
	UpdateGauge();

	// セレクトの更新
	UpdateSelect();

	// メニューの更新
	UpdateMenu();

	// タイムの更新
	UpdateTime();

	// ゲージのUIの更新
	UpdateGaugeUI();

	// 月のエフェクトの更新
	UpdateEffectMoonUI();

	// エフェクトの更新
	UpdateEffect();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	// 矩形の描画
	DrawRectangle();

	// 円形の描画
	DrawFan();
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	assert(state >= 0 && state < GAMESTATE_MAX);

	s_gameState = state;
	s_nCounterState = 0;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
GAMESTATE GetGameState(void)
{
	return s_gameState;
}

//--------------------------------------------------
// ポーズの有効無効設定
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}

//--------------------------------------------------
// ポーズの有効無効取得
//--------------------------------------------------
bool GetEnablePause(void)
{
	return s_bPause;
}
