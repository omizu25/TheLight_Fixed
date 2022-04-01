//**************************************************
//
// Hackathon ( answer.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "answer.h"
#include "rectangle.h"
#include "texture.h"
#include "color.h"
#include "mode.h"
#include "light.h"
#include "player.h"
#include "game.h"
#include "bg.h"
#include "sound.h"
#include "ui.h"

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_TIME = 120;	// タイムの最大値
}// namespaceはここまで

 //==================================================
 // スタティック変数
 //==================================================
namespace
{
bool	s_bMax;		// 最大になったかどうか
bool	s_bAnswer;	// あってるかどうか
int		s_nTime;	// タイム
}// namespaceはここまで

 //--------------------------------------------------
 // 初期化
 //--------------------------------------------------
void InitAnswer(void)
{
	s_nTime = 0;
	s_bMax = false;
	s_bAnswer = false;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitAnswer(void)
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateAnswer(void)
{
	if (s_bMax)
	{// 最大値になった
		s_nTime++;

		if (s_nTime >= MAX_TIME)
		{// 時間が来た
			if (s_bAnswer)
			{// 正解
				s_bMax = false;
				s_nTime = 0;
				s_bAnswer = true;

				// ゲーム状態の設定
				SetGameState(GAMESTATE_RESET);

				// 矩形の色の設定
				SetColorRectangle(GetIdxBG(0), GetColor(COLOR_WHITE));

				// ゲージのUIの変更
				ChangeGaugeUI();

				// 押せの色の設定
				SetColorPushPlayer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
			else
			{// 不正解
				s_bMax = true;
				s_nTime = 0;
				s_bAnswer = false;

				// モードの変更
				ChangeMode(MODE_RESULT);
			}
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawAnswer(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetAnswer(int nNowLight)
{
	if (GetColorLight(nNowLight) == GetColorPlayer(nNowLight))
	{// 正解
		if ((GetPlayer() + 1) >= GetLight())
		{// 最大になった
			s_bMax = true;
			s_nTime = 0;
			s_bAnswer = true;
		}
	}
	else
	{// 不正解
		PlaySound(SOUND_LABEL_SE_MISS);
		s_nTime = 0;
		s_bMax = true;
		s_bAnswer = false;

		// 間違った
		MistakeLight();
	}
}

//--------------------------------------------------
// 取得
// 返値  : bool / 最大値になったかどうか
//--------------------------------------------------
bool GetAnswer(void)
{
	return s_bMax;
}