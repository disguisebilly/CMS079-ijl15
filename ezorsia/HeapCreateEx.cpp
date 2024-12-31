#include "stdafx.h"
#include "HeapCreateEx.h"

auto heap1 = HeapCreate(0, 1024, 0);
auto heap2 = HeapCreate(0, 1024, 0);

__declspec(naked) void mapStruct() {
	__asm {
		pushad
		pushfd
		mov eax, heap1
		cmp[eax], 0
		jne label_jne
		popfd
		popad
		lea eax, [esi + 0x04]
		push eax
		push 0
		push 0x00402EB1
		ret
		label_jne :
		popfd
			popad
			lea eax, [esi + 0x04]
			push eax
			push 0
			push heap1
			push 0x00402EB8
			ret
	}
}

void heapCompact(int ebx) {
	//if (ebx != 0)
	//	std::cout << "进入商城" << std::endl;
	if (heap1 != NULL)
		HeapCompact(heap1, 0);
	if (heap2 != NULL)
		HeapCompact(heap2, 0);
}

__declspec(naked) void environmentSwitch() {
	__asm {
		pushad
		pushfd
		mov esi, [0x00BE2784]
		mov esi, [esi]
		mov eax, [esi]
		push 0
		push esi
		call dword ptr[eax + 0x24] //freeMap
		push ebx
		call heapCompact
		pop ebx
		popfd
		popad
		mov eax, 0x00AC4A90
		push 0x0077FB19
		ret
	}
}

bool Hook_HeapCreate(bool bEnable)
{
	static auto _HeapCreate = decltype(&HeapCreate)(GetFuncAddress("kernel32.dll", "HeapCreate"));

	decltype(&HeapCreate) Hook = [](DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) -> HANDLE
		{
			Hook_HeapCreate(false);
			std::cout << "Hook_HeapCreate:" << _ReturnAddress() << std::endl;
			HANDLE origin;
			if ((int)_ReturnAddress() == 0x00A75F02) {
				origin = heap2;
			}
			else {
				origin = _HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
				HeapCompact(origin, 0);
			}
			Hook_HeapCreate(true);
			return origin;
		};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_HeapCreate), Hook);
}

void HeapCreateEx::HOOK_HeapCreate()
{
	if (heap1 != NULL && heap2 != NULL)
		std::cout << "Hook_HeapCreate :" << (Hook_HeapCreate(true) == 1) << " heap1:" << heap1 << " heap2:" << heap2 << std::endl;
}

void HeapCreateEx::MemoryOptimization()
{
	Memory::CodeCave(mapStruct, 0x00402EAB, 5);
	Memory::CodeCave(environmentSwitch, 0x0077FB14, 5);
}
