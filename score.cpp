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
int	s_nIdxScore;	// 数の配列のインデックス
int	s_nScore;		// スコア
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitScore(void)
{
	s_nScore = GetLight() - 1;

	D3DXVECTOR3 size = D3DXVECTOR3(SCORE_WIDTH, SCORE_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.68f, SCREEN_HEIGHT * 0.25f, 0.0f);

	// 数の設定
	s_nIdxScore = SetNumber(pos, size, GetColor(COLOR_WHITE), s_nScore, DigitNumber(s_nScore), false);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitScore(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxScore);
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
// 取得
//--------------------------------------------------
int GetScore(void)
{
	return s_nScore;
}
