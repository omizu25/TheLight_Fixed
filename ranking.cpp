//**************************************************
//
// Hackathon ( ranking.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "ranking.h"
#include "rectangle.h"
#include "texture.h"
#include "number.h"
#include "color.h"
#include "mode.h"
#include "input.h"
#include "utility.h"
#include "score.h"
#include <stdio.h>
#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_RANKING = 1;						// ランキングの最大数
const float	RANKING_WIDTH = 70.0f;					// ランキングの幅
const float	RANKING_HEIGHT = 150.0f;				// ランキングの高さ
const char *TEXT_NAME = "data/TEXT/Ranking.txt";	// テキスト名

}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdx;		// 矩形のインデックス
int	s_nScore;	// スコア
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRanking(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.68f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// 数の設定
	s_nIdx = SetNumber(pos, size, GetColor(COLOR_WHITE), s_nScore, DigitNumber(s_nScore), false);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRanking(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdx);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateRanking(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRanking(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetRanking(void)
{
	int aSave[MAX_RANKING + 1];

	aSave[0] = s_nScore;

	// スコアの取得
	aSave[MAX_RANKING] = GetScore();

	for (int i = 0; i < MAX_RANKING; i++)
	{
		int nMax = aSave[i];

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (nMax < aSave[j])
			{//相手が大きかったら交代
				nMax = aSave[j];
			}
		}

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (nMax == aSave[j])
			{//最大値を探して交代
				aSave[j] = aSave[i];
				aSave[i] = nMax;
				break;
			}
		}
	}

	s_nScore = aSave[0];

	// 数の変更
	s_nIdx = ChangeNumber(s_nIdx, s_nScore);
}

//--------------------------------------------------
// ロード
//--------------------------------------------------
void LoadRanking(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(TEXT_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fscanf(pFile, "%d", &s_nScore);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);

		s_nScore = 0;
	}
}

//--------------------------------------------------
// セーブ
//--------------------------------------------------
void SaveRanking(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(TEXT_NAME, "w");

	if (pFile != NULL)
	{// ファイルが開いた場合
		fprintf(pFile, "%d\n\n", s_nScore);
	
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}
