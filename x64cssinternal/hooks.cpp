#include "hooks.h"
#include "dx9.h"
#include "interfaces.h"
#include "drawmanager.h"
#include "signatures.h"
#include "enums.h"
#include "engine_pred.h"
#include "cs.h"

using namespace detours;

void fc paint::detour(fc_rg, int mode) {
	original.fastcall<void>(rcx, mode);

	if (mode & PAINT_UIPANELS) {
		// stuff you can draw
		// make sure you do it before flush gets called.
		{
			u::draw.string(fonts::FONT_SEGOE, 5, 5, true, e_text_align::TEXT_LEFT, { 255,255,255,255 },
				"x64cssinternal by vannie74 on unknowncheats.me");
		}
		u::draw.flush(); // yea
	}
}

void fc create_move::detour(fc_rg, int sequence_number, float input_sample_frametime, bool active) {
	original.fastcall<void>(rcx, sequence_number, input_sample_frametime, active);

	
	c_user_cmd* const cmd = &i::input->m_commands()[sequence_number % 90]; // MULTIPLAYER_BACKUP

	if (!cmd)
		return;

	c_base_entity* local = i::entity_list->get_client_entity(i::engine_client->get_local_player())->as<c_base_entity>();
	if (!local)
		return;
	if (!local->is_alive())
		return;

	cs::local = local;

	c_base_combat_weapon* weapon = local->get_active_weapon();

	if (!weapon)
		return;
	
	cs::weapon = weapon;

	f::engine_pred->start(cmd); {
		// do your aimbot and shit here
	} f::engine_pred->end(cmd);

}

void fc validate_user_cmd::detour(fc_rg, c_user_cmd* cmd, int sequence_number) {
	return; // u can also validate the user cmd at the end of the chlclient hook, or atleast thats what i do on l4d2
}


void hooks::init() {
	using namespace detours;

	dx9_hooks::startup();

	paint::original = safetyhook::create_inline(memory::get_vfunc<void*>(i::engine_vgui, 14), paint::detour);
	validate_user_cmd::original = safetyhook::create_inline(signatures::validate_user_cmd, validate_user_cmd::detour);
	create_move::original = safetyhook::create_inline(memory::get_vfunc<void*>(i::chlclient, 21), create_move::detour);
}

void hooks::uninit() {
	using namespace detours;

	dx9_hooks::shutdown();

	paint::original = {};
	validate_user_cmd::original = {};
	create_move::original = {};
}