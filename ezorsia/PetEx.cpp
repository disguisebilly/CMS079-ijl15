#pragma once
#include "stdafx.h"
#include "PetEx.h"
#include "Resman.h"
#include <iomanip>

int __stdcall _checkCanEquip(int equipId, int petId) {
	std::wostringstream oss;
	if (equipId / 10000 == 180 && petId / 10000 == 500) {
		oss << L"Character/PetEquip/";
		oss << std::setw(8) << std::setfill(L'0') << equipId;
		oss << ".img/";
		oss << petId;
		std::wstring path = oss.str();
		auto pro = getIWzPropertyForPath(path);
		//std::wcout << path << " " << pro << std::endl;
		return pro != nullptr;
	}
	return 0;
}

__declspec(naked) void checkCanEquip()
{
	__asm {
		pop ecx
		pop ecx
		push[ebp + 0x8]
		push eax
		call _checkCanEquip
		push 0x00503286
		ret
	}
}

void PetEx::HookPetCheckCanEquip(bool enable)
{
	if (!enable)
		return;
	Memory::CodeCave(checkCanEquip, 0x00503227, 7);
}
