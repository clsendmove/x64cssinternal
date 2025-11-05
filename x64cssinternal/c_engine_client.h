#pragma once
#include "memory.h"
#include "typedef.h"
#include "player_info_t.h"
#include "vector.h"
#include "matrix.h"
// same as tf2 lol
// you'd be suprised how most tf2 shit also works on cs:s

class c_engine_client {
public:
	bool get_player_info(int num, player_info_t* pInfo) {
		typedef bool(fc* fn)(fc_rg, int, player_info_t*);
		return memory::get_vfunc<fn>(this, 8)(this, num, pInfo);
	}
	int get_player_for_user_id(int num) {
		typedef int(fc* fn)(fc_rg, int);
		return memory::get_vfunc<fn>(this, 9)(this, num);
	}

	int get_local_player() {
		typedef int(fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 12)(this);
	}

	float time() {
		typedef float(fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 14)(this);
	}

	vec3 get_view_angles() {
		vec3 vfunc_return{};
		typedef void(fc* fn)(fc_rg, vec3&);
		memory::get_vfunc<fn>(this, 19)(this, vfunc_return);
		return vfunc_return;
	}

	void set_view_angles(vec3& v) {
		typedef void(fc* fn)(fc_rg, vec3&);
		memory::get_vfunc<fn>(this, 20)(this, v);
	}

	int get_max_clients() {
		typedef int(fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 21)(this);
	}

	bool is_in_game() {
		typedef bool(fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 26)(this);
	}

	bool is_connected() {
		typedef bool(fc* fn)(fc_rg);
		return memory::get_vfunc<fn>(this, 27)(this);
	}

	void fire_events() {
		typedef void(fc* fn)(fc_rg);
		memory::get_vfunc<fn>(this, 56)(this);
	}

	void clientcmd_unrestricted(const char* cmd) {
		// vfunc 106
		typedef void(fc* fn)(fc_rg, const char*);
		memory::get_vfunc<fn>(this, 106)(this, cmd);
	}

	DWORD* get_net_channel_info(void)
	{
		typedef DWORD* (fc* OriginalFn)(fc_rg);
		return memory::get_vfunc<OriginalFn>(this, 72)(this);
	}
	const matrix4x4& world_to_screen_matrix(void) {
		return memory::get_vfunc<const matrix4x4 & (fc*)(void*)>(this, 36u)(this);
	}
};

add_interface(engine_client, c_engine_client);
