#pragma once
#include "vector.h"

class c_user_cmd {
public:
	void* vmt; // destructor
	int command_number; //4
	int tick_count; //8
	vec3 viewangles; //C
	float forwardmove; //18
	float sidemove; //1C
	float upmove; //20
	int	buttons; //24
	std::byte impulse; //28
	int weaponselect; //2C
	int weaponsubtype; //30
	int random_seed; //34
	short mousedx; //38
	short mousedy; //3A
	bool hasbeenpredicted; //3C;
};