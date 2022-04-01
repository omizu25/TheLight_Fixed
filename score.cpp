//**************************************************
//
// Hackathon ( score.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "main.h"
#include "score.h"
#include "rectangle.h"
#include "texture.h"
#include "number.h"
#include "color.h"
#include "mode.h"
#include "input.h"
#include "utility.h"
#include "ranking.h"
#include "light.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const float	SCORE_WIDTH = 70.0f;	// �X�R�A�̕�
const float	SCORE_HEIGHT = 150.0f;	// �X�R�A�̍���
}// namespace�͂����܂�

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
int	s_nIdx;		// ���̔z��̃C���f�b�N�X
int	s_nScore;	// �X�R�A
}// namespace�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitScore(void)
{
	D3DXVECTOR3 size = D3DXVECTOR3(SCORE_WIDTH, SCORE_HEIGHT, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (GetMode())
	{
	case MODE_GAME:		// �Q�[��
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.13f, 0.0f);
		break;

	case MODE_RESULT:	// ���U���g
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.68f, SCREEN_HEIGHT * 0.25f, 0.0f);
		break;

	case MODE_TITLE:	// �^�C�g��
	case MODE_TUTORIAL:	// �`���[�g���A��
	case MODE_NONE:		// �������Ă��Ȃ�
	default:
		assert(false);
		break;
	}

	// ���̐ݒ�
	s_nIdx = SetNumber(pos, size, GetColor(COLOR_WHITE), s_nScore, DigitNumber(s_nScore), false);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitScore(void)
{
	// �g���̂��~�߂�
	StopUseRectangle(s_nIdx);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateScore(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawScore(void)
{
	/* ��`�ŕ`�悵�Ă܂� */
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetScore(int nScore)
{
	s_nScore = nScore;

	// ���̕ύX
	s_nIdx = ChangeNumber(s_nIdx, s_nScore);
}

//--------------------------------------------------
// ���Z
//--------------------------------------------------
void AddScore(int nValue)
{
	s_nScore += nValue;

	// ���̕ύX
	s_nIdx = ChangeNumber(s_nIdx, s_nScore);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
int GetScore(void)
{
	return s_nScore;
}
