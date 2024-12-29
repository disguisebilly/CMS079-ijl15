#pragma once
#include "stdafx.h"
#undef  INTERFACE
#define INTERFACE   IWzGr2D

DECLARE_INTERFACE_IID_(IWzGr2D, IUnknown, "e576ea33-d465-4f08-aab1-e78df73ee6d9") {
    BEGIN_INTERFACE;

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /*** IWzGr2D methods ***/
    STDMETHOD(raw_Initialize)(THIS_ unsigned int uWidth, unsigned int uHeight, tagVARIANT vHwnd, tagVARIANT vBPP, tagVARIANT vRefreshRate) PURE;
    STDMETHOD(raw_Uninitialize)(THIS) PURE;
    STDMETHOD(get_nextRenderTime)(THIS_ int*) PURE;
    STDMETHOD(raw_UpdateCurrentTime)(THIS_ int tTime) PURE;
    STDMETHOD(raw_RenderFrame)(THIS) PURE;
    STDMETHOD(raw_SetFrameSkip)(THIS) PURE;
    STDMETHOD(raw_ToggleFpsPanel)(THIS) PURE;
    STDMETHOD(raw_DisableFpsPanel)(THIS) PURE;
    STDMETHOD(get_width)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_height)(THIS_ unsigned int*) PURE;
    STDMETHOD(put_screenResolution)(THIS_ unsigned int uWidth, unsigned int uHeight) PURE;
    STDMETHOD(get_bpp)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_refreshRate)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_fps100)(THIS_ unsigned int*) PURE;
    STDMETHOD(get_currentTime)(THIS_ int*) PURE;
    STDMETHOD(get_fullScreen)(THIS_ int*) PURE;
    STDMETHOD(put_fullScreen)(THIS_ int) PURE;
    STDMETHOD(get_backColor)(THIS_ unsigned int*) PURE;
    STDMETHOD(put_backColor)(THIS_ unsigned int) PURE;
    STDMETHOD(get_redTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_greenBlueTone)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(get_center)(THIS_ IWzVector2D**) PURE;
    STDMETHOD(raw_GetSnapshot)(THIS_ tagVARIANT, unsigned int, int, int) PURE;
    STDMETHOD(raw_CreateLayer)(THIS_ int nLeft, int nTop, unsigned int uWidth, unsigned int uHeight, int nZ, tagVARIANT vCanvas, tagVARIANT dwFilter, IWzGr2DLayer**) PURE;
    STDMETHOD(raw_AdjustCenter)(THIS_ int nAdjustCenterX, int nAdjustCenterY) PURE;
    STDMETHOD(raw_TakeScreenShot)(THIS_ wchar_t*, int) PURE;
    STDMETHOD(raw_SetVideoMode)(THIS_ int bMode) PURE;
    STDMETHOD(raw_SetVideoPath)(THIS_ wchar_t*, int, int) PURE;
    STDMETHOD(raw_PlayVideo)(THIS) PURE;
    STDMETHOD(raw_PauseVideo)(THIS_ int) PURE;
    STDMETHOD(raw_StopVideo)(THIS) PURE;
    STDMETHOD(get_videoStatus)(THIS_ int*) PURE;
    STDMETHOD(put_videoVolume)(THIS_ int) PURE;

    END_INTERFACE;
};
_COM_SMARTPTR_TYPEDEF(IWzGr2D, __uuidof(IWzGr2D));