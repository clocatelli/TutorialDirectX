#pragma once

#include <d3d11_1.h>
#include <DirectXColors.h>
#include "dxerr.h"


#ifdef _DEBUG
#ifndef HR
#define HR(x)                                       \
    {                                               \
    HRESULT hr = x;                                 \
if (FAILED(hr))                                     \
    {                                               \
    DXTraceW(__FILEW__, __LINE__, hr, L#x, TRUE);   \
    }                                               \
    }
#endif
#ifndef HR
#define HR(x) x;
#endif

#endif

namespace Memory
{
    template <class T> void SafeDelete(T& t)
    {
        if (t)
        {
            delete t;
            t = nullptr;
        }
    }

    template <class T> void SafeDeleteArr(T& t)
    {
        if (t)
        {
            delete[] t;
            t = nullptr;
        }
    }

    template <class T> void SafeRelease(T& t)
    {
        if (t)
        {
            t->Release();
            t = nullptr;
        }
    }

}