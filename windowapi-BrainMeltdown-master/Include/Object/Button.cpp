#include "Button.h"

Button::~Button()
{
}

bool Button::Init()
{
    m_bActive = false;
    return true;
}

int Button::Update(float fTimeElapsed)
{
    return 0;
}

void Button::Render(HDC hdc)
{
    RECT t = GetPosition();
    if (!m_bActive) Rectangle(hdc, t.left, t.top, t.right, t.bottom);
    else Rectangle(hdc, t.left, t.top + 10, t.right, t.bottom);              // draw smal
}
