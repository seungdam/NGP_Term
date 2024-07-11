#include "../Game.h"
#include "Button.h"

Button::Button(RECT pos, bool bFixed, int group)
    : m_bFixed{ bFixed }, m_iGroupControll{ group }
{
    FPOINT size{ (pos.right - pos.left) / 2.0f, (pos.bottom - pos.top) / 2.0f };
    FPOINT pivot{ pos.left + size.x, pos.top + size.y };

    SetPosition(pivot);
    SetSize(size);
}

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
