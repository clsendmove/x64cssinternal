#include "interfaces.h"
#include "signatures.h"

void interfaces::init() {
	signatures::init(); 

	i::engine_client = memory::get_interface<c_engine_client*>("engine.dll", "VEngineClient013");
	i::chlclient = memory::get_interface<c_chlclient*>("client.dll", "VClient017");
	i::entity_list = memory::get_interface<c_client_entity_list*>("client.dll", "VClientEntityList003");
	i::engine_vgui = memory::get_interface<c_engine_vgui*>("engine.dll", "VEngineVGui001");
	i::prediction = memory::get_interface<c_prediction*>("client.dll", "VClientPrediction001");
	i::game_movement = memory::get_interface<c_game_movement*>("client.dll", "GameMovement001");
	
	i::global_vars = reinterpret_cast<c_globals*>(memory::rel2abs(signatures::global_vars, 0x3));
	i::input = *reinterpret_cast<c_input**>(memory::rel2abs(signatures::input, 0x3));
	i::move_helper = *reinterpret_cast<c_move_helper**>(memory::rel2abs(signatures::move_helper, 0x3));
	i::random_seed = reinterpret_cast<int*>(memory::rel2abs(signatures::random_seed, 0x3));
}