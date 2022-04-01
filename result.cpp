//=============================================================================
//
// リザルト処理 [result.cpp]
// Author :　koduna hirohito
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "result.h"
#include "gauge.h"
#include "rectangle.h"
#include "texture.h"
#include "color.h"
#include "sound.h"
#include "light.h"
#include "mode.h"
#include "utility.h"
#include "number.h"
#include "score.h"
#include "ranking.h"
#include "input.h"
#include "bg.h"
#include "effect.h"
#include "menu.h"
#include "fade.h"
#include "cursor.h"
#include "ui.h"

#include <assert.h>

//=============================================================================
// 定義
//=============================================================================
namespace
{
const int	MAX_TIME = 900;			// タイムの最大値
const float	MENU_WIDTH = 280.0f;	// メニューの幅
const float	MENU_HEIGHT = 80.0f;	// メニューの高さ
const float	CURSOR_SIZE = 50.0f;	// カーソルのサイズ

typedef enum
{
	MENU_GAME = 0,	// ゲーム
	MENU_TITLE,		// タイトル
	MENU_MAX
}MENU;
}// namespaceはここまで

//=============================================================================
// スタティック変数
//=============================================================================
namespace
{
int	s_nIdxMoon;				// 背景の矩形のインデックス
int	s_nIdxScore;			// スコアの矩形のインデックス
int	s_nIdxBestScore;		// ベストスコアの矩形のインデックス
int	s_nIdxMenu;				// メニューの配列のインデックス
int	s_nSelectMenu;			// 選ばれているメニュー
int	s_nIdxCursor;			// カーソルの配列のインデックス
int	s_nTime;				// タイム
}// namespaceはここまで

 //=============================================================================
// スタティック関数プロトタイプ宣言
//=============================================================================
namespace
{
void Input(void);
}// namespaceはここまで

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitResult(void)
{
	PlaySound(SOUND_LABEL_BGM_RESULT);

	s_nTime = 0;
	s_nSelectMenu = 0;

	// ゲージの初期化
	InitGauge();

	// ゲージのUIの設定
	SetGaugeUI();

	// 背景の初期化
	InitBG();

	// エフェクトの初期化
	InitEffect();

	// 月の背景の初期化
	InitMoonBG();

	{// 今回のスコア
		s_nIdxScore = SetRectangle(TEXTURE_YourScore);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.25f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(750.0f, 100.0f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxScore, pos, size);
	}

	{// ベストスコア
		// 矩形の設定
		s_nIdxBestScore = SetRectangle(TEXTURE_BestScore);

		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(750.0f, 100.0f, 0.0f);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBestScore, pos, size);
	}

	{// メニュー
		// メニューの初期化
		InitMenu();

		MenuArgument menu;
		menu.nNumUse = MENU_MAX;
		menu.fLeft =  0.0f;
		menu.fRight = SCREEN_WIDTH;
		menu.fTop = SCREEN_HEIGHT * 0.7f;
		menu.fBottom = SCREEN_HEIGHT * 0.7f;
		menu.fWidth = MENU_WIDTH;
		menu.fHeight = MENU_HEIGHT;
		menu.bSort = false;

		menu.texture[MENU_GAME] = TEXTURE_play;
		menu.texture[MENU_TITLE] = TEXTURE_end;

		FrameArgument Frame;
		Frame.bUse = false;
		Frame.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		Frame.texture = TEXTURE_NONE;

		// メニューの設定
		s_nIdxMenu = SetMenu(menu, Frame);

		// 選択肢の色の設定
		SetColorOption(s_nIdxMenu, GetColor(COLOR_RED), D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

		// 選ばれていない選択肢の色の設定
		SetColorDefaultOption(s_nIdxMenu, GetColor(COLOR_WHITE));
	}

	// ランキングの初期化
	InitRanking();

	// スコアの初期化
	InitScore();

	// ランキングの設定
	SetRanking();
}

//=============================================================================
// リザルト終了処理
//=============================================================================
void UninitResult(void)
{
	// サウンドの停止
	StopSound();

	// 使うのを止める
	StopUseRectangle(s_nIdxMoon);
	StopUseRectangle(s_nIdxScore);
	StopUseRectangle(s_nIdxBestScore);

	// ゲージの終了
	UninitGauge();

	// ランキングの終了
	UninitRanking();

	// エフェクトの終了
	UninitEffect();

	// スコアの終了
	UninitScore();

	// メニューの終了
	UninitMenu();
}

//=============================================================================
// リザルト更新処理
//=============================================================================
void UpdateResult(void)
{
	// ゲージの更新
	UpdateGauge();

	// エフェクトの更新
	UpdateEffect();

	// スコアの更新
	UpdateScore();

	// ランキングの更新
	UpdateRanking();

	// メニューの更新更新
	UpdateMenu();

	s_nTime++;

	if (s_nTime >= MAX_TIME)
	{
		// モードの変更
		ChangeMode(MODE_TITLE);
	}

	// 入力
	Input();

	// ゲージのUIの更新
	UpdateGaugeUI();

	// 月のエフェクトの更新
	UpdateEffectMoonUI();
}

//=============================================================================
// リザルト描画処理
//=============================================================================
void DrawResult(void)
{
	// 矩形の描画
	DrawRectangle();
}

namespace
{
//--------------------------------------------------
// 入力
//--------------------------------------------------
void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetMoveKeyTrigger(MOVE_KEY_LEFT))
	{// Aキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_SELECT);

		s_nTime = 0;
	}
	else if (GetMoveKeyTrigger(MOVE_KEY_RIGHT))
	{// Dキーが押されたかどうか
		// 選択肢の色の初期化
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// 選択肢の変更
		ChangeOption(s_nSelectMenu);

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_SELECT);

		s_nTime = 0;
	}

	if (GetFunctionKeyTrigger(FUNCTION_KEY_DESISION))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAME:		// ゲーム
			ChangeMode(MODE_GAME);
			break;

		case MENU_TITLE:	// タイトル
			ChangeMode(MODE_TITLE);
			break;

		default:
			assert(false);
			break;
		}

		// 選択肢の決定
		DecisionOption();

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_ENTER);
	}
}
}// namespaceはここまで
