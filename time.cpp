//**************************************************
//
// Hackathon ( time.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "time.h"
#include "rectangle.h"
#include "texture.h"
#include "number.h"
#include "color.h"
#include "mode.h"
#include "fan.h"
#include "answer.h"
#include "game.h"

//==================================================
// 定義
//==================================================
namespace
{
const int	ONE_SECOND = 60;	// 1秒
const float	TIME_SIZE = 21.5f;	// タイムのサイズ
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdxTime;		// 数の配列のインデックス
int	s_nTime;		// タイム
int	s_nSecond;		// 1秒を計測
int	s_nDrawTime;	// 描画をいじるタイム
D3DXCOLOR s_col;
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitTime(void)
{
	s_nTime = 0;
	s_nSecond = 0;
	s_nIdxTime = 0;
	s_nDrawTime = 0;

	D3DXVECTOR3 size = D3DXVECTOR3(TIME_SIZE, TIME_SIZE, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TIME_SIZE * 0.5f, 0.0f);

	pos.x += DigitNumber(s_nTime) * (TIME_SIZE * 0.5f);

	// 円形の設定
	s_nIdxTime = SetFan(TEXTURE_NONE);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTime(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdxTime);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTime(void)
{
	if (GetGameState() != GAMESTATE_PLAYER)
	{
		// 円形を描画するかどうか
		SetDrawFan(s_nIdxTime, false);

		return;
	}

	if (GetAnswer())
	{// 最大値
		// 円形を描画するかどうか
		SetDrawFan(s_nIdxTime, false);

		return;
	}

	// 円形を描画するかどうか
	SetDrawFan(s_nIdxTime, true);

	s_nSecond++;

	if ((s_nSecond % s_nDrawTime) == 0)
	{
		AddDrawFan(s_nIdxTime, -1);
	}

	if ((s_nSecond % ONE_SECOND) == 0)
	{// 1秒毎に
		// 加算
		AddTime(-1);

		switch (s_nTime)
		{
		case 5:
			s_col = GetColor(COLOR_GREEN);
			break;
		case 4:
		case 3:
			s_col = GetColor(COLOR_YELLOW);
			break;
		case 2:
		case 1:
			s_col = GetColor(COLOR_RED);
			break;
		default:
			break;
		}

		s_col.a = 0.5f;

		// 円形の色の設定
		SetColorFan(s_nIdxTime, s_col);

		if (s_nTime <= 0)
		{// 制限時間が来た
			// モードの変更
			ChangeMode(MODE_RESULT);
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawTime(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetTime(int nTime)
{
	s_nTime = nTime;
	s_nDrawTime = (nTime * ONE_SECOND) / 100;
	s_nSecond = 0;

	// リセット
	ResetDrawFan(s_nIdxTime);

	s_col = GetColor(COLOR_GREEN);

	s_col.a = 0.5f;

	// 円形の色の設定
	SetColorFan(s_nIdxTime, s_col);
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosTime(D3DXVECTOR3 pos)
{
	// 位置の設定
	SetRotationPosFan(s_nIdxTime, pos, D3DX_PI, TIME_SIZE, false);
}

//--------------------------------------------------
// 加算
// 引数  : int nValue / 加算する値
//--------------------------------------------------
void AddTime(int nValue)
{
	s_nTime += nValue;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetTime(void)
{
	return s_nTime;
}
