#pragma once

DWORD fixBuddyAcceptFunc = 0x00A4987D;
DWORD fixBuddyAcceptJmp = 0x005410E1;
DWORD fixBuddyAcceptRtn = 0x005410AF;
__declspec(naked) void fixBuddyAccept() {
	__asm {
		call fixBuddyAcceptFunc
		cmp byte ptr[ebp - 33h], 9
		jz label_jmp
		jmp fixBuddyAcceptRtn

		label_jmp :
		jmp fixBuddyAcceptJmp
	}
}

class FixBuddy {
public:
	static void Hook() {
		Memory::CodeCave(fixBuddyAccept, 0x005410AA, 5);
	}
};