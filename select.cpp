//**************************************************
// 
// Hackathon ( select.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "input.h"
#include "select.h"
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
const int	MAX_SELECT = 16;	// メニューの最大数

/*↓ 選択肢 ↓*/
typedef struct
{
	D3DXVECTOR3	pos;		// 位置
	D3DXCOLOR	col;		// 色
	int			nIdx;		// 矩形のインデックス
	float		fWidth;		// 幅
	float		fHeight;	// 高さ
	bool		bDraw;		// 描画するかどうか
}Option;

/*↓ メニュー ↓*/
typedef struct
{
	D3DXVECTOR3	pos;				// 位置
	Option		Option[MAX_OPTION];	// 選択肢の情報
	int			nNumUse;			// 使用数
	int			nIdx;				// 矩形のインデックス
	float		fWidth;				// 幅
	float		fHeight;			// 高さ
	float		fInterval;			// 選択肢の間隔
	bool		bUse;				// 使用しているかどうか
}Select;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
Select	s_aSelect[MAX_SELECT];	// メニューの情報
int		s_nIdxSelect;			// 選ばれているメニューの番号
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitSelect(void)
{
	s_nIdxSelect = 0;

	// メモリのクリア
	memset(s_aSelect, 0, sizeof(s_aSelect));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitSelect(void)
{
	for (int i = 0; i < MAX_SELECT; i++)
	{
		Select *pSelect = &s_aSelect[i];

		if (!pSelect->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		for (int j = 0; j < pSelect->nNumUse; j++)
		{
			Option *pOption = &pSelect->Option[j];

			// 使うのを止める
			StopUseRectangle(pOption->nIdx);
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateSelect(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawSelect(void)
{
	/* 矩形で描画してます */
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetSelect(const SelectArgument &menu)
{
	assert(menu.nNumUse >= 0 && menu.nNumUse < MAX_OPTION);
	
	for (int i = 0; i < MAX_SELECT; i++)
	{
		Select *pSelect = &s_aSelect[i];

		if (pSelect->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pSelect->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pSelect->nNumUse = menu.nNumUse;
		pSelect->fWidth = menu.fRight - menu.fLeft;
		pSelect->fHeight = menu.fBottom - menu.fTop;
		pSelect->bUse = true;

		if (menu.bSort)
		{// 縦
			pSelect->fInterval = pSelect->fHeight / (menu.nNumUse + 1);
		}
		else
		{// 横
			pSelect->fInterval = pSelect->fWidth / (menu.nNumUse + 1);
		}

		s_nIdxSelect = i;
	
		for (int j = 0; j < pSelect->nNumUse; j++)
		{
			Option *pOption = &pSelect->Option[j];

			if (menu.bSort)
			{// 縦
				pOption->pos = D3DXVECTOR3(pSelect->pos.x, menu.fTop + (pSelect->fInterval * (j + 1)), 0.0f);
			}
			else
			{// 横
				pOption->pos = D3DXVECTOR3(menu.fLeft + (pSelect->fInterval * (j + 1)), pSelect->pos.y, 0.0f);
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

			pOption->bDraw = true;
		}

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// セレクトの色の設定
// 引数1  : int nIdxSelect / セレクトのインデックス
// 引数2  : int nIdxOption / 選択肢のインデックス
// 引数3  : D3DXCOLOR &col / 色
//--------------------------------------------------
void SetColorSelect(int nIdxSelect, int nIdxOption, COLOR color)
{
	assert(nIdxSelect >= 0 && nIdxSelect < MAX_SELECT);
	assert(nIdxOption >= 0 && nIdxOption < MAX_OPTION);

	Option *pOption = &s_aSelect[nIdxSelect].Option[nIdxOption];

	pOption->col = GetColor(color);

	// 矩形の色の設定
	SetColorRectangle(pOption->nIdx, pOption->col);
}

//--------------------------------------------------
// セレクトのテクスチャの変更
//--------------------------------------------------
void ChangeTextuteSelect(int nIdxSelect, int nIdxOption, TEXTURE texture)
{
	assert(nIdxSelect >= 0 && nIdxSelect < MAX_SELECT);
	assert(nIdxOption >= 0 && nIdxOption < MAX_OPTION);

	Option *pOption = &s_aSelect[nIdxSelect].Option[nIdxOption];

	// 矩形の色の設定
	ChangeTextureRectangle(pOption->nIdx, texture);
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetSelect(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_SELECT);

	s_nIdxSelect = 0;
	
	Select *pSelect = &s_aSelect[nIdx];

	if (!pSelect->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	for (int i = 0; i < pSelect->nNumUse; i++)
	{
		Option *pOption = &pSelect->Option[i];

		// 使うのを止める
		StopUseRectangle(pOption->nIdx);
	}

	pSelect->bUse = false;
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawSelect(int nIdxSelect, int nIdxOption, bool bDraw)
{
	assert(nIdxSelect >= 0 && nIdxSelect < MAX_SELECT);
	assert(nIdxOption >= 0 && nIdxOption < MAX_OPTION);

	Option *pOption = &s_aSelect[nIdxSelect].Option[nIdxOption];

	// 矩形の描画するかどうか
	SetDrawRectangle(pOption->nIdx, bDraw);

	pOption->bDraw = bDraw;
}

//--------------------------------------------------
// 選択肢の位置を取得
//--------------------------------------------------
D3DXVECTOR3 GetPosSelect(int nIdxSelect, int nIdxOption)
{
	assert(nIdxSelect >= 0 && nIdxSelect < MAX_SELECT);
	assert(nIdxOption >= 0 && nIdxOption < MAX_OPTION);

	Option *pOption = &s_aSelect[nIdxSelect].Option[nIdxOption];

	return pOption->pos;
}

//--------------------------------------------------
// 選択肢の色を取得
//--------------------------------------------------
D3DXCOLOR GetColSelect(int nIdxSelect, int nIdxOption)
{
	assert(nIdxSelect >= 0 && nIdxSelect < MAX_SELECT);
	assert(nIdxOption >= 0 && nIdxOption < MAX_OPTION);

	Option *pOption = &s_aSelect[nIdxSelect].Option[nIdxOption];

	return pOption->col;
}