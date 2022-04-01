//**************************************************
//
// Hackathon ( score.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "score.h"
#include "rectangle.h"
#include "texture.h"
#include "number.h"
#include "color.h"
#include "mode.h"
#include "input.h"
#include "utility.h"
#include "ranking.h"
#include "light.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const float	SCORE_WIDTH = 70.0f;	// スコアの幅
const float	SCORE_HEIGHT = 150.0f;	// スコアの高さ
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdx;		// 数の配列のインデックス
int	s_nScore;	// スコア
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitScore(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(SCORE_WIDTH, SCORE_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (GetMode())
	{
	case MODE_GAME:		// ゲーム
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.13f, 0.0f);
		break;

	case MODE_RESULT:	// リザルト
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.68f, SCREEN_HEIGHT * 0.25f, 0.0f);
		break;

	case MODE_TITLE:	// タイトル
	case MODE_TUTORIAL:	// チュートリアル
	case MODE_NONE:		// 何もしていない
	default:
		assert(false);
		break;
	}

	// 数の設定
	s_nIdx = SetNumber(pos, size, GetColor(COLOR_WHITE), s_nScore, DigitNumber(s_nScore), false);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitScore(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdx);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateScore(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawScore(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetScore(int nScore)
{
	s_nScore = nScore;

	// 数の変更
	s_nIdx = ChangeNumber(s_nIdx, s_nScore);
}

//--------------------------------------------------
// 加算
//--------------------------------------------------
void AddScore(int nValue)
{
	s_nScore += nValue;

	// 数の変更
	s_nIdx = ChangeNumber(s_nIdx, s_nScore);
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetScore(void)
{
	return s_nScore;
}
