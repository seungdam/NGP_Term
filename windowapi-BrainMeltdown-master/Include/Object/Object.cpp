#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
	if ((HBITMAP)m_tImg != NULL) m_tImg.Destroy();
}

int Object::Update(float fTimeElapsed)
{
	return 0;
}


void Object::Render(HDC hdc)
{
}