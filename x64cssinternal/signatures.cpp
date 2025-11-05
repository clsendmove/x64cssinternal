#include "signatures.h"

void signatures::init() {
	global_vars = memory::find_signature<QWORD>("engine.dll", "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 8B CA");
	validate_user_cmd = memory::find_signature<QWORD>("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 41 8B D8");
	input = memory::find_signature<QWORD>("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 85 C0 0F 84 ? ? ? ? F3 0F 10 05");
	move_helper = memory::find_signature<QWORD>("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 0F B7 D7");
	random_seed = memory::find_signature<QWORD>("client.dll", "0F B6 1D ? ? ? ? 89 9D");
}