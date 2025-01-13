#include "stdafx.h"
#include "InlinkOutlink.h"
#include <Resman.h>


typedef HRESULT(__stdcall* CWzCanvas__raw_Serialize_t)(IWzCanvas*, IWzArchive*);
static auto _CWzCanvas__raw_Serialize = reinterpret_cast<CWzCanvas__raw_Serialize_t>(0x5001EF78);

HRESULT __stdcall CWzCanvas__raw_Serialize_hook(IWzCanvas* pThis, IWzArchive* pArchive) {
	HRESULT hr = _CWzCanvas__raw_Serialize(pThis, pArchive);
	if (FAILED(hr)) {
		return hr;
	}
	IWzPropertyPtr pProperty;
	hr = pThis->Getproperty(&pProperty);
	if (FAILED(hr)) {
		return hr;
	}
	Ztl_variant_t inlink;
	hr = pProperty->getItem(L"_inlink", (_variant_t*)(&inlink));
	if (FAILED(hr)) {
		return hr;
	}
	if (inlink.vt == VT_BSTR) {
		wchar_t* sUOL;
		hr = pArchive->GetabsoluteUOL(&sUOL);
		if (FAILED(hr)) {
			return hr;
		}
		Ztl_variant_t filepath;
		filepath.vt = VT_BSTR;
		// filepath.bstrVal = ZComAPI::ZComSysAllocStringLen(sUOL, wcslen(sUOL) + wcslen(inlink.bstrVal) + 1);
		filepath.bstrVal = reinterpret_cast<wchar_t* (__cdecl*)(wchar_t*, unsigned int)>(0x00402BE9)(sUOL, wcslen(sUOL) + wcslen(inlink.bstrVal) + 1);
		wchar_t* p = wcsstr(filepath.bstrVal, L".img");
		if (p) {
			*(p + 4) = '/';
			memcpy(p + 5, inlink.bstrVal, sizeof(wchar_t) * (wcslen(inlink.bstrVal) + 1));
			hr = pProperty->put_item(L"_inlink", filepath);
			//std::wcout << inlink.bstrVal << " " << filepath.bstrVal << std::endl;
		}
	}
	return hr;
}


void HandleLinkProperty(IWzCanvasPtr pCanvas) {
	// Check for link property
	IWzPropertyPtr pProperty;
	CHECK_HRESULT(pCanvas->Getproperty(&pProperty));
	const wchar_t* asLinkProperty[] = {
		L"_inlink",
		L"_outlink",
		L"source",
	};
	size_t nLinkProperty = sizeof(asLinkProperty) / sizeof(asLinkProperty[0]);
	for (size_t i = 0; i < nLinkProperty; ++i) {
		Ztl_variant_t link;
		CHECK_HRESULT(pProperty->getItem(const_cast<wchar_t*>(asLinkProperty[i]), (_variant_t*)(&link)));
		if (link.vt == VT_BSTR) {
			Ztl_variant_t vEmpty;
			Ztl_variant_t vSource;
			Resman::getIWzResMan()->raw_GetObject(link.bstrVal, vEmpty, vEmpty, &vSource);
			//std::wcout << "HandleLinkProperty" << asLinkProperty[i] << " " << vSource.vt << " " << link.bstrVal << std::endl;
			if (vSource.vt == VT_EMPTY) break;
			// Get source canvas
			IWzCanvasPtr pSourceCanvas = IWzCanvasPtr(vSource.GetUnknown(false, false));
			int nWidth, nHeight, nFormat, nMagLevel;
			pSourceCanvas->GetSnapshot(&nWidth, &nHeight, nullptr, nullptr, (CANVAS_PIXFORMAT*)&nFormat, &nMagLevel);
			IWzRawCanvasPtr pRawCanvas;
			pSourceCanvas->GetrawCanvas(0, 0, &pRawCanvas);
			// Get origin
			Ztl_variant_t vOrigin;
			pProperty->get_item(L"origin", &vOrigin);
			IWzVector2DPtr pOrigin = IWzVector2DPtr(vOrigin.GetUnknown(false, false));
			int nOriginX, nOriginY;
			CHECK_HRESULT(pOrigin->get_x(&nOriginX));
			CHECK_HRESULT(pOrigin->get_y(&nOriginY));
			// Create target canvas
			Ztl_variant_t vMagLevel(nMagLevel, VT_I4);
			Ztl_variant_t vFormat(nFormat, VT_I4);
			CHECK_HRESULT(pCanvas->Create(nWidth, nHeight, vMagLevel, vFormat));
			CHECK_HRESULT(pCanvas->AddRawCanvas(0, 0, pRawCanvas));
			// Set target origin
			CHECK_HRESULT(pCanvas->Putcx(nOriginX));
			CHECK_HRESULT(pCanvas->Putcy(nOriginY));
			//std::wcout << "HandleLinkProperty" << asLinkProperty[i] << " " << vSource.vt << " " << link.bstrVal << std::endl;
			break;
		}
	}
}

typedef IUnknown* (__thiscall* Ztl_variant_t__GetUnknown_t)(Ztl_variant_t*, bool, bool);
auto _Ztl_variant_t__GetUnknown = reinterpret_cast<Ztl_variant_t__GetUnknown_t>(0x004032B2);

IUnknown* __fastcall Ztl_variant_t__GetUnknown_hook(Ztl_variant_t* pThis, void* _EDX, bool fAddRef, bool fTryChangeType) {
	IUnknownPtr pUnknown = _Ztl_variant_t__GetUnknown(pThis, fAddRef, fTryChangeType);
	IWzCanvasPtr pCanvas;
	if (SUCCEEDED(pUnknown.QueryInterface(__uuidof(IWzCanvas), &pCanvas))) {
		HandleLinkProperty(pCanvas);
	}
	return pUnknown;
}

void InlinkOutlink::AttachClientInlink(bool enable) {
	if (enable)
	{
		std::cout<<"Enble New Inlink Outlink" << std::endl;
		LoadLibrary(L"CANVAS.DLL");
		Memory::SetHook(true, reinterpret_cast<void**>(&_CWzCanvas__raw_Serialize), CWzCanvas__raw_Serialize_hook);
		Memory::SetHook(true, reinterpret_cast<void**>(&_Ztl_variant_t__GetUnknown), Ztl_variant_t__GetUnknown_hook);
	}

}