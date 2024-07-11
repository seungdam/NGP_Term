#pragma once

#define SAFE_DELETE(p)			if(p)	{ delete p; p = NULL; }
#define SAFE_DELETE_ARRY(p)		if(p)	{ delete[] p; p = NULL; }
#define SAFE_RELEASE(p)			if(p)	{ p->Release(); p = NULL; }


template <typename T>
void ReleaseAndCleanVector(T & p)
{
	typename T::iterator iter;
	for (iter = p.begin(); iter != p.end(); ++iter) SAFE_DELETE((*iter));

	p.clear();
}
