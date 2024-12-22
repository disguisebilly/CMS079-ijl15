#pragma once
#include <comdef.h>

class Ztl_bstr_t {
public:
    class Data_t {
    public:
        wchar_t* m_wstr;
        char* m_str;
        volatile long m_RefCount;
    };

    Data_t* m_Data;

    inline Ztl_bstr_t() {
        this->m_Data = nullptr;
    }
    inline Ztl_bstr_t(const char* s) {
        reinterpret_cast<void(__thiscall*)(Ztl_bstr_t*, const char*)>(0x00426D6E)(this, s);
    }
    inline ~Ztl_bstr_t() {
        reinterpret_cast<void(__thiscall*)(Ztl_bstr_t*)>(0x00402FA4)(this);
    }
};