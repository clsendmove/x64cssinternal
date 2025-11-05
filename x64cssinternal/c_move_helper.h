#pragma once
#include "memory.h"
#include "typedef.h"
#include "entities.h"

class c_move_helper {
public:
	void set_host(c_base_entity* player) {
		typedef void(__thiscall* fn)(void*, c_base_entity*);
		return memory::get_vfunc<fn>(this, 12)(this, player);
	}
};

add_interface(move_helper, c_move_helper);