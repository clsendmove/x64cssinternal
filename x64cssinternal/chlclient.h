#pragma once
#include "memory.h"
#include "client_class.h"

class c_chlclient {
public:
	client_class* get_all_classes() {
		typedef client_class* (fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 8)(this);
	}
};

add_interface(chlclient, c_chlclient);