//=========================================
// 
// パーティクルの処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "effect.h"
#include "rectangle.h"
#include "color.h"
#include "utility.h"
#include <assert.h>

//-----------------------------------------
// 定義
//-----------------------------------------
namespace
{
const int MAX_EFFECT = 512;	// パーティクルの最大数

//-----------------------------------------
// パーティクル構造体
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 角度
	D3DXCOLOR col;		// 色
	float fRaduus;		// 半径
	int nMaxLife;		// 最大寿命
	int nLife;			// 寿命
	int nIdx;			// RectAngel
	EFFECT_TYPE type;	// パーティクルの種類
	bool bUse;			// 使用しているかどうかe
}Effect;

}

//-----------------------------------------
// 静的変数
//-----------------------------------------
namespace
{
Effect s_aEffect[MAX_EFFECT];
int nCnt;
int nData;
}

//=========================================
// パーティクルの初期化処理
//=========================================
void InitEffect(void)
{
	ZeroMemory(s_aEffect, sizeof(s_aEffect));

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		// 矩形の設定
		pEffect->nIdx = SetRectangle(TEXTURE_NONE);

		// 矩形を描画するかどうか
		SetDrawRectangle(pEffect->nIdx, false);
	}

	nCnt = 0;
}

//=========================================
// パーティクルの終了処理
//=========================================
void UninitEffect(void)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (!pEffect->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// 使うのを止める
		StopUseRectangle(pEffect->nIdx);
	}
}

//=========================================
// パーティクルの更新処理
//=========================================
void UpdateEffect(void)
{
	Effect *pEffect;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = s_aEffect + i;

		if (!pEffect->bUse)
		{	// パーティクルが使用されていない
			continue;
		}

		// パーティクルが使用されている
		// 位置の更新
		pEffect->pos += pEffect->move;

		switch (pEffect->type)
		{
		case EFFECT_TYPE_000:
			pEffect->nLife--;											// 体力の更新
			pEffect->col.a -= (float)0.5f / pEffect->nMaxLife;			// 透明度の更新
			pEffect->fRaduus += 1.75f;	// 半径の拡大
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
			break;
		case EFFECT_TYPE_001:		// プレイヤーの移動
			pEffect->nLife--;											// 体力の更新
			pEffect->col.a -= (float)1.0f / pEffect->nMaxLife;			// 透明度の更新
			pEffect->fRaduus += 1.75f;	// 半径の拡大
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
			break;
		case EFFECT_TYPE_002:		// プレイヤーの移動
			//pEffect->nLife--;											// 体力の更新
			//pEffect->col.a -= (float)1.0f / pEffect->nMaxLife;			// 透明度の更新
			pEffect->move.x = SinCurve(nCnt, 1.0f) * 2.0f;	// 移動
			pEffect->fRaduus += SinCurve(nCnt, 1.0f) * 2.0f;	// 半径の拡大
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
			break;
		case EFFECT_TYPE_003:		// プレイヤーの移動
			pEffect->nLife--;											// 体力の更新
			pEffect->col.a -= (float)1.0f / pEffect->nMaxLife;			// 透明度の更新
			pEffect->fRaduus += 0.85f;	// 半径の拡大
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
			}
			break;
		default:
			break;
		}

		if (!pEffect->bUse)
		{	// パーティクルが使用されていない
			// 矩形を描画するかどうか
			SetDrawRectangle(pEffect->nIdx, false);
		}

		// 矩形の位置の設定
		SetPosRectangle(pEffect->nIdx, pEffect->pos, D3DXVECTOR3(pEffect->fRaduus, pEffect->fRaduus, 0.0f));

		// 矩形の色の設定
		SetColorRectangle(pEffect->nIdx, pEffect->col);
	}

	nCnt++;
	
	if (nCnt >= 100)
	{
		nCnt = 0;
	}
}

//=========================================
// パーティクルの描画処理
//=========================================
void DrawEffect(void)
{
}

//=========================================
// パーティクルの設定処理
//=========================================
void SetEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXCOLOR col)
{
	Effect *pEffect;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);

		if (pEffect->bUse)
		{// パーティクルが使用されてる
			continue;
		}

		pEffect->bUse = true;
		pEffect->type = type;
		pEffect->col = col;

		switch (type)
		{
		case EFFECT_TYPE_000:	// プレイヤーのジャンプパーティクル
			// 矩形のテクスチャの変更
			SetAddRectangle(pEffect->nIdx, true);
			ChangeTextureRectangle(pEffect->nIdx, TEXTURE_effect_002);
			////SetAddRectangle(pEffect->nIdx, true);
			////ChangeTextureRectangle(pEffect->nIdx, TEXTURE_effect_000);
			pEffect->pos = pos;
			pEffect->col.a = 1.0f;
			pEffect->fRaduus = 50.0f;
			pEffect->nMaxLife = 30;
			pEffect->nLife = pEffect->nMaxLife;
			//SetEffect(pEffect->pos,EFFECT_TYPE_001,GetColor(COLOR_WHITE));
			break;
		case  EFFECT_TYPE_001:		// プレイヤーの移動
			// 矩形のテクスチャの変更
			SetAddRectangle(pEffect->nIdx, true);
			ChangeTextureRectangle(pEffect->nIdx, TEXTURE_effect_001);
			pEffect->pos = pos;
			pEffect->col.a = 0.95f;
			pEffect->fRaduus = 30.0f;
			pEffect->nMaxLife = 30;
			pEffect->nLife = pEffect->nMaxLife;
			break;
		case  EFFECT_TYPE_002:		// プレイヤーの移動
									// 矩形のテクスチャの変更
			SetAddRectangle(pEffect->nIdx, true);
			ChangeTextureRectangle(pEffect->nIdx, TEXTURE_effect_000);
			pEffect->pos = pos;
			pEffect->col.g = 0.35f;
			pEffect->col.a = 0.25f;
			pEffect->fRaduus = 40.0f;
			pEffect->nMaxLife = 10;
			pEffect->nLife = pEffect->nMaxLife;
			break;
		case  EFFECT_TYPE_003:		// プレイヤーの移動
			// 矩形のテクスチャの変更
			SetAddRectangle(pEffect->nIdx, true);
			ChangeTextureRectangle(pEffect->nIdx, TEXTURE_effect_000);
			pEffect->pos = pos;
			pEffect->fRaduus = 40.0f;
			pEffect->nMaxLife = 170;
			pEffect->nLife = pEffect->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		// 矩形を描画するかどうか
		SetDrawRectangle(pEffect->nIdx, true);

		// 矩形の位置の設定
		D3DXVECTOR3 size = D3DXVECTOR3(pEffect->fRaduus, pEffect->fRaduus, 0.0f);
		SetPosRectangle(pEffect->nIdx, pEffect->pos, size);

		// 矩形の色の設定
		SetColorRectangle(pEffect->nIdx, pEffect->col);

		break;
	}
}
