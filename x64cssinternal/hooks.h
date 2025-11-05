#pragma once
#include "safetyhook/safetyhook.hpp"
#include "memory.h"
#include "typedef.h"
#include "interfaces.h"

namespace detours {
	// your detours here
	namespace paint {
		inline SafetyHookInline original;
		void fc detour(fc_rg, int mode);
	}

	namespace validate_user_cmd {
		inline SafetyHookInline original;
		void fc detour(fc_rg, c_user_cmd* cmd, int sequence_number);
	}

	namespace create_move {
		// CHLClient::CreateMove is ALWAYS better than Clientmode::CreateMove.
		inline SafetyHookInline original;
		void fc detour(fc_rg, int sequence_number, float input_sample_frametime, bool active);
	}
}

namespace hooks {
	void init();
	void uninit();
}