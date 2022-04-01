//**************************************************
//
// Hackathon ( tutorial.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "tutorial.h"
#include "rectangle.h"
#include "texture.h"
#include "input.h"
#include "mode.h"
#include "game.h"
#include "sound.h"
#include "ui.h"
#include "bg.h"
#include "gauge.h"
#include "effect.h"

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_TIME = 900;	// タイムの最大値
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
int		s_nIdx;		// 背景の矩形のインデックス
int		s_nTime;	// タイム
}// namespaceはここまで

 //--------------------------------------------------
 // 初期化
 //--------------------------------------------------
void InitTutorial(void)
{
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

	// 矩形の設定
	s_nIdx = SetRectangle(TEXTURE_TITLE_GamePopup);

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.8f, 0.0f);

	// 矩形の位置の設定
	SetPosRectangle(s_nIdx, pos, size);

	s_nTime = 0;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitTutorial(void)
{
	// 使うのを止める
	StopUseRectangle(s_nIdx);

	// ゲージの終了
	UninitGauge();

	// 背景の終了
	UninitBG();

	// エフェクトの終了
	UninitEffect();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateTutorial(void)
{
	s_nTime++;

	if (s_nTime >= MAX_TIME)
	{// 指定の値を越した
		// モードの変更
		ChangeMode(MODE_GAME);
	}

	if (GetFunctionKeyTrigger(FUNCTION_KEY_DESISION))
	{//決定キー(ENTERキー)が押されたかどうか
		// モードの変更
		ChangeMode(MODE_GAME);
		
		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_ENTER);
	}

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
void DrawTutorial(void)
{
	// 矩形の描画
	DrawRectangle();
}
