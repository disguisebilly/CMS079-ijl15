#pragma once
#include "stdafx.h"
#undef  INTERFACE
#define INTERFACE   IWzShape2D

DECLARE_INTERFACE_IID_(IWzShape2D, IWzSerialize, "4cfb57c7-eae3-40b3-ac98-4b2750e3642a") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzSerialize methods ***/
    STDMETHOD(get_persistentUOL)(THIS_ wchar_t**) PURE;
    STDMETHOD(raw_Serialize)(THIS_ IWzArchive*) PURE;

    /*** IWzShape2D methods ***/
    STDMETHOD(get_item)(THIS_ tagVARIANT, tagVARIANT*) PURE;
    STDMETHOD(get__NewEnum)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_count)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_x)(THIS_ int*) PURE;
    STDMETHOD(put_x)(THIS_ int) PURE;
    STDMETHOD(get_y)(THIS_ int*) PURE;
    STDMETHOD(put_y)(THIS_ int) PURE;
    STDMETHOD(get_x2)(THIS_ int*) PURE;
    STDMETHOD(put_x2)(THIS_ int) PURE;
    STDMETHOD(get_y2)(THIS_ int*) PURE;
    STDMETHOD(put_y2)(THIS_ int) PURE;
    STDMETHOD(raw_Move)(THIS_ int nX, int nY) PURE;
    STDMETHOD(raw_Offset)(THIS_ int nDX, int nDY) PURE;
    STDMETHOD(raw_Scale)(THIS_ int nXMul, int nXDiv, int nYMul, int nYDiv, int nXOrg, int nYOrg) PURE;
    STDMETHOD(raw_Insert)(THIS_ tagVARIANT vIndexOrShape, tagVARIANT vShape) PURE;
    STDMETHOD(raw_Remove)(THIS_ tagVARIANT vIndex, tagVARIANT * pvRemoved) PURE;
    STDMETHOD(raw_Init)(THIS_ int nX, int nY) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzShape2D, __uuidof(IWzShape2D));