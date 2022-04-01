//**************************************************
//
// Hackathon ( light.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _LIGHT_H_	// このマクロが定義されてなかったら
#define _LIGHT_H_	// 2重インクルード防止のマクロ定義

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitLight(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitLight(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateLight(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawLight(void);

//--------------------------------------------------
// 取得
// 返値  : int / 現在の最大数
//--------------------------------------------------
int GetLight(void);

//--------------------------------------------------
// 色の取得
// 引数  : int nNowLight / ライトの数
// 返値  : int / 現在の最大数
//--------------------------------------------------
int GetColorLight(int nNowLight);

//--------------------------------------------------
// 描画設定
// 引数  : int nNowLight / ライトの数
//--------------------------------------------------
void SetDrawLight(int nNowLight);

//--------------------------------------------------
// 間違った
//--------------------------------------------------
void MistakeLight(void);

#endif // !_LIGHT_H_
