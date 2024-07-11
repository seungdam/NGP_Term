#include "../pch.h"
#include "Step.h"

CImage Step::m_tStepImg;

bool Step::Init()
{
	if (m_tStepImg.IsNull()) m_tStepImg.Load(L"Resource/imgsp.bmp");
	return true;
}

int Step::Update(float fTimeElapsed)
{
	return 0;
}

void Step::Render(HDC hdc)
{
	if (m_bAlive == false && !m_ibuttonAliveCnt) return;

	RECT temp = (RECT)GetPosition();

	int t = 0;

	switch (m_eType) 
	{
	case STEP_FOR::SF_ALL:		t = 1;	break;
	case STEP_FOR::SF_PUR:		t = 3;	break;
	case STEP_FOR::SF_YELLOW:	t = 2;	break;
	default:					t = 1;	break;
	}

	m_tStepImg.TransparentBlt(hdc, temp, { t * TILE_IMAGE_STRIDE, 0,  t * TILE_IMAGE_STRIDE + TILE_IMAGE_SIZE, 28 }, IMAGE_TRANSPARENT);

}
