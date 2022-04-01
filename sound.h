//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// タイトル
	SOUND_LABEL_BGM_GAME,		// ゲーム
	SOUND_LABEL_BGM_RESULT,		// リザルト
	SOUND_LABEL_SE_ENTER,		// 決定音
	SOUND_LABEL_SE_SELECT,		// 選択音
	SOUND_LABEL_SE_MISS,	// ミス音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
