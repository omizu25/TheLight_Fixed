//**************************************************
// 
// Hackathon ( menu.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "input.h"
#include "menu.h"
#include "fade.h"
#include "color.h"
#include "rectangle.h"
#include "utility.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_MENU = 16;					// メニューの最大数
const float	NORMAL_CHANGE_SPEED = 0.01f;	// 通常時の変更速度
const float	DECISION_CHANGE_SPEED = 0.1f;	// 決定時の変更速度
const float	MIN_ALPHA = 0.5f;				// α値の最小値

/*↓ 選択肢 ↓*/
typedef struct
{
	D3DXVECTOR3	pos;		// 位置
	D3DXCOLOR	col;		// 色
	int			nIdx;		// 矩形のインデックス
	float		fWidth;		// 幅
	float		fHeight;	// 高さ
}Option;

/*↓ メニュー ↓*/
typedef struct
{
	D3DXVECTOR3	pos;				// 位置
	D3DXCOLOR	colStart;			// 始まりの色
	D3DXCOLOR	colEnd;				// 終わりの色
	D3DXCOLOR	colDefault;			// 選ばれてない選択肢の色
	Option		Option[MAX_OPTION];	// 選択肢の情報
	int			nNumUse;			// 使用数
	int			nIdx;				// 矩形のインデックス
	float		fWidth;				// 幅
	float		fHeight;			// 高さ
	float		fInterval;			// 選択肢の間隔
	float		fChangeSpeed;		// 変更速度
	bool		bFrame;				// 枠がいるかどうか [ true : いる false : いらない ]
	bool		bDraw;				// 描画するかどうか
	bool		bUse;				// 使用しているかどうか
}Menu;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Menu	s_aMenu[MAX_MENU];	// メニューの情報
int		s_nIdxMenu;			// 選ばれているメニューの番号
int		s_nIdxOption;		// 選ばれている選択肢の番号
int		s_nTime;		// α値変更用の時間
}// namespaceはここまで

//==================================================
// スタティック関数プロトタイプ宣言
//==================================================
namespace
{
void ChangeColor(Menu *pMenu);
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMenu(void)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nTime = 0;

	// メモリのクリア
	memset(s_aMenu, 0, sizeof(s_aMenu));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMenu(void)
{
	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		if (pMenu->bFrame)
		{// 枠を使っている
			// 使うのを止める
			StopUseRectangle(pMenu->nIdx);
		}

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			// 使うのを止める
			StopUseRectangle(pOption->nIdx);
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}
	
