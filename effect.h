//=========================================
// 
// パーティクルのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//-----------------------------------------
// include
//-----------------------------------------

//------------------------------------
// 判定位置の列挙型の定義
//------------------------------------
typedef enum
{
	EFFECT_TYPE_000 = 0,		// プレイヤーの跳躍
	EFFECT_TYPE_001,			// プレイヤーの移動
	EFFECT_TYPE_002,			// プレイヤーの移動
	EFFECT_TYPE_003,			// プレイヤーの移動
	MAX_EFFECT_TYPE
}EFFECT_TYPE;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXCOLOR col);

#endif // _EFFECT_H_
