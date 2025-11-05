#pragma once
#include "interfaces.h"

class c_nospread {
public:
	void run(c_base_entity* local, c_user_cmd* cmd);
};

add_feature_(no_spread, c_nospread);