#pragma once
#include "memory.h"
#include "typedef.h"
#include "c_user_cmd.h"
class c_input 
{
public:
	/*
		same as tf2 lol
		__int64 __fastcall GetUserCmd(__int64 a1, int a2)
		{
			__int64 result; // rax

			result = *(_QWORD *)(a1 + 264) + 72i64 * (a2 % 90); <- m_pCommands[sequence % 90];
			// hex(264) -> 0x108. i just like hex)
			if ( *(_DWORD *)(result + 8) != a2 )
				return 0i64;
			return result;
		}
	*/

	c_user_cmd* m_commands() {
		return *reinterpret_cast<c_user_cmd**>(reinterpret_cast<std::uintptr_t>(this) + 0x108);
	}

};

add_interface(input, c_input);