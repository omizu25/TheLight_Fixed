//**************************************************
//
// Hackathon ( bg.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "bg.h"
#include "rectangle.h"
#include "input.h"
#include "color.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_BG = 3;	// 背景の最大値
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int	s_nIdxBG[MAX_BG];	// 背景の矩形のインデックス
int	s_nIdxMoon;			// 月の矩形のインデックス
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBG(void)
{
	TEXTURE aTexture[MAX_BG];

	aTexture[0] = TEXTURE_BG_2;
	aTexture[1] = TEXTURE_BG_1;
	aTexture[2] = TEXTURE_BG_0;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for (int i = 0; i < MAX_BG; i++)
	{
		// 矩形の設定
		s_nIdxBG[i] = SetRectangle(aTexture[i]);

		// 矩形の位置の設定
		SetPosRectangle(s_nIdxBG[i], pos, size);
	}
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBG(void)
{
	for (int i = 0; i < MAX_BG; i++)
	{
		// 使うのを止める
		StopUseRectangle(s_nIdxBG[i]);
	}

	// 使うのを止める
	StopUseRectangle(s_nIdxMoon);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBG(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBG(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int GetIdxBG(int nIdxBG)
{
	assert(nIdxBG >= 0 && nIdxBG < MAX_BG);

	return s_nIdxBG[nIdxBG];
}

//--------------------------------------------------
// 月の初期化
//--------------------------------------------------
void InitMoonBG(void)
{
	// 矩形の設定
	s_nIdxMoon = SetRectangle(TEXTURE_BG_MOON);

	D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// 矩形の位置の設定
	SetPosRectangle(s_nIdxMoon, pos, size);
}