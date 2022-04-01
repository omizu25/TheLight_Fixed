//**************************************************
//
// Hackathon ( mode.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "fade.h"
#include "game.h"
#include "mode.h"
#include "title.h"
#include "rectangle.h"
#include "fan.h"
#include "texture.h"
#include "tutorial.h"
#include "ranking.h"
#include "result.h"

#include <assert.h>
#include <time.h>

//==================================================
// スタティック変数
//==================================================
namespace
{
MODE	s_mode = MODE_NONE;		// 現在のモード
MODE	s_modeNext = MODE_NONE;	// 次のモード
}// namesapceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMode(void)
{
	// テクスチャの読み込み
	srand((unsigned int)time(NULL));

	// テクスチャの読み込み
	LoadTexture();

	// 矩形の初期化
	InitRectangle();

	// 円形の初期化
	InitFan();

	// ランキングのロード
	LoadRanking();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMode(void)
{
	// タイトルの終了
	UninitTitle();

	// チュートリアルの終了
	UninitTutorial();

	// ゲームの終了
	UninitGame();

	// リザルトの終了
	UninitResult();

	// ランキングのセーブ
	SaveRanking();

	// 矩形の終了
	UninitRectangle();

	// 円形の終了
	UninitFan();

	// テクスチャの終了
	UninitTexture();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:	// タイトル
		UpdateTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		UpdateTutorial();
		break;

	case MODE_GAME:		// ゲーム
		UpdateGame();
		break;

	case MODE_RESULT:	// リザルト
		UpdateResult();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:	// タイトル
		DrawTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		DrawTutorial();
		break;

	case MODE_GAME:		// ゲーム
		DrawGame();
		break;

	case MODE_RESULT:	// リザルト
		DrawResult();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// 次のモードが決まってない
		return;
	}

	if (GetFade() == FADE_NONE)
	{// 何もしていない状態なら
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// フェードイン状態じゃない
		return;
	}

	switch (s_mode)
	{// 現在のモードの終了
	case MODE_TITLE:	// タイトル
		UninitTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		UninitTutorial();
		break;

	case MODE_GAME:		// ゲーム
		UninitGame();
		break;

	case MODE_RESULT:	// リザルト
		UninitResult();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}

	// 矩形の終了
	UninitRectangle();

	// 矩形の初期化
	InitRectangle();

	// 円形の終了
	UninitFan();

	// 円形の初期化
	InitFan();

	s_mode = s_modeNext;	// 現在の画面(モード)を切り替える
	
	switch (s_modeNext)
	{// 次のモードの初期化
	case MODE_TITLE:	// タイトル
		InitTitle();
		break;

	case MODE_TUTORIAL:	// チュートリアル
		InitTutorial();
		break;

	case MODE_GAME:		// ゲーム
		InitGame();
		break;

	case MODE_RESULT:	// リザルト
		InitResult();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// 変更
//--------------------------------------------------
void ChangeMode(MODE modeNext)
{
	assert(modeNext >= 0 && modeNext < MODE_MAX);

	s_modeNext = modeNext;
}