	// 色の変更
	ChangeColor(pMenu);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMenu(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame)
{
	assert(menu.nNumUse >= 0 && menu.nNumUse < MAX_OPTION);
	
	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (pMenu->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pMenu->colStart = GetColor(COLOR_WHITE);
		pMenu->colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pMenu->colDefault = GetColor(COLOR_WHITE);
		pMenu->nNumUse = menu.nNumUse;
		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;
		pMenu->fChangeSpeed = NORMAL_CHANGE_SPEED;
		pMenu->bFrame = Frame.bUse;
		pMenu->bDraw = true;
		pMenu->bUse = true;

		if (menu.bSort)
		{// 縦
			pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		}
		else
		{// 横
			pMenu->fInterval = pMenu->fWidth / (menu.nNumUse + 1);
		}

		s_nIdxMenu = i;
		s_nIdxOption = 0;

		if (Frame.bUse)
		{// 枠がいる
			// 矩形の設定
			pMenu->nIdx = SetRectangle(Frame.texture);

			fPosX = menu.fLeft + (pMenu->fWidth * 0.5f);
			fPosY = menu.fTop + (pMenu->fHeight * 0.5f);
			D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
			D3DXVECTOR3 size = D3DXVECTOR3(pMenu->fWidth, pMenu->fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(pMenu->nIdx, pos, size);

			// 矩形の色の設定
			SetColorRectangle(pMenu->nIdx, Frame.col);
		}

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			if (menu.bSort)
			{// 縦
				pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
			}
			else
			{// 横
				pOption->pos = D3DXVECTOR3(menu.fLeft + (pMenu->fInterval * (j + 1)), pMenu->pos.y, 0.0f);
			}

			pOption->col = GetColor(COLOR_WHITE);
			pOption->fWidth = menu.fWidth;
			pOption->fHeight = menu.fHeight;

			// 矩形の設定
			pOption->nIdx = SetRectangle(menu.texture[j]);

			D3DXVECTOR3 size = D3DXVECTOR3(menu.fWidth, menu.fHeight, 0.0f);

			// 矩形の位置の設定
			SetPosRectangle(pOption->nIdx, pOption->pos, size);

			// 矩形の色の設定
			SetColorRectangle(pOption->nIdx, pOption->col);
		}

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 選択肢の色の設定
//--------------------------------------------------
void SetColorOption(int nIdx, const D3DXCOLOR &colStart, const D3DXCOLOR &colEnd)
{
	assert(nIdx >= 0 && nIdx < MAX_MENU);

	Menu *pMenu = &s_aMenu[nIdx];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pMenu->colStart = colStart;
	pMenu->colEnd = colEnd;
}

//--------------------------------------------------
// 選ばれていない選択肢の色の設定
//--------------------------------------------------
void SetColorDefaultOption(int nIdx, const D3DXCOLOR &colDefault)
{
	assert(nIdx >= 0 && nIdx < MAX_MENU);

	Menu *pMenu = &s_aMenu[nIdx];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pMenu->colDefault = colDefault;

	// 選択肢の色の初期化
	InitColorOption();
}

//--------------------------------------------------
// 選択肢の色の初期化
//--------------------------------------------------
void InitColorOption(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	for (int i = 0; i < pMenu->nNumUse; i++)
	{
		Option *pOption = &pMenu->Option[i];

		pOption->col = pMenu->colDefault;

		// 矩形の色の設定
		SetColorRectangle(pOption->nIdx, pOption->col);
	}
}

//--------------------------------------------------
// 選択肢の変更
//--------------------------------------------------
void ChangeOption(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_OPTION);

	s_nIdxOption = nIdx;
	s_nTime = 0;
}

//--------------------------------------------------
// 選択肢の決定
//--------------------------------------------------
void DecisionOption(void)
{
	s_aMenu[s_nIdxMenu].fChangeSpeed = DECISION_CHANGE_SPEED;
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_MENU);

	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nTime = 0;
	
	Menu *pMenu = &s_aMenu[nIdx];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	if (pMenu->bFrame)
	{// 枠を使っている
		// 使うのを止める
		StopUseRectangle(pMenu->nIdx);
	}

	for (int i = 0; i < pMenu->nNumUse; i++)
	{
		Option *pOption = &pMenu->Option[i];

		// 使うのを止める
		StopUseRectangle(pOption->nIdx);
	}

	pMenu->bUse = false;
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawMenu(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_MENU);

	Menu *pMenu = &s_aMenu[nIdx];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	if (pMenu->bFrame)
	{// 枠を使っている
		// 矩形の描画するかどうか
		SetDrawRectangle(pMenu->nIdx, bDraw);
	}

	for (int i = 0; i < pMenu->nNumUse; i++)
	{
		Option *pOption = &pMenu->Option[i];

		// 矩形の描画するかどうか
		SetDrawRectangle(pOption->nIdx, bDraw);

		pOption->col = GetColor(COLOR_WHITE);
	}

	pMenu->bDraw = bDraw;

	pMenu->fChangeSpeed = NORMAL_CHANGE_SPEED;
	s_nIdxOption = 0;
	s_nTime = 0;
}

namespace
{
//--------------------------------------------------
// 色の変更
//--------------------------------------------------
void ChangeColor(Menu *pMenu)
{
	s_nTime++;

	Option *pOption = &pMenu->Option[s_nIdxOption];

	float fCurve = CosCurve(s_nTime, pMenu->fChangeSpeed);
	pOption->col.r = Curve(fCurve, pMenu->colStart.r, pMenu->colEnd.r);
	pOption->col.g = Curve(fCurve, pMenu->colStart.g, pMenu->colEnd.g);
	pOption->col.b = Curve(fCurve, pMenu->colStart.b, pMenu->colEnd.b);
	pOption->col.a = Curve(fCurve, pMenu->colStart.a, pMenu->colEnd.a);

	// 矩形の色の設定
	SetColorRectangle(pOption->nIdx, pOption->col);
}
}// namespaceはここまで
