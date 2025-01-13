#pragma once
#include "stdafx.h"

#undef  INTERFACE
#define INTERFACE   IWzRawCanvas

DECLARE_INTERFACE_IID_(IWzRawCanvas, IUnknown, "312126f0-c399-4111-8eab-0f96a30b6b7c") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzRawCanvas methods ***/
    STDMETHOD(get_pixelFormat)(THIS_ CANVAS_PIXFORMAT*) PURE;
    STDMETHOD(get_magLevel)(THIS_ int*) PURE;
    STDMETHOD(get_width)(THIS_ unsigned int* puWidth) PURE;
    STDMETHOD(get_height)(THIS_ unsigned int* puHeight) PURE;
    STDMETHOD(raw__LockAddress)(THIS_ int* pnPitch, tagVARIANT*) PURE;
    STDMETHOD(raw__UnlockAddress)(THIS_ tagRECT*) PURE;
    STDMETHOD(raw_SetTexture)(THIS_ unsigned int*) PURE;
    STDMETHOD(raw_GetTextureSize)(THIS_ tagRECT*) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzRawCanvas, __uuidof(IWzRawCanvas));